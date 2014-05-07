// Wrapper.h

#pragma once
#include <iostream>

using namespace System;
 
namespace Wrapper {
 
public ref class WrapperClass
 	{
 
 	public:
 		WrapperClass() {};
			int WrapperClass::LaunchComputation(array<double, 2>^ assets, 
												array<double, 1>^ vol, 
												array<double, 2>^ corr, 
												int nbActif,
												int nbDate,
												System::Collections::Generic::List<double>^% list_price,
												System::Collections::Generic::List<double>^% list_priceCouverture,
												System::Collections::Generic::List<double>^% list_sans_risq,
												System::Collections::Generic::List<double>^% list_actifs_risq
			);
			array<double, 1>^ WrapperClass::CalcVol(array<double, 2>^ histo, int nbActif, int nbDate, bool debug);
			array<double, 2>^ WrapperClass::CalcCorr(array<double, 2>^ histo, int nbActif, int nbDate, bool debug);
 	};
}