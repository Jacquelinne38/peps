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
#include <list>

//int main(int argc, char **argv) {		
namespace Wrapper {


	//Fonction qui sert d'interface entre le c# et le c++ ici c'est du c++ cli.
	//Cette la fonction de calcule qui est appelé par le projet TestWrapper qui est lui même appelé par le projet web.
	//C'est ici que tout ce passe.
	int WrapperClass::LaunchComputation(array<double, 2>^ assets, 
		array<double, 1>^ vol, 
		array<double, 2>^ corr, 
		int nbActif,
		int nbDate,
		System::Collections::Generic::List<double>^% list_price,
		System::Collections::Generic::List<double>^% list_priceCouverture,
		System::Collections::Generic::List<double>^% list_sans_risq,
		System::Collections::Generic::List<double>^% list_actifs_risq,
		array<double, 2>^% compoCli
		) {

		if(PAS > nbDate)
			return -1;
		//nbActif = 15;
		PnlMat * lm_histo = pnl_mat_create(nbActif, PAS);
		array<double, 2>^ tmp = PnlMatToArray(lm_histo);
		

		
		std::cout << std::endl;
		PnlVect * lv_vol = pnl_vect_create(nbActif);
		PnlMat * lm_corr = pnl_mat_create(nbActif, nbActif);


		ArrayHistoToPnlMat(lm_histo, assets, nbActif, PAS);
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
		

		//Générateur data
		//CreationDataHisto("../DATA/histo1.txt", 0.05, produit);
		MC_Compute moteur = MC_Compute(&produit, &model);
		PnlVect * delta = pnl_vect_create(produit.getEquities().size());
		PnlVect * gamma = pnl_vect_create(produit.getEquities().size());

		PnlMat *l_histoFix = pnl_mat_create(produit.getEquities().size(), model.mvec_fixingDate.size());
		// ICI creer la matrice path complete et surement histofix aussi

		//Composition
		PnlMat * compoPortefeuille = pnl_mat_create(produit.getEquities().size(), model.FINALDATE());
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

			model.Simul_Market(vec_delta, vec_priceCouverture, vec_actifs_risq, vec_sans_risq, delta, l_spot, t, price, compoPortefeuille);
			pnl_vect_free(&l_spot);
			//if (price > 1.23)
			//	break;
		}
		//pnl_mat_free(&l_histoFix);
		////////////////////////////

		//Création fichiers d'export
		CreerFichierData(vec_price, "../DATA/prix.txt");
		CreerFichierData(vec_priceCouverture, "../DATA/couverture.txt");
		CreerFichierData(vec_actifs_risq, "../DATA/actifs_risq.txt");
		CreerFichierData(vec_sans_risq, "../DATA/sans_risq.txt");

		list_price = NatifToManaged(vec_price);
		list_priceCouverture = NatifToManaged(vec_priceCouverture);
		list_actifs_risq = NatifToManaged(vec_actifs_risq);
		list_sans_risq = NatifToManaged(vec_sans_risq);
		//pnl_mat_print(compoPortefeuille);
		compoCli = PnlMatToArray(compoPortefeuille);


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
		//while (getchar() != '\n') ;
		
		return 0;
	}


		//Fonction qui sert d'interface entre le c# et le c++ ici c'est du c++ cli.
	//Cette fonction retourne la volatilité en fonction des valeurs historique 
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
			std::ofstream fichier("../DATA/volcalc.txt", std::ios::out | std::ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
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
	//Fonction qui sert d'interface entre le c# et le c++ ici c'est du c++ cli.
	//Cette fonction retourne la corrélation en fonction des valeurs historique 
	array<double, 2>^ WrapperClass::CalcCorr(array<double, 2>^ histo, int nbActif, int nbDate, bool debug) {
		//Passage array to pnlmatarray
		PnlMat * l_histo = pnl_mat_create(nbActif, nbDate);
		ArrayToPnlMat(l_histo, histo, nbActif, nbDate);

		PnlMat * l_corr = pnl_mat_create(nbActif, nbActif);
		Compute_mat_Cor(l_histo, l_corr);
		for(int i = 0 ; i < l_corr->n; ++i) {
			pnl_mat_set(l_corr, i, i , 1.0);
		}

		if (debug) {
			std::ofstream fichier("../DATA/corrcalc.txt", std::ios::out | std::ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
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