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
#include <iostream>
#include <string>
#include <fstream>


void print(double price, double squarePrice, const PnlVect * delta, const PnlVect * gamma, int nbPath) {
	std::cout << "AFFICHAGE DES RESULTATS"<< std::endl;
	std::cout << "[*] Prix : " ;
	std::cout << price << std::endl;
	if (price > 1.23) {
		std::cout << "NOOOOBB OUT" << std::endl;
	}
	/*std::cout << "[*] Delta : " << std::endl;
	pnl_vect_print(delta);
	std::cout << "[*] Gamma : " << std::endl;
	pnl_vect_print(gamma);
	*/
	/*
	squarePrice /= (nbPath);
	double trust = sqrt(squarePrice - price*price)  * 1.96 / pow(nbPath,0.5);
	std::cout << "[*] Intervalle de confiance : " ;
	std::cout << trust << std::endl;*/
}

void CreeFichierPrix(std::vector<double> vec, std::string nomFichier) {
	std::ofstream fichier(nomFichier, std::ios::out | std::ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
	if(fichier)
	{
		for(unsigned int i = 0 ; i < vec.size(); ++i) {
			fichier << vec[i] << std::endl;
		}
		fichier.close();
	}
	else
		std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;

}

int main(int argc, char **argv)
{
	
	Produit produit = Produit();
	double price = 0, priceSquare = 0;
	PnlVect * delta = pnl_vect_create(produit.getEquities().size());
	PnlVect * gamma = pnl_vect_create(produit.getEquities().size());
	Model model = Model(NBPATH);
	MC_Compute moteur = MC_Compute(&produit, &model);
	std::vector<double> vec_price;
	// ICI cree la matrice path complete

	// on lance le price en t
	for (int t=0; t<250; t++){
		if ( moteur.Price(&price, &priceSquare, delta, gamma, t) != 0) std::cout << "Bug" << std::endl;
		else {
			print(price, priceSquare, delta, gamma ,model.Nb_Path());
			vec_price.push_back(price);
		}
	}
	CreeFichierPrix(vec_price, "../DATA/prix.txt");
		pnl_vect_free(&delta);
		pnl_vect_free(&gamma);
	
	//pnl_vect_print(model.Diffuse_cours_histo(produit.getEquities()[3].value, 0.05/52, produit.getEquities()[3].volatility/52));

	while (getchar() != '\n') ;
	return 0;

}