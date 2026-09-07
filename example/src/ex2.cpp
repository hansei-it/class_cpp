//===========================================================
// ex2.cpp - C++ 로 확장된 함수 · namespace · string · auto
//   1) 참조 매개변수(int&) 로 Swap 만들기
//   2) 함수 오버로딩(function overloading)
//   3) 매개변수 기본값(default argument)
//   4) const 참조 매개변수로 구조체를 효율적으로 전달하기
//   5) 매크로(#define) 와 inline 함수의 차이
//   6) namespace 와 using
//   7) std::string, auto, int() (기본값 생성)
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
#include <string>
using namespace std;

namespace MyName
{
	int x = 100;
	void Print(int data)
	{
		cout << "MyName::Print() : " << data << endl;
	}
}

struct Data
{
	int a = 0;   // 멤버 기본값 (C++11)
	int b = 0;
};

// 참조 매개변수 : 원본을 직접 바꾼다
void Swap(int& a, int& b)
{
	int t = a;
	a = b;
	b = t;
}

// 함수 오버로딩 : 이름은 같고 매개변수가 다르다
void Print(int a, int b, int c = 0)   // c 는 기본값 0
{
	cout << a << ',' << b << ',' << c << endl;
}
void Print(const char* s)
{
	cout << s << endl;
}

// 매크로가 아닌 inline 함수 : 매개변수가 한 번만 평가된다
inline int Max(int a, int b)
{
	return ((a) > (b)) ? (a) : (b);
}

int main()
{
	int a = 10, b = 20;
	Print(a, b);        // 10,20,0
	Swap(a, b);
	Print(a, b);        // 20,10,0
	Print("Hello!");    // 오버로딩된 다른 Print 가 호출된다

	cout << Max(++a, ++b) << endl;

	MyName::Print(MyName::x);   // namespace 로 이름 충돌을 막는다

	string s = "Hello!";
	s += "?";                   // const char* 와 달리 문자열끼리 더할 수 있다
	cout << s << ' ' << s[0] << endl;

	auto v = Data{ 10, 20 };    // auto : 컴파일러가 형식을 알아서 정한다
	cout << v.a << ',' << v.b << endl;
	cout << int() << ',' << double() << endl;   // 0, 0
}

//----------------------------------------------------------
// [단계 13] 임시 객체 Data() 와 auto
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//struct Data
//{
//	int a = 0;
//	int b = 0;
//};
//int main()
//{
//	cout << Data().a << endl;   // 임시 객체를 만들어 바로 사용
//	cout << Data().b << endl;
//	int();
//
//	auto v = Data();
//	cout << v.a << ',' << v.b << endl;
//
//	auto v2 = Data{ 10,20 };
//	cout << v2.a << ',' << v2.b << endl;
//}

//----------------------------------------------------------
// [단계 12] int() / double() 기본값과 auto, typeid
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//int main()
//{
//	cout << int() << endl;      // 0
//	cout << double() << endl;   // 0
//
//	int n = int();
//	double d = double();
//	cout << n << endl;
//	cout << d << endl;
//
//	auto v = 10L; //C언어의 auto 변수 / C++언어의 auto type
//	cout << v << endl;
//	cout << typeid(v).name() << endl;   // long
//
//	auto v2 = 5.5F;
//	cout << v2 << endl;
//	cout << typeid(v2).name() << endl;  // float
//}

//----------------------------------------------------------
// [단계 11] string 은 비교·인덱싱·수정이 자유롭다
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//using namespace std;
//
//int main()
//{
//	string s = "Hello!";
//
//	cout << s << endl;
//	if (s == "Hello!")            // strcmp 없이 == 로 비교
//		cout << "true" << endl;
//	cout << s[0] << ',' << s[1] << endl;
//
//	s = "Hi!";
//	s[0] = 'Z';
//	cout << s << endl;
//}

//----------------------------------------------------------
// [단계 10] const char* 와 std::string 의 차이
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//using namespace std;
//
//int main()
//{
//	const char* s1 = "Hello!";
//	string s2 = "Hello!";
//
//	//s1 += "?";   // X - 포인터는 문자열을 이어 붙일 수 없다
//	s2 += "?";     // O - string 은 가능
//	cout << s1 << endl;
//	cout << s2 << endl;
//}

//----------------------------------------------------------
// [단계 9] using namespace MyName; - 이름 전체를 끌어오기
//----------------------------------------------------------
//#include <iostream>
//namespace MyName
//{
//	int x = 100;
//	void Print(int data)
//	{
//		std::cout << "MyName::Print() : " << data << std::endl;
//	}
//}
//using namespace MyName;
//int main()
//{
//	std::cout << "Hello!" << std::endl;
//	std::cout << "x : " << x << std::endl;
//	Print(100);
//}

//----------------------------------------------------------
// [단계 8] using MyName::x; - 필요한 이름만 끌어오기
//----------------------------------------------------------
//#include <iostream>
//namespace MyName
//{
//	int x = 100;
//	void Print(int data)
//	{
//		std::cout << "MyName::Print() : " << data << std::endl;
//	}
//}
//using MyName::x; //1.
//using MyName::Print;
//int main()
//{
//	std::cout << "Hello!" << std::endl;
//	std::cout << "x : " << x << std::endl;
//	Print(100);
//}

//----------------------------------------------------------
// [단계 7] namespace 정의와 :: 로 접근하기
//----------------------------------------------------------
//#include <iostream>
//namespace MyName
//{
//	int x = 100;
//	void Print(int data)
//	{
//		std::cout << "MyName::Print() : " << data << std::endl;
//	}
//}
//int main()
//{
//	std::cout << "Hello!" << std::endl;
//	std::cout << "x : " << MyName::x << std::endl;
//	MyName::Print(100);
//}

//----------------------------------------------------------
// [단계 6] inline 함수 - 매크로의 부작용이 없다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//inline int Max(int a, int b)
//{
//	return (((a) > (b)) ? (a) : (b));
//}
//
//int main()
//{
//	int n1 = 10;
//	int n2 = 20;
//	cout << Max(++n1, ++n2) << endl;   // ++ 가 한 번씩만 실행된다
//	cout << Max(++n1, ++n2) << endl;
//	cout << Max(++n1, ++n2) << endl;
//	cout << n1 << ',' << n2 << endl;
//}

//----------------------------------------------------------
// [단계 5] 매크로 #define Max 의 함정 - ++ 가 두 번 실행된다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//#define Max(a,b) (( (a) > (b)) ? (a) : (b))
//
//int main()
//{
//	int n1 = 10;
//	int n2 = 20;
//	cout << Max(++n1, ++n2) << endl;
//	cout << n1 << ',' << n2 << endl;   // n2 가 두 번 증가한다!
//}

//----------------------------------------------------------
// [단계 4] const 참조 매개변수 - 복사 없이 전달하고 수정은 막는다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//struct Data
//{
//	char name[20];
//	int age;
//	char addr[50];
//};
//void Print(int data)
//{
//	cout << "int : " << data << endl;
//}
//void Print(const Data& data) //** 복사 비용이 없고, 원본 수정도 막힌다
//{
//	//data.age = 100;   // X - const 라서 수정 불가
//	cout << "name : " << data.name << ",   " << "age:" << data.age << ','
//		<< "addr : " << data.addr << endl;
//}
//int main()
//{
//	Print(10);
//	Data d = { "홍길동", 25, "경기 파주시" };
//	Print(d);
//}

//----------------------------------------------------------
// [단계 3] 구조체를 값으로 전달 - 통째로 복사된다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//struct Data
//{
//	char name[20];
//	int age;
//	char addr[50];
//};
//void Print(int data)
//{
//	cout << "int : " << data << endl;
//}
//void Print(Data data)      // 구조체 전체가 복사된다
//{
//	cout << "name : " << data.name << ",   " << "age:" << data.age << ','
//		<< "addr : " << data.addr << endl;
//}
//int main()
//{
//	Print(10);
//	Data d = { "홍길동", 25, "경기 파주시" };
//	Print(d);
//}

//----------------------------------------------------------
// [단계 2b] 임시 객체로 인자를 묶어서 넘기기
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//struct DataParam
//{
//	int a = 0;
//	int b = 0;
//	int c = 0;
//};
//void Print(DataParam d)
//{
//	cout << d.a << ',' << d.b << ',' << d.c << endl;
//}
//
//int main()
//{
//	Print(DataParam());          // 0,0,0
//	Print(DataParam{});          // 0,0,0
//	Print(DataParam{10,20,30});  // 10,20,30
//}

//----------------------------------------------------------
// [단계 2a] 매개변수 기본값(default argument)
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//void Print(int a=0, int b=0, int c=0)
//{
//	cout << a << ',' << b << ',' << c << endl;
//}
//
//int main()
//{
//	Print(10, 20, 30);   // 10,20,30
//	Print(100);          // 100,0,0
//	Print(55,66);        // 55,66,0
//}

//----------------------------------------------------------
// [단계 2] 함수 오버로딩(function overloading)
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
////function overloading(함수 중복) <==> function overriding(함수 재정의)
//void Print(int a, int b, int c)
//{
//	cout << a << ',' << b << ',' << c << endl;
//}
//void Print(const char* s)
//{
//	cout << s << endl;
//}
//int main()
//{
//	Print(10, 20, 30);
//	Print("Hello!");
//}

//----------------------------------------------------------
// [단계 1] 참조 매개변수로 만든 Swap - C 의 포인터 방식과 비교
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//void Print(int a, int b)
//{
//	cout << a << ',' << b << endl;
//}
//void Swap(int& a, int& b)   // C 에서는 void Swap(int* a, int* b)
//{
//	int t = a;
//	a = b;
//	b = t;
//}
//int main()
//{
//	int a = 10;
//	int b = 20;
//
//	Print(a, b);
//	Swap(a, b);   // C 에서는 Swap(&a, &b);
//	Print(a, b);
//}
