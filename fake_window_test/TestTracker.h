#ifndef TEST_TRACKER_H
#define TEST_TRACKER_H

#include <iostream>
#include <Windows.h>
#include "Console.h"
#include "adams_macros.h"
#include "../winapi_DataConversion.h"

struct TestTracker
{
	enum Style { minimal, verbose };
	Style style;

	//TestTracker() { myTestNumber = -1; }
	TestTracker(TCHAR* const testName, Style s = defaultStyle )
		: style(s)
	{
		SetName(testName);
		myTestNumber = currentTest++;
		if (active)
		{
			if (myTestNumber == 1)
				BeginTests();
			if ( style == verbose ) {
				hr();
				say "Test: #" << myTestNumber stop
			}
			else
				tsay "\nTest: #" << myTestNumber << " " << Name() << " ";
				
		}
	}

	~TestTracker()
	{
		if (active)
		{
			if ( myTestNumber == totalTests ) {
				if ( style == verbose ) {
					hr();
				}
				else {
					endline
				}
				EndOfTests();
			}
		}
		if ( myTestNumber == totalTests )
			chill
	}

	void RunTest()
	{
		//if (myTestNumber == -1)
		//	return;
		if (active) 
		{
			if (style == verbose) 
			{
				tMinusTesting(Name(), tMinusTime()); 
				SayTesting(Name());
			}
		}
		Test();
		if (active)  {
			if (style == verbose) {
				SayPassed(Name()); endline
			}
			else {
				say "***PASSED!!*" stop
			}
		}
	}

	virtual void Test() = 0;

	void BeginTests()
	{
		if (active)
		{
			HWND hWnd = GetConsoleWindow();
			SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			CenterConsole();
			if ( style == verbose )
			{
				say "Hit enter to begin the tests." stop
				chill;
			}
		}
	}

	void EndOfTests()
	{
		if (active)
		{
			CenteredConsoleOutputEndl("*************************"); 
			CenteredConsoleOutputEndl("**success no errors**");
			CenteredConsoleOutputEndl("================"); 
			CenteredConsoleOutputEndl("|*End of tests*|"); 
			CenteredConsoleOutputEndl("================"); 
			CenteredConsoleOutputEndl("**Hit enter to end**"); 
			CenteredConsoleOutputEndl("*************************"); 
		}
	}

	void PauseBeforeTesting(TCHAR* const testName) {
		if (!active) return;
		tsay "you have requested a pause right before the start of testing: " << testName stop 
		chill
	}
	void SayTesting(TCHAR* const testName) {
		if (!active) return;
		tsay "Now Testing: " << testName; 
		Quice(say "."; Sleep(333)); endline
	}
	void SayPassed(TCHAR* const testName) {
		if (!active) return;
		tsay "** PASSED *** testing " << testName; 
		Thrice(say "."; Sleep(333)); endline
	}
	void tMinusTesting(TCHAR* const testName, UINT seconds )
	{
		if (!active) return;
		tsay _T("Begin Testing ") << testName << " in:" stop
		for ( UINT i = 0; i < seconds; ++i ) {
			say seconds - i;
			Twice(Sleep(333); say "."); Sleep(333);
		}
		endline endline;
	}

	static int totalTests;
	static int currentTest;
	static bool active;
	static void RegisterTest() { totalTests += 1; }
	static int tMinusTime() { static int s = 3; return s; }
	struct AddTest { AddTest() { TestTracker::RegisterTest(); } };
	TCHAR* Name() { return testName; }
	void SetName(TCHAR* testName) { tstrcpy(this->testName, testName); }
	static Style defaultStyle;
private:
	
	int myTestNumber;
	TCHAR testName[256];
};

int TestTracker::totalTests  = 0;
int TestTracker::currentTest = 1;
bool TestTracker::active = true;
TestTracker::Style TestTracker::defaultStyle = TestTracker::minimal;

//#define RegisterTestClass(x) TestTracker::AddTest addTest##x; \
//struct x; \
//BOOST_FIXTURE_TEST_CASE(run##x, x) {}				          \	
//struct x : public TestTracker								  \

#define TT_FIXTURE_TEST_CASE(f) TestTracker::AddTest addTest##f; \
BOOST_FIXTURE_TEST_CASE(run##f, f)							     \

#define TEST_NAME(x) static TCHAR* TestName() { static TCHAR* s = _T(x); return s; }

#endif