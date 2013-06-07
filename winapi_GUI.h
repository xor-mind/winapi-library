#ifndef WINAPI_GUI
#define WINAPI_GUI

#include <iostream>
#include <string>
#include "winapi_DataConversion.h"
// this is like css's X px or X %, you can use GuiMeasurements as a way to talk about lengths 
// while styling our windows. Maybe we want the padding to be 10% of the widfth of the window, 
// maybe want to use dialog units (DU) or maybe we want talk about pixel lengths.

typedef struct GuiMeasurement
{
	// template constructors allow us to pass in a generic value, either an integer, float 
	// or some user defined type type assocaited with a GUI metric.
	template <class T> GuiMeasurement(T t) { length = 0;  }
	template <class T> GuiMeasurement(T t, int length) : length(length) {}
	virtual ~GuiMeasurement() { }

	// Cloning allows for deep copying when we aren't keeping track of polymorphic types during
	// run time
	virtual GuiMeasurement * Clone() = 0; 
	virtual GuiMeasurement * Clone(int) = 0;

	// convert the measurement to pixels, allow for passing in a length or use our member length
	virtual int pixelCount() const = 0;
	virtual int pixelCount(int length) const = 0; 

	// for boost::test
	virtual std::string str() const = 0;



	int operator + (const GuiMeasurement &rhs)
	{
		return pixelCount() + rhs.pixelCount();
	}

	operator int() { return pixelCount(); }

	int operator()(int length) { return pixelCount(length); }
	int operator()()		  { return pixelCount(); }

	virtual bool operator==(int i) = 0; 
	virtual bool operator==(float f) = 0;
	bool operator==(const GuiMeasurement& gm)
	{
		return pixelCount() == gm.pixelCount();
	};
	int length;

} * pgm;

// our abstraction for pixel lengths in a GUI. Although a length can be passed it in to satisfy 
// the GuiMeasurement interface, it's not used, pixels don't need a context to have value, unlike
// a Percentage
struct Px : public GuiMeasurement
{
	Px(int px) : GuiMeasurement(px), px(px) { }
	Px(int px, int length) : GuiMeasurement(px, length), px(px) { }
	
	Px * Clone() 
	{
		return new Px(px, length);
	}
	Px * Clone(int length) 
	{
		return new Px(px, length);
	}
	
	int pixelCount() const { return px; }
	int pixelCount(int length) const { return px;}

	std::string str()  const  
	{
		return "px: " + PBNC::lexical_cast<std::string>(px);
	}

	bool operator==(int i) { return i == px; }; 
	bool operator==(float) { return false; };

	int px;
};

// our abstraction for a Percentage, it needs a length to give the percentage context, such 
// as the length of bottom of the window, or the length of the width or height of the window.
struct Percent : public GuiMeasurement
{
	Percent(float percent) : GuiMeasurement(percent), percent(percent) {}
	Percent(float percent, int length) : GuiMeasurement(percent, length), percent(percent) {}
	int pixelCount() const 
	{ 
		float f = percent;
		int f1 = this->length;
		return (int)(percent * this->length); 
	}
	int pixelCount(int length) const 
	{ 
		int f = length;
		int f1 = this->length;
		return (int)(percent * length); 
	}

	Percent * Clone() 
	{
		return new Percent(percent, length);
	}

	Percent * Clone(int length) 
	{
		return new Percent(percent, length);
	}

	std::string str() const  
	{
		return "percent: " + PBNC::lexical_cast<std::string>(percent);
	}

	bool operator==(int) { return false; }; 
	bool operator==(float f) { return f == percent; };

	float percent;
};

// these factories are used to call the appropriate constructor of GuiMeasurement 
// based on the overloaded passed in arguement.
struct GuiMeasurementFactory
{
	static pgm create(int i)   { return new Px(i); }
	static pgm create(float f) { return new Percent(f); }
	static pgm create(int px, int length)         { return new Px(px,length); }
	static pgm create(float percent, int length)  { return new Percent(percent,length); }
};

#define CreateGM(...) GuiMeasurementFactory::create(__VA_ARGS__)

// padding is how much space inside the window is blocked off
struct Padding
{
	// it uses a template constructor to implicitly construct the correct GuiMeasurement based off an implicit type.
	template <class L, class T, class R, class B >
	Padding(L l, T t, R r, B b)
		: left(CreateGM(l)), top(CreateGM(t)), right(CreateGM(r)), bot(CreateGM(b)) 
			{ }

	template <class L, class T, class R, class B >
	Padding(int w, int h, L l, T t, R r, B b)
		: width(w), height(h), left(CreateGM(l, w)), top(CreateGM(t, h)), right(CreateGM(r, w)), bot(CreateGM(b,h)) 
			{ }

	Padding(const Padding& p) 
	{
		width = p.width; height = p.height;
		left  = p.left->Clone();
		top   = p.top->Clone();
		right = p.right->Clone();
		bot   = p.bot->Clone();
	}
	
	Padding(const Padding& p, int w, int h) 
	{
		width = w; height = h;
		left  = p.left->Clone(w);
		top   = p.top->Clone(h);
		right = p.right->Clone(w);
		bot   = p.bot->Clone(h);
	}

	~Padding()
	{
		if ( left )
			delete left;
		if ( top )
			delete top;
		if ( right )
			delete right;
		if ( bot )
			delete bot;
	}

	void WidthAndHeight(int w, int h) 
	{ 
		width = w; height = h; 
		left->length = right->length = w;
		top->length = bot->length = h;
	}

	int Width()  { return left->pixelCount() + right->pixelCount(); }

	int Height() { return top->pixelCount() + bot->pixelCount(); }

	int Width(int length)  { return left->pixelCount(length) + right->pixelCount(length); }

	int Height(int length) { return top->pixelCount(length) + bot->pixelCount(length); }

	int width, height; // width and height of window the padding belongs too.
	pgm left, top, right, bot;

	bool operator == (const Padding & p)
	{
		//return ( *left == *p.left  && *top == *p.top && *right == *p.right && *p.bot == *bot );
		return true;
	}
};

//bool operator == (const RECT& left, const RECT& right);
std::ostream& operator<<(std::ostream& os, const Padding& p);
std::ostream& operator<<(std::ostream& os, const GuiMeasurement& gm);

bool operator == (const Padding& l, const Padding& r);


#endif