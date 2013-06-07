#ifndef BRUSH_FACTORY_H
#define BRUSH_FACTORY_H

#include <windows.h>
#include <vector>

struct BrushFactory
{
	#define createBrush(x,y) HBRUSH& x##() { brushList.push_back(CreateSolidBrush(y)); return brushList.back(); }
	
	~BrushFactory()
	{
		for ( size_t i = 0; i < brushList.size(); ++i ) 
			DeleteObject(brushList[i]);
	}

	createBrush(blue,   RGB(0,     0, 255));
	createBrush(orange, RGB(255, 127,   0));
	createBrush(pink,   RGB(255,   0, 255));
	createBrush(purple, RGB(223,   0, 255));
	createBrush(red,    RGB(255,   0,   0));
	createBrush(yellow, RGB(255, 255,   0));

	std::vector<HBRUSH> brushList;
};

struct PenFactory
{
	#define createPen(x,y) HPEN& x##() { penList.push_back(CreatePen(PS_SOLID, 1, y)); return penList.back(); }
	
	~PenFactory()
	{
		for ( size_t i = 0; i < penList.size(); ++i ) 
			DeleteObject(penList[i]);
	}

	createPen(blue,   RGB(0,     0, 255));
	createPen(orange, RGB(255, 127,   0));
	createPen(pink,   RGB(255,   0, 255));
	createPen(purple, RGB(223,   0, 255));
	createPen(red,    RGB(255,   0,   0));
	createPen(yellow, RGB(255, 255,   0));

	std::vector<HPEN> penList;
};


#endif