#include "winapi_error.h"
#include "winapi_DataConversion.h"

void PrintLastWinAPIError(LPTSTR lpszFunction)
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				  FORMAT_MESSAGE_FROM_SYSTEM,
				  NULL,
				  dw,
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				  (LPTSTR) &lpMsgBuf,
				  0, NULL );

		lpDisplayBuf = LocalAlloc(LMEM_ZEROINIT, 
							      2*tstrlen((LPTSTR)lpMsgBuf) + 2*tstrlen(lpszFunction) + 160); 
		wsprintf((LPTSTR)lpDisplayBuf, 
			     _T("%s failed with error %d: %s"), 
		         lpszFunction, dw, lpMsgBuf); 
		
		MessageBox(NULL, (LPTSTR)lpDisplayBuf, _T("Error"), MB_OK); 

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
}