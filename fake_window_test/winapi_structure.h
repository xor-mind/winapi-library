#ifndef WINAPI_STRUCTURE_H
#define WINAPI_STRUCTURE_H

#include <windows.h>

inline void SetPoint(POINT& lpPoint, int a, int b) { lpPoint.x = a; lpPoint.y = b; }

#endif