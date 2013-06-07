#ifndef WINDOW_MEDIATOR_H
#define WINDOW_MEDIATOR_H

#include "IWindow.h"

class WindowMediator
{
public:
	WindowMediator(IWindow* window, int x, int y, uint height, uint width, IWindow* parent = NULL);
	~WindowMediator() {}
	
private:
	IWindow * window;
};

#endif