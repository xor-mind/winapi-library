#ifndef COLOR
#define COLOR

template <class T>
class Color
{
private:
	T m_red;
	T m_green;
	T m_blue;
	T m_alpha;

public: 
	Color(T red, T green, T blue, T alpha)
	{
		m_red = red; m_green = green; m_blue = blue; m_alpha = alpha;
	}
	~Color() {}
	
	// gets and sets ------------------------------------------------------- //
	T Red() const { return m_red; }
	void Red(const T red) { m_red = red; }
	
	T Green() const { return m_green; }
	void Green(const T green) { m_green = green; }; 	
	
	T Blue() const { return m_blue; }
	void Blue(const T blue) { m_blue = blue; }
	
	T Alpha() const { return m_alpha; }
	void Alpha(const T alpha) { m_alpha = alpha; }

	Color<T> operator + ( const Color<T> rhs )
	{
		return Color<T>(m_red + rhs.Red(), m_green + rhs.Green(), m_blue + rhs.Blue(), m_alpha + rhs.Alpha());
	}

	Color<T> operator / ( const int scalar)
	{
		return Color<T>(m_red/scalar, m_green/scalar, m_blue/scalar, m_alpha/scalar);
	}

	bool operator == (const Color<T> rhs)
	{
		if ( (m_red == rhs.Red()) && (m_green == rhs.Green()) && (m_blue == rhs.Blue()) && (m_alpha == rhs.Alpha()) )
			return true;
		else
			return false;
	}

	Color<T> & operator -- ()
	{
		m_red--; m_blue--; m_green--; m_alpha--;
		return *this;
	}
};

#endif