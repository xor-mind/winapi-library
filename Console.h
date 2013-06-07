#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include "Prerequisites.h"
#include "adams_macros.h"

void CenterConsoleWindow();
void CenteredConsoleOutput(char* s, unsigned int consoleWidthInChars = 80);
inline void hr() { Eighty(say "=") endline }

#endif