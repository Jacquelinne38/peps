#pragma once
#include "pnl/pnl_random.h"


class Model
{


private :
	//Nombre de tour de la boucle de monte carlo
	int m_nbPath;

public : 

	Model(int nbPath);
	~Model();

	bool Model::CheckParameter();

	void Model::Diffuse(PnlMat * histoFixMT, PnlVect *finalSpotMT, const PnlVect *drift, const PnlVect * vol, Produit * produit, PnlRng * rng, int time);
	int Nb_Path() const { return m_nbPath; }
	void Nb_Path(int val) { m_nbPath = val; }
	PnlVect * Model::Diffuse_cours_histo(double strike, double drift, double vol);
	void Model::Diffuse_from_t(PnlMat * Path, const PnlVect *drift, const PnlVect * vol, Produit * produit, PnlRng * rng, int time);
};

