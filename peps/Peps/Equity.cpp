#include "StdAfx.h"
#include "Equity.h"


Equity::Equity(double spot ,double vol, char* Ename)
{
	value = spot;
	volatility = vol;
	name = "Ename";
}


Equity::~Equity(void)
{
}
