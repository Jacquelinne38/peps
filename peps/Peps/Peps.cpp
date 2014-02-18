// Peps.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "MC_Compute.h"
#include "Produit.h"
#include "Model.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main(int argc, char **argv)
{
	std::clock_t start;
	double duration;

	start = std::clock();

	double price = 0, priceSquare = 0;
	std::vector<double> vec_price;
	Produit produit = Produit();
	Model model = Model(NBPATH);

	PnlVect * delta = pnl_vect_create(produit.getEquities().size());
	PnlVect * gamma = pnl_vect_create(produit.getEquities().size());
	
	MC_Compute moteur = MC_Compute(&produit, &model);

	// ICI creer la matrice path complete

	//Pricing pour chaque t
	for (int t=0; t<250; t++){
		int ret = moteur.Price(&price, &priceSquare, delta, gamma, t);
		if (ret == -10) break;
		else if (ret != 0) std::cout << "Bug" << std::endl;
		
		else print(price, priceSquare, delta, gamma ,model.Nb_Path());
		vec_price.push_back(price);
	}

	CreeFichierPrix(vec_price, "../DATA/prix.txt");

	pnl_vect_free(&delta);
	pnl_vect_free(&gamma);
	
	//pnl_vect_print(model.Diffuse_cours_histo(produit.getEquities()[3].value, 0.05/52, produit.getEquities()[3].volatility/52));
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	std::cout<<"printf: "<< duration <<'\n';
	while (getchar() != '\n') ;

	_CrtDumpMemoryLeaks();

	return 0;

}