#include "FakeWindow.h"

FakeWindow::FakeWindow() {}

void FakeWindow::InitWindow(int x, int y, uint width, uint height, IWindow* parent)
{
	this->parent = parent;

	if (parent == NULL)
	{
		screen.left = x; screen.top = y; screen.right = x + width; screen.bottom = y + height;
	}

	//if (parent)
	//{
	//	x += parent->xposClientInScreen();
	//	y += parent->yposClientInScreen();
	//}
	//RECT r = { x, y, x + width, y + height }; 
	//screen = r;
	//client.left = client.top = 0; // just force a zero dimension non client area for now
	//client.right = width; client.bottom = height;
}

void FakeWindow::SetNonClientPos(int x, int y) 
{ 
	//int w = width(), h = height();
	//if ( !parent ) 
	//{
	//	screen.left = x; screen.top = y;;
	//}
	//else 
	//{
	//	screen.left = parent->xposClientInScreen() + x; 
	//	screen.top  = parent->yposClientInScreen() + y;
	//}
	//screen.right = screen.left + w; screen.bottom = screen.top + h;
}

void FakeWindow::CenterVertically(IWindow* topWindow, IWindow* bottomWidow)
{
	//// center vertically wrt to screen
	//uint distance = bottomWidow->ypos() - topWindow->y1pos(),
	//			y = midpoint<uint>(distance, height());
	//POINT p = { xpos(), topWindow->y1pos()  + y };
	//
	//// convert the coords to client coords and set the window position.
	//ScreenToClient(parent, p);
	//SetWindowPos(p.x, p.y);
}

