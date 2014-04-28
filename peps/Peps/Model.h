#pragma once
#include "pnl/pnl_random.h"
#include "Utils.h"


class Model
{


private :
	//Nombre de tour de la boucle de monte carlo
	int m_nbPath;
	/*
	*Diffuse les trajectoires pour chacun des actifs
	*/
	//void Model::Diffuse_from_t_all_Asset(Produit * produit,const PnlVect * drift, const PnlVect * vol, const PnlVect * vecAlea, const PnlMat * choleskyCor, PnlVect * spot);
	void Model::Diffuse_from_t_all_Asset(Produit * produit,const PnlVect * drift, const PnlVect * vecAlea, const PnlMat * choleskyCor, PnlVect * spot);
	void Model::Diffuse_of_dt(Produit * produit,const PnlVect * drift, const PnlVect * vecAlea, const PnlMat * choleskyCor, PnlVect * spot,const int dt);
	std::vector<int> Model::getFixingDateFromt(int time, std::vector<int> lst_time);
	void Model::setDiscretisation(DISCRETISATION_TYPE type);

	DISCRETISATION_TYPE m_discretisation;

	//DT ET NBSEMAINE SET BY DISCRETISATION in mc_compute constructor
	double m_DT;
	double  m_NBDISCRETISATION;
	int m_FINALDATE;

public : 

	Model(int nbPath);
	~Model();

	bool Model::CheckParameter();

	int Nb_Path() const { return m_nbPath; }
	void Nb_Path(int val) { m_nbPath = val; }
	/*
	*@param out path set de 0 à time avec les cours historiques et de time + 1 à PAS avec les valeurs simulées
	*/
	void Model::Diffuse_from_t(PnlMat * path, const PnlVect *drift, Produit * produit, PnlRng * rng, int time,  std::vector<int> lst_time, DISCRETISATION_TYPE l_discretisation);
	void Model::Diffuse_from_t(PnlMat * path, const PnlVect *drift, Produit * produit, PnlRng * rng, int time);

	/*
	* Calcule de la couverture risqué et non risqué
	*
	*/
	void Model::Simul_Market(
		std::vector<PnlVect *> &vec_delta, 
		std::vector<double> &vec_priceCouverture, 
		std::vector<double> &vec_actifs_risq, 
		std::vector<double> &vec_sans_risq,
		const PnlVect * delta, 
		const PnlVect* spot, 
		const int time);
	std::vector<int> mvec_fixingDate;
	DISCRETISATION_TYPE Discretisation() const { return m_discretisation; }
	double NBDISCRETISATION() const { return m_NBDISCRETISATION; }
	double DT() const { return m_DT; }
	int FINALDATE() const { return m_FINALDATE; }

};

