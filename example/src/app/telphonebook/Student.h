//===========================================================
// app/telphonebook/Student.h - Person 을 상속한 학생
//===========================================================
#pragma once
#include <iostream>
#include "Person.h"
using namespace std;

class Student : public Person
{
	int grade;
public:
	Student(const string& n = "", const string& p = "", int g = 0);
	void Print(ostream& out = cout)const;   // ostream 은 반드시 참조(&)로 받는다
};
