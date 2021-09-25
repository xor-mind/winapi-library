#ifndef WINDOW_TESTS_H
#define WINDOW_TESTS_H

#include <algorithm>  
#include <iostream>

//#include "TestTracker.h"
//#include "resource.h"

#include "..\GdiObjectFactory.h"
#include "..\winapi_error.h"
#include "..\winapi_structures.h"
#include "..\window.h"

#include <boost\assign\list_of.hpp> 
#include <boost\assign\std\vector.hpp>
#include <boost\assign\std\list.hpp>
#include <boost\function.hpp>
#include <boost\scoped_ptr.hpp>
#include <boost\shared_ptr.hpp>

// Assert w is not null and that we have a valid hWnd.
void ValidateHWND(Window* w);

int dyTwoWindows(Window* top, Window* bottom);

typedef struct BasicTestWindow : public Window
{ 
	// creates a 400x300 window at (800,100) in screen coords. This is used for testing of the
	// Window class which is an abstraction for a GUI.

	BasicTestWindow(TCHAR* const testName) { 
		//InitWindow(testName);	
	}
	virtual ~BasicTestWindow() {}

	virtual void InitWnd(TCHAR* const windowName) {
		Window::InitWnd(xpos, ypos, width, height, windowName, NULL);
	}

	virtual WND_PROC_DECL()
	{
		if ( msg == WM_CREATE ) {
			this->hWnd = hWnd;
			Create();
		}

		if ( msg == WM_GETMINMAXINFO ) { // some of the test windows want windows with a minimum size of 50x50 pixels.
			//SetPoint(((LPMINMAXINFO)lParam)->ptMinTrackSize, 50, 50);
			((LPMINMAXINFO)lParam)->ptMinTrackSize.x = ((LPMINMAXINFO)lParam)->ptMinTrackSize.y = 50;
		}

		if  (msg == WM_CLOSE ) {
			DestroyWindow();
		}

		return DEF_WND_PROC;
	}
	virtual void Test() {};
	virtual void Create() {}
	virtual void AfterCreate()
	{
		//RunTest();
	}

	// default window dimensions
	const static int xpos = 100, ypos = 100, width = 400, height = 300; 

} BTW;

void Line (HDC dc, int x1, int y1, int x2, int y2);

class TTK // test tool kit
{
protected: 
	BrushFactory bf;
	PenFactory   pf;
};

// BTW : "Basic Test Window"

#define     AppInstance				GetModuleHandle(NULL)
#define		INIT_DLG_PARAM			NULL, AppInstance, MAKEINTRESOURCE(IDD_MAINDLG)
#define     S_VAR(x, y, ...)        static x& y() { static x s_##y = __VA_ARGS__; return s_##y; }
#define		S_BRUSH(x, y)			S_VAR(HBRUSH, x, CreateSolidBrush(y))
#define     S_RECT(x, ...)			S_VAR(RECT, x, __VA_ARGS__)
#define     NBRUSH				    (HBRUSH)GetStockObject(NULL_BRUSH)
//typedef		std::vector<Window *>	WindowList;
//#define		TT						TestTracker						
#define		CreateFixture(x)		TT_FIXTURE_TEST_CASE(x)
#define		Assert(x,y)				BOOST_REQUIRE_EQUAL(x,y)
#define		isEqual(x,y)			BOOST_REQUIRE_EQUAL(x,y)
#define		AssertErr(x, y, _err)   BOOST_REQUIRE_LE( abs( x - y ), _err )

#define CreateWinapiWindowUnitTest( TestClassName, TestStringName )					\
																		\
struct TestClassName : TT, TTK										    \
{																		\
	TEST_NAME( TestStringName );                                        \
																		\
	TestClassName() : TT(TestName()) { RunTest(); }						\
	void Test();														\
};																		\
CreateFixture(TestClassName) { }										\
void TestClassName::Test()

#endif