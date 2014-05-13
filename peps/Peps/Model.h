#pragma once
#include "pnl/pnl_random.h"
#include "Utils.h"

#define DLLEXP   __declspec( dllexport )
class DLLEXP Model
{


private :
	//Nombre de tour de la boucle de monte carlo
	int m_nbPath;

	/*
	*Diffuse les trajectoires pour chacun des actifs
	*@param in : produit
	*@param in : drift du mod�le
	*@param in : vecteur des tirages effectu�s
	*@param in : matrice de cholesky des corr�lations
	*@param in/out : vecteur spot
	*/
	void Diffuse_from_t_all_Asset(Produit * produit,const PnlVect * drift, const PnlVect * vecAlea, const PnlMat * choleskyCor, PnlVect * spot);

	/*
	*Calcule le spot dt jours/semaines plus tard
	*@param in : produit
	*@param in : drift du mod�le
	*@param in : vecteur des tirages effectu�s
	*@param in : matrice de cholesky des corr�lations
	*@param in/out : vecteur spot
	*@param in : pas de temps
	*/
	void  Diffuse_of_dt(Produit * produit,const PnlVect * drift, const PnlVect * vecAlea, const PnlMat * choleskyCor, PnlVect * spot,const int dt);

	/*
	* Met dans un vecteur en premier �l�ment la date time puis les dates de fixing qui suivent
	*@param in : temps t
	*@param out : vecteur contenant les dates
	* return : liste des dates
	*/
	std::vector<int>  getFixingDateFromt(int time, std::vector<int> lst_time);

	/*
	* Permet de fixer le type de discr�tisation (DAY ou WEEK)
	*@param in : type de la discr�tisation
	*/
	void  setDiscretisation(DISCRETISATION_TYPE type);

	DISCRETISATION_TYPE m_discretisation;

	//fixer par rapport au type de discr�tisation souhait�
	double m_DT;
	double  m_NBDISCRETISATION;
	int m_FINALDATE;

public : 

	Model(int nbPath);
	~Model();

	bool Model::CheckParameter();

	int Nb_Path() const { return m_nbPath; }
	void Nb_Path(int val) { m_nbPath = val; }

	/*
	* Diffuser les actifs � partir du temps time jusqu'� la maturit� en ne calculant les valeurs qu'aux dates de fixing
	*@param in/out : matrice remplies de time � maturit� avec les valeurs calcul�es
	*@param in : drift du mod�le
	*@param in : produit
	*@param in : rng du mod�le
	*@param in : temps depuis lequel on diffuse
	*@param in : date de constatation du produit
	*@param in : type de discr�tisation
	*/
	void Model::Diffuse_from_t(PnlMat * path, const PnlVect *drift, Produit * produit, PnlRng * rng, int time,  std::vector<int> lst_time, DISCRETISATION_TYPE l_discretisation);
	
	//inutile
	void Model::Diffuse_from_t(PnlMat * path, const PnlVect *drift, Produit * produit, PnlRng * rng, int time);

	/*
	* Calcule de la couverture risqu�e et non risqu�e
	*@param in : vecteur des deltas
	*@param in : liste des actifs risqu�s d�tenus
	*@param in : liste de la quantit�s d'actifs sans risque d�tenu � chaque instant t
	*@param in : vecteur des deltas
	*@param in : vecteur spots
	*@param in : temps
	*/
	void Model::Simul_Market(
		std::vector<PnlVect *> &vec_delta, 
		std::vector<double> &vec_priceCouverture, 
		std::vector<double> &vec_actifs_risq, 
		std::vector<double> &vec_sans_risq,
		const PnlVect * delta, 
		const PnlVect* spot, 
		const int time);
	std::vector<int> mvec_fixingDate;
	DISCRETISATION_TYPE Discretisation() const { return m_discretisation; }
	double NBDISCRETISATION() const { return m_NBDISCRETISATION; }
	double DT() const { return m_DT; }
	int FINALDATE() const { return m_FINALDATE; }

};


