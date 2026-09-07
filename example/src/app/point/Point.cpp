//===========================================================
// app/point/Point.cpp - 클래스 "정의(구현)" 를 담는 소스 파일
//   헤더에는 무엇이 있는지만, 이 파일에는 어떻게 동작하는지를 적는다.
//===========================================================
#include <iostream>
#include "Point.h"
using namespace std;

//Point 멤버 함수 정의 : 이름 앞에 namespace 와 클래스 이름을 모두 붙인다
MySP::Point::Point(int x, int y) :x(x), y(y) {
}
void MySP::Point::Print() const {
	cout << this->x << ", " << this->y << endl;
}
void MySP::Point::SetX(int _x) {
	x = _x;
}
void MySP::Point::SetY(int _y) {
	y = _y;
}
int MySP::Point::GetX()const { return x; }
int MySP::Point::GetY()const { return y; }
const MySP::Point* MySP::Point::GetThis()const {
	return this;
}

//----------------------------------------------------------
// [다른 방법] namespace 블록으로 감싸면 MySP:: 를 매번 쓰지 않아도 된다
//----------------------------------------------------------
//#include <iostream>
//#include "Point.h"
//using namespace std;
//
//namespace MySP
//{
//	//Point 멤버 함수 정의
//	Point::Point(int x, int y) :x(x), y(y) {
//	}
//	void Point::Print() const {
//		cout << this->x << ", " << this->y << endl;
//	}
//	void Point::SetX(int _x) {
//		x = _x;
//	}
//	void Point::SetY(int _y) {
//		y = _y;
//	}
//	int Point::GetX()const { return x; }
//	int Point::GetY()const { return y; }
//	const Point* Point::GetThis()const {
//		return this;
//	}
//} //namespace MySP
