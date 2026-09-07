//===========================================================
// app/point/PointMain.cpp - Point 클래스를 사용하는 쪽
//   구현(Point.cpp)은 몰라도 되고, 헤더(Point.h)만 include 하면 쓸 수 있다.
//
// [빌드 방법]
//   1. 솔루션 탐색기에서 ex1.cpp ~ ex16.cpp 를 모두 "빌드에서 제외 : 예" 로 둔다.
//   2. app/point 폴더의 Point.cpp, PointMain.cpp 만 "빌드에서 제외 : 아니오" 로 바꾼다.
//   3. 실행 (Ctrl + F5)
//===========================================================
#include "Point.h"

int main()
{
	MySP::Point pt(2, 3);

	pt.Print();

	pt.SetX(10);
	pt.SetY(20);
	pt.Print();
}
