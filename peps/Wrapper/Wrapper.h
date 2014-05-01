// Wrapper.h

#pragma once
#include <iostream>

using namespace System;
 
namespace Wrapper {
 
public ref class WrapperClass
 	{
 
 	public:
 		WrapperClass() {};
 			int WrapperClass::getPriceCallEuro(array<double, 2>^ assets, int nbActif, int nbDate);
 	};
}