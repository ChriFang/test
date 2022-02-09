// testInterface.cpp: 主项目文件。

#include "stdafx.h"
#include "../interface/iMath.h"

#pragma comment(lib, "interface.lib")

using namespace System;

int main(array<System::String ^> ^args)
{
  IMathModulePtr pMathModule = CreateMathModule();
  int out = pMathModule->add(2, 8);
  Console::WriteLine(L"Add 2 and 8: ");
  Console::WriteLine(out);
  system("pause");
  
  ReleaseMathModule();
  return 0;
}
