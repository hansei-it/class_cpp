//===========================================================
// ex4.cpp - 클래스(class) 와 생성자 · 소멸자 · 객체의 수명
//   1) struct 와 class 의 차이 (기본 접근 지정자)
//   2) 생성자 오버로딩 -> 매개변수 기본값으로 하나로 합치기
//   3) 소멸자가 불리는 시점
//   4) 객체의 수명 : 지역 / 전역 / static / 동적(new)
//   5) 객체 배열을 new 로 만들기
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
using namespace std;

class Point
{
	int x, y;                       // class 는 아무것도 안 쓰면 private
public:
	Point(int _x = 0, int _y = 0)   // 기본 인자 -> 생성자 하나로 두 경우를 모두 처리
	{
		x = _x;
		y = _y;
		cout << "Point(int,int)" << endl;
	}
	~Point()
	{
		cout << "~Point()" << endl;
	}
	void Print()
	{
		cout << x << ", " << y << endl;
	}
	void SetX(int _x) { x = _x; }
	void SetY(int _y) { y = _y; }
};

int main()
{
	cout << "--- 지역 객체 ---" << endl;
	{
		Point pt1;          // Point(0,0)
		Point pt2(2, 3);    // Point(2,3)

		pt1.Print();
		pt2.Print();
		cout << "sizeof(pt2) : " << sizeof(pt2) << endl;   // int 2개 = 8
	}   // 여기서 pt2, pt1 순서로 소멸자 호출 (생성의 역순)

	cout << "--- 동적 객체 배열 ---" << endl;
	Point* pa = new Point[5];     // 생성자가 5번 호출된다

	for (int i = 0; i < 5; ++i)
	{
		pa[i].SetX(i + 1);
		pa[i].SetY(i + 1);
	}
	for (int i = 0; i < 5; ++i)
		pa[i].Print();

	delete[] pa;                  // 소멸자가 5번 호출된다
	cout << "end main() " << endl;
}

//----------------------------------------------------------
// [단계 11] new Point[5] + SetX / SetY 로 값 채우기
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int _x = 0, int _y = 0) {
//		x = _x;
//		y = _y;
//		cout << "Point(int,int)" << endl;
//	}
//	~Point()
//	{
//		cout << "~Point()" << endl;
//	}
//	void Print() {
//		cout << x << ", " << y << endl;
//	}
//	void SetX(int _x) {
//		x = _x;
//	}
//	void SetY(int _y) {
//		y = _y;
//	}
//};
//int main()
//{
//	Point* pa = new Point[5];
//
//	for (int i = 0; i < 5; ++i)
//		pa[i].Print();
//
//	for (int i = 0; i < 5; ++i)
//	{
//		pa[i].SetX(i + 1);
//		pa[i].SetY(i + 1);
//	}
//
//	for (int i = 0; i < 5; ++i)
//		pa[i].Print();
//
//	delete[] pa;
//}

//----------------------------------------------------------
// [단계 10] 객체 배열 동적 할당 - 생성자/소멸자가 5번씩 호출된다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int _x = 0, int _y = 0) {
//		x = _x;
//		y = _y;
//		cout << "Point(int,int)" << endl;
//	}
//	~Point()
//	{
//		cout << "~Point()" << endl;
//	}
//	void Print() {
//		cout << x << ", " << y << endl;
//	}
//};
//int main()
//{
//	Point* pa = new Point[5];
//
//	for (int i = 0; i < 5; ++i)
//		pa[i].Print();
//
//	delete [] pa;
//}

//----------------------------------------------------------
// [단계 9] new / delete 는 생성자 · 소멸자까지 불러 준다 (malloc/free 와의 결정적 차이)
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int _x = 0, int _y = 0) {
//		x = _x;
//		y = _y;
//		cout << "Point(int,int)" << endl;
//	}
//	~Point()
//	{
//		cout << "~Point()" << endl;
//	}
//	void Print() {
//		cout << x << ", " << y << endl;
//	}
//};
//int main()
//{
//	Point* p = new Point; //(Point*) malloc(sizeof(Point)); <- 이러면 생성자가 안 불린다
//
//	p->Print();
//
//	delete p; // free(p); <- 이러면 소멸자가 안 불린다
//	cout << "end main() " << endl;
//}

//----------------------------------------------------------
// [단계 8] static 객체는 함수마다 따로, 그리고 딱 한 번만 생성된다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int _x = 0, int _y = 0) {
//		x = _x;
//		y = _y;
//		cout << "Point(int,int)" << endl;
//	}
//	~Point()
//	{
//		cout << "~Point()" << endl;
//	}
//	void Print() {
//		cout << x << ", " << y << endl;
//	}
//};
//void CreatePoint1()
//{
//	static Point pt1;
//	pt1.Print();
//}
//void CreatePoint2()
//{
//	static Point pt2;
//	pt2.Print();
//}
//void CreatePoint3()
//{
//	static Point pt3;
//	pt3.Print();
//}
//int main()
//{
//	CreatePoint1();   // 여기서만 pt1 생성자 호출
//	CreatePoint1();
//	CreatePoint1();
//	CreatePoint2();   // pt2 는 여기서 처음 생성
//	//CreatePoint3(); // 호출하지 않으면 pt3 는 아예 생성되지 않는다
//}

//----------------------------------------------------------
// [단계 7] static 변수 복습 - 함수를 나가도 값이 유지된다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//void PrintInteger()
//{
//	int n = 0;
//	static int sn = 0;   // 최초 1회만 초기화
//
//	cout << ++n << ',' << ++sn << endl;   // 1,1  ->  1,2  ->  1,3
//}
//int main()
//{
//	PrintInteger();
//	PrintInteger();
//	PrintInteger();
//}

//----------------------------------------------------------
// [단계 6] static 지역 객체 - 3번 호출해도 생성자는 1번
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int _x = 0, int _y = 0) {
//		x = _x;
//		y = _y;
//		cout << "Point(int,int)" << endl;
//	}
//	~Point()
//	{
//		cout << "~Point()" << endl;
//	}
//	void Print() {
//		cout << x << ", " << y << endl;
//	}
//};
//void CreatePoint()
//{
//	static Point pt;   // 소멸자는 프로그램이 끝날 때 호출된다
//	pt.Print();
//}
//int main()
//{
//	CreatePoint();
//	CreatePoint();
//	CreatePoint();
//}

//----------------------------------------------------------
// [단계 5] 함수 안의 지역 객체 - 호출할 때마다 생성/소멸이 반복된다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int _x = 0, int _y = 0) {
//		x = _x;
//		y = _y;
//		cout << "Point(int,int)" << endl;
//	}
//	~Point()
//	{
//		cout << "~Point()" << endl;
//	}
//	void Print() {
//		cout << x << ", " << y << endl;
//	}
//};
//void CreatePoint()
//{
//	Point pt;
//	pt.Print();
//}
//int main()
//{
//	CreatePoint();
//	CreatePoint();
//	CreatePoint();
//}

//----------------------------------------------------------
// [단계 4] 전역 객체 / static 객체는 main() 보다 먼저 생성된다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int _x = 0, int _y = 0) {
//		x = _x;
//		y = _y;
//		cout << "Point(int,int)" << endl;
//	}
//	~Point()
//	{
//		cout << "~Point()" << endl;
//	}
//	void Print() {
//		cout << x << ", " << y << endl;
//	}
//};
//Point gpt(4, 5);   // 전역 객체 : "start main()" 보다 먼저 생성자가 찍힌다
//int main()
//{
//	cout << "start main() " << endl;
//	{
//		Point pt1;
//		Point pt2(2, 3);
//
//		pt1.Print();
//		pt2.Print();
//
//		cout << "pt1 : " << sizeof(pt1) << endl;
//		cout << "pt2 : " << sizeof(pt2) << endl;
//	}
//	cout << "end main() " << endl;
//}

//----------------------------------------------------------
// [단계 3] 소멸자(~Point) 추가 - 블록을 벗어날 때 자동 호출
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int _x = 0, int _y = 0) {
//		x = _x;
//		y = _y;
//		cout << "Point(int,int)" << endl;
//	}
//	~Point()
//	{
//		cout << "~Point()" << endl;
//	}
//	void Print() {
//		cout << x << ", " << y << endl;
//	}
//};
//int main()
//{
//	Point pt1;
//	Point pt2(2, 3);
//
//	pt1.Print();
//	pt2.Print();
//
//	cout << "pt1 : " << sizeof(pt1) << endl;
//	cout << "pt2 : " << sizeof(pt2) << endl;
//}

//----------------------------------------------------------
// [단계 2b] 매개변수 기본값으로 생성자 2개를 하나로 합치기
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int _x=0, int _y=0) {   // Point() 와 Point(int,int) 를 모두 대신한다
//		x = _x;
//		y = _y;
//		cout << "Point(int,int)" << endl;
//	}
//	void Print() {
//		cout << x << ", " << y << endl;
//	}
//};
//int main()
//{
//	Point pt1;
//	Point pt2(2, 3);
//
//	pt1.Print();
//	pt2.Print();
//
//	cout << "pt1 : " << sizeof(pt1) << endl;   // 멤버 함수는 객체 크기에 포함되지 않는다
//	cout << "pt2 : " << sizeof(pt2) << endl;
//}

//----------------------------------------------------------
// [단계 2a] 생성자 오버로딩 - 매개변수가 다른 생성자를 여러 개 둔다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point() {
//		x = 0;
//		y = 0;
//		cout << "Point()" << endl;
//	}
//	Point(int _x, int _y) {
//		x = _x;
//		y = _y;
//		cout << "Point(int,int)" << endl;
//	}
//	void Print() {
//		cout << x << ", " << y << endl;
//	}
//};
//int main()
//{
//	Point pt1;         // Point()
//	Point pt2(2, 3);   // Point(int,int)
//
//	pt1.Print();
//	pt2.Print();
//}

//----------------------------------------------------------
// [단계 1b] 생성자를 하나라도 만들면 "기본 생성자"는 사라진다 (에러 확인용)
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int _x, int _y) {
//		x = _x;
//		y = _y;
//	}
//	void Print() {
//		cout << x << ", " << y << endl;
//	}
//};
//int main()
//{
//	Point pt1;         // X - 에러 : 매개변수 없는 생성자가 없다
//	Point pt2(2, 3);   // O
//
//	pt1.Print();
//	pt2.Print();
//}

//----------------------------------------------------------
// [단계 1] struct -> class : 기본이 private 이라 x, y 에 접근할 수 없다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;     // public: 을 안 썼으므로 private
//public:
//	void Print() {
//		cout << x << ", " << y << endl;   // 초기화하지 않아 쓰레기 값이 출력된다
//	}
//};
//int main()
//{
//	Point pt1;
//	Point pt2(2, 3);   // X - 에러 : 생성자가 없다
//
//	pt1.Print();
//	pt2.Print();
//}
