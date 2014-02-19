#pragma once
//Monté carlo
#define NBPATH 10
static const double PERF_MAX = 0.25;
static const double PERF_MIN = -0.50;
#define NB_ACTIFS 4
static const double REMB_ANTI = 1.23;
static const double REMB_N_ANTI  = 0.98;
#define PAS 260
static const double TAUX_ACTUALISATION = 0.05;
//Diffuse from t l_dt 
static const double DT = 1.0/52.0;
static const double  NBSEMAINE = 52.0;
static const double H = 0.2;


//Date de fixing
#define FIXING0 0
#define FIXING1 103
#define FIXING2 155
#define FIXING3 207
#define FIXING4 259
	

//DEBUG
#define PRINTPRICE 0
#define PRINTDELTA 0
#define PRINTGAMMA 0
#define PRINTINTERVAL 0