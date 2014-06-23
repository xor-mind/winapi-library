#ifndef SPLITTER_H
#define SPLITTER_H

#include "window.h"

typedef unsigned int uint;

class Splitter : public Window
{ 
protected:

	float    splitRatio;
	uint     splitBarLength;
	int	     dragStartX, dragStartY, dragX, dragY;
	HPEN     splitPen;

	virtual      WND_PROC_DECL();
	virtual void WM_LButtonDown(POINTS pt);
	virtual void WM_LButtonUp(POINTS pt);
	virtual void WM_LButtonDrag(POINTS pt);
	virtual void WM_CaptureChanged();

public:
	Splitter(float splitRatio, uint splitBarLength) : Window(), splitRatio(splitRatio), splitBarLength(splitBarLength) 
	{ 
		splitPen = CreatePen(PS_SOLID, splitBarLength, RGB(200,200,100));
	}
	virtual ~Splitter() {/* DeleteObject(splitPen);*/ }

	//virtual Splitter * InitWnd(Window * parent) = 0;
	virtual int  x() { return 0; }
	virtual int x1() { return 0; }
	virtual int  y() { return 0; }
	virtual int y1() { return 0; }

	virtual void UpdateRatio() = 0;
	virtual Splitter * UpdatePosition() = 0;
	virtual void DrawSplitterGhost(HDC dc) = 0;
	//static virtual Window* create(float splitRatio, uint splitBarWidth, Window* parent) = 0; c++ doesn't like this

	static int & splitterCount() { static int i = 0; return i; }
};

// splits the window into a west and east half 
class WEsplitter : public Splitter
{
public:
	WEsplitter(float splitRatio, uint splitBarWidth) : Splitter(splitRatio, splitBarWidth) {  }
	virtual ~WEsplitter() { }
	
	//WEsplitter * InitWnd(Window * parent);

	int  x();
	int x1();

	void DrawSplitterGhost(HDC dc);
	void UpdateRatio();
	Splitter * UpdatePosition();
	static Window* create(float splitRatio, uint splitBarWidth, Window* parent);
};		

#endif