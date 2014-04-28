#pragma once
#include "stdafx.h"
#include "Produit.h"





static inline double Compute_dt(int date, DISCRETISATION_TYPE l_discretisation, std::vector<int> lst_time)
{
/*	if (l_discretisation = WEEK) {
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
	}*/
	for (int i = 0; i < lst_time.size() - 1; ++i) {
		if (date < lst_time[i+1])
			return (lst_time[i+1] - date);
	}
}


static void print(double price, double squarePrice, const PnlVect * delta, const PnlVect * gamma, int nbPath) {
	if(PRINTPRICE) {
		std::cout << "AFFICHAGE DES RESULTATS"<< std::endl;
		std::cout << "[*] Prix : " ;
		std::cout << price << std::endl;
		if (price > 1.23) {
			std::cout << "OUT" << std::endl;
		}
	}
	if (PRINTDELTA) {
		std::cout << "[*] Delta : " << std::endl;
		pnl_vect_print(delta);

	}
	if (PRINTGAMMA) {
		std::cout << "[*] Gamma : " << std::endl;
		pnl_vect_print(gamma);
	}
	if (PRINTINTERVAL) {
		squarePrice /= (nbPath);
		double trust = sqrt(squarePrice - price*price)  * 1.96 / pow(nbPath,0.5);
		std::cout << "[*] Intervalle de confiance : " ;
		std::cout << trust << std::endl;
	}
}

template < class ContainerT >
void tokenize(const std::string& str, ContainerT& tokens,
	const std::string& delimiters = " ", bool trimEmpty = false)
{
	std::string::size_type pos, lastPos = 0;
	while(true)
	{
		pos = str.find_first_of(delimiters, lastPos);
		if(pos == std::string::npos)
		{
			pos = str.length();

			if(pos != lastPos || !trimEmpty)
				tokens.push_back(ContainerT::value_type(str.data()+lastPos,
				(ContainerT::value_type::size_type)pos-lastPos ));

			break;
		}
		else
		{
			if(pos != lastPos || !trimEmpty)
				tokens.push_back(ContainerT::value_type(str.data()+lastPos,
				(ContainerT::value_type::size_type)pos-lastPos ));
		}

		lastPos = pos + 1;
	}
};



//USSLESS
static PnlVect * Diffuse_cours_histo(double spot, double drift, double vol) {
	double dt = 1.0/52.0;
	PnlVect * Historique = pnl_vect_create(260);
	pnl_vect_set(Historique, 0, spot);
	PnlRng * rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed (rng, time(NULL));
	PnlVect * l_vecAlea = pnl_vect_create(260);
	pnl_vect_rng_normal(l_vecAlea, 260, rng);
	//pnl_vect_print(l_vecAlea);
	for (int k = 1; k < 260; k++){		
		double l_compo1 = 0;
		double l_compo2 = 0;
		double tmp;
		l_compo1 = (drift - pow(vol,2)) * dt/2;
		l_compo2 = vol * dt * pnl_vect_get(l_vecAlea,k);
		tmp = pnl_vect_get(Historique,k-1) * exp(l_compo1 + l_compo2);
		pnl_vect_set(Historique, k, tmp);
	}
	return Historique;
}
template<typename T, typename A>
static void CreerFichierData(std::vector<T,A> vec, std::string nomFichier) {
	std::ofstream fichier(nomFichier, std::ios::out | std::ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
	if(fichier)
	{
		for(unsigned int i = 0; i < vec.size(); ++i) {
		
			fichier << vec[i] << std::endl;
		}
		fichier.close();
	}
	else
		std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;

}

static std::string CreateStrData(std::string delimiter, PnlVect * histo) {
	std::string tmp = "";
	int j = 0;
	for(int i = 0; i < histo->size; ++ i) {
		if(j == 100) j = 0;
		long double tmp1 = pnl_vect_get(histo, j);
		if (i == (histo->size -1)) {
			tmp += std::to_string(tmp1);
		} else {
			tmp += std::to_string(tmp1);
			tmp += delimiter;
		}
		++j;
	}
	return tmp;
}

static void CreationDataHisto(std::string nomFichier, double drift, Produit pro) {
	PnlVect * histo;
	std::vector<std::string> lst_data = std::vector<std::string>();
	for(int i = 0; i < 4; ++i) {
		histo = Diffuse_cours_histo(pro.getEquities()[i].value, drift, pro.getEquities()[i].volatility);
		lst_data.push_back(CreateStrData(",",histo));
	}
	CreerFichierData(lst_data, nomFichier);
}



//USSLESS NOW
void Diffuse(PnlMat * histoFixMT, PnlVect *finalSpotMT, const PnlVect *drift, const PnlVect * vol, Produit * produit, PnlRng * rng, int time) {
	double l_maturity = PAS-1;
	// le pas de temps
	double l_dt = 1;
	// le nombre d'interval de discretisation
	double l_nb_discretisation = l_maturity/l_dt;

	// A chacune de ces tours de boucle finalsoitMT passe de St a St+dt
	// et on inscrit St+dt dans une colone de histofix
	for (int k = time; k < l_nb_discretisation; k++){
		//vecteur aleatoire
		PnlVect * l_vecAlea = pnl_vect_create(produit->getEquities().size());
		pnl_vect_rng_normal_d(l_vecAlea, produit->getEquities().size(), rng);
		// Cholesky de la matrice de correlation
		PnlMat * l_CholeskyCor = pnl_mat_create(produit->getEquities().size(), produit->getEquities().size());
		l_CholeskyCor = pnl_mat_copy(produit->getMatCor());
		pnl_mat_chol(l_CholeskyCor);
		// on diffuse les trajectoires, boucle sur chacun des actifs du m_produit
		for (unsigned int j = 0; j < produit->getEquities().size(); j++){ 
			double l_compo1 = 0;
			double l_compo2 = 0;
			l_compo1 = pnl_vect_get(drift, j) - pow(produit->getEquities()[j].volatility,2) * l_dt / 2;

			PnlVect * tmp2 = pnl_mat_mult_vect(l_CholeskyCor, vol);
			double tmp = pnl_vect_get(tmp2, j) ;
			l_compo2 = tmp * pnl_vect_get(l_vecAlea, j) * sqrt(l_dt);
			pnl_vect_set(finalSpotMT, j, pnl_vect_get(finalSpotMT, j) * exp(l_compo1 + l_compo2));
			pnl_vect_free(&tmp2);
		}
		// normalement en sortie de boucle, finalspot contient la valeur en t+dt de tous les actifs
		pnl_mat_set_col(histoFixMT, finalSpotMT, k+1);
		pnl_vect_free(&l_vecAlea);
	}
	// ET donc en sortie on a l_histofixMT de rempli et finalspotMT egal a ST
}



			/*
			std::cout << std::endl;
			std::cout << "NOUVEAU COUPLE DE MATRICE" << std::endl;
			std::cout << "MATRICE SHIFTEE POSITIVEMENT" << std::endl;
			std::cout << std::endl;
			pnl_mat_print(l_pastShPos);
			std::cout << "MATRICE SHIFTEE NEGATIVEMENT" << std::endl;
			std::cout << std::endl;
			pnl_mat_print(l_pastShNeg);
			std::cout << std::endl;
			*/
		
			/*
			std::cout << "NOUVEAU COUPLE DE MATRICE" << std::endl;
			std::cout << "MATRICE SHIFTEE POSITIVEMENT" << std::endl;
			pnl_mat_print(l_histoFixShPos);
			std::cout << "MATRICE SHIFTEE NEGATIVEMENT" << std::endl;
			pnl_mat_print(l_histoFixShNeg);
			*/
	/*
			std::cout << "NOUVEAU COUPLE DE MATRICE" << std::endl;
			std::cout << "MATRICE SHIFTEE POSITIVEMENT" << std::endl;
			pnl_mat_print(l_rentPos);
			std::cout << "MATRICE SHIFTEE NEGATIVEMENT" << std::endl;
			pnl_mat_print(l_rentNeg);
			std::cout << std::endl << std::endl;
			*/

			/*
			std::cout << "nouveau couple de payoff" << std::endl;
			std::cout << ld_payoffPos << std::endl;
			std::cout << ld_payoffNeg << std::endl;
			*/