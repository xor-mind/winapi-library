#ifndef WINAPI_RAII
#define WINAPI_RAII

#include <Windows.h>

class SmartHandle
{
public:	
	SmartHandle() {} 
	SmartHandle(HANDLE& h) {}
	virtual ~SmartHandle() {};

	HANDLE get() const { return handle; }
	void set(HANDLE h) { handle = h; }

	SmartHandle& operator = (HANDLE h);
	int operator == (HANDLE h);

protected:
	HANDLE handle;
};

class SmartObjectHandle : public SmartHandle
{
public:
	SmartObjectHandle() { handle = INVALID_HANDLE_VALUE; }
	SmartObjectHandle(HANDLE& h) { handle = h; }
	~SmartObjectHandle() 
	{
		if (handle != INVALID_HANDLE_VALUE) 
		{ 
			CloseHandle(handle); 
		}
	}
};

class SmartFileHandle : public SmartHandle
{
public:
	SmartFileHandle() { handle = INVALID_HANDLE_VALUE; }
	SmartFileHandle(HANDLE& h) { handle = h; }
	~SmartFileHandle() 
	{
		if (handle != INVALID_HANDLE_VALUE) 
		{ 
			FindClose(handle); 
		}
	}

};

struct SmartBrush
{
	SmartBrush(HBRUSH brush) : brush(brush) { }
	~SmartBrush() 
	{
		if (brush) 
		{ 
			DeleteObject(brush); 
		}
	}

	operator HBRUSH() { return brush; }

	HBRUSH brush;
};

#endif


		