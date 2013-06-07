#include "winapi_GUI.h"

std::ostream& operator<<(std::ostream& os, const Padding& p)
{
	os << p.left->str().c_str()  << " " << p.top->str().c_str() << " "
	   << p.right->str().c_str() << " " << p.bot->str().c_str() << std::endl;
	return os;
}

std::ostream& operator<<(std::ostream& os, const GuiMeasurement& gm)
{
	os << gm.str().c_str() << std::endl;
	return os;
}

bool operator == (const Padding& l, const Padding& r)
{
	return ( *l.left == *r.left  && *l.top == *r.top && *l.right == *r.right && *l.bot == *r.bot );
}


//std::ostream& operator==(std::ostream& os, const G& p)
//{
//	os << p.left->str().c_str()  << _T(" ") << p.top->str().c_str() << _T(" ")
//	   << p.right->str().c_str() << _T(" ") << p.bot->str().c_str() << std::endl;
//	return os;
//}