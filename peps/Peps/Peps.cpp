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
	/*std::cout << "[*] Delta : " << std::endl;
	pnl_mat_print(delta);
	std::cout << "[*] Gamma : " << std::endl;
	pnl_mat_print(gamma);

	squarePrice /= (nbPath);
	double trust = sqrt(squarePrice - price*price)  * 1.96 / pow(nbPath,0.5);
	std::cout << "[*] Intervalle de confiance : " ;
	std::cout << trust << std::endl;*/
}

int main(int argc, char **argv)
{

	Produit produit = Produit();
	double price = 0, priceSquare = 0;
	PnlMat * delta = pnl_mat_create(produit.getEquities().size(),PAS);
	PnlMat * gamma = pnl_mat_create(produit.getEquities().size(),PAS);
	Model model = Model(NBPATH);
	MC_Compute moteur = MC_Compute(&produit, &model);
	// ICI cree la matrice path complete
	PnlMat * Path = produit.getMatHisto();
	// on lance le price en t

	if ( moteur.Price(&price, &priceSquare, delta, gamma, 0) != 0) std::cout << "Bug" << std::endl;
	else print(price, priceSquare, delta, gamma ,model.Nb_Path());

	pnl_mat_free(&delta);
	pnl_mat_free(&gamma);

	//pnl_vect_print(model.Diffuse_cours_histo(produit.getEquities()[3].value, 0.05/52, produit.getEquities()[3].volatility/52));

	while (getchar() != '\n') ;
	return 0;

}