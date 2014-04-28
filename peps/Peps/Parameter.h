#pragma once
//Monté carlo
#define NBPATH 1000
static const double PERF_MAX = 0.25;
static const double PERF_MIN = -0.50;
#define NB_ACTIFS 4
static const double REMB_ANTI = 1.23;
static const double REMB_N_ANTI  = 0.98;
#define PAS 260
static const double TAUX_ACTUALISATION = 0.05;
//Diffuse from t l_dt 

static const double H = 0.02;
static const double DRIFT = 0.05;

enum DISCRETISATION_TYPE { DAY, WEEK };
static const DISCRETISATION_TYPE DISCRETISATION = WEEK;
//Date de fixing
#define FIXING0 0
#define FIXING1 103
#define FIXING2 155
#define FIXING3 207
#define FIXING4 259
	

//DEBUG
#define PRINTPRICE 1
#define PRINTDELTA 1
#define PRINTGAMMA 0
#define PRINTINTERVAL 0
#define PRINTCOUVERTURE 1