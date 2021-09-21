// this tests my window's interface with a fake window
#define BOOST_TEST_MODULE WindowTest
#include <boost\test\included\unit_test.hpp>
#include "window_tests.h"
#include "..\gdi.h"
#include "..\wout.h"
#include "..\winapi_log_wnd.h"
#include "..\2DMath.h"

using namespace boost::assign;

boost::function<void (Window*)>  fpForceRedraw = &Window::ForceRedraw;

#define prep say			     "\n\n\t" <<
#define CreateTest            KWindow w; w.Init();
#define CreateRunTest         KWindow w; w.Init(); Window::RunGetMessageLoop();
#define CreateRunDestroyTest  KWindow w; w.Init(); w.Expunge(); Window::RunGetMessageLoop();
#define useCwDefaults		  CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT

#define test_constructors_and_destructors
//#define test_position_and_dimension
//#define test_messaging
//#define test_padding
//#define test_useful_methods
//#define test_window_distance_methods
//#define test_double_buffer
//#define test_log_window
//#define test_gdi_draw

#ifdef test_constructors_and_destructors

namespace InitiationMethods
{
	void TestChildWithinChild()
	{
	/*	BrushFactory bf;
		WindowPtr parent( CreateWnd(BTW_PARAM,NULL) ),
			      child ( CreateWnd(0, 0, 200, 200, _T("child"), bf.blue(), parent.get()) ),
			      child1( CreateWnd(0,0, 100, 100, _T("child1"), bf.red(), child.get()) );
			
		WindowList wl; wl += parent.get(),child.get(),child1.get();
			
		std::for_each(wl.begin(), wl.end(), fpForceRedraw);
		std::for_each(wl.begin(), wl.end(), ValidateHWND);
		parent->Expunge();
		Window::RunGetMessageLoop();*/
	}

	BOOST_AUTO_TEST_CASE(foobar)
	{
		Window* w = new Window();  

		BOOST_REQUIRE(w);

		w->InitWnd(BTW_PARAM, NULL);
		ValidateHWND(w);
		w->Expunge();
		Window::RunGetMessageLoop();
		/*
		w->InitWnd(BTW_PARAM, TestName(), NULL);
		ValidateHWND(w);
		w->Expunge();
		Window::RunGetMessageLoop();

		w->InitWnd(NULL, 0, 0, 0, TestName(), CS_OWNDC | CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW, 
						WS_OVERLAPPEDWINDOW | WS_VISIBLE, BTW_PARAM);
		ValidateHWND(w);
		w->Expunge();
		Window::RunGetMessageLoop();

		w->InitWnd(INIT_DLG_PARAM);
		ValidateHWND(w);
		w->Expunge();
		Window::RunGetMessageLoop();*/

		delete w;

		//TestChildWithinChild();
	}	
}

//namespace Factories
//{
//	CreateWinapiWindowUnitTest(Factories, "Factories") 
//	{
//		Window* w = WindowFactory<>::create(BTW_PARAM, NULL); 
//		ValidateHWND(w); 
//		w->Expunge();
//		Window::RunGetMessageLoop();
//		delete w;
//
//		w = WindowFactory<>::create(BTW_PARAM, CS_OWNDC | CS_DBLCLKS 
//			| CS_VREDRAW | CS_HREDRAW, WS_OVERLAPPEDWINDOW | WS_VISIBLE, NULL); 
//		ValidateHWND(w); 
//		w->Expunge();
//		Window::RunGetMessageLoop();
//		delete w;
//
//		TCHAR* windowName = _T("constructorTest");
//		HBRUSH b = CreateSolidBrush(RGB(255,0,0));
//		w = WindowFactory<>::create(BTW_PARAM, windowName, b, NULL); 
//		ValidateHWND(w); 
//		w->Expunge();
//		Window::RunGetMessageLoop();
//		delete w;
//
//		TCHAR* className = _T("constructorTestClass");
//		b = CreateSolidBrush(RGB(255,0,0));
//		w = WindowFactory<>::create(BTW_PARAM, className, windowName, CS_OWNDC | CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW, WS_OVERLAPPEDWINDOW | WS_VISIBLE, b, NULL); 
//		BOOST_REQUIRE(w); 
//		w->Expunge();
//		Window::RunGetMessageLoop();
//		delete w;
//		
//		HBRUSH redBrush = CreateSolidBrush(RGB(255,0,0));
//		w = WindowFactory<>::create(BTW_PARAM, NULL); 
//		Window *child = WindowFactory<>::create(5, 5, 50, 50, _T("child"), redBrush, w);
//		child->ForceRedraw();
//		ValidateHWND(child); 
//		w->Expunge();
//		Window::RunGetMessageLoop();
//		delete w;
//		delete child;
//		
//		w = WindowFactory<>::create(INIT_DLG_PARAM);
//		ValidateHWND(w);
//		w->Expunge();
//		Window::RunGetMessageLoop();
//		delete w;
//	}
//}

#endif

#ifdef test_position_and_dimension
	
namespace ClientScreenConversion
{
	CreateWinapiWindowUnitTest(ClientScreenConversion, "Client To Screen")
	{
		Window* w = CreateWnd();
 
		POINT p = {0, 0}, p1 = {0, 0};
		w->ClientToScreen(w->ScreenToClient(p)); 
		BOOST_REQUIRE_EQUAL(p, p1); 
		w->ScreenToClient(w->ClientToScreen(p));
		BOOST_REQUIRE_EQUAL(p, p1);
		w->Expunge();
		Window::RunGetMessageLoop();
		delete w;
	}
}

namespace WindowDimensions
{
	void TestScreenRect()
	{
		WindowPtr w( CreateWnd(BTW_PARAM, NULL)->ProcessPostQuitMessage(false) );
		RECT r = { BTW::xpos, BTW::ypos, BTW::xpos + BTW::width, BTW::ypos + BTW::height };
		BOOST_REQUIRE_EQUAL(r, w.get()->ScreenRect());	
		w->Expunge();
	}

	void TestSetPosWith4args() 
	{
		#define R1 0, 0, 50, 50
		#define R2 10, 100, 200, 400
		#define R3 10, 100, 210, 500
		RECT r1 = { R1 }, r2 = { R2 }, r3 = { R3 };

		WindowPtr w( CreateWnd(R1, NULL)->ProcessPostQuitMessage(false) );
		BOOST_REQUIRE_EQUAL(r1, w->ScreenRect());
		w->SetNonClientPos(R2); // **MVL** 
		BOOST_REQUIRE_EQUAL(r3, w->ScreenRect());
		w->Expunge();
	}

	CreateWinapiWindowUnitTest(WindowDimensions, "Window Dimensions")
	{ 
		TestScreenRect();
		TestSetPosWith4args();
	}
	
}

namespace WindowDimensionsWRtoParent
{
	CreateWinapiWindowUnitTest(WindowDimensionsWRtoParent, "Window Dimensions WRT Parent")
	{
		RECT childRect = {0, 0, 50, 50}, 
			child1Rect = {50, 100, 100, 200};

		boost::scoped_ptr<Window> p, child, child1;

		p.reset( CreateWnd(BTW_PARAM, NULL) );

		child.reset( CreateWnd(childRect.left, childRect.top, childRect.right, 
					childRect.bottom, _T("childClass"), _T(""), 0, 0, bf.blue(), p.get() ) );
		child->ForceRedraw();

		child1.reset( CreateWnd(child1Rect.left, child1Rect.top, child1Rect.right,
						child1Rect.bottom, _T("child1Class"), _T(""), 0, 0, bf.red(), p.get() ) );
		child1->ForceRedraw();

		// test child

		RECT r = { BTW::xpos + childRect.left, BTW::ypos + childRect.top, 
					r.left + childRect.right, r.top + childRect.bottom };
		OffsetRect(&r, p->dxNonClientAndClient(), p->dyNonClientAndClient());
		BOOST_REQUIRE_EQUAL(r, child->ScreenRect());
		// test child 1
		RECT r1 = { BTW::xpos + child1Rect.left, BTW::ypos + child1Rect.top, 
					r1.left + child1Rect.right, r1.top + child1Rect.bottom };
		OffsetRect(&r1, p->dxNonClientAndClient(), p->dyNonClientAndClient());
		BOOST_REQUIRE_EQUAL(r1, child1->ScreenRect());
		p->Expunge();
		Window::RunGetMessageLoop();
	}
}
	
namespace WindowPositioning
{
	void PositionTestNonClient(Window* w, int x, int y)
	{
		BOOST_REQUIRE_EQUAL(w->xPos(), x);
		BOOST_REQUIRE_EQUAL(w->yPos(), y);
	}
	// automatically adjusts x and y to take in account of the delta's between the non client and client
	void PositionTestClient(Window* w, int x, int y)
	{
		x += w->dxNonClientAndClient(); 
		y += w->dyNonClientAndClient();
		BOOST_REQUIRE_EQUAL(w->xPosClient(), x); 
		BOOST_REQUIRE_EQUAL(w->yPosClient(), y);
	}
	int GetClientWidth(Window* w) 
	{
		RECT r;
		::GetClientRect(w->Wnd(), (LPRECT)&r);
		return r.right;
	}
	int GetClientHeight(Window* w) 
	{
		RECT r;
		::GetClientRect(w->Wnd(), (LPRECT)&r);
		return r.bottom;
	}

	CreateWinapiWindowUnitTest(WindowPositioning, "Window Positioning")
	{
		// test moving window around the screen
		boost::scoped_ptr<Window> parent( CreateWnd(BTW_PARAM, NULL) );
		BOOST_REQUIRE_EQUAL(BTW::width, parent->Width());
		BOOST_REQUIRE_EQUAL(BTW::width, parent->Width());
		BOOST_REQUIRE_EQUAL(BTW::xpos + BTW::width, parent->x1Pos());
		BOOST_REQUIRE_EQUAL(BTW::ypos + BTW::height, parent->y1Pos());
		BOOST_REQUIRE_EQUAL(GetClientWidth(parent.get()), parent->ClientWidth());
		BOOST_REQUIRE_EQUAL(GetClientHeight(parent.get()), parent->ClientHeight());
		parent->SetNonClientPos(5, 5);
		PositionTestNonClient(parent.get(), 5, 5);
		PositionTestClient(parent.get(), 5, 5 );

		// test moving child windows around a window
		HBRUSH redBrush = CreateSolidBrush(RGB(255,0,0));
		boost::scoped_ptr<Window> child ( CreateWnd(5, 5, 50, 50, _T("Child"), redBrush, parent.get()) );
		child->ForceRedraw();
		child->SetNonClientPos(20, 20);
		child->ForceRedraw();
		PositionTestClient(child.get(), 20, 20);
		parent->Expunge();

		// test moving dialog
		parent.reset( CreateWnd(INIT_DLG_PARAM));
		parent->SetNonClientPos(100, 100);
		PositionTestNonClient(parent.get(), 100, 100);
		
		// test moving control within dialog
		child.reset(new Window());
		child->SetWndAndParent(GetDlgItem(parent->Wnd(), IDOK), parent.get());
		ValidateHWND(child.get());
		child->SetNonClientPos(3, 3);
		PositionTestNonClient(child.get(), 3, 3);
		parent->Expunge();
		Window::RunGetMessageLoop();
	}
}

namespace CenterWindow
{
	void Test3WindowsInAScreen()
	{
		WindowPtr top(CreateWnd(0, 0, 50, 50, _T("topClass"), NULL)->ProcessPostQuitMessage(false) ), 
				bottom(CreateWnd(0, 200, 50, 50, _T("bottomClass"), NULL)->ProcessPostQuitMessage(false) ), 
				center(CreateWnd(0, 0, 50, 50, _T("centerClass"), NULL)->ProcessPostQuitMessage(false) ); 

		center->CenterVertically(top.get(), bottom.get());
		RECT r = { 0, 100, 50, 150 };
		BOOST_REQUIRE_EQUAL(r, center->ScreenRect());

		top->Expunge(); bottom->Expunge(); center->Expunge();
	}

	void Test3Children()
	{
		BrushFactory bf;
		WindowPtr parent( CreateWnd(500, 500, 500, 500, _T("parentClass"), NULL) ),
						top( CreateWnd(0, 0, 50, 50, _T("topClass"), bf.red(), parent.get()) ),
					bottom( CreateWnd(0, 200, 50, 50, _T("bottomClass"), bf.purple(), parent.get()) ),
					center( CreateWnd(0, 0, 50, 50, _T("centerClass"), bf.blue(), parent.get()) );
		bottom->ForceRedraw(); top->ForceRedraw(); center->ForceRedraw();
		center->CenterVertically(top.get(), bottom.get());
		top->ForceRedraw(); center->ForceRedraw();
		RECT r1 = { 500, 600, 550, 650 };
		OffsetRect(&r1, parent->dxNonClientAndClient(), parent->dyNonClientAndClient());
		BOOST_REQUIRE_EQUAL(r1, center->ScreenRect());
		parent->Expunge();
		Window::RunGetMessageLoop();
	}

	void TestDialog()
	{
		WindowPtr parent(CreateWnd(INIT_DLG_PARAM) ); 
		WindowPtr top(new Window()), bottom(new Window()), center(new Window());

		top->SetWndAndParent(GetDlgItem(parent->Wnd(), IDOK), parent.get());
		bottom->SetWndAndParent(GetDlgItem(parent->Wnd(), IDCLOSE), parent.get());
		center->SetWndAndParent(GetDlgItem(parent->Wnd(), IDC_EDIT), parent.get());
				
		center->CenterVertically(top.get(), bottom.get());
				
		BOOST_REQUIRE_EQUAL(dyTwoWindows(top.get(), center.get()), 
							dyTwoWindows(center.get(), bottom.get()));
		parent->Expunge();
		Window::RunGetMessageLoop();
	}

	void Test2ChildrenAndAChildsChild()
	{
		BrushFactory bf;
		// creating a parent window with 4 child windows. going to center childchild between child1 and child2. 
		WindowPtr parent( CreateWnd(BTW_PARAM, _T("parent"), bf.blue(), NULL) ),
					child ( CreateWnd( 0, 0, BTW::width/2, BTW::height, _T("child"), bf.orange(), parent.get() ) ),
					childchild( CreateWnd( 0, 0, 50, 50, _T("childchild"), bf.pink(), child.get() ) ),
					child1( CreateWnd( BTW::width/2, 0, 50, 50, _T("child1"), bf.yellow(), parent.get() ) ),
					child2( CreateWnd( BTW::width/2, parent->ClientHeight()-50, 50, 50, _T("child2"), bf.red(), parent.get() ) );
			
		WindowList wl; wl += parent.get(), child.get(), childchild.get(), child1.get(), child2.get();
			
		std::for_each(wl.begin(), wl.end(), fpForceRedraw); // force redraw since we don't have a message pump in this specific test.
				
		childchild->CenterVertically(child1.get(), child2.get());

		parent->Expunge();
		Window::RunGetMessageLoop();	
	}

	CreateWinapiWindowUnitTest(CenterWindow,"Centering Window") 
	{
		Test3WindowsInAScreen();
		Test3Children();
		Test2ChildrenAndAChildsChild();
		TestDialog();
	}	
}

CreateWinapiWindowUnitTest(NonClientScreenRect, "ScreenRect WRT Parent")
{
	RECT pRect  = { 100, 100, 500, 500 },
		 cRect = { 50, 50, 400, 400 };

	Window * p = CreateWnd(pRect, _T("parent"), NULL, NULL);
	{
		Assert(p->ScreenRect(), p->NonClientRect());
	}

	Window * c = CreateWnd( cRect, _T("child"), NULL, p )->SetClassBrush( bf.purple() )->ForceRedraw();
	{
		cRect.right += cRect.left; cRect.bottom += cRect.top;
		Assert( cRect , c->NonClientRect() );
	}

	p->Expunge();
	Window::RunGetMessageLoop();
	delete p; delete c; 
}

CreateWinapiWindowUnitTest( GetClientRect, "GetClientRect" )
{
	WindowPtr w( ( new Window())->SetPadding( 100, 100, 100, 100 )->InitAdjustedWnd(0, 0, 600, 600, NULL) );
	RECT r = w->GetClientRect(), r1 = { 100, 100, 600 - 100*2, 600 - 100*2 };
	BOOST_REQUIRE_EQUAL( r, r1 );
	w->Expunge();
	w->RunGetMessageLoop();
}

#endif

#ifdef test_padding

namespace PaddingTest
{
	void TestParentAndChild()
	{
		BrushFactory bf;
		WindowPtr w ( CreateWnd(BTW_PARAM, Padding(0.1f, 0.15f, 0.1f, 0.15f), NULL )  );
			
		int x  = w->ClientWidth(),
			x1 = BTW::width - w->dxNonClientAndClient()*2;
			
		isEqual(x, x1);
		w->Expunge();
	}
		
	void TestParentAnd3Child()
	{
		BrushFactory bf;
		WindowPtr p ( CreateWnd(0, 0, 640, 480, Padding(1/3.f, 0.10f, 1/3.f, 0.10f), NULL)->SetClassBrush(bf.pink()) ); 
			
		// setup children
			
		int h = (int)( (1/3.f) * p->ClientHeight() ),
			w = p->ClientWidth(),
			remainder = (int)ceil( p->ClientHeight()/(float)h - 3.f);
	
		WindowPtr r ( CreateWnd(*p->pad->left, *p->pad->top, w, h, _T("redClass"), bf.red(), p.get()) ),
					y ( CreateWnd(*p->pad->left, (*p->pad->top)() + h , w, h, _T("yellowClass"), bf.yellow(), p.get()) ),
					o ( CreateWnd(*p->pad->left, (*p->pad->top)() + 2*h, w, h + remainder, _T("orangeClass"), bf.orange(), p.get()) );
		//	
		//WindowList wl; 
		//wl += r.get(), y.get(), o.get(), p.get();
		//	
		////std::for_each( wl.begin(), wl.end(), TestChild::fpSetMargin); 
		//std::for_each(wl.begin(), wl.end(), fpForceRedraw); 		
		//	
		//// test r, y, & o screen rects. 
		//RECT  rc = {  leftDiv->xPosClientInScreen(),  leftDiv->yPosClientInScreen(),  
		//		                            rc.left + w, rc.top + h };
		//isEqual(rc, r->ScreenRect());
		//rc.top += h;  rc.bottom += h;
		//isEqual(rc, y->ScreenRect());
		//rc.top += h;  rc.bottom += h;
		//isEqual(rc, o->ScreenRect());

		//RECT rc1 = { rightDiv->xPosClientInScreen(), rightDiv->yPosClientInScreen(),
		//				rc1.left + w, rc1.top + rightDiv->ClientHeight()};
		//isEqual(rc1, p->ScreenRect()); 
		p->Expunge();
		Window::RunGetMessageLoop();
	}

	CreateWinapiWindowUnitTest(PaddingTest, "Padding") 
	{
		TestParentAndChild();
		TestParentAnd3Child(); 
	}
}

#endif

#ifdef test_useful_methods

namespace PushButtons
{
	bool didIgetPushed = false;

	class MyDlg : public Window 
	{
		DLG_PROC_DECL()
		{
			if ( msg == WM_COMMAND ) 
			{
				if ( LOWORD(wParam) == IDOK )
				{
					didIgetPushed = true;
				}
			}
			return FALSE;
		}
	};

	CreateWinapiWindowUnitTest(PushButtons, "Push Button")
	{
		WindowPtr main( WindowFactory<MyDlg>::create(IDD_MAINDLG) );
		WindowPtr   ok( new Window() ); ok->SetWndAndParent(GetDlgItem(main->Wnd(), IDOK), main.get());// AD_TODO add constructor that works with control ids...
		
		ok->PushButton();
		BOOST_REQUIRE(didIgetPushed);
		main->Expunge();
	}
}

//namespace ChangePenWidth
//{
//	CreateWinapiWindowUnitTest(ChangePenWidth, "Change Pen Width")
//	{
//		WindowPtr p( CreateWnd() );
//		Window::DC dc(p.get());
//			
//		SelectGdiObj sgo(dc); sgo.Pen(pf.orange());
//			
//		HPEN pen = (HPEN) GetCurrentObject(dc, OBJ_PEN);
//			
//		LOGPEN logPen; RtlZeroMemory(&logPen, sizeof(LOGPEN));
//			
//		if (!GetObject(pen, sizeof(LOGPEN), &logPen ))
//		{
//			PrintLastWinAPIError(_T("GetObject"));
//		}
//
//		Window::RunGetMessageLoop();
//	}
//}

namespace ChangeWndClass
{
	void TestBrushChange()
	{
		prep "Brush Change";

		BrushFactory bf;
		WindowPtr parent ( CreateWnd() ),
				   child ( CreateWnd( parent.get() ) );
		child->SetClassBrush(bf.purple()); 
		parent->Expunge();
		Window::RunGetMessageLoop();
	}

	void TestCursorChange()
	{
		prep "Cursor Change\n" stop
		BrushFactory bf;
		WindowPtr parent ( CreateWnd() ),
				   child ( CreateWnd( parent.get() ) );

		child->SetClassBrush( bf.purple() )->SetClassCursor( IDC_SIZEWE ); 
		parent->Expunge();
		Window::RunGetMessageLoop();
	}

	CreateWinapiWindowUnitTest(ChangeWndClass, "Changing Wnd Class")
	{
		TestBrushChange();
		TestCursorChange();
	}
}


		
//class DlgText : TT
//{
//	TEST_NAME("Dlg Set/Get Text");
//public:
//	DlgText() : TT(TestName()) { RunTest(); }
//
//	void Test() 
//	{
//		WindowPtr main  ( CreateWnd(IDD_MAINDLG) );
//		WindowPtr child ( new Window() );
//		child->SetWndAndParent( GetDlgItem( main->Wnd(), IDCLOSE ) , main.get() );
//
//		tstring s = child->GetText();
//
//		Assert(s.c_str(), tstring(_T("Close")).c_str());
//		Assert(s.size(), tstring(_T("Close")).size());
//		Assert(s.length(), tstring(_T("Close")).length());
//				
//
//		Window::RunGetMessageLoop();
//	}
//};
//CreateFixture(DlgText) { }



CreateWinapiWindowUnitTest(SetClassNameBeforeWndInit, "SetClassName before WndInit")
{
	WindowPtr p( ( new Window() )->SetClassName( _T("Parent") )->InitWnd( 0, 0, 500, 500, NULL ) );
	{
		BOOST_REQUIRE( p->Wnd() );
	}

	WindowPtr c( ( new Window() )->SetClassName( _T("Chitlin") )->InitWnd( 0, 0, 100, 100, &*p )->SetClassBrush( bf.purple() )->ForceRedraw() );
	{
		BOOST_REQUIRE( c->Wnd() );
	}

	p->Expunge();
	Window::RunGetMessageLoop();
}

#endif

#ifdef test_window_distance_methods

namespace WindowDistance
{
	CreateWinapiWindowUnitTest(WindowDistanceTest, "Window Distance")
	{
		WindowPtr p ( CreateWnd() ), c ( CreateWnd( p.get( ) ) );
		 
		int dx = p->dxLeftAndLeft(c.get()), dx1 = p->dxRightAndRight(c.get()),
			dy = p->dyTopAndTop(c.get()),   dy1 = p->dyBotAndBot(c.get());

		AssertErr(dx, dx1, 1);
		AssertErr(dy, dy1, 1);
		p->Expunge();
		Window::RunGetMessageLoop();
	}
}
  
#endif

#ifdef test_double_buffer

namespace DoubleBuffer
{
	template <class T> Window * createChild( const TCHAR* id, Window* parent ) 
	{ 
		tstring  className = tstring(id) + _T("Class"),
			    windowName = tstring(id) + _T("Window");
		Window * w = (new T())->InitWnd(0, 0, parent->ClientWidth()/2, parent->ClientWidth()/2, (TCHAR *const)className.c_str(), (TCHAR *const)windowName.c_str(), parent); 
		return w;
	}

	class DblBuffedWnd : public WindowGDI
	{
		class C1 : public WindowGDI
		{
			class C2 : public WindowGDI
			{
				class TopHalf: public WindowGDI
				{
					class Output : public WindowGDI
					{
						Wout wout;
					public:

						~Output() {}

						HFONT MaxFontToWindowSize(HFONT oldFont, HDC dc)
						{		
							//HDC          hdc;
							TEXTMETRIC   tm;
							LOGFONT      LogFont;

							//hdc = GetDC(Wnd());

							// Set up the LogFont structure.
		
							// Make sure it fits in the grid.
							LogFont.lfHeight = 24; // Allow for whitespace.
		
							LogFont.lfWidth = LogFont.lfEscapement = LogFont.lfOrientation =
								LogFont.lfWeight = 0; // Set these guys to zero.
		
							LogFont.lfItalic = LogFont.lfUnderline = LogFont.lfStrikeOut = 
								LogFont.lfOutPrecision = LogFont.lfClipPrecision =
								LogFont.lfQuality = LogFont.lfPitchAndFamily = 0; // Set these guys to zero.

							// Let the facename and size define the font.
							LogFont.lfCharSet = ANSI_CHARSET;
							lstrcpy(LogFont.lfFaceName, L"Lucida Sans Unicode");
		
							// Create the font.
							//if ( symbolFont )
							//	DeleteObject(symbolFont);
							HFONT symbolFont = CreateFontIndirect(&LogFont);
							//HFONT hOldFont = (HFONT)SelectObject(dc, symbolFont);
							// Set the font back to edit control
							//::SendMessage(w, WM_SETFONT,(WPARAM)font_new,1);
							return symbolFont;
						}

						void SetupDoubleBuffer(GDI * primaryGDI)
						{
							WindowGDI::SetupDoubleBuffer( primaryGDI );
							wout.SetHandler(Wnd(), BackDC(), DoubleBufferedRect().left, DoubleBufferedRect().top );
						}

						void UpdateFrame()
						{
							BrushFactory bf;
							ClearScreen( bf.lightGrey() );
							HFONT newFont = MaxFontToWindowSize((HFONT)GetCurrentObject(BackDC(), OBJ_FONT), BackDC() );
							HFONT oldFont = (HFONT)SelectObject(BackDC(), newFont);
							wout.SetCursorX(0); wout.SetCursorY(0);
							wout << _T("Hello⇒⇒World");
							//TextOut(BackDC(), 0, 0, L"Hello⇒⇒World", wcslen(L"Hello⇒⇒World"));
							SelectObject(BackDC(), oldFont);
							DeleteObject(newFont);
						}
					};
					WindowPtr output;

				public:
					~TopHalf() {}
					WPD()
					{
						onCreate
						return DWP;
					}

					handleCreate()
					{
						output.reset( (new Output() )->InitWnd(0, 0, ClientWidth(), ClientHeight(), _T("OutputClass"), _T("OutputWindow"), this )->SetClassBrush(NBRUSH) );
					}

					void SetupDoubleBuffer(GDI * primaryGDI)
					{
						((Output*)&*output)->SetupDoubleBuffer( primaryGDI );
					}
					void UpdateFrame()
					{
						((Output*)&*output)->UpdateFrame();
					}	

				};
				class BotHalf : public WindowGDI
				{
				public:
					~BotHalf() {}
					WPD()
					{
						return DWP;
					}
				};
				WindowPtr topHalf;
				WindowPtr botHalf;

			public:
				~C2() {}
				WPD()
				{
					onCreate
					return DWP;
				}
				handleCreate()
				{
					topHalf.reset( ( new TopHalf() )->InitWnd(10, 0, ClientWidth() - 10, ClientHeight()/2, _T("TopHalfClass"), _T("TopHalfWindow"), parent)->SetClassBrush(NBRUSH) );
					botHalf.reset( ( new BotHalf() )->InitWnd(0, ClientHeight()/2, ClientWidth(), ClientHeight()/2, _T("BopHalfClass"), _T("BotHalfWindow"), parent)->SetClassBrush(NBRUSH) );
				}
				void UpdateFrame()
				{
					BrushFactory bf;
					ClearScreen(bf.red());
					((TopHalf*)&*topHalf)->UpdateFrame();
				}
				void SetupDoubleBuffer(GDI * primaryGDI)
				{
					WindowGDI::SetupDoubleBuffer( primaryGDI );
					if ( topHalf ) 
						((TopHalf*)&*topHalf)->SetupDoubleBuffer( PrimaryGDI() );
				}
			};

			WindowPtr c2;
		public:
			~C1() {}
			WPD()
			{
				onCreate
				return DWP;
			}
			handleCreate()
			{
				c2.reset( createChild<C2>(_T("C2"), this) );
			}
			void SetupDoubleBuffer(GDI * primaryGDI)
			{
				WindowGDI::SetupDoubleBuffer( primaryGDI );
				if ( c2 )
					((C2*)&*c2)->SetupDoubleBuffer( PrimaryGDI() );
			}
			void UpdateFrame()
			{
				BrushFactory bf;
				ClearScreen(bf.blue());
				((C2*)&*c2)->UpdateFrame();
			}
		};
		WindowPtr c1;

	public:	
		WPD()
		{
			onCreate
			//onPaint
			onSize
			onWndDestruction
			return DWP;
		}

		handleCreate()
		{
			Window::HandleSize(TrueClientWidth(), TrueClientHeight());	
			c1.reset( createChild<C1>(_T("C1"), this) );	
			SetupDoubleBuffer();
		}

		handleSize()
		{
			Window::HandleSize(w, h);
			//if ( c1 ) {
			//	c1->SetNonClientPos( 0, 0, ClientWidth()/2, ClientHeight()/2 ); 
			//}
			SetupDoubleBuffer();
		}

		void UpdateFrame()
		{
			BrushFactory bf;
			ClearScreen(bf.green());
			((C1*)&*c1)->UpdateFrame();
			GDI::UpdateFrame();
		}

		void SetupDoubleBuffer()
		{
			WindowGDI::SetupDoubleBuffer( (Window*)this );
			if ( c1 )
				((C1*)&*c1)->SetupDoubleBuffer( (GDI*)this );
		}

		void PeekMessageLoop()
		{
			static DWORD start_time = GetTickCount();

			if ( GetTickCount() - start_time >= 16 )
			{	
				UpdateFrame();
				start_time = GetTickCount();
			}
		}	
	};

	CreateUnitTest( DoubleBufferTest, "Double Buffer" )
	{
		WindowPtr parent( WindowFactory<DblBuffedWnd>::create() );
		//parent->Expunge();
		parent->RunPeekMessageLoop();
	}
}

#endif

#ifdef test_log_window

namespace TestingLogWindow
{
	CreateUnitTest( LogWindowTest, "Log Window" )
	{
		WindowPtr parent( WindowFactory<LogWnd>::create(0, 0, 240, 600, NULL) );
		((LogWnd*)&*parent)->Log(_T("Hello World!\r\nthe answer is %d"), 42);
		parent->Expunge();
		parent->RunPeekMessageLoop();
	}
}
#endif

#ifdef test_log_window

namespace TestingLogWindow
{
	CreateUnitTest( LogWindowTest, "Log Window" )
	{
		WindowPtr parent( WindowFactory<LogWnd>::create(0, 0, 240, 600, NULL) );
		((LogWnd*)&*parent)->Log(_T("Hello World!\r\nthe answer is %d"), 42);
		parent->Expunge();
		parent->RunPeekMessageLoop();
	}
}
#endif

#ifdef test_log_window

namespace TestingLogWindow
{
	CreateWinapiWindowUnitTest( LogWindowTest, "Log Window" )
	{
		WindowPtr parent( WindowFactory<LogWnd>::create(0, 0, 240, 600, NULL) );
		((LogWnd*)&*parent)->Log(_T("Hello World!\r\nthe answer is %d"), 42);
		parent->Expunge();
		parent->RunPeekMessageLoop();
	}
}
#endif

#ifdef test_gdi_draw

namespace TestingGdiDraw 
{
	HPEN thickPen;
	PenFactory pf;

	class KWindow : WindowGDI
	{
		WPD()
		{
			onPaint
			onWndDestruction
			return DWP; 
		}
		handlePaint()
		{
			Vector p(100, 100), d(200, 200);
			BackDC( dc );
			DrawRect( p, d, thickPen, BrushFactory::Yellow() );
			p += Vector( 250.f, 250.f );
			DrawRoundRect( p, d, (int)(d.x/8), (int)(d.y/8), thickPen, BrushFactory::Red() );
		}
	public:
		KWindow * Init() 
		{ 
			InitWnd( useCwDefaults, 0 ); 
			thickPen = pf.CreateThickPen( RGB(255, 255, 255), 4 );
			return this; 
		}
		using WindowGDI::Expunge;
	};

	CreateWinapiWindowUnitTest( GdiDrawTest, "Gdi Drawing" )
	{
		CreateRunTest
	}
}
#endif



