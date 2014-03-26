#include "stdafx.h"
#include "Pesp_Timer.h"



void Pesp_Timer::Start() {
	this->start = std::clock();
}

void Pesp_Timer::GetTime(std::string txt) {
	//Stop();
	this->duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	std::cout<< txt << " : " << this->duration <<'\n';
	//Start();
}

void Pesp_Timer::Stop() {
	this->duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
}


Pesp_Timer::Pesp_Timer() {
	this->duration = 0;
}
