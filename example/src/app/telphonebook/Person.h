//===========================================================
// app/telphonebook/Person.h - 전화번호부에 담기는 기본 자료
//   virtual Print / virtual ~Person 이 다형성의 핵심입니다.
//===========================================================
#pragma once
#include <iostream>
#include <string>
using namespace std;

class Person
{
protected:                      // 자식(Student, Professor)이 쓸 수 있게 protected
	string name;
	string phone;
public:
	Person(const string& n = "", const string& p = "");
	virtual ~Person();                                // 가상 소멸자 (필수)
	virtual void Print(ostream& out = cout)const;     // 화면에도 파일에도 출력
	const string& GetName()const;
	const string& GetPhone()const;
};
