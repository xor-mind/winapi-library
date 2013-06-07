#ifndef WOUT_H																			
#define WOUT_H			

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "winapi_DataConversion.h"

class Wout 
{
public:
	
	static tstring delimiters[];
	static TCHAR nline[];
	static TCHAR center[];
	static TCHAR centerx[];

protected:
	int hdcOffsetX, hdcOffsetY;
	COLORREF m_TextColor;
	int m_CursorPosX;  // x position for the beginning of the text box i.e. cursor
	int m_CursorPosY;  // y position for the beginning of the text box i.e. cursor
	int m_CursorStartingPosX; // this will reset the x position of the cursor when a newline occurs
	int m_strLength; 

	TEXTMETRIC textInfo; // to get info about current api text state for formatting purposes
	tstring m_CtstringToPrint;
	DWORD m_currtstringPos;
	HWND window;
	HDC m_hDC;
	int defReturnSpacing; // default spacing between sentences above and below eachother 
	int defBackground;
	bool centerText;

	bool centerX;
	
public:
	// initiation functions
	Wout();
	Wout(HWND hWnd);
	Wout(HWND hWnd, HDC hDC);
	virtual ~Wout() { window = NULL; if (m_hDC) ReleaseDC(window, m_hDC); }

	void SetHandler(HWND hWnd, HDC hDC) { window = hWnd; m_hDC = hDC; m_CursorStartingPosX = m_CursorPosY = 10; }
	void SetHandler(HWND hWnd, HDC hDC, int hdcOffsetX, int hdcOffsetY) { window = hWnd; m_hDC = hDC; m_CursorStartingPosX = m_CursorPosY = 10; this->hdcOffsetX = hdcOffsetX; this->hdcOffsetY = hdcOffsetY; }
	void InitDelim();
		
	// configuration functions
	void SetCursorStartingPosX(int x) { m_CursorStartingPosX = x; }
	void SetCursorX(int x) { m_CursorPosX = x; }
	void SetCursorY(int y) { m_CursorPosY = y; }
	void SetTextColor(int color) { m_TextColor = color; }

	// information about text object
	int GetAverageTextWidth();
	int GetTextHeight();

	// OverLoaded Operators
	Wout & operator << (const TCHAR *szText);
	Wout & operator << (const tstring s);
};



#endif