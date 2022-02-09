#include "MathImp.h"


static MathImp* g_pMathModule = NULL;
extern "C" IMath* CreateMathModule()
{
  if (g_pMathModule == NULL)
  {
    g_pMathModule = new MathImp();
  }
  return g_pMathModule;
}

extern "C" void ReleaseMathModule()
{
  if (g_pMathModule != NULL)
  {
    delete g_pMathModule;
  }
  g_pMathModule = NULL;
}

MathImp::MathImp() : m_cRef(1)
{
}

MathImp::~MathImp()
{
}

STDMETHODIMP MathImp::QueryInterface(REFIID riid, void **ppvObj)
{
  if (ppvObj == NULL)
  {
    return E_INVALIDARG;
  }
    
  *ppvObj = NULL;
  if (IsEqualIID(riid, __uuidof(IUnknown)) || IsEqualIID(riid, IID_IUnknown))
  {
    *ppvObj = (IUnknown *)this;
  }

	if (*ppvObj)
	{
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) MathImp::AddRef(void)
{
  return (ULONG)InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) MathImp::Release(void)
{
  LONG ret = InterlockedDecrement(&m_cRef);
  if (ret == 0)
  {
    delete this;
  }
  return ret;
}

int MathImp::add(THIS_ const int x, const int y)
{
  return x + y;
}

int MathImp::sub(THIS_ const int x, const int y)
{
  return x - y;
}