#ifndef IWINDOW_H
#define IWINDOW_H

#include "Prerequisites.h"

/* ad_todo: Why do I have an interface class? What is the need?*/
class IWindow
{
public:
	// creation methods ---------------------------------------------------- //
	virtual ~IWindow() {}
	//virtual  IWindow * InitWnd(int x, int y, uint width, uint height, IWindow* parent) = 0;
	// positioning and resizing -------------------------------------------- //
	// xPos and yPos is the window's nonclient top left pixel wrt parent, if there's no parent it uses the screen as the parent.
	virtual int xPos()  const = 0;
	virtual int yPos()  const = 0;
	// x1Pos and y1Pos is the window's nonclient right bottom pixel wrt to parent, if there's no parent it uses the screen as the parent.
	virtual int x1Pos() const = 0; // xPos + nonClient width and yPos + nonClient height position of window wrt screen
	virtual int y1Pos() const = 0; 
	virtual int Width() const = 0; // width/height of window includeing non client area (caption bar, menu, border, etc..)
	virtual int Height() const = 0; 
	// xPosClient and yPosClient is the client's top left pixel wrt to parent, if there's no parent it uses the screen as the parent.
	virtual int xPosClient()   const = 0;
	virtual int yPosClient()   const = 0;
	virtual int ClientWidth()  const = 0;// width and height of client, not including non client area ( caption bar, menu, border, etc..)
	virtual int ClientHeight() const = 0;
	virtual int xPosClientInScreen()  const = 0;
	virtual int yPosClientInScreen() const = 0;
	virtual int dxNonClientAndClient() const = 0; // delta between non client and client area
	virtual int dyNonClientAndClient() const = 0; 

	virtual void SetNonClientPos(int x, int y)  = 0; // move the window WRT parent

	virtual void CenterVertically(IWindow* topWindow, IWindow* bottomWidow) = 0;
	
protected:
	IWindow* parent; 
};

#endif