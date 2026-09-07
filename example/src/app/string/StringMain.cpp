//===========================================================
// app/string/StringMain.cpp - String 을 담는 배열 클래스 StrArray
//   String 이 복사 생성자와 operator= 를 제대로 갖췄기 때문에
//   StrArray 안에서 String 을 값으로 저장하고 대입할 수 있습니다.
//
// [빌드 방법]
//   1. ex1.cpp ~ ex16.cpp 를 모두 "빌드에서 제외 : 예" 로 둔다.
//   2. app/string 폴더의 String.cpp, StringMain.cpp 만 제외를 푼다.
//   3. 실행 (Ctrl + F5)
//===========================================================
#include <iostream>
#include <vector>
#include "String.h"
using namespace std;

class StrArray
{
public:
	typedef unsigned int size_type;
private:
	String* buf;
	size_type count;
	const StrArray& operator=(const StrArray&);   // 대입은 막아 둔다
public:
	StrArray() :buf(nullptr), count(0)
	{
		buf = new String[100];      // String 의 기본 생성자가 100번 호출된다
	}
	StrArray(const StrArray& arg) :buf(nullptr), count(arg.count)
	{
		buf = new String[100];
		for (size_type i = 0; i < count; ++i)
			buf[i] = arg.buf[i];    // String::operator= 가 호출된다
	}
	~StrArray() { delete[] buf; }
	void push_back(const String& data)
	{
		buf[count++] = data;        // String::operator= 가 호출된다
	}
	String& at(int idx) { return buf[idx]; }
	const String& at(int idx)const { return buf[idx]; }
	size_type size() const { return count; }
};

void PrintArray(const StrArray& arr)
{
	for (StrArray::size_type i = 0; i < arr.size(); ++i)
		cout << arr.at(i).c_str() << " ";
	cout << endl;
}

int main()
{
	StrArray arr;
	arr.push_back(String("10"));
	arr.push_back(String("20"));
	arr.push_back(String("30"));
	arr.push_back(String("40"));
	arr.push_back(String("50"));

	arr.at(0) = String("100");   // at() 이 참조를 반환 -> operator= 로 대입
	PrintArray(arr);             // 100 20 30 40 50
}
