// Wrapper.h

#pragma once
#include <iostream>

using namespace System;
 
namespace Wrapper {
 
public ref class WrapperClass
 	{
 
 	public:
 		WrapperClass() {};
			int WrapperClass::LaunchComputation(array<double, 2>^ assets, array<double, 1>^ vol, array<double, 2>^ corr, int nbActif, int nbDate);
			array<double, 1>^ WrapperClass::CalcVol(array<double, 2>^ histo, int nbActif, int nbDate, bool debug);
			array<double, 2>^ WrapperClass::CalcCorr(array<double, 2>^ histo, int nbActif, int nbDate, bool debug);
 	};
}