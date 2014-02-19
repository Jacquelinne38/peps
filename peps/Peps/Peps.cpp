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
	double price_couverture = 0;
	std::vector<double> vec_price;
	std::vector<PnlVect *> vec_delta;
	std::vector<double> vec_priceCouverture;
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
		vec_delta.push_back(pnl_vect_copy(delta));

		PnlVect * l_spot = pnl_vect_create(produit.getMatHisto()->m);
		pnl_mat_get_col(l_spot, produit.getMatHisto(), t);
		if (t == 0){
			price_couverture = price - pnl_vect_scalar_prod(delta, l_spot);
			
		} else {
			PnlVect * l_diffDelta = pnl_vect_copy(vec_delta[vec_delta.size()-2]);
			//pnl_vect_print(vec_delta[vec_delta.size()-2]);
			pnl_vect_minus_vect(l_diffDelta, delta);
			pnl_vect_mult_double(l_diffDelta, -1);
			price_couverture = price_couverture * exp(-TAUX_ACTUALISATION*DT) - pnl_vect_scalar_prod(l_diffDelta, l_spot);
			pnl_vect_free(&l_diffDelta);
		}
		vec_priceCouverture.push_back(price_couverture);
	//	std::cout << "Spot : " << std::endl;
		//pnl_vect_print(l_spot);
		pnl_vect_free(&l_spot);
	}

	CreerFichierData(vec_price, "../DATA/prix.txt");
	CreerFichierData(vec_priceCouverture, "../DATA/couverture.txt");


	//desaclouer le vecteur de delta
	pnl_vect_free(&delta);
	pnl_vect_free(&gamma);
	
	//pnl_vect_print(model.Diffuse_cours_histo(produit.getEquities()[3].value, 0.05/52, produit.getEquities()[3].volatility/52));
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	std::cout<<"printf: "<< duration <<'\n';
	while (getchar() != '\n') ;

	_CrtDumpMemoryLeaks();

	return 0;

}