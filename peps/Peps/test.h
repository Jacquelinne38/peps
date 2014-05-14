#pragma once
#include "Produit.h"
#include "Model.h"
#include <pnl/pnl_random.h>

/* Retourne une matrice tronquée en time remplie de 0  à time par les valeurs historiques
* size = size histo
* @param in : histo : matrice historique
* @param out : matrice tronquée
* @param in : time temps auquel on souhaite tronquer
*/
void TronqCoursHisto(const PnlMat * histo, PnlMat * outTronq, int time) {
	PnlVect* tmp = pnl_vect_create(histo->m);
	if(histo->m != outTronq->m || histo->n != outTronq->n) throw std::logic_error("[FAIL] TronqCoursHisto : matrice non compatible");
	if(time > histo->n || time < 0) throw std::logic_error("[FAIL] TronqCoursHisto : time value incompatible");

	#pragma omp parallel for
	for (int i = 0 ; i <= time; ++i) {
		pnl_mat_get_col(tmp, histo, i);
		pnl_mat_set_col(outTronq, tmp, i);
	}
	pnl_vect_free(&tmp);
}

/* renvoie la matrice contenant la valeur des actifs aux date de fixing
* a partir de la matrice contenant la valeur des actifs à chaque date
* @param in : matrice simulée plus les cours passés
* @param out : matrice aux dates de fixing
* @param in : vecteur des dates de fixing
*/
void getPathFix(const PnlMat * past,  PnlMat * fixing, std::vector<int> lst_time) {
	if (past->m != fixing->m  || fixing->n != lst_time.size()) throw std::logic_error("[FAIL] getPathFix : Matrice size incomptatible");
	if (lst_time[lst_time.size()-1] > (past->n -1)) throw std::logic_error("[FAIL] getPathFix : Valeur de temps impossible sur la matrice past");
	PnlVect * tmp = pnl_vect_create(past->m);
	for (unsigned int i = 0; i < lst_time.size(); ++i) {
		pnl_mat_get_col(tmp, past, lst_time[i]);
		pnl_mat_set_col(fixing, tmp, i);
	}
	pnl_vect_free(&tmp);
}