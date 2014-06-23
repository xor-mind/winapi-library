#ifndef GDI_WRAPPER_H
#define GDI_WRAPPER_H

#define WIN32_LEAN_AND_MEAN
#include <boost\scoped_ptr.hpp>
#include <windows.h>
#include "winapi_RAII.h"
#include "color.h"
#include "window.h"
#include "2DMath.h"

//template <class T>
//T Tabs(const T& t) { 
//	return ( t < 0 ) ? -t : t;   
//}

template <class T> 
T midpoint(const T& x, const T& x1)
{
	return (x - x1)/2;
}



struct SelectBrushPen
{
	HPEN p, op; HBRUSH b, ob; HDC dc;
	SelectBrushPen(HDC dc, HPEN p, HBRUSH b)
	{
		op = (HPEN)SelectObject(dc, p);
		ob = (HBRUSH)SelectObject(dc, b);
		this->dc = dc; this->p = p; this->b = b;
	}
	~SelectBrushPen()
	{
		SelectObject(dc, op);
		SelectObject(dc, ob);
	}
};
//
//struct CreateSelectBrushPen
//{
//	HPEN p, op; HBRUSH b, ob; HDC dc;
//	CreateSelectBrushPen( HDC dc, COLORREF penColor, COLORREF brushColor )
//	{
//		p = CreatePen( PS_SOLID, penColor, 1 );
//		b = CreateSolidBrush( brushColor );
//		op = (HPEN)SelectObject(dc, p);
//		ob = (HBRUSH)SelectObject(dc, b);
//		this->dc = dc; this->p = p; this->b = b;
//	}
//	~CreateSelectBrushPen()
//	{
//		SelectObject(dc, op);
//		SelectObject(dc, ob);
//		DeleteObject(p); 
//		DeleteObject(b);
//	}
//};

// class DIB is used for DIBs(device independent bitmaps), which are bitmaps that I want direct memory acces to the bitmap's image pixels for
// per pixel effects, such as bluring.
class DIB; 

void TransparentBlit(HDC hdc, int destX, int destY, int destWidth, int destHeight, 
					 HDC hdc2, int srcX, int srcY, UINT tranparency);

class GDI
{
	Window * w;
	GDI* primaryGDI; // not NULL if this is a secondary GDI
	HWND hwnd;
	HDC hdc; // this holds the hDC of the window I'm going to bitblt my back buffer too.
	HDC back_dc;      // for double buffering
	HBITMAP hCompBitmap, hOldCompBitmap;	// This holds the compatible bitmap for the backbuffer
	HDC dibDC;

	HPEN hpen, oldPen;
	HBRUSH hbrush, oldbrush;

	unsigned int screen_size;		
	RECT client_rect; 
	bool isSecondaryGDI;
	RECT doubleBufferedRect;
public:
	GDI() { w = NULL; hwnd = NULL; hdc = back_dc = NULL; isSecondaryGDI = false; }
	virtual ~GDI()
	{
		// clean up everything THIS class initialized, which is alot
		FreeDoubleBuffer();
	}

	// Drawing Points/Lines/Shapes/Etc...
	void DrawPoint(int x, int y, COLORREF color);
	void DrawLine(HDC deviceContext, int x0, int y0, int x1, int y1, COLORREF color);
	void DrawLine(int x0, int y0, int x1, int y1, COLORREF color);
	void DrawRect(int x0, int y0, int x1, int y1, COLORREF color);
	void DrawRect( Vector & position, Vector & dimension, HPEN pen, HBRUSH brush)
	{
		SelectBrushPen sbp( back_dc, pen, brush );
		Rectangle(back_dc, (int) position.x, (int) position.y, (int) dimension.x, (int) dimension.y);
	}
	void DrawRect( Vector & position, Vector & dimension)
	{
		Rectangle(back_dc, (int) position.x, (int) position.y, (int) dimension.x, (int) dimension.y);
	}
	void DrawRoundRect( Vector & position, Vector & dimension, int roundWidth, int roundHeight, HPEN pen, HBRUSH brush )
	{
		SelectBrushPen sbp( back_dc, pen, brush );
		RoundRect(back_dc, (int) position.x, (int) position.y, (int) dimension.x, (int) dimension.y, roundWidth, roundHeight);
	}
	void DrawRoundRect( Vector & position, Vector & dimension, int roundWidth, int roundHeight)
	{
		RoundRect(back_dc, (int) position.x, (int) position.y, (int) dimension.x, (int) dimension.y, roundWidth, roundHeight);
	}
	void DrawEllipse(int x0, int y0, int x1, int y1, COLORREF color);
	void DrawDIB(const POINT& pos, const DIB& dib);
	void BitBlt( Vector & position, Vector & dimension, HBITMAP bitmap, int srcx, int srcy )
	{
		HDC hdcMem = CreateCompatibleDC(back_dc);
		SelectObject(hdcMem, bitmap);

		TransparentBlt( back_dc, (int)position.x, (int)position.y, 
				 (int)dimension.x, (int)dimension.y, hdcMem, 0, 0, (int)dimension.x, (int)dimension.y, RGB(255, 0, 255) );

		DeleteDC(hdcMem);
	}
	
	// return the window we're double buffering
	Window* DoubleBufferedWindow() { return w; }
	virtual void SetupDoubleBuffer(Window* w)
	{
		if ( back_dc )
			FreeDoubleBuffer();
		primaryGDI = this;
		this->w = w;
		hdc = GetDC( w->Wnd() );
		client_rect = w->GetClientRect();  // should this be true?
		doubleBufferedRect = client_rect;
		doubleBufferedRect.left = doubleBufferedRect.top = 0;
		back_dc = CreateCompatibleDC(hdc); // essentially make a copy of the window hdc we want a back buffer too
		hCompBitmap = CreateCompatibleBitmap(hdc,client_rect.right,client_rect.bottom);
		hOldCompBitmap = (HBITMAP)SelectObject(back_dc, hCompBitmap);
		GdiFlush(); // must be called before rastering
	}
	virtual void SetupDoubleBuffer(Window* w, GDI * primaryGDI )
	{
		if ( !w->Wnd() )
			return;
		this->w = w;
		isSecondaryGDI = true;
		this->primaryGDI = primaryGDI;
		back_dc = primaryGDI->BackDC();
		// calculate backBufferRect of this secondaryGDI with respect to the 
		// primaryGDI, which translates to figuring out the differences 
		// between the parent window client child coords.
		POINT p = { primaryGDI->DoubleBufferedWindow()->PaddingLeft(), 
					primaryGDI->DoubleBufferedWindow()->PaddingTop()}, 
			 p1  = {0, 0};
		POINT parentPos = primaryGDI->DoubleBufferedWindow()->ClientToScreen(p);
		POINT ourWindowPos = w->ClientToScreen(p1);
		int dx = ourWindowPos.x  - parentPos.x,
			dy = ourWindowPos.y  - parentPos.y;
		doubleBufferedRect.left = dx; doubleBufferedRect.top = dy;
		doubleBufferedRect.right = w->ClientWidth() +doubleBufferedRect.left ;
		doubleBufferedRect.bottom = w->ClientHeight() + doubleBufferedRect.top;
	}

	void FreeDoubleBuffer()
	{
		if ( isSecondaryGDI )
			return;
		if ( !back_dc )
			return;
		// delete pens/brushes/etc..
		//DeleteObject(hpen);

		// Free the hbitmaps
		DeleteObject(hOldCompBitmap);
		DeleteObject(hCompBitmap);

		// Free all the HDC's
		DeleteDC(back_dc);
		back_dc = 0; hOldCompBitmap = 0; hCompBitmap = 0;

	}
	virtual void UpdateFrame()
	{
		if ( !hdc || !back_dc && !isSecondaryGDI )
			return;
		// Then we draw the back buffer to the front buffer (our window)
		::BitBlt(hdc,w->PaddingLeft(), w->PaddingTop(), client_rect.right,
			   client_rect.bottom,back_dc,0,0,SRCCOPY);
	}
	virtual void ClearScreen(HBRUSH brush)
	{
		if ( !back_dc || !w )
		{
			//"back_dc or Hwnd unassigned in GDI object" 
			__asm int 13;
		}
	
		FillRect( back_dc, &doubleBufferedRect, brush ); 
	}


	// Useful Screen Functions
	bool WINAPI ChangeWindowStyle(HWND hwnd, DWORD dwRemove, DWORD dwAdd, UINT nFlags);

	void ClearScreen(HBRUSH brush, HDC deviceContext);
	bool CreateDIB(HDC targetDC, DIB& dib); 
	//void ClearScreen(COLORREF color);
	bool ChangeDisplayMode(int pixel_width, int pixel_height, bool switch_back);
	virtual RECT DoubleBufferedRect() { return doubleBufferedRect; }
	virtual HDC BackDC() { return back_dc; }
	// set the backDC to target dc
	void BackDC(HDC dc) { back_dc = dc; }
	virtual GDI* PrimaryGDI() { return primaryGDI; }
};

struct WindowGDI : public Window, public GDI
{
	HFONT gdiTextFont;
	virtual void SelectFont(HFONT fontToUse) { gdiTextFont = fontToUse; }
	virtual ~WindowGDI() {}
	using GDI::SetupDoubleBuffer;
	virtual void SetupDoubleBuffer(GDI * primaryDC)
	{
		GDI::SetupDoubleBuffer(this, primaryDC);
	}
	virtual void UpdateFrame() {}
};

//
//struct GdiChildWnd : public GdiParentWnd
//{
//	using Window::InitWnd;
//	virtual ~GdiChildWnd() {};
//	void GetGDI() { gdi.reset( new ChildGDI() ); }
//};
//
//class ChildGDI : public GDI
//{
//	HDC parentBackDC;
//	GdiWnd* pw;
//public:
//	ChildGDI() { pw = NULL; parentBackDC = NULL; }
//	virtual ~ChildGDI() { }
//
//	void ClearScreen(HBRUSH hbr)
//	{
//		if ( ! parentBackDC || !pw)
//		{
//			// _T("back_dc or Hwnd unassigned in GDI object")
//			return;
//		}
//	
//		RECT rect = client_rect;
//		rect.right += client_rect.left; rect.bottom += client_rect.top;
//		//GetClientRect( w->Wnd(), &rect );
//		FillRect( parentBackDC, &rect, hbr );
//		//FillRect( ((GdiParentWnd*)pw->parent)->gdi->back_dc, &rect, hbr ); 
//	}
//	void SetupDoubleBuffer(Window* pw)
//	{
//		if ( !pw->Wnd() || !pw->parent )
//			return;
//		this->pw = (GdiWnd*)( pw);
//		parentBackDC = ((GdiWnd*)this->pw->parent)->BackDC();
//
//		//RECT r = pw->parent->GetClientRect();
//		client_rect = pw->GetClientRect(); 
//		//client_rect.left += 
//	}
//	void SetupDoubleBuffer(Window* pcw)
//	{
//		if ( !pcw->Wnd() || !pcw->parent )
//		
//	}
//
//	virtual HDC BackDC() 
//	{ 
//
//			return parentBackDC;
//	}
//	void UpdateFrame()
//	{
//		//if ( !pw->gdi || !pw->gdi->back_dc )
//		//	__asm int 13;
//
//		//// Then we draw the back buffer to the front buffer (our window)
//		//BitBlt(parentBackDC, 0, 0, client_rect.right,
//		//	   client_rect.bottom,back_dc,0,0,SRCCOPY);
//	}
//};

class DIB 
{
public:

	HBITMAP hBitmap;
	BYTE*   imageBits;
	POINT  dimension; // x = width, y = height

	DIB() {}
	~DIB() {}

	// DrawPixel and GetPixel's x and y parameters represent a bottom to top and left to right coord system.
	// So (0, 0), is the bottom left pixel and (MAX_X, MAX_Y) is the top right pixel. 
	void DrawPixel(int x, int y, Color<DWORD> color);
	void DrawPixel(int x, int y, Color<DWORD> color, int len);
	Color<DWORD> GetPixel(int x, int y);
	//Color<DWORD> GetAverage(int x, int y);
};

void MoveTo(HDC hdc, int x, int y);



#endif