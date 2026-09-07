//===========================================================
// app/telphonebook/Professor.h - Person 을 상속한 교수
//===========================================================
#pragma once
#include <iostream>
#include "Person.h"
using namespace std;

class Professor : public Person
{
	string position;
public:
	Professor(const string& n = "", const string& p = "",
		const string& pos = "");
	void Print(ostream& out = cout)const;
};
