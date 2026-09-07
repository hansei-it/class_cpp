//===========================================================
// ex16.cpp - 스마트 포인터 · 파일 입출력 · 종합 실습(전화번호부)
//   1) 원시 포인터의 문제 : delete 를 잊으면 메모리 누수
//   2) unique_ptr - 소유자가 하나뿐인 스마트 포인터
//   3) shared_ptr - 참조 카운트로 마지막 한 명이 해제한다
//   4) make_shared , reset , swap , use_count , weak_ptr
//   5) 클래스 멤버를 shared_ptr 로 바꾸면 소멸자가 필요 없어진다
//   6) shared_ptr 을 직접 만들어 보며 원리 이해하기
//   7) ofstream 으로 텍스트 파일에 저장하기
//   8) 종합 실습 : 메뉴 기반 전화번호부
//   -> 파일로 분리한 완성판은 src/app/telphonebook 폴더를 참고하세요.
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <conio.h>      // _getch() : 엔터 없이 키 하나를 읽는다 (Windows 전용)
using namespace std;

class Person
{
	string name;
	string phone;
public:
	Person(const string& n = "", const string& p = "") :name(n), phone(p)
	{
	}
	void Print(ostream& out = cout)const     // 화면에도, 파일에도 출력할 수 있다
	{
		out << "name:" << name << ", " << "phone:" << phone << endl;
	}
	const string& GetName()const { return name; }
	const string& GetPhone()const { return phone; }
};

class TelPhoneBook
{
	vector<Person> phoneList;
public:
	void AddTel()
	{
		cout << "이름 입력: ";
		string name;
		cin >> name;
		cout << "전화 입력: ";
		string phone;
		cin >> phone;

		phoneList.push_back(Person(name, phone));
	}
	void PrintTel()const
	{
		for (auto& per : phoneList)
			per.Print();
	}
	void SearchTel()const
	{
		cout << "찾을 이름 입력:";
		string name;
		cin >> name;

		for (auto& per : phoneList)
		{
			if (per.GetName() == name)
			{
				cout << "찾은 원소 : ";
				per.Print();
				return;
			}
		}
		cout << "찾지 못했습니다." << endl;
	}
	void RemoveTel()
	{
		cout << "삭제할 이름 입력:";
		string name;
		cin >> name;

		for (vector<Person>::size_type i = 0; i < phoneList.size(); ++i)
		{
			if (phoneList[i].GetName() == name)
			{
				cout << "삭제 원소: ";
				phoneList[i].Print();
				phoneList.erase(phoneList.begin() + i);
				return;
			}
		}
		cout << "찾지 못했습니다." << endl;
	}
	void WriteTextTel()const
	{
		ofstream fout("phonebook.txt");      // 파일을 연다 (소멸자가 알아서 닫는다)
		for (vector<Person>::size_type i = 0; i < phoneList.size(); ++i)
		{
			fout << "[" << i << "]: ";
			phoneList[i].Print(fout);        // cout 대신 fout 을 넘긴다
		}
		cout << "phonebook.txt 에 저장했습니다." << endl;
	}
};

class Util
{
public:
	static void Menu()
	{
		cout << "1. 전화 등록" << endl;
		cout << "2. 전화 번호 출력" << endl;
		cout << "3. 전화 번호 검색" << endl;
		cout << "4. 전화 번호 삭제" << endl;
		cout << "5. 전화 번호 텍스트 파일 출력" << endl;
		cout << "0. 프로그램 종료" << endl;
		cout << endl;
	}
};

int main()
{
	TelPhoneBook tpb;
	bool run = true;

	while (run)
	{
		Util::Menu();
		switch (_getch())
		{
		case '1': tpb.AddTel();        break;
		case '2': tpb.PrintTel();      break;
		case '3': tpb.SearchTel();     break;
		case '4': tpb.RemoveTel();     break;
		case '5': tpb.WriteTextTel();  break;
		case '0': run = false;         break;
		default:                       break;
		}
	}
}

//----------------------------------------------------------
// [단계 11] 메뉴 없이 함수만 순서대로 호출해 보기 (완성 직전 단계)
//----------------------------------------------------------
//int main()
//{
//	TelPhoneBook tpb;
//
//	tpb.AddTel();
//	tpb.AddTel();
//	tpb.AddTel();
//	tpb.PrintTel();
//	tpb.SearchTel();
//	tpb.RemoveTel();
//	tpb.WriteTextTel();
//	tpb.PrintTel();
//}

//----------------------------------------------------------
// [단계 10] Person 을 char* 로 직접 관리하던 버전
//   - string 을 쓰면 복사 생성자 / 소멸자 / operator= 를 만들 필요가 없다
//   - 아래 코드는 그것을 전부 손으로 만든 모습이다 (3의 법칙 복습)
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <fstream>
//using namespace std;
//
//class Person
//{
//	char* name;
//	char* phone;
//private:
//	void _copy(const char* n, const char* p)
//	{
//		name = new char[strlen(n) + 1];
//		phone = new char[strlen(p) + 1];
//		strcpy_s(name, strlen(n) + 1, n);
//		strcpy_s(phone, strlen(p) + 1, p);
//	}
//public:
//	Person(const char* n = "", const char* p = "")
//	{
//		_copy(n, p);
//	}
//	Person(const Person& arg)          // 복사 생성자
//	{
//		_copy(arg.name, arg.phone);
//	}
//	~Person() {                        // 소멸자
//		delete[] name;
//		delete[] phone;
//	}
//	void Print(ostream& out = cout)const
//	{
//		out << "name:" << name << ", " << "phone:" << phone << endl;
//	}
//	const char* GetName()const { return name; }
//	const char* GetPhone()const { return phone; }
//	const Person& operator=(const Person& arg)   // 복사 대입
//	{
//		if (this == &arg)
//			return *this;
//
//		delete[]name;
//		delete[]phone;
//		_copy(arg.name, arg.phone);
//
//		return *this;
//	}
//};
//// TelPhoneBook 안에서는 문자열 비교를 strcmp 로 해야 한다
////   if (strcmp(per.GetName(), name) == 0)   <- string 이면 == 로 끝난다

//----------------------------------------------------------
// [단계 9] ofstream - 텍스트 파일에 쓰기
//----------------------------------------------------------
//#include <iostream>
//#include <fstream>
//using namespace std;
//int main()
//{
//	ofstream fout("test.txt");        // 파일 열기 (없으면 만든다)
//	int arr[10] = { 10,20,30,40,50 };
//
//	for (int i = 0; i < 10; ++i)
//		fout << "[" << i << "]: " << arr[i] << endl;   // cout 쓰듯이 쓴다
//
//	fout.close();                     // 생략해도 소멸자가 닫아 준다
//}

//----------------------------------------------------------
// [단계 8] TelPhoneBook 설계부터 시작하기 - 무엇이 필요한지 먼저 적는다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class TelPhoneBook
//{
//public:
//	void AddTel();
//	void PrintTel()const;
//	void SearchTel()const;
//	void RemoveTel();
//	void WriteTextTel()const;
//};
//int main()
//{
//	TelPhoneBook tpb;
//
//	tpb.AddTel();       //전화 번호를 등록
//	tpb.PrintTel();     //전화 번호 출력
//	tpb.SearchTel();    //전화 검색(이름 or 전화 번호)
//	tpb.RemoveTel();    //전화 삭제(이름 or 전화 번호)
//	tpb.WriteTextTel(); //텍스트 파일의 등록 전화번호 출력
//}

//----------------------------------------------------------
// [단계 7] shared_ptr 을 직접 만들어 보기 - 참조 카운트의 원리
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	explicit Point(int x = 0, int y = 0) :x(x), y(y) {
//		cout << "Point()" << endl;
//	}
//	Point(const Point& arg) :x(arg.x), y(arg.y) {
//		cout << "copy Point()" << endl;
//	}
//	~Point() { cout << "~Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//};
//template <typename T>
//class Shared_ptr
//{
//	T* ptr;
//	int* pCount;      // 몇 명이 이 객체를 가리키는지 세는 카운터 (공유한다!)
//public:
//	Shared_ptr(T* p = nullptr) :ptr(p), pCount(nullptr) {
//		if (ptr != nullptr)
//			pCount = new int(1);
//	}
//	Shared_ptr(const Shared_ptr& arg) :ptr(arg.ptr), pCount(arg.pCount) {
//		++* pCount;                    // 복사되면 한 명 늘어난다
//	}
//	~Shared_ptr() {
//		reset();
//	}
//	int use_count()const
//	{
//		if (ptr)
//			return *pCount;
//		else
//			return 0;
//	}
//	void reset()
//	{
//		if (ptr == nullptr)
//			return;
//
//		--* pCount;                    // 한 명 줄이고
//		if (*pCount == 0)              // 마지막 한 명이었으면
//		{
//			delete pCount;             // 그때 진짜로 해제한다
//			delete ptr;
//			pCount = nullptr;
//			ptr = nullptr;
//		}
//	}
//	void reset(T* p)
//	{
//		reset();
//
//		ptr = p;
//		pCount = new int(1);
//	}
//	T* operator->()      // p->Print() 를 가능하게
//	{
//		return ptr;
//	}
//	T& operator*()       // (*p).Print() 를 가능하게
//	{
//		return *ptr;
//	}
//	const Shared_ptr& operator=(const Shared_ptr& arg)
//	{
//		if (this == &arg)
//			return *this;
//
//		reset();                       // 내가 들고 있던 것을 먼저 놓고
//		ptr = arg.ptr;
//		pCount = arg.pCount;
//		++* pCount;                    // 새 것을 같이 든다
//
//		return *this;
//	}
//};
//int main()
//{
//	Shared_ptr<Point> p1(new Point(1, 2));
//	Shared_ptr<Point> p2(new Point(3, 4));
//
//	p1 = p2;              // (1,2) 는 여기서 해제된다
//	p1->Print();
//	(*p1).Print();
//	p2->Print();
//	cout << p1.use_count() << endl;   // 2
//	p1.reset(new Point(5, 5));
//	p1->Print();
//	cout << p1.use_count() << endl;   // 1
//}

//----------------------------------------------------------
// [단계 6] 멤버를 shared_ptr 로 바꾸면 소멸자를 안 써도 된다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <memory>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	explicit Point(int x = 0, int y = 0) :x(x), y(y) {
//		cout << "Point()" << endl;
//	}
//	Point(const Point& arg) :x(arg.x), y(arg.y) {
//		cout << "copy Point()" << endl;
//	}
//	~Point() { cout << "~Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//};
//
//class Rect
//{
//	shared_ptr<Point> ploc;     // Point* 대신 shared_ptr<Point>
//	int width;
//	int height;
//public:
//	Rect(int x, int y, int w, int h) :ploc(nullptr), width(w), height(h)
//	{
//		ploc = shared_ptr<Point>(new Point(x, y));
//	}
//	Rect(const Rect& arg) :ploc(nullptr), width(arg.width), height(arg.height)
//	{
//		ploc = shared_ptr<Point>(new Point(*arg.ploc));   // 깊은 복사는 그대로 필요
//	}
//	// ~Rect() 가 없다! shared_ptr 이 알아서 해제한다
//	const Rect& operator=(const Rect& arg)
//	{
//		if (this == &arg)
//			return *this;
//		ploc.reset(new Point(*arg.ploc));   // delete 를 직접 쓰지 않는다
//		width = arg.width;
//		height = arg.height;
//
//		return *this;
//	}
//	void Print()const
//	{
//		cout << "Rect:" << ploc->GetX() << ',' << ploc->GetY() << " - " <<
//			"w:" << width << ',' << "h:" << height << endl;
//	}
//};
//int main()
//{
//	Rect r1(0, 0, 45, 60);
//	r1.Print();
//
//	Rect r2 = r1;
//	r2.Print();
//
//	r2 = r1;
//	r2.Print();
//}

//----------------------------------------------------------
// [단계 5] 원시 포인터를 멤버로 두면 소멸자 · 복사생성자 · operator= 가 전부 필요하다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	explicit Point(int x = 0, int y = 0) :x(x), y(y){
//		cout << "Point()" << endl;
//	}
//	Point(const Point& arg) :x(arg.x), y(arg.y){
//		cout << "copy Point()" << endl;
//	}
//	~Point() { cout << "~Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//};
//
//class Rect
//{
//	Point* ploc;
//	int width;
//	int height;
//public:
//	Rect(int x, int y, int w, int h) :ploc(nullptr), width(w), height(h)
//	{
//		ploc = new Point(x, y);
//	}
//	Rect(const Rect& arg) :ploc(nullptr), width(arg.width), height(arg.height)
//	{
//		ploc = new Point(*arg.ploc);
//	}
//	~Rect() { delete ploc; }              // 직접 해제해야 한다
//	const Rect& operator=(const Rect& arg)
//	{
//		if (this == &arg)
//			return *this;
//		delete ploc;
//
//		ploc = new Point(*arg.ploc);
//		width = arg.width;
//		height = arg.height;
//
//		return *this;
//	}
//	void Print()const
//	{
//		cout << "Rect:" << ploc->GetX() << ',' << ploc->GetY() << " - " <<
//			"w:" << width << ',' << "h:" << height << endl;
//	}
//};
//int main()
//{
//	Rect r1(0, 0, 45, 60);
//	r1.Print();
//
//	Rect r2 = r1;
//	r2.Print();
//
//	r2 = r1;
//	r2.Print();
//}

//----------------------------------------------------------
// [단계 4b] vector 에 담기 : Point* 직접 관리 vs shared_ptr
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <memory>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	explicit Point(int x = 0, int y = 0) :x(x), y(y) { cout << "Point()" << endl; }
//	Point(const Point& arg) :x(arg.x), y(arg.y) { cout << "copy Point()" << endl; }
//	~Point() { cout << "~Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//};
//int main()
//{
//	// (1) 원시 포인터 : erase 하기 전에 delete 를 직접 해야 한다
//	//vector<Point*> v;
//	//v.push_back(new Point(1, 1));
//	//delete* v.begin();
//	//v.erase(v.begin());
//	//for (auto p : v) delete p;
//
//	// (2) shared_ptr : erase 만 하면 알아서 해제된다
//	vector< shared_ptr<Point> > v;
//	v.push_back(make_shared<Point>(1, 1));
//	v.push_back(make_shared<Point>(2, 2));
//	v.push_back(make_shared<Point>(3, 3));
//
//	for (size_t i = 0; i < v.size(); ++i)
//		v[i]->Print();
//
//	v.erase(v.begin());     // 여기서 (1,1) 의 소멸자가 호출된다
//	cout << endl;
//
//	for (size_t i = 0; i < v.size(); ++i)
//		v[i]->Print();
//}

//----------------------------------------------------------
// [단계 4] weak_ptr - 참조 카운트를 올리지 않고 들여다보기
//----------------------------------------------------------
//#include <iostream>
//#include <memory>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	explicit Point(int x = 0, int y = 0) :x(x), y(y) { cout << "Point()" << endl; }
//	Point(const Point& arg) :x(arg.x), y(arg.y) { cout << "copy Point()" << endl; }
//	~Point() { cout << "~Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//};
//int main()
//{
//	shared_ptr<Point> p1(new Point(2, 3));
//	shared_ptr<Point> p2 = p1;
//	shared_ptr<Point> p3 = p2;
//
//	weak_ptr<Point> w1 = p1;    // 카운트를 올리지 않는다
//	weak_ptr<Point> w2 = w1;
//
//	cout << p1.use_count() << endl;   // 3
//	cout << w1.use_count() << endl;   // 3 (보기만 한다)
//
//	p1->Print();
//	w1.lock()->Print();               // lock() 으로 잠시 shared_ptr 을 얻어 쓴다
//}

//----------------------------------------------------------
// [단계 3] shared_ptr 의 도구들 : make_shared , reset , swap , use_count
//----------------------------------------------------------
//#include <iostream>
//#include <memory>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	explicit Point(int x = 0, int y = 0) :x(x), y(y) { cout << "Point()" << endl; }
//	Point(const Point& arg) :x(arg.x), y(arg.y) { cout << "copy Point()" << endl; }
//	~Point() { cout << "~Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//};
//int main()
//{
//	shared_ptr<Point> p1 = make_shared<Point>(2, 3);   // new 를 쓰지 않는 권장 방식
//	p1->Print();
//
//	shared_ptr<Point> p2(new Point(4, 5));
//	p2->Print();
//
//	p2.reset(new Point(1, 1));    // (4,5) 해제 후 새것으로 교체
//	p2->Print();
//
//	cout << endl;
//	p1.swap(p2);                  // 서로 바꿔 든다
//	p1->Print();
//	p2->Print();
//}

//----------------------------------------------------------
// [단계 2b] reset() 을 하나씩 부르며 참조 카운트가 줄어드는 것 보기
//----------------------------------------------------------
//#include <iostream>
//#include <memory>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	explicit Point(int x = 0, int y = 0) :x(x), y(y) { cout << "Point()" << endl; }
//	~Point() { cout << "~Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//};
//int main()
//{
//	shared_ptr<Point> p1(new Point(2, 3));
//	p1->Print();
//
//	shared_ptr<Point> p2 = p1;
//	cout << p1.use_count() << endl;   // 2
//
//	shared_ptr<Point> p3 = p1;
//	cout << p1.use_count() << endl;   // 3
//
//	p1.reset();                       // 3 -> 2 (아직 해제 안 됨)
//	p2.reset();                       // 2 -> 1 (아직 해제 안 됨)
//	cout << p2.use_count() << endl;   // 0 (p2 는 이제 아무것도 안 들고 있다)
//	p3.reset();                       // 1 -> 0 : 여기서 ~Point() 호출!
//	cout << p3.use_count() << endl;   // 0
//	cout << "end main()" << endl;
//}

//----------------------------------------------------------
// [단계 2] shared_ptr - 여러 개가 하나의 객체를 함께 소유한다
//----------------------------------------------------------
//#include <iostream>
//#include <memory>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	explicit Point(int x = 0, int y = 0) :x(x), y(y) { cout << "Point()" << endl; }
//	Point(const Point& arg) :x(arg.x), y(arg.y) { cout << "copy Point()" << endl; }
//	~Point() { cout << "~Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//};
//int main()
//{
//	shared_ptr<Point> p4;
//	{
//		shared_ptr<Point> p1(new Point(2, 3));
//		shared_ptr<Point> p2 = p1;    // 복사가 된다 (unique_ptr 과 다른 점)
//		shared_ptr<Point> p3 = p1;
//
//		cout << p1.use_count() << endl;   // 3
//		p1->Print();
//		p2->Print();
//		p3->Print();
//		p4 = p3;                          // 블록 밖의 p4 가 하나 더 든다
//	}
//	cout << "블록을 벗어남 - 아직 소멸자가 안 불렸다" << endl;
//	p4->Print();
//}   // 여기서 마지막 p4 가 사라지며 ~Point() 호출

//----------------------------------------------------------
// [단계 1b] unique_ptr - 소유자가 딱 하나뿐이다
//----------------------------------------------------------
//#include <iostream>
//#include <memory>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	explicit Point(int x = 0, int y = 0) :x(x), y(y) { cout << "Point()" << endl; }
//	~Point() { cout << "~Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//};
//int main()
//{
//	//스마트포인터(unique_ptr/shared_ptr - weak_ptr)
//	unique_ptr<int> p( new int(10) );
//	unique_ptr<Point> p1( new Point(2, 3) );
//
//	cout << *p << endl;
//	p1->Print();       // -> 로 원래 포인터처럼 쓴다
//	(*p1).Print();     // * 로도 쓸 수 있다
//
//	//unique_ptr<Point> p2 = p1;   // X - 복사 불가 (소유자는 한 명뿐)
//}                                // delete 를 쓰지 않아도 여기서 해제된다

//----------------------------------------------------------
// [단계 1] 원시 포인터 - delete 를 직접 써야 하고, 잊으면 메모리 누수
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	explicit Point(int x = 0, int y = 0) :x(x), y(y) { cout << "Point()" << endl; }
//	~Point() { cout << "~Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//};
//int main()
//{
//	int* p = new int(10);
//	Point* p1 = new Point(2, 3);
//
//	cout << *p << endl;
//	p1->Print();
//
//	delete p;      // 이 두 줄을 빠뜨리면 메모리 누수
//	delete p1;     // 중간에 예외가 나면 아예 실행되지도 않는다!
//}
