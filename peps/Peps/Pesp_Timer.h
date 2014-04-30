#pragma once
#include "stdafx.h"

#define DLLEXP   __declspec( dllexport )
class DLLEXP Pesp_Timer
{


private :
	std::clock_t start;
	double duration;
	double finalDuration;

public : 

	Pesp_Timer();
	void Pesp_Timer::Start();
	void Pesp_Timer::GetTime(std::string txt);
	void Pesp_Timer::Stop();

};
