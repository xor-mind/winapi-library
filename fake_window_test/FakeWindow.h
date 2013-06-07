#ifndef FAKE_WINDOW_H
#define FAKE_WINDOW_H

#include "IWindow.h"
#include <Windows.h>

class FakeWindow : public IWindow
{
public:
	// constructors/destructors/etc..
	FakeWindow();
	template <class T> static FakeWindow* createWindow(int x, int y, uint width, uint height, IWindow* parent);
	virtual ~FakeWindow() {}
	// initiation methods
	virtual void InitWindow(int x, int y, uint width, uint height, IWindow* parent);
	// positioning and resizing operation
	virtual void SetNonClientPos(int x, int y);
	virtual RECT ScreenRect() const { return screen; }
	// xPos and yPos will be the window's nonclient top left pixel wrt parent, if there's no parent it uses the screen as the parent.
	virtual int xPos()  const { return 0; }
	virtual int yPos()  const { return 0; }
	// x1Pos and y1Pos will be the window's nonclient bottom right pixel wrt to parent, if there's no parent it uses the screen as the parent.
	virtual int x1Pos() const { return 0; }
	virtual int y1Pos() const { return 0; }
	// nonclient width and height
	virtual int Width()        const { return 0; }
	virtual int Height()       const { return 0; }
	// xPosClient and yPosClient is the client's top left pixel wrt to parent, if there's no parent it uses the screen as the parent.
	virtual int xPosClient()   const { return 0; }
	virtual int yPosClient()   const { return 0; }
	virtual int ClientWidth()  const { return 0; }// width and height of client, not including non client area ( caption bar, menu, border, etc..)
	virtual int ClientHeight() const { return 0; }
	virtual int xPosClientInScreen()  const { return 0; }
	virtual int yPosClientInScreen() const { return 0; }
	virtual int dxNonClientAndClient() const { return 0; }
	virtual int dyNonClientAndClient() const { return 0; }
	
	virtual void CenterVertically(IWindow* topWindow, IWindow* bottomWidow);
	//static void ScreenToClient(IWindow* w, POINT& p)
	//{
	//	if (!w) return;

	//	p.x -= w->xposClientInScreen();
	//	p.y -= w->yposClientInScreen();
	//}

	// ownership methods
	//HWND Wnd() const { return hWnd; } // handle used in the winapi
	//void SetWnd(HWND hWnd) { this->hWnd = hWnd; }
	//void SetParent(Window* parent) { this->parent = parent; if (parent) hParent = 
	//	parent->Wnd(); }
	//void SetWindowAndParent(HWND hWnd, Window *parent) { SetWnd(hWnd); SetParent(parent); } 
	//void DestroyWindow();

protected:
	RECT client, screen;
};

template <class T> FakeWindow* FakeWindow::createWindow(int x, int y, uint width, uint height, IWindow* parent)
{ 
	FakeWindow* w = new T();
	w->InitWindow(x, y, width, height, parent);
	return w;
}

#endif