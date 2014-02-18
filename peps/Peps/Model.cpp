#include "StdAfx.h"
#include "MC_Compute.h"
#include "Model.h"

 void Model::Diffuse(PnlMat * histoFixMT, PnlVect *finalSpotMT, const PnlVect *drift, const PnlVect * vol, Produit * produit, PnlRng * rng, int time) {
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



bool Model::CheckParameter() {
	bool tmp = true;
	if(!(m_nbPath > 1)) tmp = false;
	return tmp;
}


Model::~Model()
{
}

PnlVect * Model::Diffuse_cours_histo(double spot, double drift, double vol) {
	double dt = 1;
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


void Model::Diffuse_from_t(PnlMat * path, const PnlVect *drift, const PnlVect * vol, Produit * produit, PnlRng * rng, int time) {

	// A changer
	double l_dt = 1.0/52.0; 
	
	// le vecteur spot
	PnlVect * spot = pnl_vect_create(produit->getEquities().size());
	pnl_mat_get_col(spot, path, time);
	
	// A chacune de ces tours de boucle spot passe de t a t+dt et on inscrit St+dt dans une colone de Path
	for (int k = time; k < PAS -1 ; k++){
		
		//vecteur aleatoire
		PnlVect * l_vecAlea = pnl_vect_create(produit->getEquities().size());
		pnl_vect_rng_normal_d(l_vecAlea, produit->getEquities().size(), rng);
	
		// Cholesky de la matrice de correlation
		PnlMat * l_choleskyCor = pnl_mat_create(produit->getEquities().size(), produit->getEquities().size());
		l_choleskyCor = pnl_mat_copy(produit->getMatCor());
		pnl_mat_chol(l_choleskyCor);
		
		// on diffuse les trajectoires, boucle sur chacun des actifs du m_produit
		for (unsigned int j = 0; j < produit->getEquities().size(); ++j){ 
			double l_compo1 = 0;
			double l_compo2 = 0;
			l_compo1 = (pnl_vect_get(drift, j) - pow(produit->getEquities()[j].volatility,2)  / 2.0) * l_dt;
			PnlVect * tmp2 = pnl_mat_mult_vect(l_choleskyCor, vol);
			double tmp = pnl_vect_get(tmp2, j) ;
			l_compo2 = tmp * pnl_vect_get(l_vecAlea, j) * sqrt(l_dt);
			pnl_vect_set(spot, j, pnl_vect_get(spot, j) * exp(l_compo1 + l_compo2));
			pnl_vect_free(&tmp2);
		}
		// normalement en sortie de boucle, spot contient la valeur en t+dt de tous les actifs
		// on met ces valeurs dans la matrice path
		pnl_mat_set_col(path, spot, k+1);
		pnl_vect_free(&l_vecAlea);
		pnl_mat_free(&l_choleskyCor);
	}
	pnl_vect_free(&spot);
	pnl_mat_print(path);
	// ET donc en sortie on a path de rempli
	// de 0 a time avec les cours historiques et de time + 1 a PAS avec les valeurs simulees
}

Model::Model(int nbPath)
{
	m_nbPath = nbPath;

}
