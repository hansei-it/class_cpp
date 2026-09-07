//===========================================================
// ex15.cpp - 예외 처리(exception) 와 형변환 · typeid
//   1) try / catch / throw 의 기본
//   2) catch(...) - 모든 예외를 받는 최후의 그물
//   3) 예외 클래스를 만들고 상속으로 계층을 이루기
//   4) std::exception 을 상속하면 what() 하나로 통일된다
//   5) catch 는 "구체적인 것부터" 먼저 써야 한다
//   6) C++ 의 형변환 4가지 : static_cast / const_cast / reinterpret_cast / dynamic_cast
//   7) typeid 로 실제 형식 확인하기
//   8) const 객체 · const 반환값
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
#include <exception>
#include <sstream>
using namespace std;

class Exception : public exception   // 모든 사용자 예외의 부모
{
public:
	Exception(const char* msg) :exception(msg) {}   // exception(const char*) 은 MSVC 확장
	virtual const char* what()const override { return "예외"; }
	virtual ~Exception() {}
};
class InputException : public Exception
{
public:
	InputException(const char* msg) :Exception(msg) {}
	const char* what()const override { return "입력 예외"; }
};
class OperationException : public Exception
{
	int index;
	int value;
	string message;
public:
	OperationException(const char* msg, int idx, int val) :
		Exception(msg), index(idx), value(val)
	{
		ostringstream oss;      // 문자열 스트림으로 메시지를 조립한다
		oss << "연산 예외 : " << "index : " << index << ',' << "value : " << value;
		message = oss.str();
	}
	const char* what()const override
	{
		return message.c_str();
	}
};
class OutputException : public Exception
{
public:
	OutputException(const char* msg) :Exception(msg) {}
	const char* what()const override { return "출력 예외"; }
};

void func1(bool exFlag)
{
	if (exFlag)
		throw InputException("입력 예외");

	cout << "1. 입력" << endl;
}
void func2(bool exFlag)
{
	if (exFlag)
		throw OperationException("연산 예외", 5, 100);

	cout << "2. 연산" << endl;
}
void func3(bool exFlag)
{
	if (exFlag)
		throw OutputException("출력 예외");

	cout << "3. 출력" << endl;
}

int main()
{
	try
	{
		func1(false);
		func2(true);     // 여기서 예외가 던져지면
		func3(true);     // 이 줄은 실행되지 않고
	}
	catch (const OperationException& ex)   // 구체적인 것부터 먼저
	{
		cout << ex.what() << endl;
		cout << "Log..." << endl;
	}
	catch (const exception& ex)            // 그 다음 부모 형식
	{
		cout << ex.what() << endl;
	}
	catch (...)                            // 마지막에 모든 예외
	{
		cout << "알 수 없는 예외" << endl;
	}

	cout << "--- 형변환 4종 ---" << endl;
	double d = 5.5;
	int n = static_cast<int>(d);                    // 1. 일반적인 형변환
	cout << n << ',' << d << endl;

	const int cn = 10;
	const int* pc = &cn;
	int* p2 = const_cast<int*>(pc);                 // 2. const 떼어내기 (위험!)
	cout << *p2 << endl;

	int m = 0x44434241;
	char* pch = reinterpret_cast<char*>(&m);        // 3. 메모리를 다른 눈으로 보기
	cout << pch[0] << ',' << pch[1] << ',' << pch[2] << ',' << pch[3] << endl;  // A,B,C,D
}

//----------------------------------------------------------
// [단계 12] dynamic_cast 참조 버전 - 실패하면 bad_cast 예외가 난다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Shape
//{
//public:
//	virtual ~Shape() {}
//	virtual void Draw()const = 0;
//};
//class Rect : public Shape
//{
//public:
//	void Draw()const { cout << "Rect" << endl; }
//};
//class Circle : public Shape
//{
//	int radius;
//public:
//	Circle(int r = 0) :radius(r) {}
//	void Draw()const { cout << "Circle" << endl; }
//	void PrintRadius()const { cout << "raidus : " << radius << endl; }
//};
//void PrintRadius(Shape& s)
//{
//	try
//	{
//		Circle& c = dynamic_cast<Circle&>(s);   // 참조는 nullptr 이 없으므로
//		c.PrintRadius();
//	}
//	catch (const exception& ex)                 // 실패를 예외로 알려 준다
//	{
//		cout << ex.what() << endl;
//	}
//}
//int main()
//{
//	Rect r1;
//	Rect r2;
//	Circle c1(10);
//
//	PrintRadius(r1);   // bad_cast
//	PrintRadius(r2);   // bad_cast
//	PrintRadius(c1);   // raidus : 10
//}

//----------------------------------------------------------
// [단계 11] dynamic_cast - 안전한 다운캐스팅 (실패하면 nullptr)
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Shape
//{
//public:
//	virtual ~Shape() {}      // 가상 함수가 하나라도 있어야 dynamic_cast 가 된다
//	virtual void Draw()const = 0;
//};
//class Rect : public Shape
//{
//public:
//	void Draw()const { cout << "Rect" << endl; }
//};
//class Circle : public Shape
//{
//	int radius;
//public:
//	Circle(int r = 0) :radius(r) {}
//	void Draw()const { cout << "Circle" << endl; }
//	void PrintRadius()const { cout << "raidus : "<< radius << endl; }
//};
//int main()
//{
//	Shape* shapeList[] = { new Rect, new Circle(10), new Rect, new Rect, new Circle(5) };
//
//	for (auto p : shapeList)
//	{
//		//Circle* pc = static_cast<Circle*>(p);   // 검사를 안 한다 -> 위험
//		Circle* pc = dynamic_cast<Circle*>(p);    // Circle 이 아니면 nullptr
//		if (pc != nullptr)
//			pc->PrintRadius();
//	}
//
//	for (auto p : shapeList)
//		delete p;
//}

//----------------------------------------------------------
// [단계 10] reinterpret_cast - 같은 메모리를 전혀 다른 형식으로 해석
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//int main()
//{
//	int n = 0x44434241;   // 'A'=0x41 'B'=0x42 'C'=0x43 'D'=0x44
//	//char* p = (char*)&n; //3.
//	char* p = reinterpret_cast<char*>(&n); //3.
//
//	cout << p[0] << ',' << p[1] << ',' << p[2] << ',' << p[3] << endl;  // A,B,C,D
//}

//----------------------------------------------------------
// [단계 9] const_cast - const 를 떼어낸다 (정말 필요할 때만!)
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//int main()
//{
//	int n = 10;
//	const int* p = &n;
//
//	cout << *p << ',' << n << endl;
//
//	//int* p2 = (int*)p;
//	int* p2 = const_cast<int*>(p); //2.
//	*p2 = 100;
//	cout << *p << ',' << n << endl;   // 100, 100
//}

//----------------------------------------------------------
// [단계 8] static_cast - C 의 (형식) 캐스팅을 대신하는 기본 도구
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Shape {};
//class Rect : public Shape {};
//class Circle : public Shape {};
//int main()
//{
//	int n = 10;
//	double d = 5.5;
//	//n = (int)d;
//	n = static_cast<int>(d); //1.
//	cout << n << ',' << d << endl;
//
//	Shape* ps = new Rect();
//	//Rect* pr = (Rect*)ps;
//	Rect* pr = static_cast<Rect*>(ps); //1. 검사 없이 내려간다
//	delete ps;
//}

//----------------------------------------------------------
// [단계 7] ostringstream 으로 예외 메시지를 조립하기
//----------------------------------------------------------
//#include <iostream>
//#include <exception>
//#include <sstream>
//using namespace std;
//
//class Exception : public exception
//{
//public:
//	Exception(const char* msg) :exception(msg) {}
//	virtual const char* what()const { return "예외"; }
//	virtual ~Exception() {}
//};
//class OperationException : public Exception
//{
//	int index;
//	int value;
//	string message;
//public:
//	OperationException(const char* msg, int idx, int val) :
//		Exception(msg),index(idx),value(val)
//	{
//		ostringstream oss;
//		oss << "연산 예외 : " << "index : " << index << ',' << "value : " << value << endl;
//		message = oss.str();
//	}
//	const char* what()const
//	{
//		return message.c_str();
//	}
//};
//void func2(bool exFlag)
//{
//	if (exFlag)
//		throw OperationException("연산 예외",5, 100);
//
//	cout << "2. 연산" << endl;
//}
//int main()
//{
//	try
//	{
//		func2(true);
//	}
//	catch (const exception& ex)
//	{
//		cout << ex.what() << endl;
//	}
//}

//----------------------------------------------------------
// [단계 6] catch 순서 - 구체적인 것부터 먼저 써야 한다
//   (부모 형식을 먼저 쓰면 자식 catch 는 영원히 실행되지 않는다)
//----------------------------------------------------------
//#include <iostream>
//#include <exception>
//using namespace std;
//
//class Exception : public exception
//{
//public:
//	virtual const char* what()const { return "예외"; }
//	virtual ~Exception() {}
//};
//class InputException : public Exception
//{
//public:
//	const char* what()const { return "입력 예외"; }
//};
//class OperationException : public Exception
//{
//public:
//	const char* what()const { return "연산 예외"; }
//};
//class OutputException : public Exception
//{
//public:
//	const char* what()const { return "출력 예외"; }
//};
//void func1(bool exFlag)
//{
//	if (exFlag) throw InputException();
//	cout << "1. 입력" << endl;
//}
//void func2(bool exFlag)
//{
//	if (exFlag) throw OperationException();
//	cout << "2. 연산" << endl;
//}
//void func3(bool exFlag)
//{
//	if (exFlag) throw OutputException();
//	cout << "3. 출력" << endl;
//}
//int main()
//{
//	try
//	{
//		func1(false);
//		func2(true);
//		func3(true);
//	}
//	catch (const OperationException& ex)   // 자식부터
//	{
//		cout << ex.what() << endl;
//		cout << "Log..." << endl;
//	}
//	catch (const exception& ex)            // 그 다음 부모
//	{
//		cout << ex.what() << endl;
//	}
//	catch (...)                            // 마지막에 전부
//	{
//		cout << "예외 : " << endl;
//	}
//}

//----------------------------------------------------------
// [단계 5] std::exception 을 상속하면 catch(const exception&) 하나로 받는다
//----------------------------------------------------------
//#include <iostream>
//#include <exception>
//using namespace std;
//
//class Exception : public exception   // <exception> 의 표준 예외를 상속
//{
//public:
//	virtual const char* what()const { return "예외"; }
//	virtual ~Exception() {}
//};
//class InputException : public Exception
//{
//public:
//	const char* what()const { return "입력 예외"; }
//};
//class OperationException : public Exception
//{
//public:
//	const char* what()const { return "연산 예외"; }
//};
//class OutputException : public Exception
//{
//public:
//	const char* what()const { return "출력 예외"; }
//};
//void func1(bool exFlag)
//{
//	if (exFlag) throw InputException();
//	cout << "1. 입력" << endl;
//}
//void func2(bool exFlag)
//{
//	if (exFlag) throw OperationException();
//	cout << "2. 연산" << endl;
//}
//void func3(bool exFlag)
//{
//	if (exFlag) throw OutputException();
//	cout << "3. 출력" << endl;
//}
//int main()
//{
//	try
//	{
//		func1(false);
//		func2(false);
//		func3(true);
//	}
//	catch (const exception& ex)   // 표준 예외까지 한 번에 받을 수 있다
//	{
//		cout << ex.what() << endl;
//	}
//	catch (...)
//	{
//		cout << "예외 : " << endl;
//	}
//}

//----------------------------------------------------------
// [단계 4] 예외 클래스 계층 - 부모 형식으로 받으면 자식 예외도 잡힌다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Exception
//{
//public:
//	virtual const char* what()const { return "예외"; }
//	virtual ~Exception() {}
//};
//class InputException : public Exception
//{
//public:
//	const char* what()const { return "입력 예외"; }
//};
//class OperationException : public Exception
//{
//public:
//	const char* what()const { return "연산 예외"; }
//};
//class OutputException : public Exception
//{
//public:
//	const char* what()const { return "출력 예외"; }
//};
//void func1(bool exFlag)
//{
//	if (exFlag) throw InputException();
//	cout << "1. 입력" << endl;
//}
//void func2(bool exFlag)
//{
//	if (exFlag) throw OperationException();
//	cout << "2. 연산" << endl;
//}
//void func3(bool exFlag)
//{
//	if (exFlag) throw OutputException();
//	cout << "3. 출력" << endl;
//}
//int main()
//{
//	try
//	{
//		func1(true);
//		func2(false);
//		func3(true);
//	}
//	catch(const Exception& ex)   // 부모 참조로 받고 what() 은 가상 함수라 자식 것이 불린다
//	{
//		cout << ex.what() << endl;
//	}
//	catch (...)
//	{
//		cout << "예외 : " << endl;
//	}
//}

//----------------------------------------------------------
// [단계 3] catch(...) 로 일단 전부 받기 - 무엇이 왔는지는 알 수 없다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Exception
//{
//public:
//	virtual const char* what()const { return "예외"; }
//	virtual ~Exception() {}
//};
//class InputException : public Exception
//{
//public:
//	const char* what()const { return "입력 예외"; }
//};
//void func1(bool exFlag)
//{
//	if (exFlag)
//		throw InputException();
//
//	cout << "1. 입력" << endl;
//}
//int main()
//{
//	try
//	{
//		func1(true);
//	}
//	catch (...)
//	{
//		cout << "예외 : " << endl;   // 어떤 예외인지 구분할 수 없다
//	}
//}

//----------------------------------------------------------
// [단계 2] throw 는 어떤 형식이든 던질 수 있다 (int, const char* ...)
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//void function(int data)
//{
//	throw 100;      // int 를 던진다
//}
//int main()
//{
//	try
//	{
//		function(1);
//	}
//	catch (int ex)  // 던진 형식과 맞는 catch 가 잡는다
//	{
//		cout << "예외 : " << ex << endl;
//	}
//}

//----------------------------------------------------------
// [단계 1] try / catch / throw 의 첫 모습
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//void function(int data)
//{
//	throw "임의의 예외!";   // 여기서 함수가 즉시 중단되고
//}
//int main()
//{
//	try {
//		function(10);       // 호출한 쪽으로 되돌아온 뒤
//	}
//	catch (...) //최상위 예외
//	{
//		cout << "예외 발생!" << endl;   // 여기로 점프한다
//	}
//}

//----------------------------------------------------------
// [단계 0b] typeid - 실제 형식이 무엇인지 물어보기
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Parent
//{
//public :
//	virtual void Print()const {}   // 가상 함수가 있어야 실제 형식을 알 수 있다
//};
//class Child1 : public Parent
//{
//};
//class Child2 : public Parent
//{
//public:
//	void Print_Child2()const
//	{
//		cout << "Child2 Method" << endl;
//	}
//};
//int main()
//{
//	Parent* parr[] = { new Child1, new Child2, new Child1, new Child2 };
//
//	for (Parent* x : parr)
//	{
//		if (typeid(Child2) == typeid(*x))     // 실제 형식이 Child2 일 때만
//			((Child2*)x)->Print_Child2();
//	}
//
//	for (Parent* x : parr)
//		delete x;
//}

//----------------------------------------------------------
// [단계 0a] const 와 포인터가 겹칠 때 - 읽는 순서는 "오른쪽에서 왼쪽으로"
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//int main()
//{
//	int n = 10;
//	int* p = &n;
//	int** pp = &p;        // 포인터를 가리키는 포인터
//
//	// const int * const * pp2
//	//   -> pp2 는 (const 인 (const int 를 가리키는 포인터)) 를 가리키는 포인터
//	const int * const  * pp2 = pp;
//
//	cout << **pp2 << endl;   // 읽기는 가능
//	//**pp2 = 20;            // X - 가리키는 int 가 const
//	//*pp2 = nullptr;        // X - 가리키는 포인터도 const
//	pp2 = nullptr;           // O - pp2 자신은 const 가 아니다
//}
//
// [읽는 요령] * 를 기준으로 오른쪽에서 왼쪽으로 읽는다
//   int* p            : p 는 int 를 가리키는 포인터
//   const int* p      : p 는 (const int) 를 가리키는 포인터   -> *p = 1;  X
//   int* const p      : p 는 (const 인) int 포인터            -> p = &n;  X
//   const int* const p: 둘 다 const                            -> 둘 다 X

//----------------------------------------------------------
// [단계 0] const 정리 - const 객체 / const 반환값이 막아 주는 것들
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class ConstData
//{
//	int arr[5] = { 1,2,3,4,5 };
//public:
//	ConstData() :arr{ 1, 2 } {}         // 나머지는 0 으로 채워진다
//	void Print()const
//	{
//		//arr[0] = 100;                 // X - const 멤버 함수는 멤버를 못 바꾼다
//		for (auto item : arr)
//			cout << item << " ";
//		cout << endl;
//	}
//	const int* GetArray()const          // const int* 로 반환 -> 밖에서 수정 불가
//	{
//		return arr;
//	}
//	const int& GetItem() const          // const int& 로 반환
//	{
//		return arr[0];
//	}
//	bool operator==(const ConstData& arg)const
//	{
//		//arg.arr[0] = 100;             // X - arg 도 const 이다
//		return arr == arg.arr;
//	}
//	void SetData(int idx, int data)     // const 가 아니므로 const 객체는 못 부른다
//	{
//		arr[idx] = data;
//	}
//};
//int main()
//{
//	const ConstData cd1;
//	cd1.Print();                        // O
//	//cd1.SetData(0, 100);              // X
//	//cd1.GetArray()[0] = 100;          // X
//	//cd1.GetItem() = 100;              // X
//
//	ConstData cd2;
//	cd2.SetData(0, 100);                // O
//	cd2.Print();
//}
