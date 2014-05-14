#pragma once
#include "Produit.h"
#include "Model.h"
#include <pnl/pnl_random.h>

#define DLLEXP   __declspec( dllexport )

class DLLEXP MC_Compute
{
private:


	double T;
	double K;
	double m_spot;
	double drift;
	double vol;
	PnlRng * m_rng;
	Produit * m_produit;
	Model * m_model;
	int m_sizeEquityProduct;
	Pesp_Timer _timer;

	/*
	* D�claration du vecteur spot
	* return vecteur des prix spot
	*/
	PnlVect * MC_Compute::GetInitSpot();
	
	/*
	* D�claration du vecteur volatilit�
	* return vecteur volatitli�
	*/
	PnlVect * MC_Compute::GetInitVol();
	
	/*
	*	V�rifie que tous les param�tres ont �t� d�fini
	*   Dans le cas contraire, Price ne peut �tre ex�cut�e
	*/
	bool MC_Compute::CheckParameter(); 
	
	/*
	* Permet de calculer les deltas du produit
	* @param out matrice des delta
	* @param out matrice des gamma
	* @param in matricte histofix
	* @param in payoff
	*/
    inline void MC_Compute::ComputeGrec(PnlVect * sumDelta, PnlVect* sumGamma, const PnlMat * pathTmp, const PnlMat* past, const double payoff, PnlVect* l_drift, int time);

	/*
	* Price le produit KB AKCENT au temps time
	* @param histoFix in : la valeur de chacun des actifs aux dates de fixing
	* @param payoff out
	*/
	inline void MC_Compute::PriceProduct(const PnlMat * histoFix, double * payoff, int time);
	
	/*
	* Renvoie le payoff actualis� au temps time, a partir de la matrice des rentabilit�s
	*/
	inline double DiscountedPayoff(const PnlMat *rent, int time);
	
	/*
	* Renvoie la valeur de value actualis�e entre date et time
	* @param in temps
	* @pram in value : valeur a actualiser � t0
	*/
	inline double Discount(double value, int date, int time);
	
	/*
	* Renvoie la performance liss�e du panier d'actions 
	* d�finie dans la brochure
	* @param in  matrice de fixing
	* return : performance liss�e sous forme de vecteur, 1 ligne par actif/taux
	*/ 
	inline double Perf_Liss(const PnlVect *spot);
	
	/*
	* Renvoie la perfomrance boost�e du panier d'actions
	*/
	inline void MC_Compute::Perf_Boost(const PnlVect *perf, PnlVect * ret);

	/*
	* Calcule les rentabilit�s � chaque date de constatation
	*@param in : Matrice contenant les cours des actions aux dates de fixing du produit
	*@param out : Matrice des rentabilit�s
	*/
	inline void RentFromMat(const PnlMat *histoFix, PnlMat *res);

	/*
	* Calcule les rentabilit�s d'un vecteur
	*@param in : vecteur des cours d'une action du panier
	*@param out : vecteur des rentabilit�s
	*/
	inline void MC_Compute::RentFromVect(const PnlVect *vect, PnlVect *res);

	/*
	* Calcule les rentabilit�s � la date de fixing time de la matrice de fixing
	*@param in : vecteurs des cours des actions
	*@param out : vecteur des rentabilit�s
	*@param in : date de fixing
	*/
	inline void MC_Compute::RentFromVectHisto(const PnlVect *vect, PnlVect *res, int time);

	/*
	* Calcule le payoff actualis� du produit de la maturit� � la date time
	*@param in : Matrice des rentabilit�s
	*@param in : temps auquel on souhaite actualiser le produit
	* return : payoff actualis� � la date time
	*/
	double MC_Compute::DiscountPayoffFromMaturity(const PnlMat *rent, int time);

	/*
	* V�rifie qu'il y a eu un remboursement anticip�
	*@param in : matrice des rentabilit�s des dates de fixing
	*@param in : temps auquel on souhaite v�rifi� un remboursement anticip�
	*@param out : montant rembours�
	* return : true si il y a eu remboursement anticip�
	*/
	bool MC_Compute::CheckIfRemboursementAnticipe(const PnlMat * rent, int time, double * valueRemboursement);

	/*
	* V�rifie qu'il y a eu remboursement anticip� avant la date time
	*@param in : matrice des cours historique
	*@param in : date
	* return : Renvoie true si un remboursement anticip� a eu lieu avant la date time
	*/
	bool MC_Compute::isRemb(const PnlMat * coursHisto, int time);

	/*
	* Calcule la rentabilit� du vecteur V par rapport aux cours au temps t = 0
	* @param in : vecteur des cours
	* @pram in : vecteur des cours au temps t = 0
	* @pram out : vecteur des rentabilit�s
	*/
	void MC_Compute::RentVect(PnlVect * V, PnlVect * V0, PnlVect * res);
	
	/*
	* Calcule le nombre de jours/semaines qu'il y a entre date et la prochaine date de fixing
	* @param in : date actuelle
	* @return : nombre de jours ou de semaines par rapport � la prochaine date de fixing
	*/
	double Compute_dt(int date);

	/*
	*@patram in : date actuelle
	* return : prochaine date de fixing
	*/
	int ComputeDateFix(int date);

	

public :
	MC_Compute(Produit * produit, Model * model);
	~MC_Compute();



	/*
	* Price le produit KB AKCENT au temps time
	*@param out : prix au temps time
	*@param out : prix au carr� au temps time
	*@param out : vecteur de deltas
	*@param out : vecteur des gammas
	*@param in : matrice des cours des actions du panier
	*@param in : date � laquelle on souhaite conna�tre le prix du produit
	* return : si valeur de retour �gale � z�ro il n'y a pas eu de probl�mes pour calculer le prix du produit
	*/
	int MC_Compute::Price(double * sumPrice, double *priceSquare, PnlVect * sumDelta, PnlVect * sumGamma, PnlMat * l_hisotFix, int time); 
	inline bool MC_Compute::Condition_Remb(const PnlMat * past, int time);
	void payoff();
	Pesp_Timer Timer() const { return _timer; }
	void Timer(Pesp_Timer val) { _timer = val; }

};


