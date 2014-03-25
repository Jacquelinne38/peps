#include "stdafx.h"
#include "Pesp_Timer.h"



void Pesp_Timer::Start() {
	this->start = std::clock();
}

void Pesp_Timer::GetTime(std::string txt) {
	this->duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	std::cout<< txt << " : " << this->duration <<'\n';
}


Pesp_Timer::Pesp_Timer() {

}
