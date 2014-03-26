#include "StdAfx.h"
#include "MC_Compute.h"
#include "Model.h"


void Model::Diffuse_from_t_all_Asset(Produit * produit,const PnlVect * drift, const PnlVect * vol, const PnlVect * vecAlea, const PnlMat * choleskyCor, PnlVect * spot) {
	for (unsigned int j = 0; j < produit->getEquities().size(); ++j){ 
		double l_compo1 = 0;
		double l_compo2 = 0;
		l_compo1 = (pnl_vect_get(drift, j) - pow(produit->getEquities()[j].volatility,2)  / 2.0) * (double)(DT);
		PnlVect * tmp2 = pnl_mat_mult_vect(choleskyCor, vol);
		double tmp = pnl_vect_get(tmp2, j) ;
		l_compo2 = tmp * pnl_vect_get(vecAlea, j) * sqrt((double)(DT));
		pnl_vect_set(spot, j, pnl_vect_get(spot, j) * exp(l_compo1 + l_compo2));
		pnl_vect_free(&tmp2);
	}
}

void Model::Diffuse_from_t(PnlMat * path, const PnlVect *drift, const PnlVect * vol, Produit * produit, PnlRng * rng, int time) {
	const int l_nbEq = produit->getEquities().size();
	PnlVect * l_spot = pnl_vect_create(l_nbEq);
	pnl_mat_get_col(l_spot, path, time);
	
	//foreach boucle spot passe de t a t+dt et on inscrit St+dt dans une colone de path
	for (int k = time; k < PAS -1 ; k++){
		PnlVect * l_vecAlea = pnl_vect_create(l_nbEq);
		pnl_vect_rng_normal_d(l_vecAlea, l_nbEq, rng);
	
		// Ca ca doit etre en dur dans parameters a priori
		// Cholesky de la matrice de correlation
		PnlMat * l_choleskyCor = pnl_mat_copy(produit->getMatCor());
		pnl_mat_chol(l_choleskyCor);
		
		Diffuse_from_t_all_Asset(produit, drift, vol, l_vecAlea, l_choleskyCor, l_spot);
		
		//spot contient la valeur en t+dt de tous les actifs, on met ces valeurs dans la matrice path
		pnl_mat_set_col(path, l_spot, k+1);
		pnl_vect_free(&l_vecAlea);
		pnl_mat_free(&l_choleskyCor);
	}
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



