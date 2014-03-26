#include "StdAfx.h"
#include "MC_Compute.h"
#include "test.h"



MC_Compute::MC_Compute(Produit * produit, Model * model)
{
	m_discretisation = WEEK;
	m_produit = produit;

	//Sert à simplifier la lecture du code. 
	m_sizeEquityProduct = produit->getEquities().size();
	m_model = model;
	_timer = Pesp_Timer();
	//_timer.Start();

	m_rng = pnl_rng_create (PNL_RNG_MERSENNE);
	pnl_rng_sseed (m_rng, time(NULL));

	//Vecteur de date de fixing
	static const int arr[] = {FIXING0, FIXING1, FIXING2, FIXING3, FIXING4};
	std::vector<int> lvec_fixingDate (arr, arr + sizeof(arr) / sizeof(arr[0]) );
	mvec_fixingDate = lvec_fixingDate;
}


bool MC_Compute::isRemb(PnlMat * coursHisto, int time) {
	bool tmp = false;
	if(time > mvec_fixingDate[1]){
		if(Condition_Remb(coursHisto, mvec_fixingDate[1])){
			std::cout<< "LE PAYOFF A DEJA ETE TOUCHE EN t1"<<std::endl;
			return true;
		}
		if (time > mvec_fixingDate[2]){
			if(Condition_Remb(coursHisto, mvec_fixingDate[2])){
				std::cout<< "LE PAYOFF A DEJA ETE TOUCHE EN t2"<<std::endl;
				return true;
			}
			if (time > mvec_fixingDate[3]){
				if(Condition_Remb(coursHisto, mvec_fixingDate[3])){
					std::cout<< "LE PAYOFF A DEJA ETE TOUCHE EN t3"<<std::endl;
					return true;
				}
			}
		}
	}
	return tmp;
}

int MC_Compute::Price(double * sumPrice, double *priceSquare, PnlVect * sumDelta, PnlVect * sumGamma, PnlMat * l_histoFix, int time)
{

	//reset obligatoire des paramètres
	*sumPrice = 0;
	*priceSquare = 0;
	pnl_vect_set_zero(sumDelta);
	pnl_vect_set_zero(sumGamma);
	//check model parameter
	if(!m_model->CheckParameter()) return -1;
	//Pour l'instant c'est nous qui générons la matrice historique il faudra dans le future récupérer les cours sur internet
	PnlMat * l_coursHisto = m_produit->getMatHisto();

	double l_payoff = 0;
	PnlVect *l_drift = pnl_vect_create_from_double(m_sizeEquityProduct, 0.05);
	PnlVect * l_spot = GetInitSpot();
	PnlVect * l_vol = GetInitVol();
	// La matrice past va contenir le passe (ie les valeurs historiques jusqua time)
	PnlMat * l_past = pnl_mat_create(l_coursHisto->m, l_coursHisto->n);
	//pnl_mat_print(l_coursHisto);  OK
	if(isRemb(l_coursHisto, time))
		return -10;
	TronqCoursHisto(l_coursHisto, l_past, time);
	//pnl_mat_print(l_past); OK
	//Ici la matrice l_past contient les valeurs historiques sur les colonnes de 0 à time

	//Monte carlo

	
	for (int i = 0; i < m_model->Nb_Path(); i++) {
		//PnlMat *l_histoFix = pnl_mat_create(m_sizeEquityProduct, mvec_fixingDate.size());
		m_model->Diffuse_from_t(l_past, l_drift, l_vol, m_produit, m_rng, time);
		//Timer().GetTime("Diffuse");
		//pnl_mat_print(l_past);  OK
		// en sortie la matrice past contient les valeurs historiques sur les colonnes de 0 a time
		// et les valeurs simulees de time + 1 a la derniere
		
		// A partir de Past on calcul le prix et le delta et le gamma

		//Pour calculer le prix nous avons besoins des valeurs des sous jacents qu'au date de fixing getPathFix retourne les valeurs des sous jacents aux dates de fixing
		getPathFix(l_past, l_histoFix, mvec_fixingDate);
		//Timer().GetTime("getPathFix");
		//pnl_mat_print(l_histoFix);   OK

		PriceProduct(l_histoFix, &l_payoff, time);
		
		//Timer().GetTime("Price");
		_timer.Start();
		ComputeGrec(sumDelta, sumGamma, l_histoFix, l_payoff, l_vol, l_drift, time);
		_timer.Stop();
		//Timer().GetTime("Compute grec");
		*sumPrice += l_payoff;
		//pnl_mat_free(&l_histoFix);	
		//while(1);
	}
	_timer.GetTime("Durée compute grec");
	//Moyenne du prix du delta et du gamma
	*sumPrice /= m_model->Nb_Path();

	pnl_vect_div_double(sumDelta,  m_model->Nb_Path());
	//pnl_vect_div_double(sumGamma,  m_model->Nb_Path());

	pnl_vect_free(&l_drift);
	pnl_vect_free(&l_spot);
	pnl_vect_free(&l_vol);
	pnl_mat_free(&l_past);
	pnl_mat_free(&l_coursHisto);

	return 0;
}

inline void MC_Compute::PriceProduct(const PnlMat * histoFix, double * payoff, int time) {
	//Renta est la matrice des rentabilites avec autant de lignes que d'actifs et 4 colonnes car il y a 4 intervales entres les dates de fixing (5dates)
	PnlMat *l_renta = pnl_mat_create(m_sizeEquityProduct, mvec_fixingDate.size() - 1);
	RentFromMat(histoFix, l_renta);
	//std::cout<< "SANS SHIFT" <<std::endl;
	//pnl_mat_print(l_renta);
	//std::cout<< "\n" <<std::endl;
	// On peut ensuite cacluler le prix ici le payoff en fait c'est le prix (il faut qu'on change le nom)
	*payoff = Price2(l_renta, time);
	pnl_mat_free(&l_renta);
}


inline void MC_Compute::ComputeGrec(PnlVect * sumDelta, PnlVect* sumGamma, const PnlMat * path2, const double payoff, PnlVect* l_vol, PnlVect* l_drift, int time) {
	
	//PnlMat *l_pastShPos = pnl_mat_create(past->m, past->n);
	//PnlMat *l_pastShNeg = pnl_mat_create(past->m, past->n);
	PnlMat *path = pnl_mat_copy(path2);
	//PnlMat *tmp = pnl_mat_create(path2->)
	//PnlMat * l_histoFixShNeg = pnl_mat_create(m_sizeEquityProduct, mvec_fixingDate.size());
	PnlMat *l_rentPos = pnl_mat_create(m_sizeEquityProduct, mvec_fixingDate.size() -1);
	//PnlMat *l_rentNeg = pnl_mat_create(m_sizeEquityProduct, mvec_fixingDate.size()- 1);
	double ld_payoffPos = 0;
	double ld_payoffNeg = 0;
	int li_dateFixing =  ComputeDateFix(time);

	for (int i = 0; i < m_sizeEquityProduct; i++) {
			//pnl_mat_clone(l_pastShPos, past);
			//pnl_mat_clone(l_pastShNeg, past);
			/*for (int n = time + 1; n < PAS; n++){
				// On shifte positivement et negativement
				MLET(l_pastShPos,l,n)=(MGET(past,l,n))*(1+H);
				MLET(l_pastShNeg,l,n)=(MGET(past,l,n))*(1-H);
			}*/
			//Icii on selectionne les dates de fixing et on retourne l'histoFixShPos et l_histoFixShNeg
			//getPathFix(l_pastShNeg, l_histoFixShNeg, mvec_fixingDate);

			//Positif
			for (int j = li_dateFixing; j < path->n; j++)
			{
				MLET(path,i,j) = (MGET(path,i,j))*(1+H);
			}

			//reset matrice
			//pnl_mat_set_zero(l_rentNeg);
			//pnl_mat_set_zero(l_rentPos);

			//calcul des rentabilites pour chacune des matrices de histofix shifte
			RentFromMat(path,l_rentPos);
			//Rent(l_histoFixShNeg,l_rentNeg);
			
			// Calcul le payoff a partir de la matrice des rentabilites
			ld_payoffPos = Price2(l_rentPos, time);



			//Negatif
			for (int j = li_dateFixing; j < path->n; j++)
			{
				MLET(path,i,j) = MGET(path,i,j)*(1-H)/(1+H);
			}
			RentFromMat(path,l_rentPos);  
			ld_payoffNeg = Price2(l_rentPos, time);
		/*	
			std::cout<< payoff<<std::endl;
			std::cout<< "\n"<<std::endl;

			std::cout<< "ShiftPos" << std::endl;
			pnl_mat_print(l_rentPos);
			std::cout<< "\n"<<std::endl;
			std::cout<< ld_payoffPos<<std::endl;
			std::cout<< "\n"<<std::endl;
			

			std::cout<< "shiftNeg" << std::endl;
			pnl_mat_print(l_rentNeg);
			std::cout<< "\n"<<std::endl;
			std::cout<< ld_payoffNeg<<std::endl;
			std::cout<< "\n"<<std::endl;
			*/			
			pnl_vect_set(sumDelta, i, GET(sumDelta, i)+((ld_payoffPos-ld_payoffNeg)/(2*H)));
			//pnl_vect_set(sumGamma, l ,GET(sumGamma, i)+((ld_payoffPos - 2 * payoff + ld_payoffNeg)/(pow(H,2))));
			
			for (int j = ComputeDateFix(time); j < path->n; j++)
			{
				MLET(path,i,j) = (MGET(path,i,j))/(1-H);
			}
	}
	

	//libération ressource
	pnl_mat_free(&path);
	pnl_mat_free(&l_rentPos);
	//pnl_mat_free(&l_histoFixShNeg);
	//pnl_mat_free(&l_pastShPos);
	//pnl_mat_free(&l_pastShNeg);
	//	pnl_mat_free(&l_rentNeg);
}

MC_Compute::~MC_Compute()
{
}


inline double MC_Compute::Compute_dt(int date)
{
	if (m_discretisation = WEEK) {
		if (date < 104)
		{
			return (104 - date)/52;
		} else 
		{
			return (52 - (date % 52))/52;
		}
	} else {
		if (date < 104*7)
		{
			return (104*7 - date)/365;
		} else 
		{
			return (365 - (date % 365))/365;
		}
	}
}

inline int MC_Compute::ComputeDateFix(int time)
{
	if (m_discretisation = WEEK) {
		if (time < 104)
		{
			return 1;
		} else if (time < 156)
		{
			return 2;
		} else if (time < 208) 
		{
			return 3;
		} else 
		{
			return 4;
		}
	} else {
		if (time < 104*7)
		{
			return 1;
		} else if (time < 156*7)
		{
			return 2;
		} else if (time < 208*7)
		{
			return 3;
		} else 
		{
			return 4;
		}
	}
}

//retourne la valeur actualisée de value
inline double MC_Compute::Discount(double value, int date, int time)
{
	//Pour le moment le taux sans risque est la c'est pas la qu'il devra etre
	return (value*exp(-((date-time)/NBSEMAINE * TAUX_ACTUALISATION)));
}

inline double MC_Compute::Price2(const PnlMat *rent, int time)
{
	double minCol;
	double val = 0;
	for (int i = 0; i < rent->n; i++)
	{
		minCol = pnl_mat_get(rent,0,i);
		for (int j = 0; j < rent->m; j++)
		{
			if (pnl_mat_get(rent, j, i) < minCol)
			{
				minCol = pnl_mat_get(rent, j, i);
			}
		}
		//!!!!!!\\\ i +1 ? et i!= 4 ???
		if ((minCol > -0.1) && (i != 3))
		{
			// Si on entre ici c'est qu'il y a un remboursement anticipé
			// mvec_fixingDate[i+1] est la date à laquelle le flux est touchée
			// time est la date à laquelle on calcul le prix
			//std::cout << "remboursement anticipé en "<< mvec_fixingDate[i+1] << std::endl;
		    return Discount(REMB_ANTI, mvec_fixingDate[i+1], time);
		}
	}
	PnlVect *perf = pnl_vect_create(rent->m);
	pnl_mat_get_col(perf, rent, rent->n - 1);

	//std::cout<< "VECTEUR DE PERFORMANCE DONT ON CALCUL LA PERF LISSSE" << std::endl;
	//pnl_vect_print(perf);

	val = std::max(1.0, REMB_N_ANTI + Perf_Liss(perf));
	pnl_vect_free(&perf);
	//std::cout << "remboursement en T "<< val << std::endl;
	return Discount(val, mvec_fixingDate[mvec_fixingDate.size() - 1], time);
}

inline void MC_Compute::Perf_Boost(const PnlVect *perf, PnlVect * ret)
{
	double val;
	for (int i = 0; i < perf->size; i++)
	{
		val = pnl_vect_get(perf, i);
		if (val > 0)
		{
			val *= 2;
		}
		pnl_vect_set(ret, i, val);
	}
}

inline double MC_Compute::Perf_Liss(const PnlVect *spot)
{
	double val;
	double ret = 0.0;
	PnlVect *res =	 pnl_vect_create(spot->size);
	Perf_Boost(spot, res);
	for (int i = 0; i < res->size; i++)
	{
		val = pnl_vect_get(res, i);
		if (val < PERF_MIN)
		{
			val = PERF_MIN;
		} 
		else if (val > PERF_MAX)
		{
			val = PERF_MAX;
		}
		ret += val;
	}
	pnl_vect_free(&res);
	return ret/m_sizeEquityProduct;
}



/**
* @author Yannick Pierre
* @param in matrice
* @param out resultat
* @Retourne la rentabilité param in avec : rentabilité par rapport à la date zero
*
* OKY 25/03
*/
inline void MC_Compute::RentFromMat(const PnlMat *mat, PnlMat *res)
{
	//TODO Quand on aura 20 actifs vérifié si ce n'est pas intéressant de parallèliser pour l'instant non
	//#pragma omp parallel for
	for (int i = 0; i < mat->n - 1; i++)
	{
		for (int j = 0; j < mat->m; j++) {
			pnl_mat_set(res, j, i, (MGET(mat, j,  i + 1) / MGET(mat, j, 0)) - 1);
		}
	}
}
/**
* @author Yannick Pierre
* @param in matrice
* @param out resultat
* @Retourne la rentabilité param in avec : rentabilité par rapport à la date zero
*
* OKY 25/03
*/
inline void MC_Compute::RentFromVect(const PnlVect *vect, PnlVect *res)
{
	//TODO Quand on aura 20 actifs vérifié si ce n'est pas intéressant de parallèliser pour l'instant non
	//#pragma omp parallel for
	for (int j = 0; j < vect->size-1; ++j) {
		pnl_vect_set(res, j, (pnl_vect_get(vect, j + 1) / pnl_vect_get(vect, 0)) - 1);
	}
}

inline void MC_Compute::RentFromVectHisto(const PnlVect *vect, PnlVect *res, int time) {
	//TODO Quand on aura 20 actifs vérifié si ce n'est pas intéressant de parallèliser pour l'instant non
	//#pragma omp parallel for
	for (int j = time; j < vect->size-1; ++j) {
		pnl_vect_set(res, j, (pnl_vect_get(vect, j + 1) / pnl_vect_get(vect, 0)) - 1);
	}
}

//spot est a desallouer en dehors
PnlVect * MC_Compute::GetInitSpot() {
	PnlVect * l_spot = pnl_vect_create(m_sizeEquityProduct);
	for(int k = 0; k < m_sizeEquityProduct; k++){
		pnl_vect_set(l_spot, k, m_produit->getEquities()[k].value);
	}
	return l_spot;
}

//vol est a desallouer en dehors
PnlVect * MC_Compute::GetInitVol() {
	PnlVect *vol = pnl_vect_create(m_sizeEquityProduct);
	for(int k = 0; k < m_sizeEquityProduct; k++){
		pnl_vect_set(vol, k, m_produit->getEquities()[k].volatility);
	}
	return vol;	
}


inline void MC_Compute::RentVect(PnlVect * V, PnlVect * V0, PnlVect * res) {
	for (int k = 0 ; k < res->size; k++){
		pnl_vect_set(res, k, (pnl_vect_get(V,k)/pnl_vect_get(V0,k)- 1));
	}
}

inline bool MC_Compute::Condition_Remb(PnlMat * past, int time){
	bool condSortie = false;
	PnlVect * S0 = pnl_vect_create(past->m);
	PnlVect * S1 = pnl_vect_create(past->m);
	PnlVect * rent = pnl_vect_create(past->m);
	pnl_mat_get_col(S0, past, 0);
	pnl_mat_get_col(S1, past, time);
	RentVect(S1, S0, rent);
	for (int j = 0; j< rent->size; j++){
		if ( pnl_vect_get(rent, j) > -0.1 ) condSortie = true;
	}
	pnl_vect_free(&S0);
	pnl_vect_free(&S1);
	pnl_vect_free(&rent);
	return condSortie;
}
