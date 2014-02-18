#pragma once
#include "Produit.h"
#include "Model.h"
#include <pnl/pnl_random.h>

//PnlMat * test = pnl_mat_create(m_produit->getMatHisto()->m, m_produit->getMatHisto()->n);
//TronqCoursHisto(m_produit->getMatHisto(), test, 2);
//pnl_mat_free(&test);

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