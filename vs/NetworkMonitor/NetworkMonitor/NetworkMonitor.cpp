// NetworkMonitor.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <algorithm>
#include <locale>
#include <codecvt>

#include <Windows.h>
#include <netlistmgr.h>
#include <atlbase.h>
#include <objbase.h>
#include <wtypes.h>
#include <wlanapi.h>
#include <IPHlpApi.h>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "Iphlpapi.lib")

static std::wstring GUIDToString(const GUID & guid)
{
  OLECHAR guidString[40] = { 0 };
  ::StringFromGUID2(guid, guidString, sizeof(guidString));
  return guidString;
}

static std::string UnicodeToUTF8(const std::wstring & wstr)
{
  std::string ret;
  try
  {
    std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
    ret = wcv.to_bytes(wstr);
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << std::endl;
  }
  return ret;
}

class NetWorkEvent : public INetworkConnectionEvents
{
private:
  LONG _ref;
  std::function<void()> _callback;

public:
  NetWorkEvent(const std::function<void()> & cb);

  virtual HRESULT STDMETHODCALLTYPE NetworkConnectionConnectivityChanged(
    /* [in] */ GUID connectionId,
    /* [in] */ NLM_CONNECTIVITY newConnectivity);

  virtual HRESULT STDMETHODCALLTYPE NetworkConnectionPropertyChanged(
    /* [in] */ GUID connectionId,
    /* [in] */ NLM_CONNECTION_PROPERTY_CHANGE flags);

  STDMETHODIMP QueryInterface(REFIID refIID, void **pIFace);
  virtual ULONG __stdcall AddRef(void);
  virtual ULONG __stdcall Release(void);
};

NetWorkEvent::NetWorkEvent(const std::function<void()>& cb)
  : _callback(cb)
{
}

HRESULT NetWorkEvent::NetworkConnectionConnectivityChanged(GUID connectionId, NLM_CONNECTIVITY newConnectivity)
{
  std::wcout << GUIDToString(connectionId) << " | NUL_CONNECTIVITY : " << newConnectivity << std::endl;
  if (_callback)
  {
    _callback();
  }
  return S_OK;
}

HRESULT NetWorkEvent::NetworkConnectionPropertyChanged(GUID connectionId, NLM_CONNECTION_PROPERTY_CHANGE flags)
{
  return S_OK;
}

STDMETHODIMP NetWorkEvent::QueryInterface(REFIID refIID, void ** pIFace)
{
  HRESULT hr = S_OK;
  *pIFace = NULL;
  if (IsEqualIID(refIID, IID_IUnknown))
  {
    *pIFace = (IUnknown *)this;
    ((IUnknown *)*pIFace)->AddRef();
  }
  else if (IsEqualIID(refIID, IID_INetworkConnectionEvents))
  {
    *pIFace = (INetworkConnectionEvents *)this;
    ((IUnknown *)*pIFace)->AddRef();
  }
  else
  {
    hr = E_NOINTERFACE;
  }

  return hr;
}

ULONG NetWorkEvent::AddRef(void)
{
  return (ULONG)InterlockedIncrement(&_ref);
}

ULONG NetWorkEvent::Release(void)
{
  LONG Result = InterlockedDecrement(&_ref);
  if (Result == 0)
    delete this;
  return (ULONG)Result;
}

enum NetworkType
{
  NotNetwork,
  Ethernet,
  Wlan,
};

enum WiFiQuality
{
  Weak,             // (-INF, -70dBm)
  Fair,             // [-70dBm, -60dBm)
  Good,             // [-60dBm, -50dBm)
  Excellent         // [-50dBm, +INF)
};

struct ConnectionInfo
{
  std::wstring guid;
  NetworkType type;
};

class NetworkMonitor
{
  friend void OnNotificationCallback(PWLAN_NOTIFICATION_DATA Data, PVOID context);
public:
  NetworkMonitor();
  ~NetworkMonitor();

  std::vector<ConnectionInfo> GetNetworkConnections();
  NetworkType GetNetAdpaterType(const std::wstring &guid);
  void OnNetworkStatusChange();
  WiFiQuality GetWiFiSignalQuality(const std::wstring &guid);
  void OnWiFiQualityChange(const GUID & guid);

  void ShowNetworkStatus();

private:
  std::unique_ptr<NetWorkEvent> _networkEvent;

  DWORD _cookie;
  CComPtr<INetworkListManager> _pNLM;
  CComPtr<IConnectionPointContainer> _pCpc;
  CComPtr<IConnectionPoint> _pConnectionPoint;

  HANDLE _wlanHandle;
};

static NetworkMonitor * InstanceOfNetworkMonitor()
{
  static NetworkMonitor instance;
  return &instance;
}

NetworkMonitor::NetworkMonitor()
{
  CoInitializeEx(NULL, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);

  HRESULT hr = CoCreateInstance(CLSID_NetworkListManager, NULL, CLSCTX_ALL, IID_INetworkListManager, (LPVOID *)&_pNLM);
  hr = _pNLM->QueryInterface(IID_IConnectionPointContainer, (void **)&_pCpc);
  hr = _pCpc->FindConnectionPoint(IID_INetworkConnectionEvents, &_pConnectionPoint);

  _networkEvent = std::make_unique<NetWorkEvent>(std::bind(&NetworkMonitor::OnNetworkStatusChange, this));
  hr = _pConnectionPoint->Advise((IUnknown *)_networkEvent.get(), &_cookie);
}

NetworkMonitor::~NetworkMonitor()
{
  if (_pConnectionPoint)
  {
    _pConnectionPoint->Unadvise(_cookie);
  }

  CloseHandle(_wlanHandle);

  CoUninitialize();
}

void OnNotificationCallback(PWLAN_NOTIFICATION_DATA Data, PVOID context)
{
  if (Data != NULL &&
    Data->NotificationSource == WLAN_NOTIFICATION_SOURCE_MSM &&
    Data->NotificationCode == wlan_notification_msm_signal_quality_change)
  {
    WLAN_SIGNAL_QUALITY Qality = (WLAN_SIGNAL_QUALITY)Data->pData;
    std::cout << "WiFi OnNotification Qality : " << Qality << std::endl;
    InstanceOfNetworkMonitor()->OnWiFiQualityChange(Data->InterfaceGuid);
  }
}

std::vector<ConnectionInfo> NetworkMonitor::GetNetworkConnections()
{
  std::vector<ConnectionInfo> result;

  CComPtr<IEnumNetworkConnections> enumConnectons;
  if (FAILED(_pNLM->GetNetworkConnections(&enumConnectons)))
  {
    std::cerr << "GetNetworkConnections error : " << GetLastError() << std::endl;
    return result;
  }
  
  if (enumConnectons)
  {
    ULONG lFetch;
    INetworkConnection *connection = nullptr;
    while (SUCCEEDED(enumConnectons->Next(1, &connection, &lFetch)) && nullptr != connection)
    {
      VARIANT_BOOL isConnectInternet = VARIANT_FALSE;
      connection->get_IsConnectedToInternet(&isConnectInternet);
      if (isConnectInternet == VARIANT_FALSE)
      {
        continue;
      }
      ConnectionInfo item;
      GUID guid;

      connection->GetAdapterId(&guid);
      item.guid = GUIDToString(guid);

      result.push_back(item);
    }
    if (connection)
    {
      connection->Release();
    }
  }

  for (auto it = result.begin(); it != result.end(); ++it)
  {
    it->type = GetNetAdpaterType(it->guid);
    std::wcout << "connect network guid : " << it->guid << " | type : " << it->type << std::endl;
  }
  std::partition(result.begin(), std::partition(result.begin(), result.end(), [](const ConnectionInfo & info)
  {
    return info.type != NetworkType::NotNetwork;
  }), [](const ConnectionInfo & info)
  {
    return info.type == NetworkType::Ethernet;
  });
  for (auto it = result.begin(); it != result.end(); ++it)
  {
    std::wcout << "connect network guid : " << it->guid << " | type : " << it->type << std::endl;
  }
  return result;
}

NetworkType NetworkMonitor::GetNetAdpaterType(const std::wstring & guid)
{
  unsigned long unSize = sizeof(IP_ADAPTER_INFO);
  std::unique_ptr<uint8_t[]> data = std::make_unique<uint8_t[]>(unSize);
  bool find = false;
  unsigned long unResult = GetAdaptersInfo(reinterpret_cast<PIP_ADAPTER_INFO>(data.get()), &unSize);

  if (ERROR_BUFFER_OVERFLOW == unResult)
  {
    std::cout << "=== ERROR_BUFFER_OVERFLOW" << std::endl;
    data = std::make_unique<uint8_t[]>(unSize);
    unResult = GetAdaptersInfo(reinterpret_cast<PIP_ADAPTER_INFO>(data.get()), &unSize);
  }
  if (ERROR_SUCCESS == unResult)
  {
    PIP_ADAPTER_INFO pIpAdapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(data.get());

    while (pIpAdapterInfo)
    {
      std::cout << "pIpAdapterInfo->AdapterName=" << pIpAdapterInfo->AdapterName << std::endl;
      if (UnicodeToUTF8(guid) == pIpAdapterInfo->AdapterName)
      {
        std::cout << "pIpAdapterInfo->Type=" << pIpAdapterInfo->Type << std::endl;
        return MIB_IF_TYPE_ETHERNET == pIpAdapterInfo->Type ? NetworkType::Ethernet :
          IF_TYPE_IEEE80211 == pIpAdapterInfo->Type ? NetworkType::Wlan : NetworkType::NotNetwork;
      }
      pIpAdapterInfo = pIpAdapterInfo->Next;
    }
  }

  std::cout << "return NotNetwork" << std::endl;

  return NetworkType::NotNetwork;
}

void NetworkMonitor::OnNetworkStatusChange()
{
  ShowNetworkStatus();
}

WiFiQuality NetworkMonitor::GetWiFiSignalQuality(const std::wstring & guid)
{
  WiFiQuality result = WiFiQuality::Weak;
  DWORD dwMaxClient = 2;
  DWORD dwCurVersion = 0;
  DWORD dwResult = 0;

  unsigned int i, j;

  PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
  PWLAN_INTERFACE_INFO pIfInfo = NULL;

  PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;
  PWLAN_AVAILABLE_NETWORK pBssEntry = NULL;

  int iRSSI = 0;

  if (_wlanHandle == NULL)
  {
    dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &_wlanHandle);
    if (dwResult != ERROR_SUCCESS)
    {
      std::cerr << "WlanOpenHandle failed with error: " << dwResult << std::endl;
      return result;
    }
    dwResult = WlanRegisterNotification(_wlanHandle, WLAN_NOTIFICATION_SOURCE_ALL, TRUE, WLAN_NOTIFICATION_CALLBACK(OnNotificationCallback), NULL, NULL, NULL);
    if (dwResult != ERROR_SUCCESS)
    {
      std::cerr << "WlanRegisterNotification failed with error: " << dwResult << std::endl;
      return result;
    }
  }

  dwResult = WlanEnumInterfaces(_wlanHandle, NULL, &pIfList);
  if (dwResult != ERROR_SUCCESS)
  {
    std::cerr << "WlanEnumInterfaces failed with error: " << dwResult << std::endl;
    return result;
  }
  for (i = 0; i < (int)pIfList->dwNumberOfItems; i++)
  {
    pIfInfo = (WLAN_INTERFACE_INFO *)&pIfList->InterfaceInfo[i];
    if (guid != GUIDToString(pIfInfo->InterfaceGuid) || pIfInfo->isState != wlan_interface_state_connected)
    {
      continue;
    }

    dwResult = WlanGetAvailableNetworkList(_wlanHandle,
      &pIfInfo->InterfaceGuid,
      0,
      NULL,
      &pBssList);

    if (dwResult != ERROR_SUCCESS)
    {
      std::cerr << "WlanGetAvailableNetworkList failed with error:" << dwResult << std::endl;
      return result;
    }
    for (j = 0; j < pBssList->dwNumberOfItems; j++)
    {
      pBssEntry = (WLAN_AVAILABLE_NETWORK *)& pBssList->Network[j];

      if (pBssEntry->bNetworkConnectable && (pBssEntry->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED))
      {
        if (pBssEntry->wlanSignalQuality == 0)
          iRSSI = -100;
        else if (pBssEntry->wlanSignalQuality == 100)
          iRSSI = -50;
        else
          iRSSI = -100 + (pBssEntry->wlanSignalQuality / 2);

        std::cout << "Signal Quality:\t " << pBssEntry->wlanSignalQuality << " (RSSI: " << iRSSI << " dBm)" << std::endl;

        result = iRSSI < -70 ? WiFiQuality::Weak :
          iRSSI < -60 ? WiFiQuality::Fair :
          iRSSI < -50 ? WiFiQuality::Good : WiFiQuality::Excellent;
      }
    }
  }
  if (pBssList != NULL)
  {
    WlanFreeMemory(pBssList);
    pBssList = NULL;
  }

  if (pIfList != NULL)
  {
    WlanFreeMemory(pIfList);
    pIfList = NULL;
  }
  return result;
}

void NetworkMonitor::OnWiFiQualityChange(const GUID & guid)
{
  auto nowQuality = GetWiFiSignalQuality(GUIDToString(guid));
  std::cout << "WiFi signal quality now : " << nowQuality << std::endl;
  ShowNetworkStatus();
}

void NetworkMonitor::ShowNetworkStatus()
{
  NetworkType type;
  WiFiQuality quality;
  auto connections = GetNetworkConnections();
  if (connections.empty())
  {
    type = NetworkType::NotNetwork;
  }
  else
  {
    type = connections.front().type;
    if (type == NetworkType::Wlan)
    {
      quality = GetWiFiSignalQuality(connections.front().guid);
    }
  }

  std::cout << "====== Notify ======" << std::endl;
  std::cout << "* Type : " << (type == NetworkType::NotNetwork ? "NetworkError" : type == NetworkType::Ethernet ? "Ethernet" : "WiFi") << std::endl;
  if (type == NetworkType::Wlan)
  {
    std::cout << "* Signal : " << quality + 1 << std::endl;
  }
  std::cout << std::endl;
}

inline bool IsLowerThanWindowsVista()
{
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  OSVERSIONINFOEX os;
  os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
  if (!GetVersionEx((OSVERSIONINFO *)&os))
  {
    return false;
  }
  OSVERSIONINFOEX osviex;
  ZeroMemory(&osviex, sizeof(OSVERSIONINFOEX));
  osviex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
  if (!GetVersionEx((OSVERSIONINFO *)&osviex))
  {
    return false;
  }
  if (os.dwPlatformId == VER_PLATFORM_WIN32_NT)
  {
    if ((os.dwMajorVersion < 6) && (osviex.wProductType == VER_NT_WORKSTATION))
    {
      return true;
    }
  }
  return false;
}

int main()
{
  if (IsLowerThanWindowsVista())
  {
    std::cout << "Lower than vista" << std::endl;
  }
  else
  {
    std::cout << "Vista or higher version" << std::endl;
    InstanceOfNetworkMonitor()->ShowNetworkStatus();
  }

  while (true) std::this_thread::sleep_for(std::chrono::hours(10));
  return 0;
}

