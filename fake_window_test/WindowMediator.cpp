#include "WindowMediator.h"


WindowMediator::WindowMediator(IWindow* window, int x, int y, uint width, uint height, IWindow* parent) 
	: window(window) {
	window->InitWindow(x, y, width, height, parent);
}