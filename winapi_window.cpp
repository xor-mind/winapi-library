#include "winapi_window.h"
#include "winapi_gdi.h"
#include "winapi_error.h"
#include <vector>

#define ret return
#define HasParent(x) ( parent ? defaultChild##x : default##x ) // default args in Window are named defaultChildx or defaultx, this macro helps load the correct default

inline int dxRect(RECT& r)
{
	return r.right - r.left;
}
inline int dyRect(RECT& r)
{
	return r.bottom - r.top;
}

WindowList Window::windowList;
Window* Window::currentlyCreatedWindow = NULL;

const DWORD  Window::defaultClassStyle       = CS_OWNDC | CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
const DWORD  Window::defaultChildClassStyle  = CS_OWNDC | CS_DBLCLKS;
const DWORD  Window::defaultWindowStyle      = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
const DWORD  Window::defaultChildWindowStyle = WS_CHILD | WS_VISIBLE; 
TCHAR* const Window::defaultWindowName       = _T("Winapi Window");
TCHAR* const Window::defaultChildWindowName  = _T("Child Window");
TCHAR* const Window::defaultClassName        = _T("winapiWindowClass");
TCHAR* const Window::defaultChildClassName   = _T("winapiChildWindowClass");
 
Window::Window()
{

	
	// Keep track of this object in a static list by inserting in the list a reference to this object.
	// By tracking the Object or specifically the hWnd property of Object, the generic callback handler 
	// can determine whether this object is being requested by a callback operation.
	windowList.push_back(this); 

}

Window::~Window() 
{ 
	windowList.erase(std::find(windowList.begin(), windowList.end(), this));	
}	

void Window::InitWnd()
{
	className[0] = _T('\0');
	oldWndProc = NULL;
	hWnd = hParent = NULL;
	parent = NULL;
	highlightWnd = false;
	postQuitMessage = true;
}

//Window * Window::InitWndSpec1(int x, int y, uint width, uint Height, TCHAR* const windowName, IWindow* parent)
//{
//	InitWnd();
//	if ( parent && ((Window*)parent)->pad ) 
//	{
//		InitWnd(parent, 0, 0, (className[0] != _T('\0') ) ? className : HasParent(ClassName), windowName ? windowName : HasParent(WindowName),  
//			   CS_DBLCLKS | CS_PARENTDC, HasParent(WindowStyle), ((Window*)parent)->pad->left->pixelCount() + x, ((Window*)parent)->pad->top->pixelCount() + y,  width, Height); 
//	}
//	else
//	{
//		InitWnd(parent, 0, 0, (className[0] != _T('\0') ) ? className : HasParent(ClassName), windowName ? windowName : HasParent(WindowName),  
//			   CS_DBLCLKS | CS_PARENTDC, HasParent(WindowStyle), x, y,  width, Height); 
//	}
//	return this;
//}

Window * Window::InitWnd(int x, int y, uint width, uint height, IWindow* parent)
{
	//if ( parent && ((Window*)parent)->pad ) 
	//{
		InitWnd(x, y, width, height,(TCHAR* const)HasParent(WindowName), parent);
	//}
	//else
	//{
	//}
	return this;
}

Window * Window::InitWnd(int x, int y, uint width, uint height, const Padding& p, IWindow* parent)
{
	InitWnd(x, y, width, height, (TCHAR* const)HasParent(WindowName), parent);
	pad.reset( new Padding(p, ClientWidth(), ClientHeight()) );
	return this;
}

Window * Window::InitWnd(int x, int y, uint width, uint Height, TCHAR* const windowName, IWindow* parent)
{
	InitWnd();
	if ( parent && ((Window*)parent)->pad ) 
	{
		InitWnd(parent, 0, 0, (className[0] != _T('\0') ) ? className : HasParent(ClassName), windowName ? windowName : HasParent(WindowName),  
			   HasParent(ClassStyle), HasParent(WindowStyle), ((Window*)parent)->pad->left->pixelCount() + x, ((Window*)parent)->pad->top->pixelCount() + y,  width, Height); 
	}
	else
	{
		InitWnd(parent, 0, 0, (className[0] != _T('\0') ) ? className : HasParent(ClassName), windowName ? windowName : HasParent(WindowName),  
			   HasParent(ClassStyle), HasParent(WindowStyle), x, y,  width, Height); 
	}
	return this;
}

Window * Window::InitWnd(int x, int y, uint width, uint Height, TCHAR* const className, TCHAR* const windowName, IWindow* parent)
{
	InitWnd();
	if ( parent && ((Window*)parent)->pad ) {
	InitWnd(parent, 0, 0, className, windowName ? windowName : HasParent(WindowName),  
		HasParent(ClassStyle), HasParent(WindowStyle), ((Window*)parent)->pad->left->pixelCount() + x, ((Window*)parent)->pad->top->pixelCount() + y,  width, Height); }
	else {
		InitWnd(parent, 0, 0, className, windowName ? windowName : HasParent(WindowName),  
			HasParent(ClassStyle), HasParent(WindowStyle), x, y,  width, Height); }

	return this;
}

Window * Window::InitWnd(int x, int y, uint width, uint height, HBRUSH brush, IWindow* parent)
{InitWnd();
	InitWnd(parent, 0, brush, (className[0] != _T('\0')) ? className : HasParent(ClassName), NULL,  
			   HasParent(ClassStyle), HasParent(WindowStyle), x, y,  width, height); 
	return this;
}

Window * Window::InitAdjustedWnd( int x, int y, uint width, uint height, IWindow* parent )
{
	int t1 = Window::defaultWindowStyle,
		t2 = Window::defaultChildWindowStyle,
		t3 = HasParent(WindowStyle),
		t4 = WS_OVERLAPPEDWINDOW;  // this code is diaganostic for the time being
	RECT rc = { 0, 0, width, height };
	AdjustWindowRect( &rc,  HasParent(WindowStyle), FALSE);
	InitWnd(x, y, rc.right - rc.left, rc.bottom - rc.top, NULL)->CenterWrtParent(); 
	return this;
}

WND_PROC_DECL(Window::Handler)
{
	if (currentlyCreatedWindow)
	{	
		currentlyCreatedWindow->hWnd = hWnd;
		return currentlyCreatedWindow->WndProc(hWnd, msg, wParam, lParam);
	}

	// Polymorphism for the win! 
	for (WindowList::iterator it=windowList.begin(), end=windowList.end(); it != end; ++it)
	{
		if ((*it)->Wnd() == hWnd) 
		{
			return (*it)->WndProc(hWnd, msg, wParam, lParam);
		}
	}

	return DEF_WND_PROC;
}

DLG_PROC_DECL(Window::Handler)
{
	if (currentlyCreatedWindow)
	{
		currentlyCreatedWindow->hWnd = hWnd;
		return currentlyCreatedWindow->DlgProc(hWnd, msg, wParam, lParam);
	}
	// Polymorphism for the win! 
	for (WindowList::iterator it=windowList.begin(), end=windowList.end(); it != end; ++it)
	{
		if ((*it)->Wnd() == hWnd) 
		{
			return (*it)->DlgProc(hWnd, msg, wParam, lParam);
		}
	}

	return FALSE;
}

Window * Window::InitWnd(IWindow* parent, HMENU menu, HBRUSH backgroundBrush, TCHAR* const className, TCHAR* const windowName, UINT classStyle, UINT wndFlags, int xPos, int yPos, int width, int Height)
{
	this->parent = (Window*)parent;
	hParent = ( parent ? this->parent->Wnd() : NULL );
	_tcscpy_s(this->className, _countof(this->className), className ? className : HasParent(ClassName));
	_tcscpy_s(this->windowName, _countof(this->windowName), windowName ? windowName : HasParent(WindowName)); 
	classStyle = classStyle ? classStyle : defaultClassStyle;
	wndFlags = wndFlags ? wndFlags : HasParent(WindowStyle);
	if ( this->parent ) 
	{
		if ( this->parent )
			this->parent->AddChild(this);
	}
	RtlZeroMemory(&wc, sizeof(wc));
	wc.style         = classStyle;
	wc.lpfnWndProc   = HandlerWndProc;//WndProcHandler;
	//m_wc.cbClsExtra    = 0;
	//m_wc.cbWndExtra    = 0;
	//m_wc.hIconSm       = NULL;
	wc.hInstance     = GetModuleHandle(0);
	//m_wc.hIcon         = NULL;
	wc.hCursor     = LoadCursor(NULL, IDC_ARROW);

	wc.hbrBackground = backgroundBrush ? backgroundBrush : (HBRUSH)GetStockObject(BLACK_BRUSH);
	//m_wc.lpszMenuName  = 0;
	//tstrcpy(className, _T("adamsWindowClass"));
	wc.lpszClassName = this->className;
	wc.cbSize        = sizeof(WNDCLASSEX);


	if (!RegisterClassEx(&wc))
	{
		PrintLastWinAPIError(_T("RegisterClassEx"));
		return 0;
	}
	
	CREATESTRUCT cs;
	cs.dwExStyle = 0; cs.lpszClass = this->className; cs.lpszName = windowName; cs.style = wndFlags; cs.x = xPos; cs.y = yPos; 
	cs.cx = width; cs.cy = Height; cs.hwndParent = hParent; cs.hMenu = menu; cs.hInstance = GetModuleHandle(0); cs.lpCreateParams = 0;
	
	currentlyCreatedWindow = this; HWND h;
	SetWnd(h = CreateWindowEx(cs.dwExStyle, cs.lpszClass, cs.lpszName, cs.style, cs.x, cs.y, 
		cs.cx, cs.cy, cs.hwndParent, cs.hMenu,cs.hInstance, cs.lpCreateParams));
	if ( Wnd() == NULL)
	{
		//MessageBox(0, _T("Can't create window"), _T("Shit!"), MB_OK|MB_ICONEXCLAMATION);
		PrintLastWinAPIError(_T("CreateWindowEx"));
		return 0;
	}
	currentlyCreatedWindow = NULL;

	return this;
}

Window * Window::InitWnd(IWindow* parent, HINSTANCE hInstance, LPCTSTR lpTemplate)
{
	InitWnd();
	this->parent = (Window*)parent;
	hParent = ( parent ? this->parent->Wnd() : NULL );
		if ( this->parent )
			this->parent->children.push_back(this);

	currentlyCreatedWindow = this; // let the callback wndproc know a CreateDialog funtion was just called

	SetWnd(CreateDialog(hInstance, lpTemplate, hParent, Window::HandlerDlgProc));

	currentlyCreatedWindow = NULL; // clear the CreateDialog flag

	return this;
}

Window * Window::InitWnd(int templateId)
{
	InitWnd();
	return InitWnd(NULL, AppInstance, MAKEINTRESOURCE(templateId));
}

void Window::DestroyWindow() 
{ 
	::DestroyWindow(Wnd()); 
}

void Window::UnregisterClass()
{
	::UnregisterClass(className, GetModuleHandle(0)); 
}

void Window::Expunge()
{
	if (parent)
		parent->RemoveChild(this);
	DestroyWindow(); 
	UnregisterClass();
	while( 1 )
	{
		WindowList::iterator itr = children.begin(), end = children.end();
		if ( itr == end )
			break;
		(*itr)->Expunge();
	}
	InitWnd();
}

void Window::SetWndClassName(const tstring& name)
{
	_tcscpy_s(className, _countof(className), name.c_str());
}

WND_PROC_DECL(Window::)
{
	handleDefaultMsgs

	return DEF_WND_PROC; 
}

DLG_PROC_DECL(Window::) 
{
	handleDefaultMsgs

	return FALSE; 
}

void Window::CenterVertically(IWindow* topWindow, IWindow* bottomWindow)
{
	Window* top = (Window *)topWindow;
	Window* bot = (Window *)bottomWindow;

	// center vertically wrt to screen
	int distance = bot->ScreenRect().top - top->ScreenRect().bottom,
				y = midpoint<int>(distance, Height());

	POINT p = { ScreenRect().left, top->ScreenRect().bottom + y };
		
	// convert the coords to client coords and set the window position.
	if (parent)
		parent->ScreenToClient(p.x, p.y);

	//if (parent) parent->ScreenToClient(p);
	SetNonClientPos(p.x, p.y);
}

int Window::xPos() const
{ 
	RECT r;
	::GetWindowRect(Wnd(), (LPRECT)&r); 
	if (parent == NULL) 
	{
		return r.left; 
	}
	else 
	{
		return r.left - parent->xPosClientInScreen(); 
	}
}

int Window::yPos() const
{ 
	RECT r;
	::GetWindowRect(Wnd(), (LPRECT)&r); 
	if (parent == NULL) 
	{
		return r.top; 
	}
	else 
	{
		return r.top - parent->yPosClientInScreen(); 
	}
}

int Window::x1Pos() const
{ 
	RECT r;
	::GetWindowRect(Wnd(), (LPRECT)&r); 
	if (parent == NULL) 
	{
		return r.right; 
	}
	else 
	{
		return r.right - parent->xPosClientInScreen(); 
	}
}

int Window::y1Pos() const
{ 
	RECT r;
	::GetWindowRect(Wnd(), (LPRECT)&r); 
	if (parent == NULL) 
	{
		return r.bottom; 
	}
	else 
	{
		//return r.bottom - parent->yPosClientInScreen();
		int test = parent->yPosClientInScreen();
		return r.bottom - test; 
	}
}

int Window::Width() const
{
	RECT r;
	::GetWindowRect(Wnd(), (LPRECT)&r);
	return dxRect(r) ;
}

int Window::Height() const 
{ 
	RECT r;
	::GetWindowRect(Wnd(), (LPRECT)&r);
	return dyRect(r);	
} 

int Window::TrueWidth() const
{
	RECT r;
	::GetWindowRect(Wnd(), (LPRECT)&r);
	return dxRect(r) ;
}

int Window::TrueHeight() const 
{ 
	RECT r;
	::GetWindowRect(Wnd(), (LPRECT)&r);
	return dyRect(r);	
} 

int Window::ClientWidth()  const 
{ 
	RECT r = { 0, 0, 0, 0 };
	if ( Wnd() )
		::GetClientRect(Wnd(), (LPRECT)&r);
	return r.right - (pad ? pad->Width(r.right) : 0);
}

int Window::ClientHeight() const 
{ 
	RECT r = { 0, 0, 0, 0 };
	if ( Wnd() )
		::GetClientRect(Wnd(), (LPRECT)&r);
	return r.bottom - (pad.get() ? pad->Height(r.bottom) : 0);
}

int Window::pClientWidth() const 
{ 
	return parent->ClientWidth(); 
}

int Window::pClientHeight() const 
{ 
	return parent->ClientHeight(); 
}

int Window::TrueClientWidth()  const 
{ 
	RECT r;
	::GetClientRect(Wnd(), (LPRECT)&r);
	return r.right;
}

int Window::TrueClientHeight() const 
{ 
	RECT r;
	::GetClientRect(Wnd(), (LPRECT)&r);
	return r.bottom;
}


// x/y coord deltas between the screen and client window

 // x/y coord deltas between the screen and client window

int Window::dxNonClientAndClient() const 
{ 
	return xPosClientInScreen() - ScreenRect().left; 
} // x/y coord deltas between the screen and client window

int Window::dyNonClientAndClient() const  
{ 
	return yPosClientInScreen() - ScreenRect().top; 
}

RECT Window::ScreenRect() const
{ 
	RECT r;
	GetWindowRect(Wnd(), &r); return r; 
}

POINT Window::ClientToScreen(int x, int y)
{
	POINT p = { x , y }; ::ClientToScreen(Wnd(), &p); return p; 
}
POINT & Window::ClientToScreen(POINT& p) const
{
	::ClientToScreen(Wnd(), &p);
	return p;
}
POINT & Window::ScreenToClient(POINT& p) const 
{
	::ScreenToClient(Wnd(), &p);
	return p;
}
POINT Window::ScreenToClient(int& x, int & y)
{
	POINT p = { x , y }; ::ScreenToClient(Wnd(), &p); x = p.x; y = p.y; return p; 
}

void Window::SetNonClientPos(int x, int y) 
{  
	::SetWindowPos(Wnd(), 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Window::SetNonClientPos(int x, int y, int width, int height) 
{  
	int dx = parent ? parent->PaddingLeft(): 0, 
		dy = parent ? parent->PaddingTop() : 0;
	::SetWindowPos(Wnd(), 0, x + dx, y + dy, width, height, SWP_NOZORDER);
	if ( pad ) {
		RECT rc; ::GetClientRect(Wnd(), &rc);
		pad->WidthAndHeight(rc.right, rc.bottom);
	}
}

Window* Window::ForceRedraw()
{
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);
	return this;
}

int Window::RunGetMessageLoop()
{
	MSG message;

	std::list<Window*>::iterator it, end;
	
	while(GetMessage(&message, 0, 0, 0))
	{
		size_t i=0;

		for (it =Window::windowList.begin(); it != Window::windowList.end(); ++it)
		{
			if (IsDialogMessage((*it)->Wnd(), &message) > 0)
			{
				break;
			}		
		}

		if ( it == Window::windowList.end())
		{
			TranslateMessage(&message); 
			DispatchMessage(&message);	
		}
	}

	return message.wParam;
}

int Window::RunPeekMessageLoop()
{
	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message == WM_QUIT) 
				break;
			TranslateMessage(&msg); 
			DispatchMessage(&msg);		
		}
		PeekMessageLoop();
	}

	return msg.wParam;
}

Window * Window::ShowWindow() 
{ 
	::ShowWindow(Wnd(), SW_SHOW); 
	return this; 
}

Window * Window::SetText(const TCHAR * const windowName) 
{
	SetWindowText(Wnd(), windowName); return this;
}

Window * Window::SetClassBrush(HBRUSH newBrush)
{
	HBRUSH oldBrush = (HBRUSH)SetClassLong(Wnd(), GCLP_HBRBACKGROUND, (LONG)newBrush);
	DeleteObject(oldBrush);
	return this;
}

Window * Window::SetClassCursor(TCHAR const * id)
{
	HCURSOR hCursor    = ::LoadCursor (0, id); 
	HCURSOR hOldCursor = (HCURSOR)SetClassLong(Wnd(), GCLP_HCURSOR, (LONG)hCursor);
	DeleteObject(hOldCursor);
	return this;
}

Window * Window::CaptureMouse()
{
	::SetCapture(Wnd());
	return this;
}

Window * Window::CenterWrtParent()
{
	int x, y;
	if ( parent )
	{	x = midpoint<int>(parent->TrueClientWidth(), TrueWidth()),
		y = midpoint<int>(parent->TrueClientHeight(), TrueHeight());	}
	else
	{	RECT rc; GetWindowRect(GetDesktopWindow(), &rc);
		x = midpoint<int>(rc.right, TrueWidth()),
		y = midpoint<int>(rc.bottom, TrueHeight());	 }

	SetNonClientPos(x, y);

	return this;
}

Window * Window::FillClient(HPEN pen, HBRUSH brush)
{
	DC dc(this->Wnd());

	SelectGdiObj sgo(dc); sgo.Brush(brush).Pen(pen);

	RECT rc = { *pad->left, *pad->top, ClientWidth(), ClientHeight() };

	Rectangle(dc, rc.left, rc.top, rc.left + rc.right, rc.top + rc.bottom);

	return this;
}

Window * Window::SetParent(Window* parent) 
{ 
	this->parent = parent; 
	if (parent)  {
		hParent = parent->Wnd(); 
	}
	return this;
}

Window * Window::PushButton()
{
	SendMessage(hParent, WM_COMMAND, MAKELONG(GetDlgCtrlID(Wnd()), BN_CLICKED), (LPARAM)Wnd());
	return this;
}

Window *  Window::HighlightPadding(HBRUSH b)
{
	if ( !pad ) 
		return this;

	//pad->WidthAndHeight(TrueClientWidth(), TrueClientHeight());
	DC dc(this->Wnd());

	SelectGdiObj sgo(dc); sgo.Brush(b);

	RECT rc  = { 0, 0,  *pad->left, TrueClientHeight() }; 
	Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);

	RECT rc2 = { TrueClientWidth() - *pad->right, 0,  TrueClientWidth() , TrueClientHeight() }; 
	Rectangle(dc, rc2.left, rc2.top,rc2.right,rc2.bottom);

	RECT rc3 = { 0, TrueClientHeight() - *pad->bot, TrueClientWidth(), TrueClientHeight() };
	Rectangle(dc, rc3.left, rc3.top, rc3.right, rc3.bottom);

	RECT rc1 = { 0, 0,  TrueClientWidth(), *pad->top }; 
	Rectangle(dc, rc1.left, rc1.top, rc1.right, rc1.bottom);

	return this;
}

Window *  Window::HighlightPadding(HBRUSH b, HDC dc)
{
	if ( !pad ) 
		return this;

	SelectGdiObj sgo(dc); sgo.Brush(b);

	RECT rc  = { 0, 0,  *pad->left, TrueClientHeight() }; 
	Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);

	RECT rc2 = { TrueClientWidth() - *pad->right, 0,  TrueClientWidth() , TrueClientHeight() }; 
	Rectangle(dc, rc2.left, rc2.top,rc2.right,rc2.bottom);

	RECT rc3 = { 0, TrueClientHeight() - *pad->bot, TrueClientWidth(), TrueClientHeight() };
	Rectangle(dc, rc3.left, rc3.top, rc3.right, rc3.bottom);

	RECT rc1 = { 0, 0,  TrueClientWidth(), *pad->top }; 
	Rectangle(dc, rc1.left, rc1.top, rc1.right, rc1.bottom);

	return this;
}

Window * Window::HighlightPadding( bool shouldWeHighlightThisWindowsPadding )
{
	highlightWnd = shouldWeHighlightThisWindowsPadding;
	return this;
}

Window * Window::ProcessPostQuitMessage(bool b) 
{ 
	postQuitMessage = b; 
	return this; 
} 

tstring Window::GetText()
{
	std::vector<TCHAR> v( GetWindowTextLength( Wnd() ) + 1 );
	GetWindowText( Wnd(), &v[0], v.size() );
	//return  tstring( v.begin(), v.end() );
	
	// 7/4/2012 GetWindowText and GetWindowText is lame, this makes sure there isn't an extra zero tacked
	// onto my string that I'm returning.
	tstring s;
	for ( size_t i = 0; i < v.size(); ++i)
	{
		if ( v[i] == 0 )
			break;
		else
			s += v[i];
	}
	return s;
}

int Window::dxLeftAndLeft(Window* otherWnd) const
{
	int dx;

	// if the other window is inside of us
	if ( otherWnd->hParent = Wnd() )
	{
		dx = otherWnd->ScreenRect().left - xPosClientInScreen();
	}
	else
	{
		dx = otherWnd->ScreenRect().left - ScreenRect().left;
	}

	return dx;
}

int Window::dxRightAndRight(Window* otherWnd) const
{
	// if the other window is inside of us
	if ( otherWnd->hParent = Wnd() )
	{
		ret x1PosClientInScreen() - otherWnd->ScreenRect().right;
	}
	else
	{
		ret ScreenRect().right - otherWnd->ScreenRect().right;
	}
}

int Window::dyTopAndTop(Window* otherWnd) const
{
	if ( Wnd() == otherWnd->ParentWnd() )
	{
		return otherWnd->ScreenRect().top - yPosClientInScreen();
	}
	else
	{
		return  otherWnd->ScreenRect().top - ScreenRect().top;
	}
}

int Window::dyBotAndBot(Window* otherWnd) const
{
	if ( Wnd() == otherWnd->ParentWnd() )
	{
		return y1PosClientInScreen() - otherWnd->ScreenRect().bottom;
	}
	else
	{
		return ScreenRect().bottom - otherWnd->ScreenRect().bottom;
	}
}

HFONT Window::GetCurrentFont() 
{
	DC dc( this->Wnd() );
	return (HFONT)GetCurrentObject(dc, OBJ_FONT);
}