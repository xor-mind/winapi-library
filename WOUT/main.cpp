#define BOOST_TEST_MODULE WOUT_Test

#include <boost\test\included\unit_test.hpp>
#include <gdiplus.h>
#include <TestTracker.h>

#include <winapi\Prerequisites.h>
#include <winapi\window.h>
#include <winapi\wout.h>

#define BASIC_WINDOW_TEST
//#define CHANGE_TEXT_SIZE_TEST
//#define UTILITY_METHODS

#ifdef BASIC_WINDOW_TEST
namespace BasicWindowTest
{

class TextWindow : public Window
{
public:
	WPD()
	{
		onCreate
		onPaint
		onWndDestruction
		return DEF_WND_PROC; 
	}
	handleCreate()
	{
		wout.Wnd( Wnd() );
		wout.SetCursorStartingPosX( 10 );
		wout.SetTextColor( RGB( 0, 0, 255 ) );
	}
	handlePaint()
	{
		wout.DC( dc );
		wout.SetCursor( 10, 10 );
		wout << L"Hello World" << Wout::endl;
		wout << L"Testing 1, 2, 3" << Wout::endl;
	}
private:
	Wout wout;
};

CreateUnitTest(WOUT, "WOUT test")
{
	WindowPtr w( WindowFactory<TextWindow>::create( 0, 0, 400, 400, nullptr )->CenterWrtParent() );
	Window::RunGetMessageLoop();
}

}

#endif

#ifdef CHANGE_TEXT_SIZE_TEST

namespace ChangeTextSizeTest
{

class TextWindow : public Window
{
public:
	WPD()
	{
		onCreate
		onPaint
		onWndDestruction
		return DEF_WND_PROC; 
	}
	handleCreate()
	{
		wout.Wnd( Wnd() );
		wout.SetCursorStartingPosX( 10 );
		wout.SetTextColor( RGB( 0, 0, 255 ) );
	}
	handlePaint()
	{
		{
			using namespace Gdiplus;

			Graphics graphics( dc );
		
			// Pen can also be constructed using a brush or another pen.  There is a second parameter - a width which defaults to 1.0f
			Pen	blue (Color(255, 0, 0, 255));
			Pen red  (Color(255, 255, 0, 0));
			Sleep(2000);
			int max = 256;
			int y = max;
			for (int x = 0; x < max; x += 5)
			{
				//graphics.DrawLine(&blue, 0, y, x, 0);
				graphics.DrawLine(&red, max, x, y, max);  
				y -= 5;
				Sleep(100);
			}		

			//for (int y = 0; y < 256; y++)
			//{
			//	Pen pen(Color(y, 0, 255,0));  // A green pen with shifting alpha
			//	graphics.DrawLine(&pen, 0, y, 256, y);
			//	// The sleep is to slow it down so you can watch the effect
			//	Sleep(20);
			//}

			//for (int x = 0; x < 256; x++)
			//{
			//	Pen pen(Color(x, 255, 0, 255));  // A green pen with shifting alpha
			//	graphics.DrawLine(&pen, x, 100, x, 200);
			//	// The sleep is to slow it down so you can watch the effect
			//	Sleep(20);
			//}
		}
		//wout.DC( dc );
		//wout.SetCursor( 10, 10 );
		////wout.SetTextHeight( 16 );
		//wout << L"Hello World" << Wout::endl;
		//wout << L"Testing 1, 2, 3" << Wout::endl;
		//wout << L"Current text size: " << std::to_wstring( wout.GetTextHeight() ) << Wout::endl;
		//wout << L"Changing text size to 24..." <<  Wout::endl;
		//wout << Wout::endl;

		//wout.SetTextHeight( 24 );
	
		//wout << L"Current text size: " << std::to_wstring( wout.GetTextHeight() ) << Wout::endl;

		//wout.RestoreOldFont();
	}
private:
	Wout wout;
};

CreateUnitTest(WOUT, "WOUT text size test")
{
	ULONG_PTR m_gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	WindowPtr w( WindowFactory<TextWindow>::create( 0, 0, 400, 400, nullptr )->CenterWrtParent() );
	
	
	Window::RunGetMessageLoop();
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
}
}
#endif

#ifdef UTILITY_METHODS

namespace UtilityMethods
{

class TextWindow : public Window
{
public:
	WPD()
	{
		onCreate
		onPaint
		onWndDestruction
		return DEF_WND_PROC; 
	}
	handleCreate()
	{
		wout.Wnd( Wnd() );
		wout.SetCursorStartingPosX( 10 );
		wout.SetTextColor( RGB( 0, 0, 255 ) );
	}
	handlePaint()
	{
		wout.DC( dc );
		wout << wout.GetFontName() << Wout::endl;
	}
private:
	Wout wout;
};

CreateUnitTest(WOUT, "WOUT test")
{
	WindowPtr w( WindowFactory<TextWindow>::create( 0, 0, 400, 400, nullptr )->CenterWrtParent() );
	Window::RunGetMessageLoop();
}

}

#endif