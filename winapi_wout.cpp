#include "winapi_wout.h"
#include "winapi_DataConversion.h"

tstring Wout::delimiters[] = { _T("\\n"), _T("\\c") };

TCHAR Wout::nline[] = _T("\\n");
TCHAR Wout::center[]  = _T("\\c");
TCHAR Wout::centerx[] = _T("\\x");

//Constructors////////////////////////////////////////////////////////////////////////////////////

Wout::Wout()
	: centerText(false), centerX(false), hdcOffsetX(0), hdcOffsetY(0)
{
	Wout(NULL, NULL);
}

Wout::Wout(HWND hWnd, HDC hDC) 
	: hdcOffsetX(0), hdcOffsetY(0)
{
	m_CursorPosX = m_CursorPosY = 10;

	window = hWnd;
	m_hDC = hDC;

	m_TextColor = RGB(255, 255, 255);
	defReturnSpacing = 15;
	defBackground = RGB(0, 0, 0);
}

Wout::Wout(HWND hWnd)
	: hdcOffsetX(0), hdcOffsetY(0)
{
	m_CursorStartingPosX = m_CursorPosY = m_CursorPosX = 10; 
	centerText = centerX = false;
	window = hWnd;
	m_hDC = GetDC(hWnd);

	m_TextColor = RGB(255, 255, 255);
	defReturnSpacing = 15;
	defBackground = RGB(0, 0, 0);
}

//Overloaded Operators//////////////////////////////////////////////////////////////////////////////////////////////

Wout & Wout::operator << (const TCHAR * szText) 
{			 
	// Error Checking on
	if (window == NULL || m_hDC == NULL)
	{
		MessageBox(NULL, _T("m_hDC or m_hWnd is NULL in Wout object"), _T("Bad Error Yo!"), MB_ICONEXCLAMATION | MB_OK); 
		return (*this);
	}
	
	GetTextMetrics(m_hDC, &textInfo);
	::SetTextColor(m_hDC, m_TextColor);
	SetBkMode(m_hDC, TRANSPARENT); // AD_TODO: this shouldn't be a constant, change it to a variable
	
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
			::SetTextColor(m_hDC, m_TextColor);
			TextOut(m_hDC, 
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
			::SetTextColor(m_hDC, m_TextColor);
			TextOut(m_hDC, 
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
//void Wout::CenterText(tstring s)
//{

//}

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
	GetTextMetrics(m_hDC, &textInfo);
	return textInfo.tmAveCharWidth;
}

int Wout::GetTextHeight()
{
	GetTextMetrics(m_hDC, &textInfo);
	return textInfo.tmHeight;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
