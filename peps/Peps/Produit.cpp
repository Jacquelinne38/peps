
#include "StdAfx.h"
#include "Produit.h"
#include "pnl\pnl_matrix.h"

#include <fstream>
#include <vector>
using namespace std;

Produit::Produit(PnlMat * assets, PnlMat * corr, PnlVect * vol) {
	m_historique = assets;
	for(int i = 0; i < assets->m ; ++i) {
		Equity tmp = Equity(pnl_mat_get(assets, i, 0),pnl_vect_get(vol, i), i);
		ListEquit.push_back(tmp);
	}
	m_matCor = corr;
	m_matCholCorr = pnl_mat_copy(this->getMatCor());
	pnl_mat_chol(m_matCholCorr);
	SetInitVol();
}


	Produit::Produit(PnlMat * assets, int nbActif, int nbDate) {
		m_historique = assets;
		Equity barclays = Equity(100, 0.3, "barclays");
		Equity sony = Equity (50, 0.3, "sony");
		//Equity arcelor = Equity (200, 0.25, "arcelor");
		Equity alcoa = Equity(150, 0.3, "alcoa");
		Equity daiichi = Equity(75, 0.3, "daiichi");
		ListEquit.push_back(daiichi);
		ListEquit.push_back(alcoa);
		ListEquit.push_back(barclays);
		ListEquit.push_back(sony);
		//ListEquit.push_back(arcelor);
		//Fx y;
		//ListFx.push_back(y);
		// clairement ici ne pas mettre 
		m_matCor = pnl_mat_create(ListEquit.size(),ListEquit.size());
		pnl_mat_set(m_matCor, 0, 0, 0.09);
		pnl_mat_set(m_matCor, 0, 1, 0);
		pnl_mat_set(m_matCor, 0, 2, 0);
		pnl_mat_set(m_matCor, 0, 3, 0);
		//pnl_mat_set(m_matCor, 0, 4, 0.38);

		pnl_mat_set(m_matCor, 1, 0, 0);
		pnl_mat_set(m_matCor, 1, 1, 0.09);
		pnl_mat_set(m_matCor, 1, 2, 0);
		pnl_mat_set(m_matCor, 1, 3, 0);
		//pnl_mat_set(m_matCor, 1, 4, 0.21);


		pnl_mat_set(m_matCor, 2, 0, 0);
		pnl_mat_set(m_matCor, 2, 1, 0);
		pnl_mat_set(m_matCor, 2, 2, 0.09);
		pnl_mat_set(m_matCor, 2, 3, 0);
		//pnl_mat_set(m_matCor, 2, 4, 0.17);

		
		pnl_mat_set(m_matCor, 3, 0, 0);
		pnl_mat_set(m_matCor, 3, 1, 0);
		pnl_mat_set(m_matCor, 3, 2, 0);
		pnl_mat_set(m_matCor, 3, 3, 0.09);
		//pnl_mat_set(m_matCor, 3, 4, 0.2);

		/*pnl_mat_set(m_matCor, 4, 0, 0.38);
		pnl_mat_set(m_matCor, 4, 1, 0.21);
		pnl_mat_set(m_matCor, 4, 2, 0.17);
		pnl_mat_set(m_matCor, 4, 3, 0.2);
		pnl_mat_set(m_matCor, 4, 4, 1);*/

		m_matCholCorr = pnl_mat_copy(this->getMatCor());
		pnl_mat_chol(m_matCholCorr);
		SetInitVol();
}

Produit::Produit()
{
		LoadDataHisto();
		//dans ce constructeur on doit faire appel aux constructeurs d'equity avec les bons parametres pour creer tous les equity et fx
		Equity barclays = Equity(100, 0.3, "barclays");
		Equity sony = Equity (50, 0.3, "sony");
		//Equity arcelor = Equity (200, 0.25, "arcelor");
		Equity alcoa = Equity(150, 0.3, "alcoa");
		Equity daiichi = Equity(75, 0.3, "daiichi");
		ListEquit.push_back(daiichi);
		ListEquit.push_back(alcoa);
		ListEquit.push_back(barclays);
		ListEquit.push_back(sony);
		//ListEquit.push_back(arcelor);
		//Fx y;
		//ListFx.push_back(y);
		// clairement ici ne pas mettre 
		m_matCor = pnl_mat_create(ListEquit.size(),ListEquit.size());
		pnl_mat_set(m_matCor, 0, 0, 0.09);
		pnl_mat_set(m_matCor, 0, 1, 0);
		pnl_mat_set(m_matCor, 0, 2, 0);
		pnl_mat_set(m_matCor, 0, 3, 0);
		//pnl_mat_set(m_matCor, 0, 4, 0.38);

		pnl_mat_set(m_matCor, 1, 0, 0);
		pnl_mat_set(m_matCor, 1, 1, 0.09);
		pnl_mat_set(m_matCor, 1, 2, 0);
		pnl_mat_set(m_matCor, 1, 3, 0);
		//pnl_mat_set(m_matCor, 1, 4, 0.21);


		pnl_mat_set(m_matCor, 2, 0, 0);
		pnl_mat_set(m_matCor, 2, 1, 0);
		pnl_mat_set(m_matCor, 2, 2, 0.09);
		pnl_mat_set(m_matCor, 2, 3, 0);
		//pnl_mat_set(m_matCor, 2, 4, 0.17);

		
		pnl_mat_set(m_matCor, 3, 0, 0);
		pnl_mat_set(m_matCor, 3, 1, 0);
		pnl_mat_set(m_matCor, 3, 2, 0);
		pnl_mat_set(m_matCor, 3, 3, 0.09);
		//pnl_mat_set(m_matCor, 3, 4, 0.2);

		/*pnl_mat_set(m_matCor, 4, 0, 0.38);
		pnl_mat_set(m_matCor, 4, 1, 0.21);
		pnl_mat_set(m_matCor, 4, 2, 0.17);
		pnl_mat_set(m_matCor, 4, 3, 0.2);
		pnl_mat_set(m_matCor, 4, 4, 1);*/

		m_matCholCorr = pnl_mat_copy(this->getMatCor());
		pnl_mat_chol(m_matCholCorr);
		SetInitVol();
}


//vol est a desallouer en dehors
void Produit::SetInitVol() {
	this->m_volatility = pnl_vect_create(this->getEquities().size());
	for(int k = 0; k < this->getEquities().size(); k++){
		pnl_vect_set(this->m_volatility, k, this->getEquities()[k].volatility);
	}	
}

Produit::~Produit()
{
	pnl_mat_free(&m_matCor);
	pnl_mat_free(&m_historique);
	pnl_mat_free(&m_matCholCorr);
	pnl_vect_free(&m_volatility);
}

// Ces fonctions doivent prendre le temps en parametre
// ces fonctions sont responsables de l'initialisation des structures
void Produit::UpDat_HistoFixing(){}
void Produit::UpDate_ListEquit(){}
void Produit::UpDate_listFx(){}


void Produit::LoadDataHisto() {

	//ou histo1 ou histo2
	fstream fichier("C:/Users/pizzix-game/Documents/GitHub/peps/peps/DATA/data.txt");
	vector <string> monTableau;

	if ( !fichier )
		cout << "fichier inexistant";
	else	{
		bool continuer = true;

		while( !fichier.eof() )
		{
			monTableau.push_back("");
			getline(fichier, monTableau.back());
			int ligne = monTableau.size() - 1;
			if(monTableau[ligne].empty())
				monTableau.pop_back();
		}
		cout << "nombre de lignes : " << monTableau.size() << endl;
		std::vector<std::string> split;
		tokenize(monTableau[0], split, ",");
		int msize = split.size();
		m_historique = pnl_mat_create(monTableau.size(), PAS);
		for(unsigned int i = 0 ; i < monTableau.size(); i ++) {
			split.clear();
			tokenize(monTableau[i], split, ",");
			for (int j = 0 ; j < PAS; j++) {
				pnl_mat_set(m_historique, i,j, atof(split[j].data()));
			}
		}
		//pnl_mat_print(m_historique);
	}
}


