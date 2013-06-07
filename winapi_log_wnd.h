#ifndef  WINAPI_LOG_WND
#define  WINAPI_LOG_WND

#include "winapi_window.h"
#include <stdio.h>
#include <stdarg.h>

class LogWnd : public Window
{
	HWND m_hEditWnd;

	WPD()
	{
		if ( msg == WM_CREATE )
		{					
			RECT rect = GetClientRect();

			m_hEditWnd = CreateWindow(_T("EDIT"), NULL,  
				WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL, 
				0, 0, rect.right, rect.bottom, Wnd(), NULL, AppInstance, NULL);

			SendMessage(m_hEditWnd, WM_SETFONT, (LPARAM) GetStockObject(SYSTEM_FIXED_FONT), 0);
			SendMessage(m_hEditWnd, EM_LIMITTEXT, 2 * 1024 * 1024, 0);
			return 0;	
		}

		if ( msg == WM_SIZE )
		{
			MoveWindow(m_hEditWnd, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);	
			return 0;
		}

		onWndDestruction
		return DWP;		
	}

public:
	LogWnd() { }
	virtual ~LogWnd() { }
		
	void Log(const TCHAR* format, ...)
	{
		TCHAR buffer[1024];

		va_list ap;

		va_start(ap, format);
		vswprintf(buffer, 1024, format, ap);
		SendMessage(m_hEditWnd, EM_SETSEL, 0xFFFFFF, 0xFFFFFF);
		SendMessage(m_hEditWnd, EM_REPLACESEL, 0, (LPARAM) buffer);

		va_end(ap);
	}
	void DumpRegion(const TCHAR * mess, HRGN hRgn, bool detail, int p1=0)
	{
		if ( mess )
			Log(mess, p1);

		if ( hRgn==NULL )
			Log(_T(" NULL"));
		else
		{
			RECT rect;

			memset(& rect, 0, sizeof(rect));
	
			switch ( GetRgnBox(hRgn, & rect) )
			{
				case NULLREGION: 
					Log(_T(" NULLREGION ")); break;

				case SIMPLEREGION:
					Log(_T(" SIMPLEREGION ")); break;

				case COMPLEXREGION:
					Log(_T(" COMPLEXREGION ")); break;

				default:
					Log(_T(" Error ")); break;
			}

			Log(_T(" RgnBox=[%d, %d, %d, %d) "), rect.left, rect.top, rect.right, rect.bottom);

			int size = GetRegionData(hRgn, 0, NULL);
			int rectcount = 0;

			if ( size )
			{
				RGNDATA * pRegion = (RGNDATA *) new char[size];
				GetRegionData(hRgn, size, pRegion);

				const RECT * pRect = (const RECT *) & pRegion->Buffer;
				rectcount = pRegion->rdh.nCount;

				Log(_T("%d rectangles"), rectcount);

				if ( detail )
				{
					Log(_T("\r\n"));
					for (unsigned i=0; i<pRegion->rdh.nCount; i++)
						Log(_T("rect %d [%d, %d, %d, %d)\r\n"), i, pRect[i].left, pRect[i].top, pRect[i].right, pRect[i].bottom);
				}

				delete [] (char *) pRegion;
			}
			else
				Log(_T("0 rectangle"));
		}

		Log(_T("\r\n"));
	}

};

#endif