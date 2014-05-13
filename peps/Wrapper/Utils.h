
#pragma once
#include "stdafx.h"
#include <iostream>
#include "Parameter.h"


using namespace System;


/************************************************************************/
/* 
		Classe utils pour transfert de donnée entre code managé et non managé.

*/
/************************************************************************/

static System::Collections::Generic::List<double>^ NatifToManaged(std::vector<double> vector) {
	System::Collections::Generic::List<double>^ lst =  gcnew System::Collections::Generic::List<double>();
	for(int i = 0 ; i < vector.size(); ++i) {
		lst->Add(vector[i]);
	}
	return lst;
}



static void ArrayHistoToPnlMat(PnlMat * pnmat, array<double, 2>^ assets, int ligne, int colonne) {
	if(DISCRETISATION == WEEK)
	for(int i = 0 ; i < ligne; i ++) {
		int k = 0;
		for (int j = 0; j < colonne*5; j=j+5) {
			pnl_mat_set(pnmat, i,k, assets[i, j]);
			//std::cout << "Actif " << i << " " << j << " " << "value : " << assets[i, j] << std::endl;
			k++;
		}
	} else {
		for(int i = 0 ; i < ligne; i ++) {
			for (int j = 0; j < colonne; j++) {
				pnl_mat_set(pnmat, i,j, assets[i, j]);
				//std::cout << "Actif " << i << " " << j << " " << "value : " << assets[i, j] << std::endl;
			}
		}
	}
}

static void ArrayToPnlMat2(PnlMat * pnmat, array<double, 2>^ assets, int ligne, int colonne) {
	for(int i = 0 ; i < ligne; i ++) {
		for (int j = 0 ; j < colonne; j++) {
			pnl_mat_set(pnmat, i,j, assets[i, j]);
		}
	}
}

static void ArrayToPnlMat(PnlMat * pnmat, array<double, 2>^ assets, int ligne, int colonne) {
	for(int i = 0 ; i < ligne; i ++) {
		for (int j = 0; j < colonne; j++) {
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