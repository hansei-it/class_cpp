//===========================================================
// ex13.cpp - 전역 연산자 오버로딩 · friend · 형변환 연산자
//   1) 멤버 operator+ 로는 p1 + 10 만 되고 10 + p1 은 안 된다
//   2) 전역 함수 operator+(int, const Point&) 로 해결하기
//   3) cout << p1 을 만드는 ostream& operator<<
//   4) friend - private 멤버에 접근하도록 허락하기
//   5) cin >> p1 을 만드는 istream& operator>>
//   6) 형변환 연산자 operator int() , operator const char*()
//   7) explicit - 원치 않는 자동 형변환 막기
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
#include <string>
using namespace std;

class Point
{
	int x, y;
public:
	Point() :x(0), y(0) {}
	explicit Point(int x) :x(x), y(0) {}   // explicit : Point p = 10; 을 막는다
	Point(int x, int y) :x(x), y(y) {}

	void Print()const { cout << x << ',' << y << endl; }
	int GetX()const { return x; }
	int GetY()const { return y; }

	Point operator+(int n)const             // p1 + 10
	{
		return Point(x + n, y + n);
	}
	operator int()const                     // (int)p1 또는 int n = p1;
	{
		return x;
	}
	operator const char* ()const            // (const char*)p1
	{
		return "class Point";
	}

	// 전역 함수에게 private 멤버 접근을 허락한다
	friend Point operator+(int n, const Point& pt);
	friend ostream& operator<<(ostream& out, const Point& pt);
	friend istream& operator>>(istream& in, Point& pt);
};

Point operator+(int n, const Point& pt)     // 10 + p1
{
	return Point(n + pt.x, n + pt.y);
}
ostream& operator<<(ostream& out, const Point& pt)   // cout << p1
{
	out << pt.x << ',' << pt.y;
	return out;      // ostream 을 반환해야 cout << p1 << p2 처럼 이어 쓸 수 있다
}
istream& operator>>(istream& in, Point& pt)          // cin >> p1
{
	cout << "pt(x,y) 입력: ";
	in >> pt.x >> pt.y;
	return in;
}

int main()
{
	Point p1(2, 3);
	Point p2;

	p2 = p1 + 10;      // p1.operator+(10)
	p2.Print();        // 12,13

	p2 = 10 + p1;      // operator+(10, p1)
	p2.Print();        // 12,13

	cout << p1 << " " << p2 << "  " << "Hello!" << endl;

	int n = p1;                              // operator int()
	const char* name = (const char*)p1;      // operator const char*()
	cout << n << " / " << name << endl;

	//Point p3 = 100;      // X - explicit 때문에 막힌다
	Point p3 = Point(100); // O - 명시적으로 써야 한다
	p3.Print();

	//cin >> p1 >> p2;     // 필요하면 주석을 풀고 실행해 보세요
}

//----------------------------------------------------------
// [단계 8] String 클래스 종합 - operator= , += , 형변환, <<
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//using namespace std;
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
//	const String& operator=(const String& arg)
//	{
//		if (this == &arg)      // 자기 대입 방지
//			return *this;
//		assign(arg.buf);       // 이미 만든 assign 을 재사용
//		return *this;
//	}
//	const String& operator+=(const String& arg)
//	{
//		if (this == &arg)
//			return *this;
//		append(arg.buf);
//
//		return *this;
//	}
//	operator const char* ()const   // 형변환 연산자
//	{
//		return buf;
//	}
//};
//ostream& operator<<(ostream& out, const String& s)
//{
//	out << s.c_str();
//	return out;
//}
//int main()
//{
//	String s("hello!");
//	cout << s << endl;
//
//	s = "ABC";       // const char* -> String 임시객체 -> operator=
//	cout << s << endl;
//
//	s += "DEF";
//	s += s;          // 자기 대입 검사 덕분에 안전하다
//	cout << s << endl;
//
//	//const char* str = (const char*)s;
//	const char* str = s.operator const char *();
//	cout << str << endl;
//}

//----------------------------------------------------------
// [단계 7] explicit - 생성자가 하나짜리면 자동 형변환이 일어난다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point() :x(0), y(0) {}
//	explicit Point(int x) :x(x), y(0) {}   // explicit 를 빼면 Point p = 10; 이 된다
//	Point(int x, int y) :x(x), y(y) {}
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	operator int()const
//	{
//		return x;
//	}
//	operator const char* ()const
//	{
//		return "class Point";
//	}
//};
//int main()
//{
//	Point p1(2, 3);
//	Point p3;
//
//	int n = 100;
//	//p3 = n;        // X - explicit 때문에 막힌다
//	p3 = (Point)n;   // O - Point(n) 을 명시적으로 호출
//	p3.Print();
//}

//----------------------------------------------------------
// [단계 6] 형변환 연산자 - 클래스를 다른 형식처럼 쓰이게 한다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) {}
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	operator int()const            // 반환형을 쓰지 않는다 (이름이 곧 반환형)
//	{
//		return x;
//	}
//	operator const char*()const
//	{
//		return "class Point";
//	}
//};
//int main()
//{
//	Point p1(2, 3);
//
//	int n = p1;                              // 자동으로 operator int() 호출
//	cout << "n: " << n << endl;              // 2
//	const char* name = (const char*)p1;
//	cout << "p1: " << name << endl;          // class Point
//}

//----------------------------------------------------------
// [단계 5] friend istream& operator>> - cin >> p1 만들기
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) {}
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	Point operator+(int n)const
//	{
//		return Point(x + n, y + n);
//	}
//	friend Point operator+(int n, const Point& pt);
//	friend ostream& operator<<(ostream& out, const Point& pt);
//	friend istream& operator>>(istream& in, Point& pt);
//};
//Point operator+(int n, const Point& pt)
//{
//	return Point(n + pt.GetX(), n + pt.GetY());
//}
//ostream& operator<<(ostream& out, const Point& pt)
//{
//	//out << pt.GetX() << ',' << pt.GetY();
//	out << pt.x << ',' << pt.y;      // friend 라서 private 에 직접 접근 가능
//	return out;
//}
//istream& operator>>(istream& in, Point& pt)   // 입력은 원본을 바꿔야 하므로 const 없음
//{
//	cout << "pt(x,y) 입력: ";
//	in >> pt.x >> pt.y;
//
//	return in;
//}
//int main()
//{
//	Point p1(2, 3);
//	Point p2(3, 4);
//
//	cin >> p1 >> p2;
//	cout << p1 << " " << p2 << "  " << "Hello!" << endl;
//}

//----------------------------------------------------------
// [단계 4] friend - 전역 함수에게 private 접근을 허락한다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) {}
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	Point operator+(int n)const
//	{
//		return Point(x + n, y + n);
//	}
//	friend Point operator+(int n, const Point& pt);
//	friend ostream& operator<<(ostream& out, const Point& pt);
//};
//Point operator+(int n, const Point& pt)
//{
//	return Point(n + pt.GetX(), n + pt.GetY());
//}
//ostream& operator<<(ostream& out, const Point& pt)
//{
//	//out << pt.GetX() << ',' << pt.GetY();
//	out << pt.x << ',' << pt.y;   // Get 함수 없이 바로 쓴다
//	return out;
//}
//int main()
//{
//	Point p1(2, 3);
//	Point p2(3, 4);
//
//	cout << p1 << " " << p2 << "  " << "Hello!" << endl;
//}

//----------------------------------------------------------
// [단계 3] cout << p1 의 정체 - operator<<(cout, p1) 을 부르는 것뿐이다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) {}
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	Point operator+(int n)const
//	{
//		return Point(x + n, y + n);
//	}
//};
//Point operator+(int n, const Point& pt)
//{
//	return Point(n + pt.GetX(), n + pt.GetY());
//}
//ostream& operator<<(ostream& out, const Point& pt)
//{
//	out << pt.GetX() << ',' << pt.GetY();
//	return out;    // 반환해야 << 를 이어 붙일 수 있다
//}
//int main()
//{
//	Point p1(2, 3);
//
//	cout << "Hello!" << endl;
//	operator <<(cout , "Hello!").operator <<(endl);   // 위 줄과 완전히 같은 뜻
//	cout << p1 << endl;
//	operator <<(cout, p1).operator <<(endl);
//}

//----------------------------------------------------------
// [단계 2] 전역 operator+(int, const Point&) - 10 + p1 을 가능하게
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) {}
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	Point operator+(int n)const
//	{
//		return Point(x + n, y + n);
//	}
//};
//Point operator+(int n, const Point& pt)   // 왼쪽 피연산자가 int 라서 멤버로는 못 만든다
//{
//	return Point(n + pt.GetX(), n + pt.GetY());
//}
//int main()
//{
//	Point p1(2, 3);
//	Point p2(3, 4);
//
//	p2 = 10 + p1; //operator+( 10, p1 );
//	p2.Print();   // 12,13
//}

//----------------------------------------------------------
// [단계 1] 멤버 operator+ 는 왼쪽이 반드시 자기 자신이어야 한다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) {}
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	Point operator+(int n)const
//	{
//		return Point(x + n, y + n);
//	}
//};
//int main()
//{
//	Point p1(2, 3);
//	Point p2(3, 4);
//
//	p2 = p1 + 10; //p1.operator+(int n);
//	p2.Print();   // 12,13
//
//	//p2 = 10 + p1;   // X - 에러 : int 에는 이런 operator+ 가 없다
//}
