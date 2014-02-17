#pragma once
#include "Equity.h"
#include "Fx.h"
#include <pnl\pnl_matrix.h>
#include <pnl\pnl_matrix_double.h>
#include <vector>
#include <algorithm>

class Produit
{


private :
	// Déclaration de la liste d'actifs utiles
	std::vector<Equity>  ListEquit;
	//Déclaration de la liste de taux utiles
	std::vector<Fx>  ListFx;
	//MatCor = pnl_mat_create(4,5);	

	//Déclaration de la matrice de covariance
	PnlMat *m_matCor;

	//Manque la déclaration de l'histofixing
	// matrice de 5 colonnes(t0, t1, t2, t3 et T) et de (listEquity.length + listFx.length)
public : 
	// toutes ces fonctions prennent le temps en parametre
	void UpDat_HistoFixing();
	void UpDate_ListEquit();
	void UpDate_listFx();

	void Fonction();
	
	Produit();
	~Produit();

	std::vector<Equity> getEquities()  { return ListEquit;  };
	PnlMat * getMatCor() { return m_matCor; };
};
