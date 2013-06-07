#include "winapi_library.h"
#include <windows.h>
 
WINAPI_MAIN
{
    Window w(0, 0, 400, 300);
    return Window::RunGetMessageLoop();
}
