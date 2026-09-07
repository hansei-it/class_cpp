//===========================================================
// ex6.cpp - 복사 생성자와 깊은 복사 : String 클래스 만들기
//   1) std::string 을 먼저 써 보고, 같은 것을 직접 만들어 본다
//   2) 포인터 멤버(char* buf) 를 가진 클래스의 생성자 / 소멸자
//   3) append / assign - 동적 메모리 재할당
//   4) 얕은 복사(shallow copy) 의 문제 : 이중 해제(double free)
//   5) 복사 생성자로 깊은 복사(deep copy) 구현하기
//   6) 대입 연산자를 private 으로 막아 두기 (다음 강의 ex7 에서 구현)
//   -> 파일로 분리한 예제는 src/app/string 폴더를 참고하세요.
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
#include <string>
#include <cstring>
using namespace std;

class String
{
public:
	typedef unsigned int Size_type;
private:
	char* buf;
	Size_type length;
	const String& operator=(const String& arg);   // 선언만 하고 정의 안 함 -> 대입 금지
public:
	String(const char* s = "") :buf(nullptr), length(0) {
		length = (Size_type)strlen(s);
		buf = new char[length + 1];       // +1 은 널 종료 문자('\0') 자리
		strcpy_s(buf, length + 1, s);
	}
	String(const String& arg) :buf(nullptr), length(arg.length)   // 복사 생성자
	{
		buf = new char[length + 1];       // 새 메모리를 따로 확보 -> 깊은 복사
		strcpy_s(buf, length + 1, arg.buf);
	}
	~String() { delete[] buf; }
	Size_type size()const { return length; }
	char at(Size_type i)const { return buf[i]; }
	void append(const char* s) {          // 뒤에 이어 붙이기
		int tlength = length + (Size_type)strlen(s);
		char* t = new char[tlength + 1];  // 더 큰 메모리를 새로 잡고
		strcpy_s(t, length + 1, buf);     // 옮겨 담은 뒤
		strcat_s(t, tlength + 1, s);

		delete[] buf;                     // 예전 메모리는 해제
		buf = t;
		length = tlength;
	}
	void assign(const char* s) {          // 새 문자열로 갈아 끼우기
		delete[] buf;

		length = (Size_type)strlen(s);
		buf = new char[length + 1];
		strcpy_s(buf, length + 1, s);
	}
	const char* c_str()const { return buf; }
};

int main()
{
	String s("Hello!");

	for (String::Size_type i = 0; i < s.size(); ++i)
		cout << s.at(i) << " ";
	cout << endl;

	s.append("12345");
	cout << s.c_str() << endl;      // Hello!12345

	s.assign("ABC");
	cout << s.c_str() << endl;      // ABC

	String s2 = s;                  // 복사 생성자 호출 -> 깊은 복사
	cout << s2.c_str() << endl;     // ABC
	//s2 = s;                       // X - 에러 : operator= 를 private 으로 막아 두었다
}

//----------------------------------------------------------
// [단계 7] 복사 생성자가 호출되는 3가지 경우를 눈으로 확인하기
//   Point 로 간단히 실험합니다. "copy Point()" 가 언제 찍히는지 세어 보세요.
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y)
//	{
//		cout << "Point() " << endl;
//	}
//	Point(const Point& arg) :x(arg.x), y(arg.y)
//	{
//		cout << "copy Point() " << endl;
//	}
//	void Print()const { cout << x << ',' << y << endl; }
//};
//void PrintPoint(Point arg)     // 2. 함수의 인수(값 객체)로 전달
//{
//	arg.Print();
//}
//Point pt;
//Point CreatePoint()
//{
//	return pt;                 // 3. 객체가 리턴(값 객체)
//}
//int main()
//{
//	Point pt1(2, 3);
//	Point pt2 = pt1;   //1. 경우 : 다른 객체로 초기화
//
//	PrintPoint(pt1);   //2. 함수의 인수(값 객체)로 전달
//
//	CreatePoint();     //3. 객체가 리턴(값 객체)
//}

//----------------------------------------------------------
// [단계 6b] 참조로 바꾸면 복사가 사라진다
//   위 단계 7 에서 값(Point) 을 참조(Point&) 로만 바꿔 보세요.
//   "copy Point()" 가 몇 번으로 줄어드는지 비교하는 것이 핵심입니다.
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y)
//	{
//		cout << "Point() " << endl;
//	}
//	Point(const Point& arg) :x(arg.x), y(arg.y)
//	{
//		cout << "copy Point() " << endl;
//	}
//	void Print()const { cout << x << ',' << y << endl; }
//};
//void PrintPoint(Point& arg)    // 값 -> 참조 : 복사가 일어나지 않는다
//{
//	arg.Print();
//}
//Point pt;
//Point& CreatePoint()           // 값 -> 참조 : 복사가 일어나지 않는다
//{
//	return pt;                 // 전역 객체라서 참조 반환이 안전하다 (ex7 참고)
//}
//int main()
//{
//	Point pt1(2, 3);
//	Point& pt2 = pt1;  //1. 참조로 받으면 복사 생성자가 안 불린다
//
//	PrintPoint(pt1);   //2. 참조 매개변수
//
//	CreatePoint() = Point(0, 0);   // 참조 반환이므로 왼쪽 값이 된다 (ex7)
//	pt.Print();
//}

//----------------------------------------------------------
// [단계 6] 복사 생성자 추가 - String s2 = s; 가 안전해진다
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <cstring>
//using namespace std;
//
//class String
//{
//public:
//	typedef unsigned int Size_type;
//private:
//	char* buf;
//	Size_type length;
//	const String& operator=(const String& arg);   // 대입은 아직 막아 둔다
//public:
//	String(const char* s = "") :buf(nullptr), length(0) {
//		length = (Size_type)strlen(s);
//		buf = new char[length + 1];
//		strcpy_s(buf, length + 1, s);
//	}
//	String(const String& arg) :buf(nullptr), length(arg.length)
//	{
//		buf = new char[length + 1];
//		strcpy_s(buf, length + 1, arg.buf);
//	}
//	~String() { delete[] buf; }
//	Size_type size()const { return length; }
//	char at(Size_type i)const { return buf[i]; }
//	void append(const char* s) {
//		int tlength = length + (Size_type)strlen(s);
//		char* t = new char[tlength + 1];
//		strcpy_s(t, length + 1, buf);
//		strcat_s(t, tlength + 1, s);
//
//		delete[] buf;
//		buf = t;
//		length = tlength;
//	}
//	void assign(const char* s) {
//		delete[] buf;
//
//		length = (Size_type)strlen(s);
//		buf = new char[length + 1];
//		strcpy_s(buf, length + 1, s);
//	}
//	const char* c_str()const { return buf; }
//};
//int main()
//{
//	String s("Hello!");
//
//	for (String::Size_type i = 0; i < s.size(); ++i)
//		cout << s.at(i) << " ";
//	cout << endl;
//
//	s.append("12345");
//	cout << s.c_str() << endl;
//
//	s.assign("ABC");
//	cout << s.c_str() << endl;
//
//	String s2 = s;
//	cout << s2.c_str() << endl;
//}

//----------------------------------------------------------
// [단계 5] 복사 생성자가 없으면? - 얕은 복사로 프로그램이 죽는다 (실험용)
//   String s2 = s; 를 하면 컴파일러가 만든 복사 생성자가 buf "주소"만 복사한다.
//   -> s 와 s2 가 같은 메모리를 가리키고, 소멸자가 두 번 delete[] 해서 크래시.
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <cstring>
//using namespace std;
//
//class String
//{
//public:
//	typedef unsigned int Size_type;
//private:
//	char* buf;
//	Size_type length;
//public:
//	String(const char* s = "") :buf(nullptr), length(0) {
//		length = (Size_type)strlen(s);
//		buf = new char[length + 1];
//		strcpy_s(buf, length + 1, s);
//	}
//	// 복사 생성자가 없다!
//	~String() { delete[] buf; }
//	Size_type size()const { return length; }
//	char at(Size_type i)const { return buf[i]; }
//	const char* c_str()const { return buf; }
//};
//int main()
//{
//	String s("Hello!");
//	String s2 = s;          // 얕은 복사 : buf 주소만 복사된다
//
//	cout << s.c_str() << endl;
//	cout << s2.c_str() << endl;
//}                           // 소멸자가 두 번 delete[] -> 크래시!

//----------------------------------------------------------
// [단계 4] append() 구현 - 더 큰 메모리를 새로 잡아 옮겨 담는다
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <cstring>
//using namespace std;
//
//class String
//{
//public:
//	typedef unsigned int Size_type;
//private:
//	char* buf;
//	Size_type length;
//public:
//	String(const char* s = "") :buf(nullptr), length(0) {
//		length = (Size_type)strlen(s);
//		buf = new char[length + 1];
//		strcpy_s(buf, length + 1, s);
//	}
//	~String() { delete[] buf; }
//	Size_type size()const { return length; }
//	char at(Size_type i)const { return buf[i]; }
//	void append(const char* s) {
//		int tlength = length + (Size_type)strlen(s);
//		char* t = new char[tlength + 1];
//		strcpy_s(t, length + 1, buf);
//		strcat_s(t, tlength + 1, s);
//
//		delete[] buf;
//		buf = t;
//		length = tlength;
//	}
//	void assign(const char* s) {
//		delete[] buf;
//
//		length = (Size_type)strlen(s);
//		buf = new char[length + 1];
//		strcpy_s(buf, length + 1, s);
//	}
//	const char* c_str()const { return buf; }
//};
//int main()
//{
//	String s("Hello!");
//
//	for (String::Size_type i = 0; i < s.size(); ++i)
//		cout << s.at(i) << " ";
//	cout << endl;
//
//	s.append("12345");
//	cout << s.c_str() << endl;
//
//	s.assign("ABC");
//	cout << s.c_str() << endl;
//}

//----------------------------------------------------------
// [단계 3] assign() 구현 - 기존 메모리를 지우고 새로 할당한다
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <cstring>
//using namespace std;
//
//class String
//{
//public:
//	typedef unsigned int Size_type;
//private:
//	char* buf;
//	Size_type length;
//public:
//	String(const char* s = "") :buf(nullptr), length(0) {
//		length = strlen(s);
//		buf = new char[length + 1];
//		strcpy_s(buf, length + 1, s);
//	}
//	~String() { delete[] buf; }
//	Size_type size()const { return length; }
//	char at(Size_type i)const { return buf[i]; }
//	void append(const char* s) {
//		// 아직 비어 있음
//	}
//	void assign(const char* s) {
//		delete[] buf;
//
//		length = strlen(s);
//		buf = new char[length + 1];
//		strcpy_s(buf, length + 1, s);
//	}
//	const char* c_str()const { return buf; }
//};
//int main()
//{
//	String s("Hello!");
//
//	for (String::Size_type i = 0; i < s.size(); ++i)
//		cout << s.at(i) << " ";
//	cout << endl;
//
//	s.append("12345");
//	cout << s.c_str() << endl;
//
//	s.assign("ABC");
//	cout << s.c_str() << endl;
//}

//----------------------------------------------------------
// [단계 2] 생성자에서 동적 할당, 소멸자에서 해제 (RAII 의 기본형)
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <cstring>
//using namespace std;
//
//class String
//{
//public:
//	typedef unsigned int Size_type;
//private:
//	char* buf;
//	Size_type length;
//public:
//	String(const char* s = ""):buf(nullptr),length(0) {
//		length = strlen(s);
//		buf = new char[length + 1];       // 생성자에서 확보
//		strcpy_s(buf, length + 1, s);
//	}
//	~String() { delete[] buf; }           // 소멸자에서 해제 -> free 를 잊을 수 없다
//	Size_type size()const { return length; }
//	char at(Size_type i)const { return buf[i]; }
//	void append(const char* s) {
//
//	}
//	void assign(const char* s) {
//
//	}
//	const char* c_str()const { return buf; }
//};
//int main()
//{
//	String s("Hello!");
//
//	for (String::Size_type i = 0; i < s.size(); ++i)
//		cout << s.at(i) << " ";
//	cout << endl;
//
//	s.append("12345");
//	cout << s.c_str() << endl;
//
//	s.assign("ABC");
//	cout << s.c_str() << endl;
//}

//----------------------------------------------------------
// [단계 1b] 껍데기부터 만들기 - 인터페이스를 std::string 과 똑같이 맞춘다
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//using namespace std;
//
//class String
//{
//public:
//	typedef unsigned int Size_type;
//public:
//	String(const char* s=""){}
//
//	Size_type size() { return 0; }
//	char at(Size_type i) { return 'A'; }
//	void append(const char* s) {
//
//	}
//	void assign(const char* s) {
//
//	}
//	const char* c_str() { return ""; }
//};
//int main()
//{
//	String s("Hello!");
//
//	for (String::Size_type i = 0; i < s.size(); ++i)
//		cout << s.at(i) << " ";
//	cout << endl;
//
//	s.append("12345");
//	cout << s.c_str() << endl;
//
//	s.assign("ABC");
//	cout << s.c_str() << endl;
//}

//----------------------------------------------------------
// [단계 1] 먼저 std::string 을 써 본다 - 우리가 만들 목표 모습
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//using namespace std;
//
//int main()
//{
//	string s("Hello!");
//
//	for (string::size_type i = 0; i < s.size(); ++i)
//		cout << s.at(i) << " ";
//	cout << endl;
//
//	s.append("12345");
//	cout << s.c_str() << endl;
//
//	s.assign("ABC");
//	cout << s.c_str() << endl;
//}
