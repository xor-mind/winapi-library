#ifndef GLOBAL_OPERATORS_H
#define GLOBAL_OPERATORS_H

#include <Windows.h>
#include <iostream>

bool operator == (const RECT& left, const RECT& right);
std::ostream& operator<<(std::ostream& os, const RECT& r);

bool operator == (const POINT& left, const POINT& right);
std::ostream& operator<<(std::ostream& os, const POINT& p);


#endif