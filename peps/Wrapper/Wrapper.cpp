// Il s'agit du fichier DLL principal.

#include "stdafx.h"

#include "Wrapper.h"
#include "Equity.h"

#include "Wrapper.h"
#include "MC_Compute.h"
#include "Produit.h"
#include "Model.h"
#include "Pesp_Timer.h"
#define _CRTDBG_MAP_ALLOC
//#define _DEBUG 0
#include <stdlib.h>
#include <crtdbg.h>


//int main(int argc, char **argv) {		
namespace Wrapper {
	int WrapperClass::LaunchComputation(array<double, 2>^ assets, array<double, 1>^ vol, array<double, 2>^ corr, int nbActif, int nbDate) {
		if(PAS > nbDate)
			return -1;

		PnlMat * lm_histo = pnl_mat_create(nbActif, PAS);
		PnlVect * lv_vol = pnl_vect_create(nbActif);
		PnlMat * lm_corr = pnl_mat_create(nbActif, nbActif);

		ArrayToPnlMat(lm_histo, assets, nbActif, PAS);
		ArrayToPnlMat(lm_corr, corr, nbActif, nbActif);
		ArrayToPnlVect(lv_vol, vol, nbActif);

		Pesp_Timer _timer = Pesp_Timer();
		_timer.Start();

		double price = 0, priceSquare = 0;

		std::vector<double> vec_actifs_risq;
		std::vector<double> vec_sans_risq;
		std::vector<double> vec_priceCouverture;

		std::vector<double> vec_price;	
		std::vector<PnlVect *> vec_delta;
		//Produit produit = Produit(lm_histo, nbActif, nbDate);
		//Produit produit = Produit();
		Produit produit = Produit(lm_histo, lm_corr, lv_vol);
		Model model = Model(NBPATH);
		

		//G�n�rateur data
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

		//Cr�ation fichiers d'export
		CreerFichierData(vec_price, "../DATA/prix.txt");
		CreerFichierData(vec_priceCouverture, "../DATA/couverture.txt");
		CreerFichierData(vec_actifs_risq, "../DATA/actifs_risq.txt");
		CreerFichierData(vec_sans_risq, "../DATA/sans_risq.txt");

		std::vector<double> vecbisdelta;
		for(unsigned int i = 0 ; i< vec_delta.size(); ++i ) {
			vecbisdelta.push_back(pnl_vect_get(vec_delta[i], 0));
		}
		CreerFichierData(vecbisdelta, "../DATA/delta.txt");


		pnl_vect_free(&delta);
		pnl_vect_free(&gamma);
		pnl_mat_free(&l_histoFix);

		_timer.GetTime("Temps exec : ");


		_CrtDumpMemoryLeaks();
		while (getchar() != '\n') ;
		
		return 0;
	}

	array<double, 1>^ WrapperClass::CalcVol(array<double, 2>^ histo, int nbActif, int nbDate, bool debug) {

		//Passage array to pnlmatarray
		PnlMat * l_histo = pnl_mat_create(nbActif, nbDate);
		ArrayToPnlMat(l_histo, histo, nbActif, nbDate);

		PnlVect * lv_vol = pnl_vect_create(nbActif);
		for(int i = 0 ; i < nbActif; ++ i) {
			PnlVect * tmp = pnl_vect_create(nbDate);
			pnl_mat_get_row(tmp, l_histo, i);
			pnl_vect_set(lv_vol, i, Compute_Volatility(tmp));
		}
		if (debug) 
		{
			std::ofstream fichier("../DATA/volcalc.txt", std::ios::out | std::ios::trunc);  // ouverture en �criture avec effacement du fichier ouvert
			if(fichier)
			{
				for(int i = 0 ; i < nbActif; ++i) {
					fichier << "Vol actif : " << i << " value : " << pnl_vect_get(lv_vol, i) << std::endl;
				}
				fichier.close();
			}
		}

		//c++/cli to c#
		array<double, 1>^ la_histo = PnlVectToArray(lv_vol);
		pnl_mat_free(&l_histo);
		pnl_vect_free(&lv_vol);
		return la_histo;
	}

	array<double, 2>^ WrapperClass::CalcCorr(array<double, 2>^ histo, int nbActif, int nbDate, bool debug) {
		//Passage array to pnlmatarray
		PnlMat * l_histo = pnl_mat_create(nbActif, nbDate);
		ArrayToPnlMat(l_histo, histo, nbActif, nbDate);


		PnlMat * l_corr = pnl_mat_create(nbActif, nbActif);
		Compute_mat_Cor(l_histo, l_corr);

		if (debug) {
			std::ofstream fichier("../DATA/corrcalc.txt", std::ios::out | std::ios::trunc);  // ouverture en �criture avec effacement du fichier ouvert
			if(fichier)
			{
				for(int i = 0; i < l_corr->m; ++i) {
					fichier << "c(";
					for(int j = 0 ; j < l_corr->n ; ++j) {
						fichier << MGET(l_corr, i, j);
						if(!(j == l_corr->n -1))
							fichier << ",";
					}
					if(!(i == l_corr->n -1))
						fichier <<"),"; 
					else 
						fichier <<")";
				}
				fichier.close();
			}
		}
		array<double, 2>^ la_corr = PnlMatToArray(l_corr);
		pnl_mat_free(&l_corr);
		pnl_mat_free(&l_histo);
		return la_corr;
	}
}