// Peps.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"


#include <iostream>
#include "MC_Compute.h"
#include "Produit.h"
#include "Model.h"


#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <omp.h>
#include <pnl/pnl_random.h>
#include <ctime>
#include <algorithm>


void print(double price, double squarePrice, const PnlMat * delta, const PnlMat * gamma, int nbPath) {
	std::cout << "AFFICHAGE DES RESULTATS"<< std::endl;
	std::cout << "[*] Prix : " ;
	std::cout << price << std::endl;
	std::cout << "[*] Delta : " << std::endl;
	pnl_mat_print(delta);
	std::cout << "[*] Gamma : " << std::endl;
	pnl_mat_print(gamma);

	squarePrice /= (nbPath);
	double trust = sqrt(squarePrice - price*price)  * 1.96 / pow(nbPath,0.5);
	std::cout << "[*] Intervalle de confiance : " ;
	std::cout << trust << std::endl;
}

int main(int argc, char **argv)
{
	//ce produit doit etre rempli avec les equity, les fx et la matdecorrel aussi
	//son histofix doit etre quasi vide a part la premiere colonne remplie des spots.
	Produit produit = Produit();
	double price = 0, priceSquare = 0;
	PnlMat * delta = pnl_mat_create(produit.getEquities().size(),5);
	PnlMat * gamma = pnl_mat_create(produit.getEquities().size(),5);
	Model model = Model(NBPATH);
	MC_Compute moteur = MC_Compute(&produit, &model);
	// on lance le price
	if ( moteur.Price(&price, &priceSquare, delta, gamma) != 0) std::cout << "Bug" << std::endl;
	else print(price, priceSquare, delta, gamma ,model.Nb_Path());

	pnl_mat_free(&delta);
	pnl_mat_free(&gamma);

	while (getchar() != '\n') ;
	return 0;

}