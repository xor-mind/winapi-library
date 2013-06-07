#include "Console.h"
#include <Windows.h>

using namespace std;

void CenterConsoleWindow()
{
	HWND console = GetConsoleWindow();
	
	RECT r; GetWindowRect(console, &r);
	int		w = r.right - r.left, h = r.bottom - r.top, 
			x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2,
			y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;
	
	MoveWindow(console, x, y, w, h, TRUE);

}

void CenteredConsoleOutput(char* s, unsigned int consoleWidthInChars)
{
	int l = strlen(s);
	int pos = static_cast<int>( (consoleWidthInChars - l) / 2);
	for(int i = 0; i < pos; i++)
		cout << " ";
	cout << s;
}

