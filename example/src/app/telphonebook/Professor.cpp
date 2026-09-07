//===========================================================
// app/telphonebook/Professor.cpp
//===========================================================
#include "Professor.h"

Professor::Professor(const string& n, const string& p,
	const string& pos) :Person(n, p), position(pos) { }

void Professor::Print(ostream& out)const
{
	out << "name:" << name << ", " << "phone:" << phone <<
		", " << "position:" << position << endl;
}
