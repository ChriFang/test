#pragma once

#include <Unknwn.h>

#undef INTERFACE
#define INTERFACE IMath
DECLARE_INTERFACE_(IMath, IUnknown) 
{
  STDMETHOD_(int, add)(THIS_ const int x, const int y) PURE;
  STDMETHOD_(int, sub)(THIS_ const int x, const int y) PURE;
};



#ifdef __cplusplus
extern "C" {
#endif

typedef IMath *IMathModulePtr;

IMathModulePtr __declspec(dllexport) CreateMathModule();
void __declspec(dllexport) ReleaseMathModule();

#ifdef __cplusplus
}
#endif