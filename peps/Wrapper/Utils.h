
#pragma once
#include "stdafx.h"
#include <iostream>


using namespace System;

static void ArrayToPnlMat(PnlMat * pnmat, array<double, 2>^ assets, int ligne, int colonne) {
	for(int i = 0 ; i < ligne; i ++) {
		for (int j = 0 ; j < colonne; j++) {
			pnl_mat_set(pnmat, i,j, assets[i, j]);
		}
	}
}

static void ArrayToPnlVect(PnlVect * vect, array<double, 1>^ assets, int ligne) {
	for(int i = 0; i < ligne; ++i) {
		pnl_vect_set(vect, i, assets[i]);
	}
}
static array<double, 2>^ PnlMatToArray(PnlMat * pnmat) {
	//int size = (pnmat->m)*(pnmat->n);
	array<double, 2>^ tmp = gcnew array<double, 2>(pnmat->m, pnmat->n);
	for(int i = 0 ; i < pnmat->m; i ++) {
		for (int j = 0 ; j < pnmat->n; j++) {
				tmp[i, j] = MGET(pnmat, i,j);
		}
	}
	return tmp;
}
static array<double, 1>^ PnlVectToArray(PnlVect * pnvect) {
	array<double, 1>^ tmp = gcnew array<double, 1>(pnvect->size);
	for(int i = 0 ; i < pnvect->size; i ++) {
			tmp[i] = pnl_vect_get(pnvect, i);
	}
	return tmp;
}