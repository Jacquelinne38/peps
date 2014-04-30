#pragma once
#define DLLEXP   __declspec( dllexport )
namespace ComputeEngine{
	DLLEXP void calleuro (double &ic);
}