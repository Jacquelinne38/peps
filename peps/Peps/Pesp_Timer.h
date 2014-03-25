#pragma once
#include "stdafx.h"


class Pesp_Timer
{


private :
	std::clock_t start;
	double duration;

public : 

	Pesp_Timer();
	void Pesp_Timer::Start();
	void Pesp_Timer::GetTime(std::string txt);

};
