// class Window wraps the WndProc/DialogProc and some window related winapi stuff into one a single class. 

#ifndef WINAPI_WINDOW_H
#define WINAPI_WINDOW_H

#include <windows.h>
#include <list>
#include "winapi_DataConversion.h"
#include "winapi_error.h"
#include "IWindow.h"

#define WINAPI_MAIN int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#define defaultButtonSize 28

// get's a control's rect relative to its parent

UINT RectWidth(const RECT& r);
UINT RectHeight(const RECT& r);
void GetClientRectRelativeToParent(HWND hwnd, RECT& r);
void GetWindowRectRelativeToParent(HWND hwnd, RECT& r);
RECT CalculateNonClientArea(HWND hwnd);
UINT PercentageOfWindowWidth(HWND window, float percentage);
UINT PercentageOfWindowHeight(HWND window, float percentage);

class IWinapiWindow : public IWindow {
	public: virtual HWND Wnd() const = 0;
};

class Window : public IWinapiWindow
{
public:

	Window();
	Window(int x, int y, uint width, uint height, IWindow* parent = NULL)
	{
		Init();
		InitWindow(x, y, width, height, parent);
	}
	Window(HWND hwnd)  { NaiveWrap(hwnd); }
	virtual ~Window();

	// Window operations
	void InitWindow(int x, int y, uint width, uint height, IWindow* parent = NULL);
	void CreateWnd(HWND hWnd); // this just assigns the hwnd state to its own and then calls send message with WM_INITDIALOG/WM_CREATE	
	virtual HWND InitiateWnd(HWND parentWnd, int wndFlags, int xPos, int yPos, int width, int height);
	virtual HWND InitiateWnd(HWND parentWnd, HMENU menu, UINT classStyle, int wndFlags, int xPos, int yPos, int width, int height);
	virtual HWND InitiateDlg(HINSTANCE hInstance, LPCTSTR lpTemplate, HWND hWndParent);
	virtual HWND SubclassWnd(HWND hWnd);
	void NaiveWrap(HWND windowToWrap) { window = windowToWrap; }
	//virtual HWND SubclassDlg(HWND hWnd);
	void ResizeWindow() {}; // AD_TODO: implement

	// repositioning methods
	void CenterVerticallyBetweenTwoWindows(Window topWnd, Window bottomWnd);

	// accessors
	//WPARAM Wparam() const { return wparam; }
	void Wnd(HWND hwnd) { window = hwnd; } 
	HWND Wnd() const { return window; }
	void SetWndClassName(const tstring& name);
	HDC DC() const;
	
	
	UINT ClientWidth() {  GetClientRect(); return client.right;  }
	UINT ClientHeight() { GetClientRect(); return client.bottom; }
	static std::list<Window*>* GetWindowList();

	// General functions that can be applied to any hWnd
	static void ClientResize(HWND hWnd, int nWidth, int nHeight);
	operator HWND() { return window; }
	void GetClientRect()                  { ::GetClientRect(Wnd(), &client); }
	RECT& GetClientRectRelativeToParent() { ::GetClientRectRelativeToParent(Wnd(), client); return client; }
	RECT& GetWindowRectRelativeToParent() { ::GetWindowRectRelativeToParent(Wnd(), client); return client; };

	int NonClientLeft() { return GetWindowRectRelativeToParent().left; }
	int RightWindow()   { return GetWindowRectRelativeToParent().right; }
	int TopWindow()		{ return GetWindowRectRelativeToParent().top; }
	int BottomWindow()  { return GetWindowRectRelativeToParent().bottom; }

	int LeftClient()   { return GetClientRectRelativeToParent().left; }
	int RightClient()  { return GetClientRectRelativeToParent().right; }
	int TopClient()    { return GetClientRectRelativeToParent().top; }
	int BottomClient() { return GetClientRectRelativeToParent().bottom; }

	RECT client, screen;

	// static functions related to windows and the winapi
	static int RunGetMessageLoop();
protected: 
	// c++ callback handler methods. pass these to the type needing a callback function.
	static LRESULT CALLBACK WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static BOOL    CALLBACK DlgProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { if ( msg == WM_CLOSE) PostQuitMessage(0); return (DefWindowProc(hWnd, msg, wParam, lParam)); }
	virtual BOOL    CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { if ( msg == WM_CLOSE) PostQuitMessage(0); return TRUE; }

	// member states
	HANDLE instance; // hInstance of the module that owns the window
	HWND window;//, parent;
	HDC  m_hDC;
	WNDCLASSEX m_wc;
	TCHAR m_WinClassName[0x400];
	//UINT message; WPARAM wparam; LPARAM lparam;
	WNDPROC originalWndProc;//, originalDialogProc;
	

	// member containers of states 
	static std::list<Window*>* m_WindowList;

	static Window* windowThatisBeingCreated; // InitiateWnd/Dlg sets this before calling CreateWnd/Dialog,
											 // this is so the static callback Wnd/Dlg Proc can find the Window 
											 // that the windows message belongs too. These are window messages
											 // CreateDialog and CreateWindowEx send before returning flow.


	void WM_Error(LPCTSTR errorMessage); // a function posts an error message and quits via WM_DESTROY

	
	
private:
	void Init(); // common stuff that needs to be done in the constructor
	
//static const int Center;

};

// todo: implement sensible exception handling
//struct WinapiWidowException : WinapiGeneralException
//{
//	char const* what() const throw() { return "General Winapi Window Exception"; } 
//};
//
//struct CantCreateWindowException : WinapiWidowException
//{
//	char const* what() const throw() { return "Could not create window"; }; 
//};

#endif
