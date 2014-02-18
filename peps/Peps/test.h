#pragma once
#include "Produit.h"
#include "Model.h"
#include <pnl/pnl_random.h>

//PnlMat * test = pnl_mat_create(m_produit->getMatHisto()->m, m_produit->getMatHisto()->n);
//TronqCoursHisto(m_produit->getMatHisto(), test, 2);
//pnl_mat_free(&test);

/* Retourne une matrice tronqué en time rempli de 0 size = size histo
* @param in : histo : matrice historique
* @param out : matrice tronqué
* @param in : time temps a la quel on veut tronquer
*/
void TronqCoursHisto(const PnlMat * histo, PnlMat * outTronq, int time) {
	PnlVect* tmp = pnl_vect_create(histo->m);
	if(histo->m != outTronq->m || histo->n != outTronq->n) throw std::logic_error("[FAIL] TronqCoursHisto : matrice non compatible");
	if(time > histo->n || time < 0) throw std::logic_error("[FAIL] TronqCoursHisto : time value incompatible");

	pnl_mat_set_zero(outTronq);
	for (int i = 0 ; i <= time; ++i) {
		pnl_mat_get_col(tmp, histo, i);
		pnl_mat_set_col(outTronq, tmp, i);
	}
	//debug
	//pnl_mat_print(outTronq);
	pnl_vect_free(&tmp);
}

/* remploi matrice au date de fixing
* @param in : matrice simulé plus passé
* @param out : matrice aux dates de fixing
* @param in : vecteur des dates de fixing
*/
//Pour tester on crée un tableau avec les date de fixing
//static const int arr[] = {FIXING0, FIXING1, FIXING2, FIXING3, FIXING4};
//std::vector<int> lvec_fixingDate (arr, arr + sizeof(arr) / sizeof(arr[0]) );
//PnlMat * test = pnl_mat_create(m_produit->getMatHisto()->m,lvec_fixingDate.size());	
//getPathFix(m_produit->getMatHisto(), test, lvec_fixingDate);
//pnl_mat_free(&test);
void getPathFix(const PnlMat * past,  PnlMat * fixing, std::vector<int> lst_time) {
	if (past->m != fixing->m  || fixing->n != lst_time.size()) throw std::logic_error("[FAIL] getPathFix : Matrice size incomptatible");
	if (lst_time[lst_time.size()-1] > past->n) throw std::logic_error("[FAIL] getPathFix : Valeur de temps impossible sur la matrice past");
	PnlVect * tmp = pnl_vect_create(past->m);
	for (int i = 0; i < lst_time.size(); ++i) {
		pnl_mat_get_col(tmp, past, lst_time[i]);
		std::cout << std::endl;
		pnl_mat_set_col(fixing, tmp, i);
	}
	//debug
	pnl_mat_print(fixing);
	pnl_vect_free(&tmp);
}