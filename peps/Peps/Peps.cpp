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
	Pesp_Timer _timer = Pesp_Timer();
	_timer.Start();

	double price = 0, priceSquare = 0;
	
	// variable locales utiles pour le calcul du prix de couverture
	// elles doivent bouger dans la fonctio qi sera cree pour ca
	double price_couverture = 0;
	double actifs_risq = 0;
	double sans_risq = 0;
	std::vector<double> vec_actifs_risq;
	std::vector<double> vec_sans_risq;
	std::vector<double> vec_priceCouverture;
	
	std::vector<double> vec_price;	
	std::vector<PnlVect *> vec_delta;
	Produit produit = Produit();
	Model model = Model(NBPATH);

	//Générateur data
	//CreationDataHisto("../DATA/histo1.txt", 0.05, produit);

	PnlVect * delta = pnl_vect_create(produit.getEquities().size());
	PnlVect * gamma = pnl_vect_create(produit.getEquities().size());
	
	MC_Compute moteur = MC_Compute(&produit, &model);

	PnlMat *l_histoFix = pnl_mat_create(produit.getEquities().size(), moteur.mvec_fixingDate.size());
	// ICI creer la matrice path complete et surement histofix aussi

	//Pricing pour chaque t
	for (int t=0; t<250; t++){
		std::cout<< t <<std::endl;
		int ret = moteur.Price(&price, &priceSquare, delta, gamma, l_histoFix, t);
		if (ret == -10) break;
		else if (ret != 0) std::cout << "Bug" << std::endl;
		else print(price, priceSquare, delta, gamma ,model.Nb_Path());
		vec_price.push_back(price);
		vec_delta.push_back(pnl_vect_copy(delta));

		// a mettre dans une fonction du genre refresh spot
		// declarer l_spot avant
		PnlVect * l_spot = pnl_vect_create(produit.getMatHisto()->m);
		pnl_mat_get_col(l_spot, produit.getMatHisto(), t);

		
		///////////// LA PARTIE QUI CONCERNE LE CALCUL DE LA COUVERTURE DOIT ETRE FAIT AILLEURS
		if (t==0){
			actifs_risq = pnl_vect_scalar_prod(delta, l_spot);
			sans_risq = price - pnl_vect_scalar_prod(delta, l_spot);
			price_couverture = actifs_risq + sans_risq; 
		} else {
			//PnlVect * l_diffDelta = pnl_vect_copy(vec_delta[vec_delta.size()-2]);
			//pnl_vect_minus_vect(l_diffDelta, delta);
			//pnl_vect_mult_double(l_diffDelta, -1);
			//pnl_vect_print(l_diffDelta);
			actifs_risq = pnl_vect_scalar_prod(delta, l_spot);
			sans_risq = sans_risq * exp(TAUX_ACTUALISATION*DT) - pnl_vect_scalar_prod(delta, l_spot) + pnl_vect_scalar_prod(vec_delta[vec_delta.size()-2], l_spot);
			price_couverture = actifs_risq + sans_risq;
			//pnl_vect_free(&l_diffDelta);
		}
		vec_priceCouverture.push_back(price_couverture);
		vec_actifs_risq.push_back(actifs_risq);
		vec_sans_risq.push_back(sans_risq);
		pnl_vect_free(&l_spot);
	}
	//pnl_mat_free(&l_histoFix);
	////////////////////////////

	//Création fichiers d'export
	CreerFichierData(vec_price, "../DATA/prix.txt");
	CreerFichierData(vec_priceCouverture, "../DATA/couverture.txt");
	CreerFichierData(vec_actifs_risq, "../DATA/actifs_risq.txt");
	CreerFichierData(vec_sans_risq, "../DATA/sans_risq.txt");

	std::vector<double> vecbisdelta;
	for(int i = 0 ; i< vec_delta.size(); ++i ) {
		vecbisdelta.push_back(pnl_vect_get(vec_delta[i], 0));
	}
	CreerFichierData(vecbisdelta, "../DATA/delta.txt");


	pnl_vect_free(&delta);
	pnl_vect_free(&gamma);
	pnl_mat_free(&l_histoFix);

			_timer.GetTime("Temps exec : ");
	while (getchar() != '\n') ;

	_CrtDumpMemoryLeaks();

	return 0;

}