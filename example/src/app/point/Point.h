//===========================================================
// app/point/Point.h - 클래스 "선언" 을 담는 헤더 파일
//   ex5 에서 배운 선언/정의 분리를 실제 파일 단위로 옮긴 예제입니다.
//   #pragma once : 이 헤더가 여러 번 include 되어도 한 번만 처리된다.
//===========================================================
#pragma once

//Point 클래스 선언
namespace MySP
{
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
}
