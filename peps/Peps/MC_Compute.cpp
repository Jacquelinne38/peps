#include "StdAfx.h"
#include "MC_Compute.h"
#include "test.h"


MC_Compute::MC_Compute(Produit * produit, Model * model)
{
	m_produit = produit;

	//Sert à simplifier la lecture du code. 
	m_sizeEquityProduct = produit->getEquities().size();
	m_model = model;
	_timer = Pesp_Timer();
	//_timer.Start();

	m_rng = pnl_rng_create (PNL_RNG_MERSENNE);
	pnl_rng_sseed (m_rng, time(NULL));

	//Vecteur de date de fixing

}


bool MC_Compute::isRemb(const PnlMat * coursHisto, int time) {
	bool tmp = false;
	if(time > m_model->mvec_fixingDate[1]){
		if(Condition_Remb(coursHisto, m_model->mvec_fixingDate[1])){
			std::cout<< "LE PAYOFF A DEJA ETE TOUCHE EN t1"<<std::endl;
			return true;
		}
		if (time > m_model->mvec_fixingDate[2]){
			if(Condition_Remb(coursHisto, m_model->mvec_fixingDate[2])){
				std::cout<< "LE PAYOFF A DEJA ETE TOUCHE EN t2"<<std::endl;
				return true;
			}
			if (time > m_model->mvec_fixingDate[3]){
				if(Condition_Remb(coursHisto, m_model->mvec_fixingDate[3])){
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
	double l_payoff = 0;
	PnlMat * l_coursHisto = m_produit->getMatHisto();
	PnlVect *l_drift = pnl_vect_create_from_double(m_sizeEquityProduct, DRIFT);
	PnlMat * l_past = pnl_mat_create(l_coursHisto->m, l_coursHisto->n);
	PnlVect * tmpDelta = pnl_vect_create(m_sizeEquityProduct);
	pnl_vect_set_double(tmpDelta, 0.0);
	pnl_mat_set_zero(l_past);

	//check model parameter
	if(!m_model->CheckParameter()) return -1;
	if(isRemb(l_coursHisto, time))
		return -10;
	TronqCoursHisto(l_coursHisto, l_past, time);

	pnl_vect_set_zero(sumDelta);
	pnl_vect_set_zero(sumGamma);

	for (int i = 0; i < m_model->Nb_Path(); i++) {
		//_timer.Start();
		//!!! changer le model connais mvcec et dis
		m_model->Diffuse_from_t(l_past, l_drift, m_produit, m_rng, time, m_model->mvec_fixingDate, m_model->Discretisation());
		//_timer.GetTime("Diffuse");
		//Pour calculer le prix nous avons besoins des valeurs des sous jacents qu'au date de fixing getPathFix retourne les valeurs des sous jacents aux dates de fixing
		getPathFix(l_past, l_histoFix, m_model->mvec_fixingDate);
		//pnl_mat_print(l_past);

		//_timer.GetTime("getPathFix");
		
	//	_timer.Start();
		ComputeGrec(sumDelta, sumGamma, l_histoFix, l_past, l_payoff, l_drift, time);
		//pnl_vect_plus_vect(sumDelta, tmpDelta);
		PriceProduct(l_histoFix, &l_payoff, time);
		//_timer.Stop();
		//_timer.GetTime("Compute grec");
		*sumPrice += l_payoff;
	}

	//_timer.GetTime("Durée compute grec");
	//Moyenne du prix du delta et du gamma
	*sumPrice /= m_model->Nb_Path();

	
	pnl_vect_div_double(sumDelta,  m_model->Nb_Path());
	//pnl_vect_div_double(sumGamma,  m_model->Nb_Path());

	pnl_vect_free(&l_drift);
	pnl_mat_free(&l_past);
	pnl_mat_free(&l_coursHisto);

	return 0;
}

inline void MC_Compute::PriceProduct(const PnlMat * histoFix, double * payoff, int time) {
	PnlMat *l_renta = pnl_mat_create(m_sizeEquityProduct, m_model->mvec_fixingDate.size() - 1);
	RentFromMat(histoFix, l_renta);
	*payoff = DiscountedPayoff(l_renta, time);
	pnl_mat_free(&l_renta);
}


inline void MC_Compute::ComputeGrec(PnlVect * sumDelta, PnlVect* sumGamma, const PnlMat * pathTmp, const PnlMat* past, const double payoff, PnlVect* l_drift, int time) {
	
	PnlMat *pathFix = pnl_mat_copy(pathTmp);
	PnlMat *l_rentPos = pnl_mat_create(m_sizeEquityProduct, m_model->mvec_fixingDate.size() -1);
	double ld_payoffPos = 0;
	double ld_payoffNeg = 0;
	int li_dateFixing =  ComputeDateFix(time);
	//std::cout << "date fixing : " << li_dateFixing << std::endl;
	for (int i = 0; i < m_sizeEquityProduct; i++) {
			//Positif
		//pnl_mat_print(pathFix);	

			for (int j = li_dateFixing; j < pathFix->n; j++)
			{
				//std::cout << "avant " << MGET(pathFix,i,j) << std::endl;
				double tmppo = (pnl_mat_get(pathFix,i,j))*(1.02);
				MLET(pathFix,i,j) = tmppo;
				//std::cout << "après " << MGET(pathFix,i,j) << std::endl;
			}
			//pnl_mat_print(pathFix);
		
			//A voir si on peu optimiser en prennant juste l'actif et non toute la matrice
			//car ici on calcule seulement pour un actif or on boucle sur tous
			RentFromMat(pathFix,l_rentPos);
			
			//pnl_mat_print(pathFix);
			if(i == 1) {
				/*std::cout << "Positif " << std::endl;
				pnl_mat_print(l_rentPos);
			*/
			}
			// Calcul le payoff a partir de la matrice des rentabilites
			ld_payoffPos = DiscountedPayoff(l_rentPos, time);
			//Negatif
			for (int j = li_dateFixing; j < pathFix->n; j++)
			{
				double tmpne = pnl_mat_get(pathFix,i,j)*(0.98)/(1.02);
				MLET(pathFix,i,j) =tmpne;
			}
			RentFromMat(pathFix,l_rentPos);
			ld_payoffNeg = DiscountedPayoff(l_rentPos, time);	
			pnl_vect_set(sumDelta, i, GET(sumDelta, i)+((ld_payoffPos-ld_payoffNeg)/(2*H*pnl_mat_get(past, i, time))));
			//pnl_vect_set(sumGamma, l ,GET(sumGamma, i)+((ld_payoffPos - 2 * payoff + ld_payoffNeg)/(pow(H,2))));
			//std::cout << "pos : " << ld_payoffPos << " neg :" << ld_payoffNeg << std::endl;
			if(i == 1) {
 			/*	std::cout << "Negatif " << std::endl;
 				pnl_mat_print(l_rentPos);*/

			}
			for (int j = ComputeDateFix(time); j < pathFix->n; j++)
			{
				MLET(pathFix,i,j) = (MGET(pathFix,i,j))/(1-H);
			}
					
	}
	//libération ressource
	pnl_mat_free(&pathFix);
	pnl_mat_free(&l_rentPos);
}

MC_Compute::~MC_Compute()
{
}




inline int MC_Compute::ComputeDateFix(int time)
{
	if (m_model->Discretisation() == WEEK) {
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
		if (time < 104*5)
		{
			return 1;
		} else if (time < 156*5)
		{
			return 2;
		} else if (time < 208*5)
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
	return (value*exp(-((date-time)/m_model->NBDISCRETISATION() * TAUX_ACTUALISATION)));
}

bool MC_Compute::CheckIfRemboursementAnticipe(const PnlMat * rent, int time, double * valueRemboursement) {
	PnlVect *tmp = pnl_vect_create(rent->n);
	for (int i = 0; i < rent->n -1; i++)
	{	
		pnl_mat_get_col(tmp, rent, i);
		//pnl_mat_print(rent);
		//std::cout << std::endl;
		if ((pnl_vect_min(tmp) > -0.1))
		{
			// Si on entre ici c'est qu'il y a un remboursement anticipé
			// mvec_fixingDate[i+1] est la date à laquelle le flux est touchée
			// time est la date à laquelle on calcul le prix
			//std::cout << "remboursement anticipé en "<< mvec_fixingDate[i+1] << std::endl;
			*valueRemboursement = Discount(REMB_ANTI, m_model->mvec_fixingDate[i+1], time);
			pnl_vect_free(&tmp);
			return true;
		}
	}
	pnl_vect_free(&tmp);
	return false;
}

inline bool MC_Compute::Condition_Remb(const PnlMat * past, int time){
	bool condSortie = false;
	PnlVect * S0 = pnl_vect_create(past->m);
	PnlVect * S1 = pnl_vect_create(past->m);
	PnlVect * rent = pnl_vect_create(past->m);
	pnl_mat_get_col(S0, past, 0);
	pnl_mat_get_col(S1, past, time);
	RentVect(S1, S0, rent);
	//for (int j = 0; j< rent->size; j++){

	if ( pnl_vect_min(rent) > -0.1 ) condSortie = true;
	//}
	pnl_vect_free(&S0);
	pnl_vect_free(&S1);
	pnl_vect_free(&rent);
	return condSortie;
}


double MC_Compute::DiscountPayoffFromMaturity(const PnlMat *rent, int time) {
	PnlVect *perf = pnl_vect_create(rent->m);
	pnl_mat_get_col(perf, rent, rent->n - 1);
	double discounted = std::max(1.0, REMB_N_ANTI + Perf_Liss(perf));
	pnl_vect_free(&perf);
	return Discount(discounted, m_model->mvec_fixingDate[m_model->mvec_fixingDate.size() - 1], time);
}

inline double MC_Compute::DiscountedPayoff(const PnlMat *rent, int time)
{
	double discounted = 0;
	if(CheckIfRemboursementAnticipe(rent, time, &discounted))
		return discounted;
	return DiscountPayoffFromMaturity(rent, time);
}

inline void MC_Compute::Perf_Boost(const PnlVect *perf, PnlVect * ret)
{
	double val = 0.0;
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
	double val = 0.0;
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

inline void MC_Compute::RentFromMat(const PnlMat *mat, PnlMat *res) {
	for(int i = 0 ; i < mat->m ; ++i) {
		for(int j = 1; j < mat->n; ++j) {
			double rent = (pnl_mat_get(mat, i, j) / pnl_mat_get(mat, i, 0))-1;
			pnl_mat_set(res, i, j-1, rent);
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
//inline void MC_Compute::RentFromMat(const PnlMat *mat, PnlMat *res)
//{
//	TODO Quand on aura 20 actifs vérifié si ce n'est pas intéressant de parallèliser pour l'instant non
//	#pragma omp parallel for
//	for (int i = 0; i < mat->n - 1; i++)
//	{
//		for (int j = 0; j < mat->m; j++) {
//			pnl_mat_set(res, j, i, (MGET(mat, j,  i + 1) / MGET(mat, j, 0)) - 1);
//		}
//	}
//	std::cout << "Rentabilité" << std::endl;
//	pnl_mat_print(res);
//}
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

