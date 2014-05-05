#pragma once
#include <string> 

#define DLLEXP   __declspec( dllexport )

class DLLEXP Equity
{
public:

	double value;
	double volatility;
	std::string name;
	Equity(double spot, double vol, std::string Ename);
	Equity(double spot, double vol, int);
	~Equity(void);
};

