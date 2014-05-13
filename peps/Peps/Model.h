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
	*@param in : drift du modèle
	*@param in : vecteur des tirages effectués
	*@param in : matrice de cholesky des corrélations
	*@param in/out : vecteur spot
	*/
	void Diffuse_from_t_all_Asset(Produit * produit,const PnlVect * drift, const PnlVect * vecAlea, const PnlMat * choleskyCor, PnlVect * spot);

	/*
	*Calcule le spot dt jours/semaines plus tard
	*@param in : produit
	*@param in : drift du modèle
	*@param in : vecteur des tirages effectués
	*@param in : matrice de cholesky des corrélations
	*@param in/out : vecteur spot
	*@param in : pas de temps
	*/
	void  Diffuse_of_dt(Produit * produit,const PnlVect * drift, const PnlVect * vecAlea, const PnlMat * choleskyCor, PnlVect * spot,const int dt);

	/*
	* Met dans un vecteur en premier élément la date time puis les dates de fixing qui suivent
	*@param in : temps t
	*@param out : vecteur contenant les dates
	* return : liste des dates
	*/
	std::vector<int>  getFixingDateFromt(int time, std::vector<int> lst_time);

	/*
	* Permet de fixer le type de discrétisation (DAY ou WEEK)
	*@param in : type de la discrétisation
	*/
	void  setDiscretisation(DISCRETISATION_TYPE type);

	DISCRETISATION_TYPE m_discretisation;

	//fixer par rapport au type de discrétisation souhaité
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
	* Diffuser les actifs à partir du temps time jusqu'à la maturité en ne calculant les valeurs qu'aux dates de fixing
	*@param in/out : matrice remplies de time à maturité avec les valeurs calculées
	*@param in : drift du modèle
	*@param in : produit
	*@param in : rng du modèle
	*@param in : temps depuis lequel on diffuse
	*@param in : date de constatation du produit
	*@param in : type de discrétisation
	*/
	void Model::Diffuse_from_t(PnlMat * path, const PnlVect *drift, Produit * produit, PnlRng * rng, int time,  std::vector<int> lst_time, DISCRETISATION_TYPE l_discretisation);
	
	//inutile
	void Model::Diffuse_from_t(PnlMat * path, const PnlVect *drift, Produit * produit, PnlRng * rng, int time);

	/*
	* Calcule de la couverture risquée et non risquée
	*@param in : vecteur des deltas
	*@param in : liste des actifs risqués détenus
	*@param in : liste de la quantités d'actifs sans risque détenu à chaque instant t
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


