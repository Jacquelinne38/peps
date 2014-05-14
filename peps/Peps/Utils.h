#pragma once
#include "stdafx.h"
#include "Produit.h"
#define DLLEXP   __declspec( dllexport )



/* Calcul le nombre de jours/semaines entre la date actuelle (contenur dans lst_time)
* et la prochaine date de fixing
*/
static inline double Compute_dt(int date, DISCRETISATION_TYPE l_discretisation, std::vector<int> lst_time)
{
	for (int i = 0; i < lst_time.size() - 1; ++i) {
		if (date < lst_time[i+1])
			return (lst_time[i+1] - date);
	}
}

/* Affichage des résultats
* on peut afficher le delta du premier actif, le prix.
* Modifier dans parameter.h la valeur des bouléens pour permettre ou non l'affichage
*/ 
 static  void  print(double price, double squarePrice, const PnlVect * delta, const PnlVect * gamma, int nbPath) {
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



//USELESS
static PnlVect * Diffuse_cours_histo(double spot, double drift, double vol) {
	double dt = 1.0/52.0;
	PnlVect * Historique = pnl_vect_create(260);
	pnl_vect_set(Historique, 0, spot);
	PnlRng * rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed (rng, time(NULL));
	PnlVect * l_vecAlea = pnl_vect_create(260);
	pnl_vect_rng_normal(l_vecAlea, 260, rng);
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
	std::ofstream fichier(nomFichier, std::ios::out | std::ios::trunc);
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

/* Calcul de la volatilité à partir d'un vecteur
* fonction nécessaire au calcul de la volatilité historique
* @param X le vecteur dont on cherche la volatilité
*/
static double Compute_Volatility(const PnlVect * X)
{
	double lnS1 = 0;
	double lnS2 = 0;
	double diff = 0;
	double mean = 0;
	double vol = 0;
	PnlVect * R = pnl_vect_create_from_zero(X->size - 1);

	for (int i = 0 ; i < R->size; i++)
	{
		lnS1 = log(pnl_vect_get(X,i));
		lnS2 = log(pnl_vect_get(X,i+1));
		diff = lnS2 - lnS1;
		mean += diff;
		pnl_vect_set(R,i, diff);
	}
	mean /= X->size - 1;
	for (int j = 0 ; j < R->size; j++)
	{
		vol += pow((pnl_vect_get(R,j) - mean),2);
	}
	vol *= 1.0/((double)R->size-1.0);
	vol /= PAS;
	vol = sqrt(vol);
	pnl_vect_free(&R);
	return vol;
}

 /**
 * @param in vecteurs X et Y
 * @param out correlation entre les deux vecteurs
 * @Retourne un double : la correlation entre les deux vecteurs
 *
 */
  static double Correl(const PnlVect * X, const PnlVect * Y)
  {
	  double volX = Compute_Volatility(X);
	  double volY = Compute_Volatility(Y);
	  double meanX = 0;
	  double meanY = 0;
	  double multR = 0;
	  double retX = 0;
	  double retY = 0;
	  double ret = 0;

	  for (int i = 0; i< X->size - 1; i++)
	  {
		  retX = log(pnl_vect_get(X, i+1)) - log(pnl_vect_get(X, i));
		  retY = log(pnl_vect_get(Y,i+1)) - log(pnl_vect_get(Y,i));
		  meanX += retX;
		  meanY += retY;
		  multR += retX*retY;
		  	 
	  }
	  meanX /= X->size - 1;
	  meanY /= X->size - 1;
	  ret = (multR - X->size*meanX*meanY)/((X->size - 1)*volX*volY);
	  ret /= PAS; 
	  return ret;
  }

/**
* @param in la matrice historique. La valeur de chacun des actifs a chacune des dates
* @param out la matrice de correlation des actifs
* @Retourne la matrice de correlation
* @other etant donne que la matrice histo fait nxT il faut que MatCorr fasse nxn
*/
static void Compute_mat_Cor(const PnlMat* Histo, PnlMat* MatCorr)
{
	if (MatCorr->m != Histo->m || MatCorr->n != Histo->m)
	{
		// ICI lever une erreur
	}
	PnlVect* X = pnl_vect_create(Histo->m);
	PnlVect* Y = pnl_vect_create(Histo->m);
	for (int j = 0 ; j < Histo->m; j++)
	{
		for(int k = 0; k < Histo->m; k++)
		{
			pnl_mat_get_row(X, Histo, j);
			pnl_mat_get_row(Y, Histo, k);
			pnl_mat_set(MatCorr, j,k, Correl(X,Y));
		}
	}
	pnl_vect_free(&X);
	pnl_vect_free(&Y);
}

/* Calcul de la moyenne d'un vecteur
* @param Y
* le vecteur dont on veut calculer la moyenne
*/
static double Compute_Mean(const PnlVect * Y)
{
	double mean = 0;
	for(int i = 0; i < Y->size; i++)
	{
		mean += pnl_vect_get(Y,i);
	}
	return mean/Y->size;
}
