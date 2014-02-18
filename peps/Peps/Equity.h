#pragma once
#include <string> 


class Equity
{
public:

	double value;
	double volatility;
	std::string name;
	Equity(double spot, double vol, std::string Ename);
	~Equity(void);
};

