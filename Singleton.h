#ifndef SINGLETON_H
#define SINGLETON_H

template <class T>
class Singleton
{
public:
	~Singleton() {}

	static T* GetInstance()
	{
		if ( t() == 0 )
		{
			t() = new T();
		}

		return t();
	}

private:
	Singleton() {}
	static T*& t() { static T* _t = 0; return _t; }
};

#endif