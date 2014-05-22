/*
Brush and pen factories for creating brushes and pens with certain colors. The factory objects keep track of all objects and delete
them when going out of scope. The factories may also be used as singletons.
*/
#ifndef FACTORIES_H
#define FACTORIES_H

#include <windows.h>
#include <vector>

#define RGB_ORANGE RGB( 255, 127,   0 ) 
#define RGB_BLUE   RGB(   0,   0, 255 )

struct BrushFactory
{
	#define createBrush(x,y) HBRUSH& x##() { brushList.push_back(CreateSolidBrush(y)); return brushList.back(); }
	
	~BrushFactory()
	{
		for ( size_t i = 0; i < brushList.size(); ++i ) 
			DeleteObject(brushList[i]);
	}

	createBrush(green,   RGB(20,     218, 20));
	createBrush(blue,   RGB(0,     0, 255));
	createBrush(lightGrey, RGB(210, 210, 210));
	createBrush(orange, RGB(255, 127,   0));
	createBrush(pink,   RGB(255,   0, 255));
	createBrush(purple, RGB(223,   0, 255));
	createBrush(red,    RGB(255,   0,   0));
	createBrush(yellow, RGB(255, 255,   0));
	createBrush(white, RGB(255, 255, 255));

	std::vector<HBRUSH> brushList;

	static HBRUSH Orange() { static HBRUSH b = bf().orange();    return b; }
	static HBRUSH Red()    { static HBRUSH b = bf().red();    return b; }
	static HBRUSH Yellow() { static HBRUSH b = bf().yellow(); return b; }
	static BrushFactory & bf() { static BrushFactory bf; return bf; }
};

struct PenFactory
{
	#define createPen(x,y) HPEN& x##() { penList.push_back(CreatePen(PS_SOLID, 1, y)); return penList.back(); }
	
	~PenFactory()
	{
		for ( size_t i = 0; i < penList.size(); ++i ) 
			DeleteObject(penList[i]);
	}

	HPEN CreateThickPen( COLORREF color, int thickness )
	{
		penList.push_back( CreatePen(PS_SOLID, thickness, color) ); 
		return penList.back();
	}

	createPen(blue,   RGB(0,     0, 255));
	createPen(orange, RGB(255, 127,   0));
	createPen(pink,   RGB(255,   0, 255));
	createPen(purple, RGB(223,   0, 255));
	createPen(red,    RGB(255,   0,   0));
	createPen(yellow, RGB(255, 255,   0));
	createPen(white, RGB(255, 255, 255));

	std::vector<HPEN> penList;

	static HPEN White()   { static HPEN p = pf().white(); return p; }
	static PenFactory & pf() { static PenFactory pf; return pf; }
};
#endif