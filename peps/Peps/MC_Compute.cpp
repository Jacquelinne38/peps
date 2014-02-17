#include "StdAfx.h"
#include "MC_Compute.h"



MC_Compute::MC_Compute(Produit * produit, Model * model)
{
	m_discretisation = WEEK;
	m_produit = produit;
	m_sizeEquityProduct = produit->getEquities().size();
	m_model = model;
	m_rng = pnl_rng_create (PNL_RNG_MERSENNE);
	pnl_rng_sseed (m_rng, time(NULL));
}


//TODO 
// les dates sont mal gerees
// on ne gere pas les differents monnaies
// a quel moment doit on passer le spot en EUR
// comment g�rer la maturit� en actualisant
// faire attention a diffuser en CZK mais que la valeur des actifs soient stock�es avec leur monnaie respective
int MC_Compute::Price(double * sumPrice, double *priceSquare, PnlMat * sumDelta, PnlMat * sumGamma)
{
	if(!m_model->CheckParameter()) return -1;

	// Payoff prendra le payoff du m_produit a chaque tour de boucle
	double l_payoff = 0;
	//ATTRIBUT A METTRE DANS EQUITY
	// on devra avoir une m�thode getinitdrift
	// mais avant on doit creer la classe currency
	// chaque currency a un drift
	PnlVect *l_drift = pnl_vect_create_from_double(m_sizeEquityProduct, 0.05);
	PnlVect * l_spot = GetInitSpot();
	PnlVect * l_vol = GetInitVol();

	//MonteCarlo
	for (int i = 0; i < m_model->Nb_Path(); i++) {
		PnlMat *l_histoFixMT = pnl_mat_create(m_sizeEquityProduct, PAS);
		PnlVect *l_finalSpotMT =  pnl_vect_copy(l_spot);
		PnlMat *l_histoFix;
		pnl_mat_set_col(l_histoFixMT, l_finalSpotMT, 0);

		m_model->Diffuse(l_histoFixMT, l_finalSpotMT, l_drift, l_vol, m_produit, m_rng, 0);
		
		// A partir de histofix on calc le prix et le delta et le gamma
		l_histoFix = pnl_mat_copy(l_histoFixMT);
		PriceProduct(l_histoFix, &l_payoff);
		ComputeGrec(sumDelta, sumGamma, l_histoFix, l_payoff, l_vol, l_drift);

		//On somme les payoff
		*sumPrice += l_payoff;
		*priceSquare += pow(l_payoff, 2);

		pnl_mat_free(&l_histoFixMT);
		pnl_vect_free(&l_finalSpotMT);
		pnl_mat_free(&l_histoFix);
		
	}

	//Moyenne du prix
	*sumPrice /= m_model->Nb_Path();
	pnl_mat_div_double(sumDelta,  m_model->Nb_Path());
	pnl_mat_div_double(sumGamma,  m_model->Nb_Path());

	pnl_vect_free(&l_drift);
	pnl_vect_free(&l_spot);
	pnl_vect_free(&l_vol);

	return 0;
}

inline void MC_Compute::PriceProduct(const PnlMat * histoFix, double * payoff) {
	// Renta est la matrice des rentabilites autant de ligne que d'actifs et 4 colonnes car il y a 4 intervales de discretisation (5dates)
	PnlMat *l_renta = pnl_mat_create(m_sizeEquityProduct, PAS);
	// On calcul le prix du m_produit //Pour ca on a besoin de la rentabilite du portefeuille
	Rent(histoFix, l_renta);
	// On peut ensuite calculer son payoff //Ici la fonction price2 devrait retourner la date ou le payoff a ete touche
	*payoff = Price2(l_renta);
	pnl_mat_free(&l_renta);
}


inline void MC_Compute::ComputeGrec(PnlMat * sumDelta, PnlMat* sumGamma, const PnlMat * histoFix, const double payoff, PnlVect* l_vol, PnlVect* l_drift) {
	PnlMat *l_histoFixShPos;
	PnlMat *l_histoFixShNeg;
	// le delta est une matrice a autant de ligne que d'actifs et 5 colonnes car on observe 5 fois.
	//!\\ A modif
	PnlMat *l_gamma = pnl_mat_create(m_sizeEquityProduct,PAS);
	PnlMat *l_delta = pnl_mat_create(m_sizeEquityProduct,PAS);
	double ld_payoffPos = 0;
	double ld_payoffNeg = 0;

	for (int l = 0; l < m_sizeEquityProduct; l++) {
		//!\\\\\\\\\\\\\___ Virer ce 5
		for (int m = 0; m < PAS; m++) {
			//calcul de histofixshifte
			l_histoFixShPos = pnl_mat_copy(histoFix);
			l_histoFixShNeg = pnl_mat_copy(histoFix);
			
			// On shifte positivement et negativement
			MLET(l_histoFixShPos,l,m)=(MGET(histoFix,l,m))*1.05;
			MLET(l_histoFixShNeg,l,m)=(MGET(histoFix,l,m))*0.95;
			
			// Puis on diffuse a partir de m+1 sauf si m==5 auquel cas il n'y a rien a faire
			if (m != PAS ){
				PnlVect * spot = pnl_vect_create(m_sizeEquityProduct);
				pnl_mat_get_col(spot, l_histoFixShPos, m);
				m_model->Diffuse(l_histoFixShPos, spot, l_drift, l_vol, m_produit, m_rng, m+1);
				m_model->Diffuse(l_histoFixShNeg, spot, l_drift, l_vol, m_produit, m_rng, m+1);
				pnl_vect_free(&spot);
			}

			//calcul des rentabilites pour chacune des matrices de histofix shifte
			PnlMat *l_rentPos = pnl_mat_create(m_sizeEquityProduct, (histoFix->n - 1));
			PnlMat *l_rentNeg = pnl_mat_create(m_sizeEquityProduct, (histoFix->n - 1));
			Rent(l_histoFixShPos,l_rentPos);
			Rent(l_histoFixShNeg,l_rentNeg);


			// Calcul le payoff a partir de la matrice des rentabilites
			ld_payoffPos = Price2(l_rentPos);
			ld_payoffNeg = Price2(l_rentNeg);				

			// On modifie delta(l,m) et gamma(l,m)
			pnl_mat_set(l_delta, l, m, ((ld_payoffPos-ld_payoffNeg)/0.1));
			pnl_mat_set(l_gamma, l, m ,((ld_payoffPos - 2 * payoff + ld_payoffNeg)/(pow(0.05,2))));
			
			pnl_mat_free(&l_rentPos);
			pnl_mat_free(&l_rentNeg);
		}
	}
	// On somme les delta
	pnl_mat_plus_mat(sumDelta, l_delta);
	// On somme les gamma
	pnl_mat_plus_mat(sumGamma, l_gamma);

	//lib�ration ressource
	pnl_mat_free(&l_histoFixShPos);
	pnl_mat_free(&l_histoFixShNeg);
	pnl_mat_free(&l_gamma);
	pnl_mat_free(&l_delta);
}




MC_Compute::~MC_Compute()
{
}



//Toutes les fonctions suivantes doivent etre implementer et on doit quasiment resumer price() 
//a l appel de ces fonctions. (+ d'autres)
// cette fonction prend en argument historique de fixing
// elle ressort le payoff du m_produit
void MC_Compute::PayOff(void)
{
}



// cette fonction prend en argument le temps et ressort un dt
double MC_Compute::Compute_dt(int date)
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

// cette fonction prend en argument le temps et ressort la matrice de fixing
void MC_Compute::Compute_path(void)
{
}

// fonction qui prend en parametre le temps et une valeure et actualise la valeur a t0
inline double MC_Compute::Discount(double value, int date)
{
	//Pour le moment le taux sans risque est la c'est pas la qu'il devra etre
	double taux_ssR = 0.05;
	return (value*exp(-(date * taux_ssR)));
}



inline double MC_Compute::Price2(const PnlMat *rent)
{
	double minCol;
	double val;
	for (int i = 0; i < rent->n; i++)
	{
		minCol = 0;
		for (int j = 0; j < rent->m; j++)
		{
			if (pnl_mat_get(rent, j, i) < minCol)
			{
				minCol = pnl_mat_get(rent, j, i);
			}
		}
		if ((minCol > -0.1) && (i != 3))
		{
		    return Discount(REMB_ANTI, i);
		}
	}
	PnlVect *perf = pnl_vect_create(rent->m);
	pnl_mat_get_col(perf, rent, rent->n - 1);
	val = std::max(1.0, REMB_N_ANTI + Perf_Liss(perf));
	pnl_vect_free(&perf);
	return Discount(val, (rent->n -1));
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

// fonction qui prend en parametre la matrice de fixing
// et elle retourne la performance lissee sous forme de vecteur, 1 ligne par actif/taux
inline double MC_Compute::Perf_Liss(const PnlVect *spot)
{
	double val;
	double ret = 0;
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
	return ret/NB_ACTIFS;
}

inline void MC_Compute::Rent(const PnlMat *histoFix, PnlMat *res)
{
	double rent;
	for (int i = 0; i < histoFix->n - 1; i++)
	{
		for (int j = 0; j < NB_ACTIFS; j++)
		{
			rent = (pnl_mat_get(histoFix, j,  i + 1) / pnl_mat_get(histoFix, j, 0)) - 1;
			pnl_mat_set(res, j, i, rent);
		}
	}
}


PnlVect * MC_Compute::GetInitSpot() {
	PnlVect * l_spot = pnl_vect_create(m_sizeEquityProduct);
	for(int k = 0; k < m_sizeEquityProduct; k++){
		pnl_vect_set(l_spot, k, m_produit->getEquities()[k].value);
	}
	return l_spot;
}

PnlVect * MC_Compute::GetInitVol() {
	PnlVect *vol = pnl_vect_create(m_sizeEquityProduct);
	for(int k = 0; k < m_sizeEquityProduct; k++){
		pnl_vect_set(vol, k, m_produit->getEquities()[k].volatility);
	}
	return vol;	
}
