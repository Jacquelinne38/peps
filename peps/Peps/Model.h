#pragma once
#include "pnl/pnl_random.h"


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
public : 

	Model(int nbPath);
	~Model();

	bool Model::CheckParameter();

	int Nb_Path() const { return m_nbPath; }
	void Nb_Path(int val) { m_nbPath = val; }
	/*
	*@param out path set de 0 à time avec les cours historiques et de time + 1 à PAS avec les valeurs simulées
	*/
	void Model::Diffuse_from_t(PnlMat * Path, const PnlVect *drift, Produit * produit, PnlRng * rng, int time);
};

