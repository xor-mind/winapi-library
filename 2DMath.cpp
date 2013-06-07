#include <cmath>
#include "2Dmath.h"

//int round(float f) 
//{
//	return (int)( (f > 0.0f) ? floor(f + 0.5f) : ceil(f - 0.5f) );
//}

// Get the Vectors Magnitude squared
float Vector::LengthSqr() const
{
	return (x*x + y*y);
}

// Length of Resultant
float Vector::Length() const
{
	return (float)sqrt(x*x + y*y);
}

void Vector::RotateVectorCounterClockwise(float angle)
{
		float     x0 = x,
	              cosa = cos(angle),
			      sina = sin(angle); 
		
		// point rotation as derived from the angle sum formulas in trig, sin(a+b) and cos(a+b)
		x = x0*cosa-y*sina;
		y = y*cosa+x0*sina;
}

Vector Vector::operator*(const float rhs)
{
	return (Vector(*this) *= rhs);
}

Vector Vector::operator/(const float rhs)
{
	return (Vector(*this) /= rhs);
}

Vector Vector::operator+(const Vector& rhs)
{
	return (Vector(*this) += rhs);
}

Vector Vector::operator-(const Vector& rhs)
{
	return (Vector(*this) -= rhs);
}

Vector& Vector::operator-=(const Vector& rhs)
{
	x-=rhs.x;
	y-=rhs.y;
	return (*this);
}

//Vector& Vector::operator*=(const float rhs)
//{
//	x*=rhs;
//	y*=rhs;
//	return (*this);
//}

Vector& Vector::operator/=(const float rhs)
{
	x/=rhs;
	y/=rhs;
	return (*this);
}
/*
Vector Vector::operator/(const int rhs)
{
	return (Vector(*this) /= rhs);
}

Vector& Vector::operator/=(const int rhs)
{
	x = (int)x/rhs;
	y = (int)y/rhs;
	return (*this);
}*/


int Vector::operator!=(const Vector& rhs)
{
	if (x != rhs.x || y != rhs.y)
		return 1;
	else
		return 0;
}

int Vector::operator==(const Vector& rhs)
{
	if (!(*this != rhs))
		return 1;
	else
		return 0;
}



//// ------------------------------------------------------------------------------------------------------------- //
//// IntVector implementation 
//int IntVector::LengthSqr() const
//{
//	return (x*x + y*y);
//}
//
//int IntVector::Rect() const
//{
//	return (x*y);
//}
//
//int IntVector::operator==(const IntVector& rhs)
//{
//	if (this->x == rhs.x && this->y == rhs.y)
//		return 1;
//	else
//		return 0;
//}
//
//int IntVector::operator!=(const IntVector& rhs)
//{
//	return (!(*this == rhs));
//}
//
//IntVector IntVector::operator* (const int rhs)
//{
//	return (IntVector(*this) *= rhs);
//}
//
//IntVector& IntVector::operator*=(const int rhs)
//{
//	this->x *= rhs;
//	this->y *= rhs;
//	return (*this);
//}
//
//IntVector IntVector::operator+ (const IntVector& rhs)
//{
//	return (IntVector(*this)+=rhs);
//}
//
//IntVector& IntVector::operator+=(const IntVector& rhs)
//{
//	this->x += rhs.x;
//	this->y += rhs.y;
//	return (*this);
//}