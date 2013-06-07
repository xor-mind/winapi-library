#include <string>
#include "winapi_library.h"

std::list<Window*>* Window::m_WindowList = NULL;
Window* Window::windowThatisBeingCreated = NULL;

// Window's IWindow interace implementation -------------------------------- //

// if there's a parent, x and y are wrtt parent, else x and y are wrtt screen.
void Window::InitWindow(int x, int y, uint width, uint height, IWindow* parent = NULL)
{
	this->parent = parent;

	//if (parent)
	//{
	//	x += parent->xposClientInScreen();
	//	y += parent->yposClientInScreen();
	//}
	//RECT r = { x, y, x + width, y + height }; 
	//screen = r;
	//client.left = client.top = 0; // just force a zero dimension non client area for now
	//client.right = width; client.bottom = height;


	InitiateWnd(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, width, height);
}

// Window's implementation ------------------------------------------------- //

Window::Window()
{
	Init();
}

Window::Window(int x, int y, uint width, uint height, IWindow* parent = NULL)
{
	Init();
	InitWindow(x, y, width, height, parent);
}


Window::~Window()
{
	// 
}

void Window::Init()
{
	window = 0;

	//strcpy_s(m_WinClassName, "ssWindowClass");
	tstrcpy(m_WinClassName, _T("ssWindowClass"));
	// AD_TODO: there are other ways of implementing callbacks other than static 
	// functions, in the future i'd like to check them out.

	// Keep track of this object in a static list by inserting in the list a reference to this object.
	// This is a key part of the c++ callback handler implementation. By tracking the Object or 
	// specifically the hWnd property of Object, the generic callback handler can determine whether
	// this object is being requested by a callback operation.
	{
		if (m_WindowList == NULL)
			m_WindowList = new std::list<Window*>;
		m_WindowList->push_back(this); 
	}
}


void Window::SetWndClassName(const tstring& name)
{
	tstrcpy(m_WinClassName, name.c_str());
}

LRESULT CALLBACK Window::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if ( windowThatisBeingCreated != NULL)
	{
		return windowThatisBeingCreated->WndProc(hWnd, msg, wParam, lParam);
	}

	// Polymorphism for the win! 
	for (std::list<Window*>::iterator it=m_WindowList->begin(), end=m_WindowList->end(); it!=end; ++it)
	{
		if ((*it)->Wnd() == hWnd) 
		{
			//(*it)->message = msg;
			return (*it)->WndProc(hWnd, msg, wParam, lParam);
		}
	}

	return (DefWindowProc(hWnd, msg, wParam, lParam));
}

BOOL CALLBACK Window::DlgProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if ( windowThatisBeingCreated != NULL)
	{
		return windowThatisBeingCreated->DlgProc(hWnd, msg, wParam, lParam);
	}

	for (std::list<Window*>::iterator it = m_WindowList->begin(), end=m_WindowList->end(); it != end; ++it)
	{
		if ((*it)->Wnd() == hWnd)
		{
			//(*it)->message = msg;
			return (*it)->DlgProc(hWnd, msg, wParam, lParam);
		}
	}

	return FALSE;
}

std::list<Window*>* Window::GetWindowList()
{
	return Window::m_WindowList;
}

HDC Window::DC() const 
{
	return  m_hDC;
}

HWND Window::InitiateWnd(HWND parentWnd, int wndFlags, int xPos, int yPos, int width, int height)
{
	InitiateWnd(parentWnd, 0, CS_OWNDC | CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW, 
		        wndFlags, xPos, yPos, width, height);
	return window;
}

HWND Window::InitiateWnd(HWND parentWnd, HMENU menu, UINT classStyle, int wndFlags, int xPos, int yPos, int width, int height)
{
	RtlZeroMemory(&m_wc, sizeof(m_wc));
	m_wc.style         = classStyle;
	m_wc.lpfnWndProc   = WndProcHandler;
	//m_wc.cbClsExtra    = 0;
	//m_wc.cbWndExtra    = 0;
	//m_wc.hIconSm       = NULL;
	m_wc.hInstance     = GetModuleHandle(0);
	//m_wc.hIcon         = NULL;
	//m_wc.hCursor       = NULL;
	m_wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//(HBRUSH)(COLOR_WINDOW + 1);
	//m_wc.lpszMenuName  = 0;
	m_wc.lpszClassName = m_WinClassName;
	m_wc.cbSize        = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&m_wc))
	{
		PrintLastWinAPIError(_T("RegisterClassEx"));
		//MessageBox(NULL, _T("Can't register winclass"), _T("Error!"), MB_OK);
		return 0;
	}

	//// center the windows if need be
	//if (xPos == Center)
	//{
	//	xPos = GetMonitorWidth()/2 - width/2;
	//}

	//if (yPos == Center)
	//{
	//	yPos = GetMonitorHeight()/2 - height/2;
	//}

	windowThatisBeingCreated = this; // let the callback wndproc know a CreateWindow funtion was just called

	if (!(window = CreateWindowEx(0, m_WinClassName, _T(""), wndFlags, xPos, yPos, width, 
								 height, parentWnd, menu, GetModuleHandle(0), 0)))
	{
		PrintLastWinAPIError(_T("CreateWindowEx"));
		return 0;
	}
	parent = parentWnd;
	windowThatisBeingCreated = NULL; // clear the CreatingWindow flag

	//LookForMissedMessages(); // now that the window has been initiated, look for missed messages 

	return window;
}

HWND Window::InitiateDlg(HINSTANCE hInstance, LPCTSTR lpTemplate, HWND hWndParent)
{
	parent = hWndParent;

	windowThatisBeingCreated = this; // let the callback wndproc know a CreateDialog funtion was just called

	window = CreateDialog(hInstance, lpTemplate, parent, Window::DlgProcHandler);

	windowThatisBeingCreated = NULL; // clear the CreateDialog flag

	//LookForMissedMessages(); // now that the window has been initiated, look for missed messages 

	return window;
}

HWND Window::SubclassWnd(HWND hWnd)
{
	// AD_TODO: Add check to make sure this is a non dialog window.
	window = hWnd;

	originalWndProc = (WNDPROC)SetWindowLong(window, GWL_WNDPROC, (LONG)(Window::WndProcHandler));

	return window;
}

//HWND Window::SubclassDlg(HWND hWnd)
//{
//	// AD_TODO: Add check to make sure this is a dialog window.
//	window = hWnd;
//
//	m_originalDialogProc = (WNDPROC)SetWindowLong(window, GWL_WNDPROC, (LONG)(Window::DlgProcHandler))))
//
//	return window;
//}

void Window::CreateWnd(HWND hWnd) // this just assigns the hwnd state to its own and looks for any missed messages
{
	window = hWnd;
	// ; // now that the window has been initiated, look for missed messages 
}

void Window::WM_Error(LPCTSTR errorMessage)
{
	MessageBox(window, errorMessage, _T("Error!"), MB_OK|MB_ICONEXCLAMATION);
	PostMessage(window, WM_CLOSE, 0, 0);
}

void Window::ClientResize(HWND hWnd, int nWidth, int nHeight)
{
	RECT rcClient, rcWind;
	POINT ptDiff;
	::GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWind);
	ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
	ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
	MoveWindow(hWnd,rcWind.left, rcWind.top, nWidth + ptDiff.x, nHeight + ptDiff.y, TRUE);
}


// positioning methods ----------------------------------------------------- //


void Window::CenterVerticallyBetweenTwoWindows(Window topWnd, Window bottomWnd)
{ // assuming all windows are in the same parent
	
	// - find the yposition to place the window
	// - find the distance between the two windows
	// - compute the center based on the distance and 
	//   the height of the window to be centered.
	UINT distance = bottomWnd.TopWindow() - topWnd.BottomWindow(),
		 y = midpoint<UINT>(distance, ClientHeight());

	SetWindowPos(Wnd(), NULL, 5, y, 0, 0, SWP_NOSIZE);
}

// -==============-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=================- //

UINT PercentageOfWindowWidth(HWND window, float percentage)
{
	percentage *= .01f; // convert precentage into decimal representation
	RECT r;
	GetClientRect(window, &r);
	return UINT(r.right * percentage);
}

UINT PercentageOfWindowHeight(HWND window, float percentage)
{
	percentage *= .01f; // convert precentage into decimal representation
	RECT r;
	GetClientRect(window, &r);
	return UINT(r.bottom * percentage);
}

UINT RectWidth(const RECT& r) {
	return abs(r.right - r.left);
}

UINT RectHeight(const RECT& r) {
	return abs(r.bottom - r.top);
}

RECT CalculateNonClientArea(HWND hwnd)
{
	RECT window, client, nonclient;
	GetWindowRect(hwnd, &window), GetClientRect(hwnd, &client);
	int nonClientWidth  = RectWidth(window) - RectWidth(client),
		nonClientHeight = RectHeight(window) - RectHeight(client);
		
	nonclient.top = nonclient.left = 0; 
	nonclient.right = nonClientWidth; nonclient.bottom = nonClientHeight;
	return nonclient;
}

void GetClientRectRelativeToParent(HWND hwnd, RECT& r)
{
	GetClientRect(hwnd, &r);
	UINT w = RectWidth(r), h = RectHeight(r);
	POINT p = {0, 0};
	ClientToScreen(hwnd, &p);
	ScreenToClient(GetParent(hwnd), &p);
	r.left = p.x, r.top = p.y, r.right = r.left + w, r.bottom = r.top + h;
}

void GetWindowRectRelativeToParent(HWND hwnd, RECT& r) 
{
   // Get the position of the window relative to the entire screen
   GetWindowRect(hwnd, &r);

   // Now convert those with regards to the control's parent
   ScreenToClient(GetParent(hwnd), (LPPOINT) &((LPPOINT)&r)[0]);
   ScreenToClient(GetParent(hwnd), (LPPOINT) &((LPPOINT)&r)[1]);
}

// static methods related to windows and the winapi
int Window::RunGetMessageLoop()
{
	MSG Msg;
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}


