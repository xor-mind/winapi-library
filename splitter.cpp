#include "splitter.h"

#define r2 R2_XORPEN

void Line (HDC dc, int x1, int y1, int x2, int y2)
{
	MoveToEx (dc, x1, y1, 0);
	LineTo (dc, x2, y2);
}

LRESULT CALLBACK Splitter::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
		case WM_CAPTURECHANGED: { WM_CaptureChanged(); return 0; }
		case WM_LBUTTONDOWN:	{ WM_LButtonDown(MAKEPOINTS (lParam)); return 0; }
		case WM_LBUTTONUP:		{ WM_LButtonUp(MAKEPOINTS (lParam)); return 0; }
		case WM_MOUSEMOVE:		{ if (wParam & MK_LBUTTON) { WM_LButtonDrag(MAKEPOINTS (lParam)); } return 0; }
		
		default: break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void       Splitter::WM_CaptureChanged()
{
	// We are losing capture, End drag selection, Erase previous divider
	HDC dc = GetDC(hParent);
	HPEN oldPen = (HPEN)SelectObject(dc, splitPen);
	SetROP2(dc, r2 );
	DrawSplitterGhost(dc);
	SelectObject(dc, oldPen);
	ReleaseDC(hParent, dc);
}
void       Splitter::WM_LButtonDown(POINTS pt)
{
	CaptureMouse();

	dragStartX = x() + splitBarLength / 2 -  pt.x;
	dragStartY = y() + splitBarLength / 2 -  pt.y;

	//// Draw a divider using XOR mode
	DC dc( parent->Wnd() );
	HPEN oldPen = (HPEN)SelectObject(dc, splitPen);
	int oldMode = SetROP2 (dc, r2);
	DrawSplitterGhost(dc);
	SetROP2 (dc, oldMode);
	SelectObject(dc, oldPen);
}
void       Splitter::WM_LButtonUp(POINTS pt)
{
	// Calling ReleaseCapture will send us the WM_CAPTURECHANGED
	ReleaseCapture ();

	dragX = dragStartX + pt.x; dragY = dragStartY + pt.y;

	if (dragX < 0 ) dragX = 0; 
	
	else if (dragX + (int)splitBarLength >= (int)parent->ClientWidth() ) 
		dragX = parent->ClientWidth() - splitBarLength;
	
	if (dragY < 0) dragY = 0;

	else if (dragY + (int)splitBarLength >= (int)parent->ClientHeight()  ) 
		dragY = parent->ClientHeight() - splitBarLength;

	UpdateRatio();
}
void       Splitter::WM_LButtonDrag(POINTS pt)
{
	// Erase previous divider and draw new one
	DC dc(parent->Wnd());
	HPEN oldPen = (HPEN)SelectObject(dc, splitPen);
	int oldMode = SetROP2 (dc, r2);

	DrawSplitterGhost(dc); // erase old one
	dragX = dragStartX + pt.x;
	dragY = dragStartY + pt.y;
	DrawSplitterGhost(dc); // draw new one

	SetROP2 (dc, oldMode);

	SelectObject(dc, oldPen);
}

//WEsplitter * WEsplitter::InitWnd(Window * parent) 
//{
//	this->parent = parent;
//	return (WEsplitter *)Window::InitWnd(parent, 0, NBRUSH, _T("splitter"), 0, 0, 0, x(), 0, splitBarLength, parent->ClientHeight()); 
//}

Splitter * WEsplitter::UpdatePosition() 
{
	SetNonClientPos(x(), 0, splitBarLength, parent->ClientHeight());
	return this;
}

void WEsplitter::UpdateRatio()
{
	splitRatio = dragX / (float)parent->ClientWidth();
	PostMessage(hParent, WM_SIZE, 0, 0);
}
void WEsplitter::DrawSplitterGhost(HDC dc)
{
	Line(dc, dragX, 0, dragX, Height() - 1);
}

Window   * WEsplitter::create(float splitRatio, uint splitBarWidth, Window* parent) 
{ 
	splitterCount()++;
	return ( new WEsplitter(splitRatio, splitBarWidth) ) ->InitWnd(0, 0, 0, 0, _T("Split"), NULL, parent)->SetClassCursor( IDC_SIZEWE )->SetClassBrush(NBRUSH) ; 
}

int  WEsplitter ::  x() { return (int)(pClientWidth() * splitRatio); }
int  WEsplitter :: x1() { return (int)(pClientWidth() * splitRatio + splitBarLength); }

