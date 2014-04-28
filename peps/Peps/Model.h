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

};

