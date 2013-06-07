#include "Bitmap.h"
#include "OleCtl.h"
#include <cmath>
uint Bitmap::backgroundMinimalWidth  = 50;
uint Bitmap::backgroundMinimalHeight = 50;

// Load a Bitmap from a resource
BOOL Bitmap::Load (HINSTANCE hInst, const TCHAR * resName)
{
	Free ();

	_hBitmap = (HBITMAP) ::LoadImage (hInst, resName,
		IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	
	return _hBitmap != 0;
}

// Load a Bitmap from a resource
BOOL Bitmap::Load (HINSTANCE hInst, int id)
{
	Free ();
	
	_hBitmap = (HBITMAP) ::LoadImage (hInst, MAKEINTRESOURCE (id),
		IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	
	return _hBitmap != 0;
}

// Load a Bitmap Directly from a file
BOOL Bitmap::Load (const TCHAR *  path)
{
	Free ();

	_hBitmap = (HBITMAP) ::LoadImage (0, path, 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	return _hBitmap != 0;
}

void Bitmap::DrawBackground(HWND hWnd, HDC hDC)
{
	int width, height;
	GetSize(width, height);
	
	HDC bitmapDC = CreateCompatibleDC(hDC); SelectObject(bitmapDC, _hBitmap);

	HDC bitmap1DC = NULL;CreateCompatibleDC(hDC);
	HBITMAP hCompBitmap = NULL;

	int xScaler =  width  < (int)backgroundMinimalWidth  ? (int)std::ceil(backgroundMinimalWidth  / (float)width)  :  0;
	int yScaler =  height < (int)backgroundMinimalHeight ? (int)std::ceil(backgroundMinimalHeight / (float)height) :  0;  
	
	int scaledWidth = width, scaledHeight = height;

	if ( xScaler || yScaler )
	{
		 bitmap1DC = CreateCompatibleDC(hDC);
		// ok do backgroudn!
		scaledWidth  = xScaler ? xScaler * width  : width,
		scaledHeight = yScaler ? yScaler * height : height;
		hCompBitmap = CreateCompatibleBitmap(bitmapDC, scaledWidth, scaledHeight);
		bitmap1DC = CreateCompatibleDC(hDC);
		(HBITMAP)SelectObject(bitmap1DC, hCompBitmap);
	
		int x, y;

		for ( x = 0; x < scaledWidth; x += width )
			for ( y = 0; y < scaledHeight; y += height )
			{
				BitBlt(bitmap1DC, x, y, width, height, bitmapDC, 0, 0, SRCCOPY);
			}
	}
	else
		bitmap1DC = bitmapDC;

	RECT MainWndRect; GetClientRect(hWnd, &MainWndRect);
	int x = MainWndRect.right; 
	int y = MainWndRect.bottom; 

	int j_max =  MainWndRect.bottom,
		i_max =  MainWndRect.right;
	int temp = scaledWidth;
	for ( int j = 0; j < j_max; j += scaledHeight )
	{
		scaledWidth = temp; x = MainWndRect.right; 
		for ( int i = 0; i < i_max; i+= scaledWidth )
		{
			if (scaledWidth > x)
			{
				scaledWidth = x;
			}
			if (scaledHeight > y)
			{
				scaledHeight = y;
			}
			
			BitBlt(hDC, i, j, scaledWidth, scaledHeight, bitmap1DC, 0, 0, SRCCOPY);	
			
			x -= scaledWidth;
			
			if (scaledWidth==0 || scaledHeight==0)
				break;
		}
		y-=scaledHeight;
	}			
	//BitBlt(hDC, 0, 0, r.right, r.bottom, hdcMem2, 0, 0, SRCCOPY);
	DeleteDC(bitmapDC);	
	if ( bitmap1DC != bitmapDC )
		DeleteDC(bitmap1DC);	
	//DeleteDC(hdcMem2);
	if ( hCompBitmap ) 
		DeleteObject(hCompBitmap);
}

//
//
//void DrawBitmap(HDC hDC, Bitmap &bMap)
//{
//	BITMAP bm;
//
//	::GetObject(bMap, sizeof(bm), &bm);
//
//	HDC memdc = ::CreateCompatibleDC(NULL);
//	HBITMAP h = (HBITMAP)::SelectObject(memdc, bMap);
//	::BitBlt(hDC, 0, 0, bm.bmWidth, bm.bmHeight, memdc, 0, 0, SRCCOPY);
//	::SelectObject(memdc, h);
//
//	// watch out for those 
//	// tricky memory leaks
//	::DeleteDC(memdc);
//	::DeleteObject(h);
//}
//
//char * getPath(char *Path)
//{
//	char *ret = (char *)::GlobalAlloc(0, MAX_PATH);
//	memset(ret, 0, MAX_PATH);
//	LPTSTR lpFilePart;
//
//	::GetFullPathName(Path, MAX_PATH, ret, &lpFilePart);
//
//	return ret;
//}
//
//// load *.bmp, *.jpg, *.gif, *.ico, *.emf, or *.wmf files
//Bitmap LoadImage(char *szFile)
//{
//	LPPICTURE gpPicture;
//	// open file
//	HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
//	assert(INVALID_HANDLE_VALUE != hFile);
//
//	// get file size
//	DWORD dwFileSize = GetFileSize(hFile, NULL);
//	assert(-1 != dwFileSize);
//
//	LPVOID pvData = NULL;
//	// alloc memory based on file size
//	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
//	assert(NULL != hGlobal);
//
//	pvData = GlobalLock(hGlobal);
//	assert(NULL != pvData);
//
//	DWORD dwBytesRead = 0;
//	// read file and store in global memory
//	BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);
//	assert(FALSE != bRead);
//	GlobalUnlock(hGlobal);
//	CloseHandle(hFile);
//
//	LPSTREAM pstm = NULL;
//	// create IStream* from global memory
//	HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
//	assert(SUCCEEDED(hr) && pstm);
//
//	// Create IPicture from image file
//	hr = ::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (LPVOID *)&gpPicture);
//	assert(SUCCEEDED(hr) && gpPicture);	
//	pstm->Release();
//
//	OLE_HANDLE m_picHandle;
//
//	gpPicture->get_Handle(&m_picHandle);
//
//	HDC hdc = CreateCompatibleDC(NULL);
//
//	SelectObject(hdc, (HGDIOBJ) m_picHandle);
//
//	Bitmap ret = (HBITMAP)GetCurrentObject(hdc, OBJ_BITMAP);
//
//	DeleteDC(hdc);
//
//	return ret;
//}