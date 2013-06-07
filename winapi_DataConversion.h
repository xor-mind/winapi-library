#ifndef DATACONVESION_H
#define DATACONVESION_H

#include <windows.h>
#include <string>
#include <sstream>
#include <tchar.h>

//#define MakePtr(cast, ptr, addValue ) (cast)( (DWORD_PTR)(ptr) + (DWORD_PTR)(addValue) )

#ifdef _UNICODE
#define tstring std::wstring
#define tstringstream std::wstringstream
#define tstrlen wcslen
#define tstrcpy wcscpy
#else
#define tstring std::string
#define tstringstream std::stringstream
#define tstrlen strlen
#define tstrcpy strcpy
#endif

#define tstr tstring

#define lexc PBNC::lexical_cast

namespace PBNC // prevent boost namespace collisions
{

template<class TO, class FROM> 
TO tlexical_cast(FROM blah) 
{ 
	TO t; 
	tstringstream ss; 
	ss<<blah; 
	ss>>t; 
	return t; 
} 

template<class TO, class FROM> 
TO lexical_cast(FROM blah) 
{ 
	TO t; 
	std::stringstream ss; 
	ss<<blah; 
	ss>>t; 
	return t; 
} 
}

#define ToString PBNC::tlexical_cast<tstring> 
//class totchar 
//{
//private:
//	BOOL autoDelete;
//	LPTSTR buffer;
//
//public:
//	totchar(LPCSTR text, BOOL autoDelete = TRUE)
//		: autoDelete(autoDelete)
//	{
//		_ASSERTE(text);
//		int length = strlen(text) + 1;
//		buffer = new TCHAR [length];
//		#if defined(UNICODE) || defined(_UNICODE)
//		mbstowcs(buffer, text, length);
//		#else
//		strcpy(buffer, text);
//		#endif
//	}
//
//	totchar(LPCWSTR text, BOOL autoDelete = TRUE)
//		: autoDelete(autoDelete)
//	{
//		_ASSERTE(text);
//		int length = wcslen(text) + 1;
//		buffer = new TCHAR [length];
//		#if defined(UNICODE) || defined(_UNICODE)
//		wcscpy(buffer, text);
//		#else
//		wcstombs(buffer, text, length);
//		#endif
//	}
//
//	~totchar()
//	{
//		if (autoDelete) {
//			_ASSERTE(buffer);
//			delete [] buffer;
//		}
//	}
//	operator LPTSTR()
//	{
//		_ASSERTE(buffer);
//		return (LPTSTR) buffer;
//	}
//	operator LPCTSTR()
//	{
//		_ASSERTE(buffer);
//		return (LPCTSTR) buffer;
//	}
//};

#endif