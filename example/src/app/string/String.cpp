//===========================================================
// app/string/String.cpp - String 클래스의 구현
//===========================================================
#include <cstring>
#include "String.h"

String::String(const char* s) :buf(nullptr), length(0) {
	length = (Size_type)strlen(s);
	buf = new char[length + 1];
	strcpy_s(buf, length + 1, s);
}
String::String(const String& arg) :buf(nullptr), length(arg.length)
{
	buf = new char[length + 1];
	strcpy_s(buf, length + 1, arg.buf);
}
String::~String() { delete[] buf; }

String::Size_type String::size()const { return length; }
char String::at(Size_type i)const { return buf[i]; }

void String::append(const char* s) {
	int tlength = length + (Size_type)strlen(s);
	char* t = new char[tlength + 1];
	strcpy_s(t, length + 1, buf);
	strcat_s(t, tlength + 1, s);

	delete[] buf;
	buf = t;
	length = tlength;
}
void String::assign(const char* s) {
	delete[] buf;

	length = (Size_type)strlen(s);
	buf = new char[length + 1];
	strcpy_s(buf, length + 1, s);
}
const char* String::c_str()const { return buf; }

//복사 대입 연산자 : 기존 메모리를 지우고 새로 확보한 뒤 값을 옮긴다
const String& String::operator=(const String& arg)
{
	if (this == &arg)      // 자기 대입 방지 : s = s;
		return *this;

	delete[] buf;

	length = (Size_type)strlen(arg.buf);
	buf = new char[length + 1];
	strcpy_s(buf, length + 1, arg.buf);

	return *this;
}
