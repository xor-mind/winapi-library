#include "winapi_RAII.h"

// SmartHandle::SmartHandle()
// { 
//	handle = INVALID_HANDLE_VALUE; 
// }
//
//SmartHandle::SmartHandle(HANDLE& h) 
// { 
//	 handle = h; 
// }

SmartHandle& SmartHandle::operator = (HANDLE h)
{
	handle = h;
	return (*this);
}

int SmartHandle::operator == (HANDLE h)
{
	return (handle == h);
}