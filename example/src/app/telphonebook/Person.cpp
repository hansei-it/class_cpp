//===========================================================
// app/telphonebook/Person.cpp
//===========================================================
#include "Person.h"
#include <iostream>
#include <string>
using namespace std;

Person::Person(const string& n, const string& p) :name(n), phone(p)
{
}
Person::~Person() { }

void Person::Print(ostream& out)const
{
	out << "name:" << name << ", " << "phone:" << phone << endl;
}
const string& Person::GetName()const
{
	return name;
}
const string& Person::GetPhone()const
{
	return phone;
}
