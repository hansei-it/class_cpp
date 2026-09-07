//===========================================================
// app/string/String.h - 직접 만드는 문자열 클래스의 선언
//   ex6 · ex7 에서 만든 String 클래스를 헤더 / 소스로 분리한 것입니다.
//   포인터 멤버를 가진 클래스가 반드시 갖춰야 할 3가지(3의 법칙):
//     1) 소멸자        ~String()
//     2) 복사 생성자   String(const String&)
//     3) 복사 대입     operator=(const String&)
//===========================================================
#pragma once

class String
{
public:
	typedef unsigned int Size_type;
private:
	char* buf;
	Size_type length;
public:
	String(const char* s = "");
	String(const String& arg);
	~String();
	Size_type size()const;
	char at(Size_type i)const;
	void append(const char* s);
	void assign(const char* s);
	const char* c_str()const;
	const String& operator=(const String& arg);
};
