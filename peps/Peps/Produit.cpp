#include "StdAfx.h"
#include "Produit.h"
#include "pnl\pnl_matrix.h"

#include <fstream>
#include <vector>
using namespace std;

template < class ContainerT >
void tokenize(const std::string& str, ContainerT& tokens,
	const std::string& delimiters = " ", bool trimEmpty = false)
{
	std::string::size_type pos, lastPos = 0;
	while(true)
	{
		pos = str.find_first_of(delimiters, lastPos);
		if(pos == std::string::npos)
		{
			pos = str.length();

			if(pos != lastPos || !trimEmpty)
				tokens.push_back(ContainerT::value_type(str.data()+lastPos,
				(ContainerT::value_type::size_type)pos-lastPos ));

			break;
		}
		else
		{
			if(pos != lastPos || !trimEmpty)
				tokens.push_back(ContainerT::value_type(str.data()+lastPos,
				(ContainerT::value_type::size_type)pos-lastPos ));
		}

		lastPos = pos + 1;
	}
};


void Produit::SetDataHisto() {
	fstream fichier("../DATA/data.txt");
	vector <string> monTableau;

	if ( !fichier )
		cout << "fichier inexistant";
	else
	{
		bool continuer = true;

		while( !fichier.eof() )
		{
			monTableau.push_back("");//creation d'une ligne vide

			getline(fichier, monTableau.back());//lecture d'une ligne du fichier

			int ligne = monTableau.size() - 1;//je recupere la taille du tableau (-1 pour la ligne 0)

			if(monTableau[ligne].empty())//si la ligne est vide
				monTableau.pop_back();//on la retire du tableau
		}

		cout << "nombre de lignes : " << monTableau.size() << endl;//j'affiche le nombre de lignes pour test
		std::vector<std::string> split;
		tokenize(monTableau[0], split, ",");
		int msize = split.size();
		m_historique = pnl_mat_create(monTableau.size(), msize);
		for(int i = 0 ; i < monTableau.size(); i ++) {
			split.clear();
			tokenize(monTableau[i], split, ",");
			for (int j = 0 ; j < msize; j++) {
				pnl_mat_set(m_historique, i,j, atof(split[j].data()));
			}
		}
		pnl_mat_print(m_historique);
	}
	
}

Produit::Produit()
{
	SetDataHisto();
		//dans ce constructeur on doit faire appel aux constructeurs d'equity avec les bons parametres pour creer tous les equity et fx
		Equity barclays = Equity(100, 0.2, "barclays");
		Equity sony = Equity (50, 0.3, "sony");
		Equity arcelor = Equity (200, 0.25, "arcelor");
		Equity alcoa = Equity(150, 0.2, "alcoa");
		Equity daiichi = Equity(75, 0.26, "daiichi");
		ListEquit.push_back(daiichi);
		ListEquit.push_back(alcoa);
		ListEquit.push_back(barclays);
		ListEquit.push_back(sony);
		ListEquit.push_back(arcelor);
		//Fx y;
		//ListFx.push_back(y);
		// clairement ici ne pas mettre 
		m_matCor = pnl_mat_create(5,5);
		pnl_mat_set(m_matCor, 0, 0, 1);
		pnl_mat_set(m_matCor, 0, 1, 0.3);
		pnl_mat_set(m_matCor, 0, 2, 0.4);
		pnl_mat_set(m_matCor, 0, 3, 0.35);
		pnl_mat_set(m_matCor, 0, 4, 0.38);

		pnl_mat_set(m_matCor, 1, 0, 0.3);
		pnl_mat_set(m_matCor, 1, 1, 1);
		pnl_mat_set(m_matCor, 1, 2, 0.2);
		pnl_mat_set(m_matCor, 1, 3, 0.34);
		pnl_mat_set(m_matCor, 1, 4, 0.21);


		pnl_mat_set(m_matCor, 2, 0, 0.4);
		pnl_mat_set(m_matCor, 2, 1, 0.2);
		pnl_mat_set(m_matCor, 2, 2, 1);
		pnl_mat_set(m_matCor, 2, 3, 0.23);
		pnl_mat_set(m_matCor, 2, 4, 0.17);

		
		pnl_mat_set(m_matCor, 3, 0, 0.35);
		pnl_mat_set(m_matCor, 3, 1, 0.34);
		pnl_mat_set(m_matCor, 3, 2, 0.23);
		pnl_mat_set(m_matCor, 3, 3, 1);
		pnl_mat_set(m_matCor, 3, 4, 0.2);

		pnl_mat_set(m_matCor, 4, 0, 0.38);
		pnl_mat_set(m_matCor, 4, 1, 0.21);
		pnl_mat_set(m_matCor, 4, 2, 0.17);
		pnl_mat_set(m_matCor, 4, 3, 0.2);
		pnl_mat_set(m_matCor, 4, 4, 1);
		
		// on doit aussi initialiser la matrice de corr (gros taff) 

		// on doit aussi initialiser l historiq fixing
}


Produit::~Produit()
{
	pnl_mat_free(&m_matCor);
	pnl_mat_free(&m_historique);
}

// Ces fonctions doivent prendre le temps en parametre
// ces fonctions sont responsables de l'initialisation des structures
void Produit::UpDat_HistoFixing(){}
void Produit::UpDate_ListEquit(){}
void Produit::UpDate_listFx(){}

