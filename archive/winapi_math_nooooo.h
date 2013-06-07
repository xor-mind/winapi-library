// contains some simple math abstractions to make programming the GDI part of the library easier
#ifndef WINAPI_MATH_H
#define WINAPI_MATH_H

namespace wl // wl := winapi-library
{

template <class T>
class Vector
{
public:
	T x, y;

	Vector() {};
	Vector(T x, T y) : x(x), y(y) {}
	
	int operator == (const Vector& rhs) 
	{
		if ( x == rhs.x && y == rhs.y )
			return 1;
		else
			return 0;
	}
	
	int operator != (const Vector& rhs)
	{
		if ( ! ( *this == rhs) )
			return 1;
		else
			return 0;
	}

	Vector& operator *= (const T rhs)
	{
		x*=rhs;
		y*=rhs;
		return (*this);
	}	

	Vector operator * (const T rhs)
	{
		return (Vector(*this) *= rhs);
	}
	
	Vector& Vector::operator /= (const float rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	Vector operator / (const T rhs)
    {
		return (Vector(*this) /= rhs);
	}
	
	Vector& Vector::operator += (const Vector& rhs)
	{
		x+=rhs.x;
		y+=rhs.y;
		return (*this);
	}

	Vector Vector::operator + (const Vector& rhs)
	{
		return (Vector(*this) += rhs);
	}

	Vector& Vector::operator -= (const Vector& rhs)
	{
		x-=rhs.x;
		y-=rhs.y;
		return (*this);
	}

	Vector Vector::operator - (const Vector& rhs)
	{
		return (Vector(*this) -= rhs);
	}
};

template<class T> Vector<T> operator*( T lhs, const Vector<T>& rhs )
{
	return (Vector<T>(rhs) *= lhs);
}
	
}

#endif