#ifndef WINAPI_DRAG_H
#define WINAPI_DRAG_H

#include "winapi_window.h"

class Drag
{
public:
	Drag(Window * wnd, POINTS pt) : wnd(wnd), currentPos(pt) { isCaptured(false); }
	virtual ~Drag() {};
	virtual void UpdateCursorPosition(POINTS pt) { currentPos = pt; }
	virtual void SetCapture() { ::SetCapture(wnd->Wnd()); isCaptured(true); }
	virtual void ReleaseCapture() { ::ReleaseCapture(); isCaptured(false); }
	virtual void SetCusor() = 0;
	virtual int dx(POINTS newCursorPos) const = 0;
	virtual int dy(POINTS newCursorPos) const = 0;

	bool isCaptured() const { return captured; }
	void isCaptured(bool captured) { this->captured = captured; }

protected:
	bool captured;
	POINTS currentPos;
	Window * wnd;
};

class VerticalDrag : public Drag
{
public:
	VerticalDrag(Window * wnd, POINTS pt) : Drag(wnd, pt) { }
	~VerticalDrag() {};

	void SetCusor() { wnd->SetClassCursor( IDC_SIZENS ); } 
	int dx(POINTS newCursorPos) const { return 0; }
	int dy(POINTS newCursorPos) const { return newCursorPos.y - currentPos.y; }
};

class HorizontalDrag : public Drag
{
public:
	HorizontalDrag(Window * wnd, POINTS pt) : Drag(wnd, pt) { }
	~HorizontalDrag() {};

	void SetCusor() { wnd->SetClassCursor( IDC_SIZEWE ); } 
	int dx(POINTS newCursorPos) const { return newCursorPos.x - currentPos.x; }
	int dy(POINTS newCursorPos) const { return 0; }
};

class CartesianDrag : public Drag
{
public:
	CartesianDrag(Window * wnd, POINTS pt) : Drag(wnd, pt) { }
	~CartesianDrag() {};
	void SetCusor() { wnd->SetClassCursor( IDC_SIZEALL ); } 
	int dx(POINTS newCursorPos) const { return newCursorPos.x - currentPos.x; }
	int dy(POINTS newCursorPos) const { return newCursorPos.y - currentPos.y; }
};

class NoDrag : public Drag
{
public:
	NoDrag(Window * wnd, POINTS pt) : Drag(wnd, pt) { }
	~NoDrag() {};

	void SetCapture() { isCaptured(false); }
	void ReleaseCapture() {}
	void SetCusor() { wnd->SetClassCursor( IDC_ARROW ); } 
	int dx(POINTS newCursorPos) const { return 0; }
	int dy(POINTS newCursorPos) const { return 0; }
};

#endif