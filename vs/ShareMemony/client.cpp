#include <iostream>  
#include <windows.h>  
#include <string>
using namespace std;

#define BUF_SIZE 4096

HANDLE g_EventRead;		// 读信号灯
HANDLE g_EventWrite;	// 写信号灯


DWORD __stdcall ReadThread(const LPVOID lp)
{
	while (true)
	{
		WaitForSingleObject(g_EventRead, INFINITE); // 等待读数据的信号
	// 将数据拷贝到共享内存

	// 将共享内存数据拷贝出来
		char szBuffer[BUF_SIZE]{ 0 };
		
		strcpy_s(szBuffer, (char*)lp);
		
		std::cout << "client read data success: " << szBuffer << endl;

		ResetEvent(g_EventRead); /* 将读取信号关闭  */
		SetEvent(g_EventWrite);
	}
}

int main()
{
	// 打开共享的文件对象
	HANDLE hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, NULL, "ShareMemoryPDU");

	if (0 == hMapFile)
	{
		// 打开共享内存句柄失败
		std::cout << "open share file failed" << endl;
		return 0;
	}
	LPVOID lpBase = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (0 == lpBase)
	{
		return 0;
	}

	g_EventRead = CreateEventA(NULL, TRUE, FALSE, "EventRead");
	if (nullptr == g_EventRead)
	{
		return 0;
	}

	g_EventWrite = CreateEventA(NULL, TRUE, TRUE, "EventWrite");
	if (nullptr == g_EventRead)
	{
		return 0;
	}

	HANDLE handle = CreateThread(NULL, 0, ReadThread, lpBase, 0, NULL);
	if (0 == handle)
	{
		return 0;
	}
	
	WaitForSingleObject(handle, INFINITE);
	// 解除文件映射
	
	UnmapViewOfFile(lpBase);
	// 关闭内存映射文件对象句柄
	CloseHandle(hMapFile);


	return 0;
}