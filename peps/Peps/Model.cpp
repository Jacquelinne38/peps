#include "StdAfx.h"
#include "MC_Compute.h"
#include "Model.h"

void Model::Diffuse_from_t_all_Asset(Produit * produit,const PnlVect * drift, const PnlVect * vecAlea, const PnlMat * choleskyCor, PnlVect * spot) {
	PnlVect * tmp2;
	double tmp;
	double l_compo1 = 0;
	double l_compo2 = 0;
	tmp2 = pnl_mat_mult_vect(choleskyCor, produit->Volatility());
	//#pragma omp parallel for
	for (int j = 0; j < produit->getEquities().size(); ++j){ 
		l_compo1 = (pnl_vect_get(drift, j) - pow(produit->getEquities()[j].volatility,2)  / 2.0) * (double)(DT);		
		tmp = pnl_vect_get(tmp2, j) ;
		l_compo2 = tmp * pnl_vect_get(vecAlea, j) * sqrt((double)(DT));
		pnl_vect_set(spot, j, pnl_vect_get(spot, j) * exp(l_compo1 + l_compo2));
	}
	pnl_vect_free(&tmp2);
}
void Model::Diffuse_of_dt(Produit * produit,const PnlVect * drift, const PnlVect * vecAlea, const PnlMat * choleskyCor, PnlVect * spot,const int dt) {
	PnlVect * tmp2;
	double tmp;
	double l_compo1 = 0;
	double l_compo2 = 0;
	tmp2 = pnl_mat_mult_vect(choleskyCor, produit->Volatility());
	//#pragma omp parallel for
	for (int j = 0; j < produit->getEquities().size(); ++j){ 
		l_compo1 = (pnl_vect_get(drift, j) - pow(produit->getEquities()[j].volatility,2)  / 2.0) * (double)(dt*DT);		
		tmp = pnl_vect_get(tmp2, j) ;
		l_compo2 = tmp * pnl_vect_get(vecAlea, j) * sqrt((double)(dt*DT));
		pnl_vect_set(spot, j, pnl_vect_get(spot, j) * exp(l_compo1 + l_compo2));
	}
	pnl_vect_free(&tmp2);
}

/*
* Renvoi le nombre de date de fixing entre t et T
*/
std::vector<int> Model::getFixingDateFromt(int time, std::vector<int> lst_time) {
	std::vector<int> FixingDateFromT;
	FixingDateFromT.push_back(time);
	if(lst_time.size() < 1) return FixingDateFromT;
	for (int i = 0; i < lst_time.size(); ++i) {
		if (time < lst_time[i]) FixingDateFromT.push_back(lst_time[i]);
	}
	return FixingDateFromT;
}

void Model::Diffuse_from_t(PnlMat * path, const PnlVect *drift, Produit * produit, PnlRng * rng, int time,  std::vector<int> lst_time, DISCRETISATION_TYPE l_discretisation) {
	const int l_nbEq = produit->getEquities().size();
	PnlVect * l_spot = pnl_vect_create(l_nbEq);
	pnl_mat_get_col(l_spot, path, time);
	PnlVect * l_vecAlea = pnl_vect_create(l_nbEq);
	std::vector<int> fixingDateFromT = getFixingDateFromt(time, lst_time);
	//foreach boucle spot passe de t a t+dt et on inscrit St+dt dans une colone de path
	for (int k = 0; k < fixingDateFromT.size() - 1; k++){
		pnl_vect_rng_normal_d(l_vecAlea, l_nbEq, rng);
		int dt = Compute_dt(fixingDateFromT[k], l_discretisation, lst_time);
		//std::cout << dt << std::endl;
		Diffuse_of_dt(produit, drift, l_vecAlea, produit->MatCholCorr(), l_spot, dt);
		//spot contient la valeur en t+dt de tous les actifs, on met ces valeurs dans la matrice path
		pnl_mat_set_col(path, l_spot, fixingDateFromT[k+1]);
	//	pnl_vect_print(l_spot);
	}
//	std::cout << std::endl;
	pnl_vect_free(&l_vecAlea);
	pnl_vect_free(&l_spot);
}

void Model::Diffuse_from_t(PnlMat * path, const PnlVect *drift, Produit * produit, PnlRng * rng, int time) {
	const int l_nbEq = produit->getEquities().size();
	PnlVect * l_spot = pnl_vect_create(l_nbEq);
	pnl_mat_get_col(l_spot, path, time);
	PnlVect * l_vecAlea = pnl_vect_create(l_nbEq);
	//foreach boucle spot passe de t a t+dt et on inscrit St+dt dans une colone de path
	for (int k = time; k < PAS -1 ; k++){
		pnl_vect_rng_normal_d(l_vecAlea, l_nbEq, rng);
		Diffuse_from_t_all_Asset(produit, drift, l_vecAlea, produit->MatCholCorr(), l_spot);
		//spot contient la valeur en t+dt de tous les actifs, on met ces valeurs dans la matrice path
		pnl_mat_set_col(path, l_spot, k+1);
	}
	pnl_vect_free(&l_vecAlea);
	pnl_vect_free(&l_spot);
}

bool Model::CheckParameter() {
	bool tmp = true;
	if(!(m_nbPath > 1)) tmp = false;
	return tmp;
}


Model::Model(int nbPath)
{
	m_nbPath = nbPath;

}



Model::~Model()
{
}



