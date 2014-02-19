#pragma once
#include "Produit.h"
#include "Model.h"
#include <pnl/pnl_random.h>

class MC_Compute
{
private:

	enum DISCRETISATION_TYPE { DAY, WEEK };

	double T;
	double K;
	double m_spot;
	double drift;
	double vol;
	PnlRng * m_rng;
	Produit * m_produit;
	Model * m_model;
	int m_sizeEquityProduct;
	DISCRETISATION_TYPE m_discretisation;
	/*
	* Déclaration du vecteur spot
	* return vecteur des prix spot
	*/
	PnlVect * MC_Compute::GetInitSpot();
	/*
	* Déclaration du vecteur volatilité
	* return vecteur volatitlié
	*/
	PnlVect * MC_Compute::GetInitVol();
	/*
	*	Check if all paramater if define don't start price if not
	*/
	bool MC_Compute::CheckParameter(); 
	/*
	* @param out matrice des delta
	* @param out matrice des gamma
	* @param in matricte histofix
	* @param in payoff
	*/
    inline void MC_Compute::ComputeGrec(PnlVect * sumDelta, PnlVect* sumGamma, const PnlMat * histoFix, const double payoff, PnlVect* l_vol, PnlVect* l_drift, int time);


	/*
	* @param histoFix in
	* @param payoff out
	*/
	inline void MC_Compute::PriceProduct(const PnlMat * histoFix, double * payoff, int time);
	inline double Price2(const PnlMat *rent, int time);
	/*
	* @param in temps
	* @pram in value : valeur a actualiser à t0
	*/
	inline double Discount(double value, int date, int time);
	/*
	* @param in  matrice de fixing
	* return : performance lissée sous forme de vecteur, 1 ligne par actif/taux
	*/ 
	inline double Perf_Liss(const PnlVect *spot);
	/*
	in
	out
	*/
	inline void MC_Compute::Perf_Boost(const PnlVect *perf, PnlVect * ret);
	//Fonction qui calcule les rentabilités à chaque date de constatation
	inline void Rent(const PnlMat *histoFix, PnlMat *res);
	/*double MC_Compute::Diffusion(const double spot, const double drift, const double dt, const double nbAlea, const PnlMat *matCor, const  Produit produit, const int equity);
	double MC_Compute::ComputeNextStep(const double drift, 
								   const double volatility, 
								   const double dt, 
								   const double nbAlea,
								   const PnlMat *matCor);*/

	/*
	* Renvoi true si il y a eu lieu a un remboursement avant la date t passé en paramètre
	*/
	bool MC_Compute::isRemb(PnlMat * coursHisto, int time);

	/*@param in v
	* @pram in v0
	* @pram out res
	*/
	void MC_Compute::RentVect(PnlVect * V, PnlVect * V0, PnlVect * res);
	/*
	* @param in temps
	* @return out dt
	*/
	double Compute_dt(int date);

public :
	MC_Compute(Produit * produit, Model * model);
	~MC_Compute();

	std::vector<int> mvec_fixingDate;

	/*
	* out price
	* out pricesquare
	* out Delta
	* out gamma
	* renvoi -10 si la t trop grand par rapport au remboursement
	*/
	int MC_Compute::Price(double * sumPrice, double *priceSquare, PnlVect * sumDelta, PnlVect * sumGamma, int time); 
	inline bool MC_Compute::Condition_Remb(PnlMat * past, int time);
	void payoff();

};


