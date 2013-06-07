// math stuff that other libraries haven't taken care of
#ifndef _2D_MATH_H
#define _2D_MATH_H

#include <cmath>

typedef unsigned int uint;

// important math constants
const float PI = 3.14159f;

// essential math functions
template <class T> 
T ABS(T n)
{
	return ( (n<0) ? -n : n);
}

//int round(float f);


// essential math concepts
class Vector
{
public:

	float x, y;

	Vector(float x0,float y0):x(x0),y(y0){}
	Vector() {};

	Vector floor() { return Vector( std::floor(x), std::floor(y) ); }
	Vector Round() { return Vector( (x > 0.0f) ? ::floor(x + 0.5f) : ceil(x - 0.5f), (y > 0.0f) ? ::floor(y + 0.5f) : ceil(y - 0.5f) ); }

	float	Length() const; // Get the Vector2D's magnitude.	
	float	LengthSqr() const; // Get the Vector2D's magnitude squared.

	// Vector Transformation Functions
	void RotateVectorCounterClockwise(float angle);

	int     operator==(const Vector& rhs);
	int     operator!=(const Vector& rhs);
	Vector  operator* (const float rhs);
	Vector  operator/ (const float rhs);
	Vector&  operator*= (const float rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	Vector& operator*= (const Vector& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}
	Vector& operator/= (const float rhs);
	Vector  operator+ (const Vector& rhs);
	Vector& operator+=(const Vector& rhs)
	{
		x+=rhs.x;
		y+=rhs.y;
		return (*this);
	}
	Vector  operator- (const Vector& rhs);
	Vector& operator-=(const Vector& rhs);
	//Vector& operator/= (const int rhs);
	//Vector operator/ (const int rhs);
	
};

namespace math
{
class Rectangle
{
public:

	Rectangle()
	{ 
		left = top = right = bottom = 0; 
	}
	Rectangle(int left, int top, int right, int bottom)
		: left(left), top(top), right(right), bottom(bottom) {}
	Rectangle(Vector pos, Vector dim)
	{
		left = (int)pos.x; top = (int)pos.y;
		right = (int)(pos.x + dim.x);
		bottom = (int)(pos.y + dim.y);
	}
	~Rectangle() {}

	int left, top, right, bottom;

	bool Intersect(Rectangle& otherRect)
	{
		if ( left <= otherRect.right && right >= otherRect.left )
		{
			if ( top <= otherRect.bottom && bottom >= otherRect.top )
			{
				return true;
			}
		}

		return false;
	}
};
}

//// a vector that's made for ints and concrete mathematics
//class IntVector
//{
//public:
//
//	int x, y;
//
//	IntVector(int x0, int y0):x(x0),y(y0){}
//	IntVector(const Vector& v):x((int)v.x), y((int)v.y) { }
//	IntVector() {};
//
//	int	LengthSqr() const; // sqrt(x^2+y^)^2;
//	int Rect() const; // x*y;
//
//	// Vector Transformation Functions
//	//void RotateVectorCounterClockwise(float angle);
//
//	int        operator==(const IntVector& rhs);
//	int        operator!=(const IntVector& rhs);
//	IntVector  operator* (const int rhs);
//	IntVector& operator*=(const int rhs);
//	IntVector  operator+ (const IntVector& rhs);
//	IntVector& operator+=(const IntVector& rhs);
//	
//};
#endif
