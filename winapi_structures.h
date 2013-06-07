// extends a bunch of winapi structures for use in c++ systems
#ifndef WINAPI_STRUCTURES
#define WINAPI_STRUCTURES

#include <windows.h>
#include <iostream>

// global operators for RECT
bool operator == (const RECT& left, const RECT& right);
std::ostream& operator<<(std::ostream& os, const RECT& r);

// global operators for POINT
bool operator == (const POINT& left, const POINT& right);
std::ostream& operator<<(std::ostream& os, const POINT& p);

#endif