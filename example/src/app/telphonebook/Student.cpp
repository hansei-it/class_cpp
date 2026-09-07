//===========================================================
// app/telphonebook/Student.cpp
//===========================================================
#include "Student.h"

Student::Student(const string& n, const string& p, int g) :
	Person(n, p), grade(g) { }

void Student::Print(ostream& out)const
{
	out << "name:" << name << ", " << "phone:" << phone <<
		", " << "grade:" << grade << endl;
}

// [주의] ostream 은 복사할 수 없습니다.
//   void Print(ostream out)const  <- X : 컴파일 에러
//   void Print(ostream& out)const <- O
