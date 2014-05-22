/*
winapi_window.h

Objects: 

Window: This objects provides methods for manipulating a window.. Very Similiar to
an MFC CWindow. All Windows, including child Windows should be managed with smart pointers to avoid memory leaks.

*/

#ifndef WINAPI_WINDOW_H
#define WINAPI_WINDOW_H

#include "IWindow.h"
#include <Windows.h>
#include "winapi_DataConversion.h"
#include <list>
#include <boost\scoped_ptr.hpp>
#include "winapi_GUI.h"
#include <algorithm>

#pragma warning(disable:4003) // removes warning about using macros with empty parans, useful to take advantage of syntx such as WND_PROC_DECL(); 
 
#define WIN_MAIN              int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#define WND_PROC_PARAM		  HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam
#define WND_PROC_DECL(x)      LRESULT CALLBACK x##WndProc(WND_PROC_PARAM)
#define DEF_WND_PROC          DefWindowProc(hWnd, msg, wParam, lParam)
#define DLG_PROC_DECL(x)      BOOL CALLBACK x##DlgProc(WND_PROC_PARAM)
#define WPD					  WND_PROC_DECL
#define DPD                   DLG_PROC_DECL
#define DWP					  DEF_WND_PROC
#define AppInstance			  GetModuleHandle(NULL)
#define AppHandle			  AppInstance
#define NBRUSH			      (HBRUSH)GetStockObject(NULL_BRUSH)
#define GetWH				  int w = ClientWidth(), h = ClientHeight(); 

#define onCreate			  if ( msg == WM_CREATE )        { HandleCreate(wParam, lParam); return 0; }
#define onCommand			  if ( msg == WM_COMMAND )       { HandleCommand(wParam, lParam); return 0; }
#define onEraseBackground     if ( msg == WM_ERASEBKGND )    { return HandleEraseBackground(wParam, lParam); }
#define onInitDialog          if ( msg == WM_INITDIALOG )    { HandleInitDialog(wParam, lParam); return FALSE; }
#define onKeyDown		      if ( msg == WM_KEYDOWN )       { HandleKeyDown(wParam, lParam); return 0; }
#define onLButtonDown         if ( msg == WM_LBUTTONDOWN )   { HandleLButtonDown(MAKEPOINTS(lParam)); return 0; }
#define onLButtonUp		      if ( msg == WM_LBUTTONUP )     { HandleLButtonUp(MAKEPOINTS(lParam)); return 0;}
#define onMinMaxInfo		  if ( msg == WM_GETMINMAXINFO ) { HandleGetMinMaxInfo(lParam); return 0; }
#define onMouseMove		      if ( msg == WM_MOUSEMOVE )     { HandleMouseMove(MAKEPOINTS(lParam)); return 0; }
#define onPaint			      if ( msg == WM_PAINT )		 { beg_paint HandlePaint(dc); end_paint return 0; }
#define onSize			      if ( msg == WM_SIZE )			 { GetWH HandleSize(w, h); return 0; }
#define onPosition            if ( msg == WM_WINDOWPOSCHANGED ) { HandlePosition(); return 0; }
#define onWndDestruction	  if ( msg == WM_DESTROY || msg == WM_CLOSE )		 { HandleWndDestruction(); return 0;}

#define handleCreate(x)		     void x##HandleCreate(WPARAM wParam, LPARAM lParam)
#define handleInitDialog(x)      void x##HandleInitDialog(WPARAM wParam, LPARAM lParam)
#define handleCommand(x)		 void x##HandleCommand(WPARAM wParam, LPARAM lParam)
#define handleEraseBackground(x) int  x##HandleEraseBackground(WPARAM wParam, LPARAM lParam)
#define handleKeyDown(x)		 void x##HandleKeyDown(WPARAM wParam, LPARAM lParam)
#define handleLButtonDown(x)	 void x##HandleLButtonDown(POINTS mouse)
#define handleLButtonUp(x)	     void x##HandleLButtonUp(POINTS mouse)
#define handleMinMaxInfo(x)	     void x##HandleGetMinMaxInfo(LPARAM lParam)
#define handleMouseMove(x)	     void x##HandleMouseMove(POINTS mouse)
#define handlePaint(x)		     void x##HandlePaint(HDC dc)
#define handlePosition(x)        void x##HandlePosition()
#define handleSize(x)		     void x##HandleSize(int w, int h)
#define handleWndDestruction(x)  void x##HandleWndDestruction()

#define handleDefaultMsgs	  onPaint onSize onMinMaxInfo onWndDestruction

#define processMsg(x)         if ( msg == x )
#define processCommand(x)     if ( LOWORD(wParam) == x )
#define beg_paint			  HDC dc = BeginPaint(Wnd(), &ps);
#define end_paint			  EndPaint(Wnd(), &ps);

#define sendLButtonDownToParent if ( msg == WM_LBUTTONDOWN ) { int x = LOWORD(lParam), y = HIWORD(lParam); POINT p = ClientToScreen(x, y); POINT p1 = parent->ScreenToClient(p); int lparam = ( p1.y << 16 ) | p1.x; SendMessage(hParent, WM_LBUTTONDOWN, lParam, 0); }
#define sendKeyDownToParent(k) SendMessage(hParent, WM_KEYDOWN, k, NULL );

class Window;
typedef std::list<Window*> WindowList;

class DC 
{
	HWND hWnd; HDC dc;
public:
	DC( HWND hWnd ) : hWnd(hWnd) { dc = GetDC(hWnd); }
	~DC()			      { ReleaseDC(hWnd, dc); }
	operator HDC () { return dc; }
};

class Window : public IWindow
{
public:

	// constructors/destructors/etc..
	Window();
	virtual ~Window();
	// initiation methods
	virtual void InitWnd();

	// postion and dimension assocaited with non client dimension
	virtual Window  * InitWnd(int x, int y, uint width, uint height, IWindow* parent);
	virtual Window  * InitWnd(int x, int y, uint width, uint height, const Padding& p, IWindow* parent);
	virtual Window  * InitWnd(int x, int y, uint width, uint height, TCHAR* const windowName, IWindow* parent);
	virtual Window  * InitWnd(int x, int y, uint width, uint height, TCHAR* const className, TCHAR* const windowName, IWindow* parent);
	virtual Window  * InitWnd(int x, int y, uint width, uint height, HBRUSH brush, IWindow* parent);	
	virtual Window  * InitWnd(IWindow* parent, HMENU menu, HBRUSH backgroundBrush, TCHAR* const className, TCHAR* const windowName, UINT classStyle, UINT wndFlags, int xPos, int yPos, int width, int height);
	virtual Window  * InitWnd(IWindow* parent, HINSTANCE hInstance, LPCTSTR lpTemplate);
	virtual Window  * InitWnd(int templateId);
	// position associated with non client position and dimension associated with client dimension
	virtual Window  * InitAdjustedWnd( int x, int y, uint width, uint height, IWindow* parent );

	// positioning and resizing operation
	virtual void SetNonClientPos(int x, int y);
	virtual void SetNonClientPos(int x, int y, int width, int height);
	virtual RECT ScreenRect() const;
	virtual RECT NonClientRect() const // non client rect with respect to parent
	{
		RECT r = ScreenRect(); 
		if ( parent )
		{
			parent->ScreenToClient( ( * (POINT*)&r.left ) );
			parent->ScreenToClient( ( * (POINT*)&r.right ) );
			return r;
		}
		else
			return r;
	}
	virtual RECT GetTrueClientRect() const 
	{
		RECT r; ::GetClientRect( Wnd(), &r );
		return r;
	}
	virtual RECT GetClientRect() const
	{
		RECT r; ::GetClientRect( Wnd(), &r );
		if ( pad )
		{
			r.left  += (*pad->left)(); r.top += (*pad->top)();
			r.right = ClientWidth();
			r.bottom = ClientHeight(); 
		}
		if ( parent )
		{
			r.left += xPosClient(); 
			r.top  += yPosClient();
		}
		return r;
	}

	// xPos and yPos will be the window's nonclient top left pixel wrt parent, if there's no parent it uses the screen as the parent.
	virtual int xPos()  const;
	virtual int yPos()  const;
	// x1Pos and y1Pos will be the window's nonclient bottom right pixel wrt to parent, if there's no parent it uses the screen as the parent.
	virtual int x1Pos() const;
	virtual int y1Pos() const;
	// nonclient width and height
	virtual int Width()        const; 
	virtual int Height()       const;
	virtual int TrueWidth()        const; 
	virtual int TrueHeight()       const;
	// xPosClient and yPosClient is the client's top left pixel wrt to parent, if there's no parent it uses the screen as the parent.
	virtual int xPosClient()   const
	{ 
		if ( !parent ) {
			return xPosClientInScreen();
		}
		return xPosClientInScreen() - parent->xPosClientInScreen();
	}
	virtual int yPosClient()   const
	{
		if ( !parent ) {
			return yPosClientInScreen();
		}
		return yPosClientInScreen() - parent->yPosClientInScreen();
	}

	virtual int ClientWidth()  const;// width and height of client, not including non client area ( caption bar, menu, border, etc..)
	virtual int ClientHeight() const;
	virtual int TrueClientWidth()  const;// width and height of client, not including non client area ( caption bar, menu, border, etc..)
	virtual int TrueClientHeight() const;
	virtual int xPosClientInScreen()  const
	{ 
		POINT p = { 0, 0 };
		p.x += pad ? (*pad->left)() : 0;
		ClientToScreen(p);
		return p.x;
	}

	virtual int x1PosClientInScreen()  const
	{
		return xPosClientInScreen() + ClientWidth();
	}
	virtual int yPosClientInScreen() const
	{ 
		POINT p = { 0, 0 };
		p.y += pad ? (*pad->top)() : 0;
		ClientToScreen(p);
		return p.y;
	}
	virtual int y1PosClientInScreen() const
	{
		return yPosClientInScreen() + ClientHeight();
	}

	virtual int dxNonClientAndClient() const;
	virtual int dyNonClientAndClient() const; 
	// functions for quick access to the parent
	virtual int pClientHeight() const;
	virtual int pClientWidth() const;

	int dxLeftAndLeft(Window* otherWnd) const;
	int dxRightAndRight(Window* otherWnd) const;
	int dyTopAndTop(Window* otherWnd) const;
	int dyBotAndBot(Window* otherWnd) const;

	virtual POINT ClientToScreen(int x, int y);
	virtual POINT & ClientToScreen(POINT& p) const;
	virtual POINT & ScreenToClient(POINT& p) const;
	virtual POINT ScreenToClient(int& x, int& y);
	virtual POINT ScreenToClient(long& x, long& y) { return ScreenToClient((int&)x, (int&)y); }
	// void CenterHorizontally(IWindow* wrToThisWindow);
	virtual void CenterVertically(IWindow* topWindow, IWindow* bottomWidow);

	tstring GetText();

	virtual handleCreate()          {}
	virtual handleCommand()		    {}
	virtual handleInitDialog()		{}
	virtual handleEraseBackground() { return 0; }
	virtual handleKeyDown()         {}
	virtual handleLButtonDown()	    {}
	virtual handleLButtonUp()	    {}
	virtual handleMinMaxInfo()	    { ((LPMINMAXINFO)lParam)->ptMinTrackSize.x = 50; ((LPMINMAXINFO)lParam)->ptMinTrackSize.y = 50; }
	virtual handleMouseMove()	    {}
	virtual handlePaint()		    { HighlightPadding(); } 
	virtual handlePosition()        { }
	virtual handleSize()		    { if ( pad ) pad->WidthAndHeight(TrueClientWidth(), TrueClientHeight()); }
	virtual handleWndDestruction()  { DestroyWindow(); if ( postQuitMessage ) { PostQuitMessage(0); } }

	// sets and gets for Window properties
	HWND Wnd() const { return hWnd; } // handle used in the winapi
	HWND ParentWnd() const { return hParent; }
	void SetWnd(HWND hWnd) { this->hWnd = hWnd; }
	Window * SetParent(Window* parent);
	Window * SetWndAndParent(HWND hWnd, Window *parent) { SetWnd(hWnd); SetParent(parent); ::SetParent(hWnd, parent->Wnd()); return this; } 
	WNDPROC oldWndProc;
	Window * SetWndAndParentAndSubclass(HWND hWnd, Window *parent) 
	{
		SetWndAndParent(hWnd, parent); 
		oldWndProc = (WNDPROC)SetWindowLongPtr (Wnd(), GWLP_WNDPROC, (LONG_PTR)HandlerWndProc);
		
		return this; 
	} 
	void UnregisterClass();
	void DestroyWindow();
	// destroys window and unregisters wndclass
	void Expunge(); 
	void SetWndClassName(const tstring& name);

	Window* ForceRedraw();

	void AddChild( Window* child )
	{
		children.push_back( child );
	}
	void RemoveChild( Window* child )
	{
		children.erase(std::find(children.begin(), children.end(), child));	
	}

	template <class LEFT, class TOP, class RIGHT, class BOTTOM >
	Window * SetPadding(LEFT l, TOP t, RIGHT r, BOTTOM b) 
	{
		pad.reset(new Padding(ClientWidth(), ClientHeight(), l, t, r, b)); 
		return this; 
	}
	int PaddingLeft()
	{
		return pad ? (*pad->left)() : 0;
	}
	int PaddingTop()
	{
		return pad ? (*pad->top)() : 0;
	}

	// methods having to do with message handling and wndprocs
	static int RunGetMessageLoop();
	int RunPeekMessageLoop();
	virtual void PeekMessageLoop() { };
	
	static  WND_PROC_DECL(Handler);
	static  DLG_PROC_DECL(Handler);
	virtual WND_PROC_DECL();
	virtual DLG_PROC_DECL();

	Window*  parent;
	WindowList children;

	boost::scoped_ptr<Padding> pad;

	// really useful methods
	Window * CaptureMouse();
	Window * CenterWrtParent();
	Window * FillClient(HPEN pen, HBRUSH brush); // fill the client with the choosen color
	Window * HighlightPadding( HBRUSH b = (HBRUSH)GetStockObject(WHITE_BRUSH) );
	Window * HighlightPadding(HBRUSH b, HDC dc);
	Window * HighlightPadding( bool shouldWeHighlightThisWindowsPadding );
	Window * PushButton();
	Window * SetClassBrush(HBRUSH newBrush);
	Window * SetClassCursor(TCHAR const * id);
	Window * SetText(const TCHAR * const windowName);
	Window * ShowWindow();
	Window * ProcessPostQuitMessage(bool b);
	Window * SetClassName(TCHAR* className) { _tcscpy_s(this->className, _countof(this->className), className); return this; }

	HFONT GetCurrentFont();

	int MonitorWidth()  const { return GetSystemMetrics(SM_CXSCREEN); }
	int MonitorHeight() const { return GetSystemMetrics(SM_CYSCREEN); }

protected:
	// all used for creating windows
	static Window* currentlyCreatedWindow;
	
	static WindowList windowList;

protected:
	RECT client, screen;
	HWND hWnd, hParent;
	PAINTSTRUCT ps;
	WNDCLASSEX wc;
	TCHAR className[256], windowName[256];
	bool postQuitMessage;
	bool highlightWnd;
	
public:
	const static DWORD  defaultClassStyle;
	const static DWORD  defaultWindowStyle;
	const static DWORD  defaultChildClassStyle;
	const static DWORD  defaultChildWindowStyle;
	static TCHAR* const defaultClassName;
	static TCHAR* const defaultChildClassName;
	static TCHAR* const defaultWindowName;
	static TCHAR* const defaultChildWindowName; 
};

#define CreateWnd(...)							 WindowFactory<>::create(__VA_ARGS__)
// #define CreateDiv(x, y, w, h, className, parent) WindowFactory<>::create(x, y, w, h, className, _T(""), CS_DBLCLKS | CS_OWNDC, WS_CHILD | WS_VISIBLE , (HBRUSH)GetStockObject(NULL_BRUSH), parent)

typedef boost::scoped_ptr<Window> WindowPtr;

template <class T = Window> 
struct WindowFactory
{
	
	static Window * create() 
	{ 
		RECT rc = { 0, 0, 600, 600 };
		AdjustWindowRect( &rc,  WS_OVERLAPPEDWINDOW, FALSE);
		Window * w = (new T())->SetPadding(100, 100, 100, 100)->InitWnd(0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL)->CenterWrtParent(); 
		return w;
	}

	static Window * create(Window* parent) 
	{ 
		if ( !parent )
			return 0;
		if ( parent->pad )
			return (new T())->InitWnd(parent->pad->left->pixelCount(), parent->pad->top->pixelCount(), parent->ClientWidth(), parent->ClientHeight(), (HBRUSH)GetStockObject(WHITE_BRUSH), parent); 
		else
			return (new T())->InitWnd(0, 0, parent->ClientWidth(), parent->ClientHeight(), (HBRUSH)GetStockObject(WHITE_BRUSH), parent); 
	}

	static Window * create(Window* parent, TCHAR* className, TCHAR* wndName) 
	{ 
		if ( !parent )
			return 0;
		if ( parent->pad )
			return (new T())->InitWnd(parent->pad->left->pixelCount(), parent->pad->top->pixelCount(), parent->ClientWidth(), parent->ClientHeight(), className, wndName, parent); 
		else
			return (new T())->InitWnd(0, 0, parent->ClientWidth(), parent->ClientHeight(), className, wndName, parent); 
	}

	//static Window * create(Window* parent, TCHAR* className) 
	//{ 
	//	if ( !parent )
	//		return 0;
	//	else 
	//		return (new T())->InitWnd(parent, className);
	//	
	//}

	static Window * create(int templateId)
	{
		return (new T())->InitWnd(templateId);
	}

	static Window * create(int x, int y, uint width, uint height, IWindow* parent)
	{ 
		Window* w = new T();
		w->InitWnd(x, y, width, height, parent);
		return w;
	}

	static Window * create(RECT r, IWindow* parent)
	{ 
		Window* w = new T();
		w->InitWnd(r.left, r.top, r.right, r.bottom, parent);
		return w;
	}

	static Window * create(RECT r, TCHAR* className, TCHAR* windowName, IWindow* parent)
	{ 
		Window* w = new T();
		w->InitWnd(r.left, r.top, r.right, r.bottom, className, windowName, parent);
		return w;
	}

	static Window * create(int x, int y, uint width, uint height, const Padding& p, IWindow* parent)
	{ 
		Window* w = new T();
		w->InitWnd(x, y, width, height, p, parent);
		return w;
	}

	static Window * create(int x, int y, uint width, uint height, UINT classStyle, UINT wndFlags, IWindow* parent)
	{ 
		Window* w = new T();
		w->InitWnd(parent, 0, 0, 0, _T(""), classStyle, wndFlags, x, y,  width, height);
		return w;
	}

	static Window * create(int x, int y, uint width, uint height, TCHAR* const className, IWindow* parent)
	{
		Window * w = new T();
		w->InitWnd(parent, 0, 0, className, 0, 0, 0, x, y, width, height);
		return w;
	}

	static Window * create(int x, int y, uint width, uint height, TCHAR* const className, HBRUSH brush, const Padding& p, IWindow* parent)
	{ 
		Window * w = new T();
		w->InitWnd(parent, 0, brush, className, 0, 0, 0, x, y, width, height);
		w->pad.reset(new Padding(p, w->ClientWidth(), w->ClientHeight()));
		return w;
	}

	static Window * create(int x, int y, uint width, uint height, HBRUSH brush, IWindow* parent)
	{ 
		Window * w = new T();
		w->InitWnd(parent, 0, brush, 0, 0, 0, 0, x, y, width, height);
		return w;
	}

	static Window * create(int x, int y, uint width, uint height, TCHAR* const className, HBRUSH brush, IWindow* parent)
	{ 
		Window * w = new T();
		w->InitWnd(parent, 0, brush, className, 0, 0, 0, x, y, width, height);
		
		return w;
	}

	static Window * create(int x, int y, uint width, uint height, TCHAR* const className, TCHAR* const windowName, 
							UINT classStyle, UINT wndFlags, HBRUSH background, IWindow* parent)
	{ 
		Window * w = new T();
		w->InitWnd(parent, 0, background, className, windowName, classStyle, wndFlags, x, y, width, height);
		return w;
	}

	static Window * create(IWindow* parent, HINSTANCE hInstance, LPCTSTR lpTemplate)
	{
		Window* w = new T();
		w->InitWnd(parent, hInstance, lpTemplate);
		return w;
	}

	private: WindowFactory() {} // singleton mode = ON
};
	
#define WFC(x,y) WindowFactory<x>::create(y)

//class Margin
//{
//public:
//	
//	Margin( Window * w ) : w(w) {}
//	 ~Margin() {}
//	 void ApplyMargins(int & x, int & y, int & width, int & height) = 0;
//	Window * w; // the window associated with this Margin object
//};
//
//class MarginPercent : public Margin
//{
//public:
//	MarginPercent(Window * w) : Margin(w) { left = top = right = bot = 0.0f; }
//	
//	MarginPercent(float left, float top, float right, float bot, Window * w) 
//	: Margin(w), left(left), top(top), right(right), bot(bot) {  }
//	
//	void ApplyMargins(int & x, int & y, int & width, int & height)
//	{
//		// this code is experimentation, I still need to better define and understand the concept of "margins".
//		//RECT r;
//		//GetClientRect(w->hParent, &r); // get width and height of containing rect
//
//		//x += (int)(left  * r.right);
//		//x -= (int)(right * r.right);
//
//		//y += (int)(top * r.bottom);
//		//y -= (int)(bot * r.bottom);
//
//		//if ( isWidthAndHeightProportionalToMargins )
//		//{
//		//	width -= (int)(left  * r.right);
//		//	width += (int)(right * r.right);
//		//
//		//	height -= (int)(top * r.bottom);
//		//	height += (int)(bot * r.bottom);
//		//}
//	}
//	
//	float left, top, right, bot;
//};

// this is like css's X px or X %, you can use GuiMeasurements as a way to talk about lengths 
// while styling our windows. Maybe we want the padding to be 10% of the widfth of the window, 
// maybe want to use dialog units (DU) or maybe we want talk about pixel lengths.

struct TransparentWnd : public Window 
{
	TransparentWnd() { hilightWnd = false; } 
			
	static void DrawOutLine(HWND hwnd, HDC dc)
	{ 
		HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
		HBRUSH hbrOld = (HBRUSH)SelectObject(dc, brush);
			
		RECT rc;
		::GetClientRect(hwnd, &rc);
			
		rc.left += 2; rc.top +=2; rc.right -= 1; rc.bottom -= 1;

		// Draw selection rectangle.
		PatBlt( dc, rc.left,    rc.top-2,    rc.right - 1, 1, PATCOPY);
		PatBlt( dc, rc.left,    rc.bottom,   rc.right - 1, 1, PATCOPY);
		PatBlt( dc, rc.left-2,  rc.top,      1, rc.bottom - 1, PATCOPY);
		PatBlt( dc, rc.right,   rc.top,      1, rc.bottom - 1, PATCOPY);

		// clean up brush
		SelectObject(dc, GetStockObject(NULL_BRUSH));
		DeleteObject(brush);
		SelectObject(dc, hbrOld);
	}

	WND_PROC_DECL()
	{
		PAINTSTRUCT ps;

		if ( msg == WM_KEYDOWN )
			return SendMessage(hParent, msg, wParam, lParam);

		if ( msg == WM_COMMAND )
			return SendMessage(hParent, msg, wParam, lParam);

		if ( msg == WM_PAINT ) 
		{
			HDC hDC = BeginPaint(hWnd, &ps);
			if ( hilightWnd )
			{
				DrawOutLine(hWnd, hDC);
				//HPEN hPen = CreatePen(PS_DASHDOTDOT, 1, RGB(255, 25, 5));
				//HPEN oldPen = (HPEN)SelectObject(hDC, hPen);
				//
				////SelectObject(hDC, GetStockObject(BLACK_BRUSH));
				///// get our parents brush, how the fuck do we do that...
				//HBRUSH pBrush = (HBRUSH)GetClassLong(hParent, GCL_HBRBACKGROUND);
				//HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, pBrush);
				//RECT rc; GetClientRect(Wnd(), &rc);
				//Rectangle(hDC, 0, 0, rc.right, rc.bottom);	
				//SelectObject(hDC, oldPen);
				//SelectObject(hDC, oldBrush);
				//DeleteObject(hPen);
			}
			EndPaint(hWnd, &ps);

		}
		//if ( msg == WM_ERASEBKGND )
		//{ 
		//	if (currentlyCreatedWindow == this)
		//		return DEF_WND_PROC;

		//	SendMessage(hParent, WM_ERASEBKGND, wParam, lParam); 
		//	return TRUE; 
		//}

		return DEF_WND_PROC;
	}
	void HighlightWindow(bool hilightWnd) { this->hilightWnd = hilightWnd; }
	bool hilightWnd;
};
#define CreateTransparentWnd WindowFactory<TransparentWnd>::create
typedef TransparentWnd* pTW;

// smart gdi object selector that restores the original brush when falling out of scope
class SelectGdiObj
{
	HBRUSH oldBrush; 
	HPEN oldPen; 
	HDC dc;

public:
	SelectGdiObj(HDC dc) : dc(dc), oldBrush(NULL), oldPen(NULL) {}
	~SelectGdiObj()
	{
		if (oldPen) SelectObject(dc, oldPen);
		if (oldBrush) SelectObject(dc, oldBrush);
	}

	SelectGdiObj & Brush(HBRUSH b) 
	{
		oldBrush = (HBRUSH)SelectObject(dc, b);
		return *this;
	}

	SelectGdiObj & Pen(HPEN p)
	{
		 oldPen = (HPEN)SelectObject(dc, p); 
		 return *this;
	}
};



#endif