#ifndef WINAPI_THREAD
#define WINAPI_THREAD

#include <windows.h>
#include "Prerequisites.h"

struct Thread
{
	static DWORD WINAPI ThreadProcHandler( LPVOID lpParameter )
	{
		Thread* t = (Thread *)lpParameter;
		t->ThreadProc(0);
		return 0;
	}

	virtual DWORD WINAPI ThreadProc( LPVOID lpParameter ) = 0;
	
	void CreateThread()
	{
		HANDLE hThread = ::CreateThread(0, 0, ThreadProcHandler, this, 0, 0);
		CloseHandle(hThread);
	}
};

#endif