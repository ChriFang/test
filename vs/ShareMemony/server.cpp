#include <windows.h>
#include <iostream> 
using namespace std;

#define BUF_SIZE 4096

HANDLE g_EventRead;		// 读信号灯
HANDLE g_EventWrite;	// 写信号灯
// 定义共享数据
char szBuffer[] = "LinXi07";
/* 读取con1串口的线程 */
DWORD __stdcall WriteThread(const LPVOID lp)
{
	while (true)
	{
		WaitForSingleObject(g_EventWrite, INFINITE); // 等待读数据的信号

		// 将数据拷贝到共享内存
		strcpy((char*)lp, szBuffer);
		cout << "server sent, waiting for client: " << (char*)lp << endl;
		
		Sleep(1000);
		
		SetEvent(g_EventRead);
		ResetEvent(g_EventWrite);
	}
	return DWORD();
}

int main()
{

	// 创建共享文件句柄 
	HANDLE hMapFile = CreateFileMappingA(
		INVALID_HANDLE_VALUE,   // 物理文件句柄  NVALID_HANDLE_VALUE  则创建一个进程间共享的对象
		NULL,   // 默认安全级别
		PAGE_READWRITE,   // 可读可写
		0,   // 高位文件大小
		BUF_SIZE,   // 低位文件大小
		"ShareMemoryPDU"   // 映射文件名，即共享内存的名称
	);

	if (0 == hMapFile)
	{
		return 0;
	}

	// 映射缓存区视图 , 得到指向共享内存的指针
	// 将hFileMapping共享内存衍射到本进程的地址空间中
	LPVOID lpBase = MapViewOfFile(
		hMapFile,            // 共享内存的句柄
		FILE_MAP_ALL_ACCESS, // 可读写许可
		0,
		0,
		BUF_SIZE
	);

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

	HANDLE handle = CreateThread(NULL, 0, WriteThread, lpBase, 0, NULL);

	WaitForSingleObject(handle, INFINITE);
	
	// 解除文件映射
	UnmapViewOfFile(lpBase);
	
	// 关闭内存映射文件对象句柄
	CloseHandle(hMapFile);
	return 0;
}