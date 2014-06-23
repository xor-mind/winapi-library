#include "wout.h"
#include "winapi_DataConversion.h"

tstring Wout::delimiters[] = { _T("\\n"), _T("\\c") };

TCHAR Wout::endl[] = _T("\\n");
TCHAR Wout::nline[] = _T("\\n");
TCHAR Wout::center[]  = _T("\\c");
TCHAR Wout::centerx[] = _T("\\x");

//Constructors////////////////////////////////////////////////////////////////////////////////////

Wout::Wout() : window( nullptr ), hDC( nullptr )
{
	SetDefaultValues();
}

Wout::Wout(HWND hWnd)
	: window( hWnd )
{
	hDC = GetDC( window );
	SetDefaultValues();
}

Wout::Wout(HWND hWnd, HDC hDC) 
	: window( hWnd ), hDC( hDC ) 
{
	SetDefaultValues();
}

void Wout::SetDefaultValues() 
{
	centerText = centerX = false;
	hdcOffsetX = hdcOffsetY = 0;
	m_CursorStartingPosX = m_CursorPosX = m_CursorPosY = 10;

	m_TextColor = RGB(255, 255, 255);
	defReturnSpacing = 15;
	defBackground = RGB(0, 0, 0);
	oldFont = newFont = nullptr;
}

//Overloaded Operators//////////////////////////////////////////////////////////////////////////////////////////////

Wout & Wout::operator << (const TCHAR * szText) 
{			 
	// Error Checking on
	if (window == NULL || hDC == NULL)
	{
		MessageBox(NULL, _T("hDC or m_hWnd is NULL in Wout object"), _T("Bad Error Yo!"), MB_ICONEXCLAMATION | MB_OK); 
		return (*this);
	}
	
	GetTextMetrics(hDC, &textInfo);
	::SetTextColor(hDC, m_TextColor);
	SetBkMode(hDC, TRANSPARENT); // AD_TODO: this shouldn't be a constant, change it to a variable
	
	m_CtstringToPrint = szText;
	m_currtstringPos = 0;

	RECT rect;
	GetClientRect(window, &rect);
	
	if ( centerText )
	{
		SetCursorY(rect.bottom/2 - GetTextHeight()/2);
		SetCursorX(rect.right/2 - (GetAverageTextWidth()*tstrlen(szText))/2);
		centerText = false;
	}
	if (centerX)
	{
		SetCursorX(rect.right/2 - (GetAverageTextWidth()*tstrlen(szText))/2);
		centerX = false;
	}

	// Algorithm to put text on screen :)
	while (1)
	{
		if (m_currtstringPos == m_CtstringToPrint.size())
		{
			break;
		}
		
		int indexFound = (int)m_CtstringToPrint.find_first_of('\\', m_currtstringPos); 
		
		if (indexFound == tstring::npos)
		{
			int tstringSizeToPrint = (int)m_CtstringToPrint.size() - m_currtstringPos;
			// Print it
			::SetTextColor(hDC, m_TextColor);
			TextOut(hDC, 
					m_CursorPosX + hdcOffsetX, 
					m_CursorPosY + hdcOffsetY, 
					m_CtstringToPrint.substr(m_currtstringPos, tstringSizeToPrint).c_str(), 
					tstringSizeToPrint);
			m_CursorPosX += tstringSizeToPrint*textInfo.tmAveCharWidth; 
			break;
		}
		else
		{
			// so we found some delimiter x indices away from our current tstring position.
			// lets print the tstring up until that index(delimiter), 
			// then we will take care of the delimiter in the InitDelimiter function
			// and then well go to the top of this loop, until we are done with the tstring.
			int tstringSizeToPrint = (indexFound) - m_currtstringPos;
			// Print it
			::SetTextColor(hDC, m_TextColor);
			TextOut(hDC, 
					m_CursorPosX + hdcOffsetX, 
					m_CursorPosY + hdcOffsetY, 
					m_CtstringToPrint.substr(m_currtstringPos, tstringSizeToPrint).c_str(), 
					tstringSizeToPrint);
			m_CursorPosX += tstringSizeToPrint*textInfo.tmAveCharWidth; 

			m_currtstringPos += tstringSizeToPrint; // so now we're pointing at the delimiter
			InitDelim();
			continue;
		}
	}

	return (*this); 
}

Wout & Wout::operator << (tstring s)
{
	return ( (*this) << s.c_str());
}

//Misc Methods/////////////////////////////////////////////////////////////////////////////////////

void Wout::InitDelim()
{
	// lets move our currtstringPos past the delimiter
	m_currtstringPos++;
	// now lets see what delim we have
	switch (m_CtstringToPrint[m_currtstringPos]) 
	{
		case 'n': {
			m_CursorPosY += textInfo.tmHeight;
			m_CursorPosX = m_CursorStartingPosX; 
		
		}	break;

		case 'd': 
		{
			m_CursorPosY += textInfo.tmHeight;
		}	break;

		case 'c': {
			centerText = true;
		
		} break;

		case 't': {
			 m_CursorPosX += 3*textInfo.tmAveCharWidth; 

		case 'x': {
			centerX = true;
				  }

		} break;
		default : break;	
	}

	// now that we have processed the delimiter code
	// we will move on from it to the next character into the tstring
	m_currtstringPos++;
}

// information about text object

int Wout::GetAverageTextWidth()
{
	GetTextMetrics(hDC, &textInfo);
	return textInfo.tmAveCharWidth;
}

int Wout::GetTextHeight()
{
	GetTextMetrics(hDC, &textInfo);
	return textInfo.tmHeight;
}

int Wout::GetTextName()
{
	GetTextMetrics(hDC, &textInfo);
	return textInfo.
}

void Wout::SetTextHeight( int height ) 
{	
	if ( newFont ) {
		DeleteObject( newFont );
	}
	//else {
		// only get the old font if the new font is not in use because we would write over the original font info.
	//	oldFont = (HFONT)GetCurrentObject( hDC, OBJ_FONT);	
	//}

	LOGFONT      LogFont;
	// Set up the LogFont structure.
		
	// Make sure it fits in the grid.
	LogFont.lfHeight = height; // Allow for whitespace.
		
	LogFont.lfWidth = LogFont.lfEscapement = LogFont.lfOrientation =
		LogFont.lfWeight = 0; // Set these guys to zero.
		
	LogFont.lfItalic = LogFont.lfUnderline = LogFont.lfStrikeOut = 
		LogFont.lfOutPrecision = LogFont.lfClipPrecision =
		LogFont.lfQuality = LogFont.lfPitchAndFamily = 0; // Set these guys to zero.

	// Let the facename and size define the font.
	LogFont.lfCharSet = ANSI_CHARSET;
	lstrcpy(LogFont.lfFaceName, L"Lucida Sans Unicode");
		
	// Create the font.
	//if ( symbolFont )
	//	DeleteObject(symbolFont);
	newFont = CreateFontIndirect(&LogFont);
	//HFONT hOldFont = (HFONT)SelectObject(dc, symbolFont);
	// Set the font back to edit control
	//::SendMessage(w, WM_SETFONT,(WPARAM)font_new,1);

	oldFont = (HFONT)SelectObject(hDC, newFont);
}

void Wout::RestoreOldFont() 
{
	if ( newFont == nullptr ) {
		// error, trying restore old font when it's already been restored.
		__asm int 13;
	}
	SelectObject( hDC, oldFont );
	DeleteObject( newFont );
	newFont = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
