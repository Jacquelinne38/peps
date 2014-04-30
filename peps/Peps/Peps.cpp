// Peps.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "MC_Compute.h"
#include "Produit.h"
#include "Model.h"

#define _CRTDBG_MAP_ALLOC
//#define _DEBUG 0
#include <stdlib.h>
#include <crtdbg.h>



int main(int argc, char **argv)
{
	Pesp_Timer _timer = Pesp_Timer();
	_timer.Start();

	double price = 0, priceSquare = 0;

	

	std::vector<double> vec_actifs_risq;
	std::vector<double> vec_sans_risq;
	std::vector<double> vec_priceCouverture;
	
	std::vector<double> vec_price;	
	std::vector<PnlVect *> vec_delta;
	Produit produit = Produit();
	Model model = Model(NBPATH);

	//Générateur data
	//CreationDataHisto("../DATA/histo1.txt", 0.05, produit);
	MC_Compute moteur = MC_Compute(&produit, &model);
	PnlVect * delta = pnl_vect_create(produit.getEquities().size());
	PnlVect * gamma = pnl_vect_create(produit.getEquities().size());

	PnlMat *l_histoFix = pnl_mat_create(produit.getEquities().size(), model.mvec_fixingDate.size());
	// ICI creer la matrice path complete et surement histofix aussi

	//Pricing pour chaque t
	for (int t=0; t<model.FINALDATE(); t++){
		//std::cout<< t <<std::endl;
		int ret = moteur.Price(&price, &priceSquare, delta, gamma, l_histoFix, t);
		if (ret == -10) break;
		else if (ret != 0) std::cout << "Bug" << std::endl;
		else print(price, priceSquare, delta, gamma ,model.Nb_Path());
		vec_price.push_back(price);
		vec_delta.push_back(pnl_vect_copy(delta));
		pnl_vect_print(delta);
		std::cout << "coucou" << delta->size <<std::endl;
		// a mettre dans une fonction du genre refresh spot
		// declarer l_spot avant

		PnlMat * l_histo =  produit.getMatHisto();
		PnlVect * l_spot = pnl_vect_create(l_histo->m);
		pnl_mat_get_col(l_spot, l_histo, t);
		pnl_mat_free(&l_histo);

		model.Simul_Market(vec_delta, vec_priceCouverture, vec_actifs_risq, vec_sans_risq, delta, l_spot, t);
		

		pnl_vect_free(&l_spot);
	}
	//pnl_mat_free(&l_histoFix);
	////////////////////////////

	//Création fichiers d'export
	CreerFichierData(vec_price, "../../../DATA/prix.txt");
	CreerFichierData(vec_priceCouverture, "../../../DATA/couverture.txt");
	CreerFichierData(vec_actifs_risq, "../../../DATA/actifs_risq.txt");
	CreerFichierData(vec_sans_risq, "../../../DATA/sans_risq.txt");

	std::vector<double> vecbisdelta;
	for(unsigned int i = 0 ; i< vec_delta.size(); ++i ) {
		vecbisdelta.push_back(pnl_vect_get(vec_delta[i], 0));
	}
	CreerFichierData(vecbisdelta, "../../../DATA/delta.txt");


	pnl_vect_free(&delta);
	pnl_vect_free(&gamma);
	pnl_mat_free(&l_histoFix);

			_timer.GetTime("Temps exec : ");


	_CrtDumpMemoryLeaks();
		while (getchar() != '\n') ;
	return 0;

}