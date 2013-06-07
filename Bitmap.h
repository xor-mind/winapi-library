#ifndef BITMAP_H
#define BITMAP_H

#include <windows.h>
#include <tchar.h>

typedef unsigned int uint;

class Bitmap  
{
public:
	Bitmap ()							: _hBitmap (0)				 {}
	Bitmap (HBITMAP hBitmap)			: _hBitmap (hBitmap)		 {}
	Bitmap (Bitmap & bmp)				: _hBitmap (bmp.Release ())  {}
	Bitmap (HDC canvas, int dx, int dy) : _hBitmap (0)				 { CreateCompatible (canvas, dx, dy); }
	~Bitmap ()				 { Free (); }	

	void operator = (Bitmap & bmp)
	{
		if (bmp._hBitmap != _hBitmap)
		{
			Free ();
			_hBitmap = bmp.Release ();
		}
	}
	operator HBITMAP () { return _hBitmap; }

	HBITMAP Release ()
	{
		HBITMAP h = _hBitmap;
		_hBitmap = 0;
		return h;
	}

	BOOL Load (HINSTANCE hInst, TCHAR const * resName);
	BOOL Load (HINSTANCE hInst, int id);
	BOOL Load (const TCHAR * path);

	void GetSize (int & width, int & height)
	{
		BITMAP bm;
		::GetObject (_hBitmap, sizeof (bm), & bm);
		width = bm.bmWidth;
		height = bm.bmHeight;
	}


	void CreateCompatible (HDC canvas, int width, int height)
	{
		Free ();
		_hBitmap = ::CreateCompatibleBitmap (canvas, width, height);
	}
	void DrawBackground(HWND hWnd, HDC hDC);
	void Free ()
	{
		if (_hBitmap)
			::DeleteObject (_hBitmap);
	}

	static uint backgroundMinimalWidth;
	static uint backgroundMinimalHeight;

protected:
	HBITMAP _hBitmap;
};

#endif