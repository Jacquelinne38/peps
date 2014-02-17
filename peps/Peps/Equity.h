#pragma once
#include <string> 


class Equity
{
public:

	double value;
	double volatility;
	char* name;
	Equity(double spot, double vol, char* Ename);
	~Equity(void);
};

