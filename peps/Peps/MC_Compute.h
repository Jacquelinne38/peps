#pragma once
#include "Produit.h"
#include "Model.h"
#include <pnl/pnl_random.h>

class MC_Compute
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
    inline void MC_Compute::ComputeGrec(PnlMat * sumDelta, PnlMat* sumGamma, const PnlMat * histoFix, const double payoff, PnlVect* l_vol, PnlVect* l_drift);


	/*
	* @param histoFix in
	* @param payoff out
	*/
	inline void MC_Compute::PriceProduct(const PnlMat * histoFix, double * payoff);
	inline double Price2(const PnlMat *rent);
	inline double Discount(double value, int date);
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

public :
	MC_Compute(Produit * produit, Model * model);
	~MC_Compute();

	/*
	* out price
	* out pricesquare
	* out Delta
	* out gamma
	*/
	int MC_Compute::Price(double * sumPrice, double *priceSquare, PnlMat * sumDelta, PnlMat * sumGamma);

	void payoff();
	void Compute_dt();
	void Compute_path();
	void PayOff();
	
};


