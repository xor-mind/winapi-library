#include "gdi.h"
#include "winapi_DataConversion.h"

void TransparentBlit(HDC hdc, int destX, int destY, int destWidth, int destHeight, 
					 HDC hdc2, int srcX, int srcY, UINT tranparency)
{
	unsigned char* pImageBits;
	unsigned char* pBackBits;
	BITMAPINFO bmBitmapInfo = {0};
	HBITMAP hBitmap, hBitmap2, hOldBitmap, hOldBitmap2;
	HDC compHDC;
	HDC compHDC2;
	
	// Create a valid address for our image bits
	pImageBits = (unsigned char *)malloc(sizeof(unsigned char));
	memset(pImageBits, 0, sizeof(unsigned char));

	// Fill in our BitmapInfo structure (we want a 24 bit image)
	bmBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmBitmapInfo.bmiHeader.biCompression = BI_RGB;
	bmBitmapInfo.bmiHeader.biHeight = destHeight;
	bmBitmapInfo.bmiHeader.biWidth = destWidth;	
	bmBitmapInfo.bmiHeader.biBitCount = 24;
	bmBitmapInfo.bmiHeader.biClrUsed = 0;
	bmBitmapInfo.bmiHeader.biPlanes = 1;

	// Create 2 DIB Sections.  One for the Front Buffer and one for the BackBuffer
	hBitmap  = CreateDIBSection(hdc,  &bmBitmapInfo, DIB_RGB_COLORS,(void **)&pImageBits, 0,0);	
	hBitmap2  = CreateDIBSection(hdc,  &bmBitmapInfo, DIB_RGB_COLORS,(void **)&pBackBits, 0,0);

	// Create a compatible DC for the front buffer and Select our Dib Section into it
	compHDC = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(compHDC, hBitmap);
	
	// Create a compatible DC for the back buffer and Select our Dib Section into it
	compHDC2 = CreateCompatibleDC(hdc2);
	hOldBitmap2 = (HBITMAP)SelectObject(compHDC2, hBitmap2);

	// Blit the front buffer to our compatible DC that will hold the destination bits
	BitBlt(compHDC, 0, 0, destWidth, destHeight, hdc, destX, destY, SRCCOPY);

	// Blit the back buffer to our compatible DC that will hold the source bits
	BitBlt(compHDC2, 0, 0, destWidth, destHeight, hdc2, srcX, srcY, SRCCOPY);

	// Loop through the 24 bit image (Times 3 for R G and B)
	for(int i = 0; i < destHeight * destWidth * 3; i += 3)
	{
		// Check if the current pixel being examined isn't the transparent color
		// Remember, the image bits are stored (Blue, Green, Red), not (Red, Green, Blue)
		// We use the system macros to abstract the R G B data
		if((pBackBits[i]   != GetBValue(tranparency)) || 
		   (pBackBits[i+1] != GetGValue(tranparency)) || 
		   (pBackBits[i+2] != GetRValue(tranparency)))
		{
			// Assign the desired pixel to the foreground
			pImageBits[i]     = pBackBits[i];
			pImageBits[i + 1] = pBackBits[i + 1];
			pImageBits[i + 2] = pBackBits[i + 2];
		}
	}  

	// Blit the transparent image to the front buffer (Voila!)
	BitBlt(hdc, destX, destY, destWidth, destHeight, compHDC, 0, 0, SRCCOPY);

	// Cleanup the monochrome bitmaps
	SelectObject(compHDC, hOldBitmap);
	SelectObject(compHDC2, hOldBitmap2);	

	// Free GDI Resources
	DeleteObject(hBitmap);
	DeleteObject(hBitmap2);
	DeleteDC(compHDC);
	DeleteDC(compHDC2);
}

//COLORREF GDI::red = RGB(255,0,0), GDI::blue= RGB(0,0,255), GDI::green= RGB(0,255,0);
//SmartBrush GDI::redBrush = CreateSolidBrush(red), GDI::blueBrush = CreateSolidBrush(blue), 
//		   GDI::greenBrush = CreateSolidBrush(green);

void MoveTo(HDC hdc, int x, int y)
{
	MoveToEx(hdc, x, y, NULL);
}

//Constructors/Destructors/Etc////////////////////////////////////////////////////////////////////


//GDI Draw Methods//////////////////////////////////////////////////////////////////////////////
void GDI::DrawPoint(int x0, int y0, COLORREF color)
{
	SetPixel(back_dc, (int) x0, (int) y0, color);
}

void GDI::DrawLine(int x0, int y0, int x1, int y1, COLORREF color)
{
	hpen = CreatePen(PS_SOLID, 1, color);
	oldPen = (HPEN)SelectObject(back_dc, hpen);

	MoveToEx(back_dc, (int)x0, (int)y0, NULL);
	LineTo(back_dc, (int)x1, (int)y1);
	
	SelectObject(back_dc, oldPen);
	DeleteObject(hpen);
}

void GDI::DrawLine(HDC deviceContext, int x0, int y0, int x1, int y1, COLORREF color)
{
	hpen = CreatePen(PS_SOLID, 1, color);
	oldPen = (HPEN)SelectObject(deviceContext, hpen);

	MoveToEx(deviceContext, (int)x0, (int)y0, NULL);
	LineTo(deviceContext, (int)x1, (int)y1);

	SelectObject(deviceContext, oldPen);
	DeleteObject(hpen);
}

void GDI::DrawEllipse(int x0, int y0, int x1, int y1, COLORREF color)
{
	hbrush = CreateSolidBrush(color);
	oldbrush = (HBRUSH)SelectObject(back_dc, hbrush);

	Ellipse(back_dc, x0, y0, x1, y1);

	SelectObject(back_dc, oldPen);
	DeleteObject(hbrush);
}

void GDI::DrawRect(int x0, int y0, int x1, int y1, COLORREF color)
{
	hbrush = CreateSolidBrush(color);
	oldbrush = (HBRUSH)SelectObject(back_dc, hbrush);

	Rectangle(back_dc, (int) x0, (int) y0, (int) x1, (int) y1);

	SelectObject(back_dc, oldbrush);
	DeleteObject(hbrush);
}

void GDI::DrawDIB(const POINT& pos, const DIB& dib)
{
	SelectObject(dibDC, dib.hBitmap);
	
	::BitBlt(back_dc, 
		   (int)pos.x,//(int)(origin.x + pos.x), 
		   (int)pos.y,//(int)(origin.y - pos.y), 
		   (int)dib.dimension.x, 
		   (int)dib.dimension.y,
		   dibDC,
		   0,0,
		   SRCCOPY);
}
//General GDI Functions/////////////////////////////////////////////////////////////////////////

bool GDI::CreateDIB(HDC targetDC, DIB& dib)
{
	BITMAPINFO bmBitmapInfo;
	RtlZeroMemory(&bmBitmapInfo, sizeof(bmBitmapInfo));
	
	BITMAP bm = {0};
	GetObject((HBITMAP)GetCurrentObject(targetDC, OBJ_BITMAP), sizeof(BITMAP), &bm);

	bmBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmBitmapInfo.bmiHeader.biCompression = BI_RGB;
	bmBitmapInfo.bmiHeader.biHeight = (int)dib.dimension.y; // bottom-up DIB, height is positive
	bmBitmapInfo.bmiHeader.biWidth = (int)dib.dimension.x;	
	bmBitmapInfo.bmiHeader.biBitCount = bm.bmBitsPixel;
	bmBitmapInfo.bmiHeader.biClrUsed = 0;
	bmBitmapInfo.bmiHeader.biPlanes = bm.bmPlanes;
	bmBitmapInfo.bmiHeader.biSizeImage =  (int)(dib.dimension.y*dib.dimension.x*4);

	dib.hBitmap = CreateDIBSection(targetDC,  
									&bmBitmapInfo, 
									DIB_RGB_COLORS,
									(void **)&dib.imageBits, 
									0,
									0);	
	return true;
}

bool WINAPI GDI::ChangeWindowStyle(HWND hwnd, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	//ASSERT(IsWindow(hwnd));
	DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
	
	if(dwStyle == dwNewStyle) 
	{
		return false;
	}
	
	SetWindowLong(hwnd, GWL_STYLE, dwNewStyle);
	
	if(nFlags != 0)
	{
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
		SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
	}
	
	return true;
}

bool GDI::ChangeDisplayMode(int pixel_width, int pixel_height, bool switch_back)
{
	// If we're switching back to "our normal resolution"
	if(switch_back)
	{
		// By passing in NULL we're saying "Go back to screen resolution logged in the registry"
		// 0 says "do this dynamically (like now)"
		// **Note this should not fail, that's why we don't check (could if you wanted to)
		ChangeDisplaySettings(NULL,0);
			return true;
	}

	DEVMODE dev_mode = {0};
	LONG result;

	// This call is going to get us the current display Settings -- By parameter:
	// NULL -- This specifies which display device (NULL means current display device)
	// ENUM_CURRENT_SetTINGS -- Retrieve the current Settings for the display device
	// dev_mode -- Address of DEVMODE structure to fill with the display Settings
	if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dev_mode))
		return false;

	// We have a DEVMODE filled with all the current Settings
	// so lets change the ones we want to
	dev_mode.dmPelsWidth = pixel_width;
	dev_mode.dmPelsHeight = pixel_height;
	
	// Now we'll try to change to the new Settings
	// CDS_FULLSCREEN -- flag says this change is "temporary" (there are other flags you can pass in)
	result = ChangeDisplaySettings(&dev_mode,CDS_FULLSCREEN);

	// ChangeDisplaySettings() returns DISP_CHANGE_SUCCESSFUL when it's successful
	if(result == DISP_CHANGE_SUCCESSFUL)
		return true;
	else
		return false; // No changing to full screen :(
}

// DIB class implementation ------------------------------------------------ //
Color<DWORD> DIB::GetPixel(int x, int y)
{
	if ( (x >= dimension.x) || (y >= dimension.y))
	{
		return Color<DWORD>(0, 0, 0, 0);
	}

	BYTE alpha = imageBits[DWORD(x*4 + y*dimension.x*4) + 3];
	BYTE red   = imageBits[DWORD(x*4 + y*dimension.x*4) + 2];
	BYTE green = imageBits[DWORD(x*4 + y*dimension.x*4) + 1];
	BYTE blue  = imageBits[DWORD(x*4 + y*dimension.x*4)];

	return Color<DWORD>(red, green, blue, alpha);
};
void DIB::DrawPixel(int x, int y, Color<DWORD> color)
{
	if (imageBits)
	{
		if ( x >= dimension.x )
			return;
		imageBits[DWORD(x*4 + y*dimension.x*4)]      = (BYTE)color.Blue(); // bits 23-16 are blue.
		imageBits[DWORD(x*4 + y*dimension.x*4) + 1 ] = (BYTE)color.Green(); // bits 15 to 8 are green.
		imageBits[DWORD(x*4 + y*dimension.x*4) + 2]  = (BYTE)color.Red(); // bits 7 to 0 are red.
		imageBits[DWORD(x*4 + y*dimension.x*4) + 3]  = (BYTE)color.Alpha(); // alphamask byte
	}
}

void DIB::DrawPixel(int x, int y, Color<DWORD> color, int len)
{	
	if (imageBits)
	{
		for ( int i = 0; i < len; i++)
		{
			if ( (i+x) >= dimension.x )
				break;
			imageBits[DWORD((i+x)*4 + y*dimension.x*4)]      = (BYTE)color.Blue();; // bits 23-16 are blue.
			imageBits[DWORD((i+x)*4 + y*dimension.x*4) + 1 ] = (BYTE)color.Green(); // bits 15 to 8 are green.
			imageBits[DWORD((i+x)*4 + y*dimension.x*4) + 2]  = (BYTE)color.Red(); // bits 7 to 0 are red.
			imageBits[DWORD((i+x)*4 + y*dimension.x*4) + 3]  = (BYTE)color.Alpha(); // alphamask byte
		}
	}
}
