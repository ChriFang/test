#pragma once

#include "iMath.h"

class MathImp : public IMath
{
public:
  MathImp();
  virtual ~MathImp();

public:
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, _COM_Outptr_ void** ppvObj) override;
  STDMETHOD_(ULONG, AddRef) (THIS) override;
  STDMETHOD_(ULONG, Release) (THIS) override;

public:
  STDMETHOD_(int, add)(THIS_ const int x, const int y) override;
  STDMETHOD_(int, sub)(THIS_ const int x, const int y) override;

private:
  volatile LONG m_cRef;
};