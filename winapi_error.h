#ifndef WINAPI_ERROR_H
#define WINAPI_ERROR_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <exception>

void PrintLastWinAPIError(LPTSTR lpszFunction);

// todo: implement sensible exception handling
//struct WinapiGeneralException : virtual std::exception 
//{
//	char const* what() const throw() { return "General Winapi Exception"; } 
//};

#endif 