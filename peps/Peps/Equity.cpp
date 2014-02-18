#include "StdAfx.h"
#include "Equity.h"


Equity::Equity(double spot ,double vol, std::string Ename)
{
	value = spot;
	volatility = vol;
	name = Ename;
}


Equity::~Equity(void)
{
}
