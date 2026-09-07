//===========================================================
// ex5.cpp - 접근자 · const · this · 선언과 정의의 분리
//   1) Get / Set 접근자로 private 멤버 다루기
//   2) const 멤버 함수 - "이 함수는 멤버를 바꾸지 않습니다" 라는 약속
//   3) const 객체 / const 참조 매개변수와 const 멤버 함수의 관계
//   4) 멤버 초기화 리스트  : x(x), y(y)
//   5) this 포인터
//   6) 클래스 선언과 멤버 함수 정의를 분리하기 (Point::)
//   -> 실제 파일 분리 예제는 src/app/point 폴더를 참고하세요.
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
using namespace std;

//------------------- 클래스 선언(무엇이 있는가) -------------------
class Point
{
	int x, y;
public:
	Point(int x = 0, int y = 0);
	void Print() const;
	void SetX(int _x);
	void SetY(int _y);
	int GetX() const;
	int GetY() const;
	const Point* GetThis() const;
};

//------------------- 멤버 함수 정의(어떻게 동작하는가) -------------------
Point::Point(int x, int y) :x(x), y(y) {   // 멤버 초기화 리스트
}
void Point::Print() const {
	cout << this->x << ", " << this->y << endl;
}
void Point::SetX(int _x) {
	x = _x;
}
void Point::SetY(int _y) {
	y = _y;
}
int Point::GetX()const { return x; }
int Point::GetY()const { return y; }
const Point* Point::GetThis()const {
	return this;      // 자기 자신의 주소
}

void PrintPoint(const Point& arg)   // const 참조 -> const 멤버 함수만 호출 가능
{
	arg.Print();
}

int main()
{
	Point pt(2, 3);
	pt.Print();
	cout << &pt << ',' << pt.GetThis() << endl;   // 두 주소가 같다

	pt.SetX(pt.GetX() + 10);
	pt.SetY(pt.GetY() + 10);
	pt.Print();

	const Point cpt(4, 5);   // const 객체
	cpt.Print();             // Print() 가 const 라서 호출 가능
	PrintPoint(cpt);
}

//----------------------------------------------------------
// [단계 9] 클래스 선언과 정의를 분리하기 - 헤더/소스 분리의 준비 단계
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0);   // 선언만
//	void Print() const;
//	void SetX(int _x);
//	void SetY(int _y);
//	int GetX()const;
//	int GetY()const;
//	const Point* GetThis()const;
//};
//Point::Point(int x, int y):x(x),y(y) {   // 클래스이름:: 을 붙여 정의
//}
//void Point::Print() const {
//	cout << this->x << ", " << this->y << endl;
//}
//void Point::SetX(int _x) {
//	x = _x;
//}
//void Point::SetY(int _y) {
//	y = _y;
//}
//int Point::GetX()const { return x; }
//int Point::GetY()const { return y; }
//const Point* Point::GetThis()const {
//	return this;
//}
//
//int main()
//{
//	Point pt(2, 3);
//	pt.Print();
//	cout << &pt << ',' << pt.GetThis() << endl;
//
//	Point pt2(4, 5);
//	pt2.Print();
//	cout << &pt2 << ',' << pt2.GetThis() << endl;
//}

//----------------------------------------------------------
// [단계 8] this 를 반환해서 객체마다 주소가 다름을 확인하기
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) {
//		this->x = x;      // 매개변수 이름과 멤버 이름이 같을 때 this-> 로 구분
//		(*this).y = y;    // this-> 와 (*this). 은 같은 뜻
//	}
//	void Print() const {
//		cout << this->x << ", " << this->y << endl;
//	}
//	void SetX(int _x) {
//		x = _x;
//	}
//	void SetY(int _y) {
//		y = _y;
//	}
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	const Point* GetThis()const {
//		return this;
//	}
//};
//int main()
//{
//	Point pt(2, 3);
//	pt.Print();
//	cout << &pt << ',' << pt.GetThis() << endl;    // 같은 주소
//
//	Point pt2(4, 5);
//	pt2.Print();
//	cout << &pt2 << ',' << pt2.GetThis() << endl;  // pt 와는 다른 주소
//}

//----------------------------------------------------------
// [단계 7] this 포인터 - 멤버 함수는 "어느 객체인지"를 숨은 인자로 받는다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int x=0, int y=0) {
//		this->x = x;
//		(*this).y = y;
//	}
//	void Print() const {
//		cout << this->x << ", " << this->y << endl;
//	}
//	void SetX(int _x) {
//		x = _x;
//	}
//	void SetY(int _y) {
//		y = _y;
//	}
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//};
//int main()
//{
//	Point pt(2, 3);
//	pt.Print();      //thiscall : 내부적으로 Print(&pt) 처럼 동작한다
//
//	Point pt2(4, 5);
//	pt2.Print();
//}

//----------------------------------------------------------
// [단계 6] const 객체는 const 멤버 함수만 호출할 수 있다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int x, int y):x(x),y(y) {
//	}
//	void Print() const {
//		cout << x << ", " << y << endl;
//	}
//	void SetX(int _x) {
//		x = _x;
//	}
//	void SetY(int _y) {
//		y = _y;
//	}
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//};
//int main()
//{
//	Point pt(2, 3);
//	pt.Print();
//
//	const Point cpt(4, 5);
//	cpt.Print();      // O : Print() 가 const 라서 가능
//	//cpt.SetX(10);   // X : SetX() 는 const 가 아니라서 에러
//}

//----------------------------------------------------------
// [단계 5] 멤버 초기화 리스트 - 대입이 아니라 "초기화" 이다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//class Point
//{
//	int x, y;
//public:
//	Point(int _x = 0, int _y = 0) :x(_x), y(_y) {   // 여기서 바로 초기화된다
//	}
//	void Print() const {
//		cout << x << ", " << y << endl;
//	}
//	void SetX(int _x) {
//		x = _x;
//	}
//	void SetY(int _y) {
//		y = _y;
//	}
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//};
//int main()
//{
//	Point pt(2, 3);
//	pt.Print();
//
//	const Point cpt(4, 5);
//	cpt.Print();
//
//	// 비교 : 아래 두 줄의 차이와 같다
//	int a;          // 선언 후
//	a = 10;         // 대입     <- 생성자 몸통에서 x = _x;
//
//	int b = 20;     // 선언과 동시에 초기화 <- 초기화 리스트 :x(_x)
//}

//----------------------------------------------------------
// [단계 4] const 멤버 함수 안에서 멤버를 바꾸면 에러 (에러 확인용)
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
//	}
//	void Print() const {
//		x = 100;      // X - 에러 : const 멤버 함수는 멤버를 수정할 수 없다
//		cout << x << ", " << y << endl;
//	}
//	void SetX(int _x) {
//		x = _x;
//	}
//	void SetY(int _y) {
//		y = _y;
//	}
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//};
//void PrintPoint(const Point& arg)
//{
//	arg.Print();
//}
//int main()
//{
//	Point pt(2, 3);
//
//	PrintPoint(pt);
//}

//----------------------------------------------------------
// [단계 3] Print() 에 const 를 붙여서 해결
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
//	}
//	void Print() const {   // <= const 를 붙였다
//		cout << x << ", " << y << endl;
//	}
//	void SetX(int _x) {
//		x = _x;
//	}
//	void SetY(int _y) {
//		y = _y;
//	}
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//};
//void PrintPoint(const Point& arg)
//{
//	arg.Print();   // 이제 호출된다
//}
//int main()
//{
//	Point pt(2, 3);
//
//	PrintPoint(pt);
//}

//----------------------------------------------------------
// [단계 2] const 참조 매개변수로 받으면 일반 멤버 함수를 못 부른다 (에러 확인용)
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
//	}
//	void Print() {          // const 가 없다
//		cout << x << ", " << y << endl;
//	}
//	void SetX(int _x) {
//		x = _x;
//	}
//	void SetY(int _y) {
//		y = _y;
//	}
//	int GetX() { return x; }
//	int GetY() { return y; }
//};
//void PrintPoint(const Point& arg)
//{
//	arg.Print();   // X - 에러 : const 객체에서 비-const 함수 호출 불가
//}
//int main()
//{
//	Point pt(2, 3);
//
//	PrintPoint(pt);
//}

//----------------------------------------------------------
// [단계 1] Get / Set 접근자 - private 멤버를 안전하게 읽고 쓰기
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
//	}
//	void Print() {
//		cout << x << ", " << y << endl;
//	}
//	void SetX(int _x) {   // setter : 값을 쓴다
//		x = _x;
//	}
//	void SetY(int _y) {
//		y = _y;
//	}
//	int GetX() { return x; }   // getter : 값을 읽는다
//	int GetY() { return y; }
//};
//int main()
//{
//	Point pt(2, 3);
//
//	pt.Print();
//
//	pt.SetX(pt.GetX() + 10);
//	pt.SetY(pt.GetY() + 10);
//	pt.Print();
//}
