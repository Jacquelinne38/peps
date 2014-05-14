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
	// D�claration de la liste d'actifs utiles
	std::vector<Equity>  ListEquit;
	//D�claration de la liste de taux utiles
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

	/* cette m�thode retourne la liste des equities qui sont utilis�es
	*/ 
	std::vector<Equity> getEquities()  { return ListEquit;  };
	
	/* Cette m�thode retourne la matrice historique
	* c'est � dire la matrice qui contient pour chaque actif et chaque date la valeur observ�e sur le march�
	*/
	PnlMat * getMatHisto() const { return pnl_mat_copy(m_historique); }
	
	/* La m�thode getMatCor calcul la matrice de corr�lation
	* Cette matrice contient en (i,j) la corr�lation entre l'actif i et j
	* elle est d�finie positibe et sym�trique
	*/
	PnlMat * getMatCor() { return m_matCor; };
	
	/* Cette m�thode retourne la volatilit� des actifs
	* la volatilit� retourn�e ici n'est pas la volatilit� historique
	*/
	void SetInitVol();

	/* M�thode qui permet d'acc�der � la d�composition de cholesky
	* de la matrice de corr�lation, cette matrice est triangulaire inf�rieure
	*/
	PnlMat * MatCholCorr() const { return m_matCholCorr; }

	/* setter de la matrice de cholesky
	*/
	void MatCholCorr(PnlMat * val) { m_matCholCorr = val; }
	
	/* getter de la volatlit�
	*/
	PnlVect * Volatility() const { return m_volatility; }
	
	/* setter de la volatilit�
	*/
	void Volatility(PnlVect * val) { m_volatility = val; }
};
