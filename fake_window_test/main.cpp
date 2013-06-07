// this tests my window's interface with a fake window
#define BOOST_TEST_MODULE WindowTest

#include <boost\test\included\unit_test.hpp>
#include "Console.h"
#include "FakeWindow.h"
#include "global_operators.h"
#include "adams_macros.h"
#include "TestTracker.h"

typedef FakeWindow TWindow;

namespace WindowTest
{
	void ValidateHWND(TWindow* w)
	{
		BOOST_REQUIRE(w); //BOOST_REQUIRE(w->Wnd());
	}

	int dyTwoWindows(TWindow* top, TWindow* bottom) 
	{
		return bottom->yPos() - top->y1Pos();
	}
}

typedef struct BasicTestWindow : public TestTracker, public TWindow
{ 
	// creates a 400x300 window at (800,100) in screen coords. This is used for testing of the
	// Window class which is an abstraction for a GUI.

	BasicTestWindow(TCHAR* const testName) : TestTracker(testName) { 
		//InitWindow(testName);	
	}
	virtual ~BasicTestWindow() {}

	virtual void Test() {};

	virtual void AfterCreate()
	{
		RunTest();
	}

	// default window dimensions
	const static int xpos = 100, ypos = 100, width = 400, height = 300; 

} BTW;

#define      BTW_PARAM		BTW::xpos, BTW::ypos, BTW::width, BTW::height
#define    appInstance      GetModuleHandle(NULL)
#define INIT_DLG_PARAM		NULL, appInstance, MAKEINTRESOURCE(IDD_MAINDLG)
#define BRUSH_NAME(x,y)     static HBRUSH x() { static HBRUSH brush = CreateSolidBrush(y); return brush; }


#define test_constructors_and_destructors

#ifdef test_constructors_and_destructors

	struct TestInitiationMethods : public TestTracker
	{
		TestInitiationMethods() : TestTracker(TestName()) { RunTest(); }

		void Test() 
		{
			TWindow* w = new TWindow();
			w->InitWindow(BTW_PARAM, NULL);
			WindowTest::ValidateHWND(w);
			delete w;
		}

	private:
		TEST_NAME("Initiation Methods");
	};																																							
	
	struct TestFactories : public TestTracker
	{
		TestFactories() : TestTracker(TestName()) { RunTest();}
	
		void Test() 
		{
			TWindow* w = TWindow::createWindow<TWindow>(BTW_PARAM, NULL); 
			WindowTest::ValidateHWND(w); 
			delete w;
			
			// test parent
			w = TWindow::createWindow<TWindow>(BTW_PARAM, NULL); 
			TWindow *child = TWindow::createWindow<TWindow>(5, 5, 50, 50, w);
			WindowTest::ValidateHWND(w);
			WindowTest::ValidateHWND(child);
			delete w; delete child;
		}

		TEST_NAME("Factories"); 
	}; 

	TT_FIXTURE_TEST_CASE( TestInitiationMethods ) {}

	TT_FIXTURE_TEST_CASE( TestFactories ) {}

#endif



