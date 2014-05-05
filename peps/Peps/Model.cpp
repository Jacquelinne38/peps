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
		l_compo1 = (pnl_vect_get(drift, j) - pow(produit->getEquities()[j].volatility,2)  / 2.0) * (double)(m_DT);		
		tmp = pnl_vect_get(tmp2, j) ;
		l_compo2 = tmp * pnl_vect_get(vecAlea, j) * sqrt((double)(m_DT));
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

    PnlVect * tmp3 = pnl_vect_create_from_zero(choleskyCor->m);
    double tmp4;
   
    //#pragma omp parallel for
    for (int j = 0; j < produit->getEquities().size(); ++j){
       
        // Premiere version
        /*
        l_compo1 = (pnl_vect_get(drift, j) - pow(produit->getEquities()[j].volatility,2)  / 2.0) * (double)(dt*m_DT);       
        tmp = pnl_vect_get(tmp2, j) ;
        l_compo2 = tmp * pnl_vect_get(vecAlea, j) * sqrt((double)(dt*m_DT));
        pnl_vect_set(spot, j, pnl_vect_get(spot, j) * exp(l_compo1 + l_compo2));
        */
       
        // deuxieme version (je pense que c est la bonne)
        // Pierre 28/4
        pnl_mat_get_row(tmp3,choleskyCor, j);
        tmp4 = pnl_vect_scalar_prod(tmp3, vecAlea);
        l_compo1 = (pnl_vect_get(drift, j) - pow(produit->getEquities()[j].volatility,2)  / 2.0) * (double)(dt*m_DT);   
        l_compo2 = sqrt((double)(dt*m_DT)) * produit->getEquities()[j].volatility * tmp4;
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
		std::cout << path->m << std::endl; 
		std::cout << path->n << std::endl; 
		std::cout << l_spot->size << std::endl; 
		std::cout << fixingDateFromT[k+1] << std::endl;
		pnl_mat_set_col(path, l_spot, fixingDateFromT[k+1]);

		//pnl_vect_print(l_spot);
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
	//TODO PAS doit être égal au nb que l'on a de date donc produit->nbDate histo size quoi
	for (int k = time; k < PAS -1 ; k++){
		pnl_vect_rng_normal_d(l_vecAlea, l_nbEq, rng);
		Diffuse_from_t_all_Asset(produit, drift, l_vecAlea, produit->MatCholCorr(), l_spot);
		//spot contient la valeur en t+dt de tous les actifs, on met ces valeurs dans la matrice path
		pnl_mat_set_col(path, l_spot, k+1);
	}
	pnl_vect_free(&l_vecAlea);
	pnl_vect_free(&l_spot);
}


void Model::Simul_Market(std::vector<PnlVect *> &vec_delta, 
						 std::vector<double> &vec_priceCouverture, 
						 std::vector<double> &vec_actifs_risq, 
						 std::vector<double> &vec_sans_risq,
						 const PnlVect * delta, 
						 const PnlVect* spot, 
						 const int time) {
	double price_couverture = 0;
	double actifs_risq = 0;
	double sans_risq = 0;
	if (time == 0){
		actifs_risq = pnl_vect_scalar_prod(delta, spot);
		sans_risq = 1.0 - pnl_vect_scalar_prod(delta, spot);
		price_couverture = actifs_risq + sans_risq; 
	} else {
		actifs_risq = pnl_vect_scalar_prod(delta, spot);
		sans_risq = vec_sans_risq[vec_sans_risq.size() -1] * exp(TAUX_ACTUALISATION * m_DT) - pnl_vect_scalar_prod(delta, spot) + pnl_vect_scalar_prod(vec_delta[vec_delta.size()-2], spot);
		price_couverture = actifs_risq + sans_risq;
	}
	if ( PRINTCOUVERTURE )
		std::cout << "Ris : " << actifs_risq << "   Sans : " << sans_risq << " Price : " << price_couverture << std::endl;
	vec_priceCouverture.push_back(price_couverture);
	vec_actifs_risq.push_back(actifs_risq);
	vec_sans_risq.push_back(sans_risq);
}

bool Model::CheckParameter() {
	bool tmp = true;
	if(!(m_nbPath > 1)) tmp = false;
	return tmp;
}


Model::Model(int nbPath)
{
	setDiscretisation(DISCRETISATION);
	m_nbPath = nbPath;

}

void Model::setDiscretisation(DISCRETISATION_TYPE type) {
	m_discretisation = type;
	if(type == WEEK) {
		m_DT = 1.0/52.0;
		m_NBDISCRETISATION = 52.0;
		m_FINALDATE = 260;
		static const int arr[] = {FIXING0, FIXING1, FIXING2, FIXING3, FIXING4};
		std::vector<int> lvec_fixingDate (arr, arr + sizeof(arr) / sizeof(arr[0]) );
		mvec_fixingDate = lvec_fixingDate;
	}
	else if (type == DAY) {
		m_DT = 1.0/260.0;
		m_NBDISCRETISATION = 260.0;
		m_FINALDATE = 260*4;
		static const int arr[] = {FIXING0*7, FIXING1*7, FIXING2*7, FIXING3*7, FIXING4*7};
		std::vector<int> lvec_fixingDate (arr, arr + sizeof(arr) / sizeof(arr[0]) );
		mvec_fixingDate = lvec_fixingDate;
	}
	else {
		m_DT = -1;
		m_NBDISCRETISATION = -1;
		std::cout << "[*] ERROR DISCRETISATION TYPE NOT CORRECTLY SET" << std::endl;
	}
}




Model::~Model()
{
}



