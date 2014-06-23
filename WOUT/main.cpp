#define BOOST_TEST_MODULE WOUT_Test

#include <boost\test\included\unit_test.hpp>
#include <TestTracker.h>

#include <winapi\Prerequisites.h>
#include <winapi\window.h>
#include <winapi\wout.h>

//#define BASIC_WINDOW_TEST
#define CHANGE_TEXT_SIZE_TEST

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
		wout.DC( dc );
		wout.SetCursor( 10, 10 );
		//wout.SetTextHeight( 16 );
		wout << L"Hello World" << Wout::endl;
		wout << L"Testing 1, 2, 3" << Wout::endl;
		wout << L"Current text size: " << std::to_wstring( wout.GetTextHeight() ) << Wout::endl;
		wout << L"Changing text size to 24..." <<  Wout::endl;
		wout << Wout::endl;

		wout.SetTextHeight( 24 );
	
		wout << L"Current text size: " << std::to_wstring( wout.GetTextHeight() ) << Wout::endl;

		wout.RestoreOldFont();
	}
private:
	Wout wout;
};

CreateUnitTest(WOUT, "WOUT text size test")
{
	WindowPtr w( WindowFactory<TextWindow>::create( 0, 0, 400, 400, nullptr )->CenterWrtParent() );
	Window::RunGetMessageLoop();
}
}
#endif
