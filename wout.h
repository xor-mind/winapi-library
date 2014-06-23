// wout ( windows output). A text library to make rendering text to a window as easy as using
// cout in a console program. 
//
// You need to give the Wout object a hWnd and a hDC, then you need to choose the default starting x and y position for the text. After that, it's as simple as
// wout << "Hello World!" << endl;
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
	static TCHAR endl[];

public:
	// initiation functions
	Wout();
	Wout(HWND hWnd);
	Wout(HWND hWnd, HDC hDC);
	void SetDefaultValues();
	virtual ~Wout() { window = NULL; if (hDC) ReleaseDC(window, hDC); }

	void SetHandler(HWND hWnd, HDC hDC) { window = hWnd; this->hDC = hDC; m_CursorStartingPosX = m_CursorPosY = 10; }
	void SetHandler(HWND hWnd, HDC hDC, int hdcOffsetX, int hdcOffsetY) { window = hWnd; this->hDC = hDC; m_CursorStartingPosX = m_CursorPosY = 10; this->hdcOffsetX = hdcOffsetX; this->hdcOffsetY = hdcOffsetY; }
	void Wnd( HWND hWnd ) { window = hWnd; }
	void DC( HDC hDC ) { this->hDC = hDC; }
	void InitDelim();
		
	// configuration functions
	void SetCursorStartingPosX(int x) { m_CursorStartingPosX = x; }
	void SetCursorX(int x) { m_CursorPosX = x; }
	void SetCursorY(int y) { m_CursorPosY = y; }
	void SetCursor( int x, int y ) { m_CursorPosX = x; m_CursorPosY = y; }
	void SetTextColor(int color) { m_TextColor = color; }
	// change font height in pixels
	void SetTextHeight( int height ); // currently uses Lucida Sans Unicode font.
	void RestoreOldFont();

	// information about text object
	int GetAverageTextWidth();
	int GetTextHeight();
	TCHAR* GetTextName();
	// OverLoaded Operators
	Wout & operator << (const TCHAR *szText);
	Wout & operator << (const tstring s);

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
	HDC hDC;
	int defReturnSpacing; // default spacing between sentences above and below eachother 
	int defBackground;
	bool centerText;

	bool centerX;

	HFONT oldFont, newFont; // fonts that are used to create new styles of text and restore old one
};



#endif