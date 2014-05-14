#pragma once
#include "Equity.h"
#include "Fx.h"
#include <pnl\pnl_matrix.h>
#include <pnl\pnl_matrix_double.h>
#include <vector>
#include <algorithm>
#define DLLEXP   __declspec( dllexport )
class DLLEXP Produit
{


private :
	// Déclaration de la liste d'actifs utiles
	std::vector<Equity>  ListEquit;
	//Déclaration de la liste de taux utiles
	std::vector<Fx>  ListFx;
	//MatCor = pnl_mat_create(4,5);	

	PnlMat *m_historique;
	PnlMat *m_matCor;
	PnlMat *m_matCholCorr;
	PnlVect *m_volatility;
	

	void Produit::LoadDataHisto();


public : 
	void UpDat_HistoFixing();
	void UpDate_ListEquit();
	void UpDate_listFx();

	void Fonction();
	
	Produit();
	Produit(PnlMat * assets, int nbActif, int nbDate);
	Produit::Produit(PnlMat * assets, PnlMat * corr, PnlVect * vol);

	~Produit();

	/* cette méthode retourne la liste des equities qui sont utilisées
	*/ 
	std::vector<Equity> getEquities()  { return ListEquit;  };
	
	/* Cette méthode retourne la matrice historique
	* c'est à dire la matrice qui contient pour chaque actif et chaque date la valeur observée sur le marché
	*/
	PnlMat * getMatHisto() const { return pnl_mat_copy(m_historique); }
	
	/* La méthode getMatCor calcul la matrice de corrélation
	* Cette matrice contient en (i,j) la corrélation entre l'actif i et j
	* elle est définie positibe et symétrique
	*/
	PnlMat * getMatCor() { return m_matCor; };
	
	/* Cette méthode retourne la volatilité des actifs
	* la volatilité retournée ici n'est pas la volatilité historique
	*/
	void SetInitVol();

	/* Méthode qui permet d'accéder à la décomposition de cholesky
	* de la matrice de corrélation, cette matrice est triangulaire inférieure
	*/
	PnlMat * MatCholCorr() const { return m_matCholCorr; }

	/* setter de la matrice de cholesky
	*/
	void MatCholCorr(PnlMat * val) { m_matCholCorr = val; }
	
	/* getter de la volatlité
	*/
	PnlVect * Volatility() const { return m_volatility; }
	
	/* setter de la volatilité
	*/
	void Volatility(PnlVect * val) { m_volatility = val; }
};
