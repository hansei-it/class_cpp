//===========================================================
// ex3.cpp - 동적 메모리(new/delete) 와 struct 에서 class 로
//   1) new / delete, new[] / delete[]  (C 의 malloc/free 대체)
//   2) struct 멤버 기본값과 객체 배열
//   3) C 스타일 : struct + 전역 함수  ->  C++ : struct 안에 멤버 함수
//   4) 생성자(constructor) 와 소멸자(destructor)
//   5) 접근 지정자 private / public - 캡슐화의 시작
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
using namespace std;

struct Point
{
private:                          // 밖에서 접근 불가
	int x = 0;
	int y = 0;
	char _mark() { return ','; }  // 내부에서만 쓰는 도우미 함수
public:                           // 밖에서 접근 가능
	void Print()
	{
		cout << x << _mark() << y << endl;
	}
	void SetX(int _x)
	{
		x = _x;
	}
};
///////////////////////
int main()
{
	Point pt;
	Point pt2;
	Point pt3;

	//if (pt.y >= 0)   // X - 에러 : y 는 private 이라 접근할 수 없다
	//	pt.y = 100;

	pt.SetX(10);   // 값 변경은 반드시 public 함수를 통해서
	pt.Print();
	pt2.SetX(20);
	pt2.Print();
	pt3.SetX(30);
	pt3.Print();
}

//----------------------------------------------------------
// [단계 13] private 을 붙이면 외부 접근이 막힌다 (에러 확인용)
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//struct Point
//{
//private:
//	int x = 0;
//	int y = 0;
//	char _mark() { return ','; }
//public:
//	void Print()
//	{
//		cout << x << _mark() << y << endl;
//	}
//	void SetX(int _x)
//	{
//		x = _x;
//	}
//};
///////////////////////////
//int main()
//{
//	Point pt;
//	Point pt2;
//	Point pt3;
//
//	if( pt.y >= 0 )    // X - private 멤버라 컴파일 에러
//	   pt.y = 100;
//
//	pt.SetX(10);
//	pt.Print();
//	pt2.SetX(20);
//	pt2.Print();
//	pt3.SetX(30);
//	pt3.Print();
//}

//----------------------------------------------------------
// [단계 12] 아직 private 이 없으면 밖에서 마음대로 바꿀 수 있다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//struct Point
//{
//	int x = 0;
//	int y = 0;
//	void Print()
//	{
//		cout << x << ", " << y << endl;
//	}
//	void SetX(int _x)
//	{
//		x = _x;
//	}
//};
///////////////////////////
//int main()
//{
//	Point pt;
//	Point pt2;
//	Point pt3;
//
//	pt.y = 100;   // 누구나 직접 수정 가능 -> 위험하다
//
//	pt.SetX(10);
//	pt.Print();
//	pt2.SetX(20);
//	pt2.Print();
//	pt3.SetX(30);
//	pt3.Print();
//}

//----------------------------------------------------------
// [단계 11] SetX() 로 값 바꾸기 - 객체마다 값이 따로 저장된다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//struct Point
//{
//	int x = 0;
//	int y = 0;
//	void Print()
//	{
//		cout << x << ", " << y << endl;
//	}
//	void SetX(int _x)
//	{
//		x = _x;
//	}
//};
//int main()
//{
//	Point pt;
//	Point pt2;
//	Point pt3;
//
//	pt.SetX(10);
//	pt.Print();
//	pt2.SetX(20);
//	pt2.Print();
//	pt3.SetX(30);
//	pt3.Print();
//}

//----------------------------------------------------------
// [단계 10] 객체를 여러 개 만들어 보기
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//struct Point
//{
//	int x = 0;
//	int y = 0;
//	void Print()
//	{
//		cout << x << ", " << y << endl;
//	}
//};
//int main()
//{
//	Point pt;
//	Point pt2;
//	Point pt3;
//
//	pt.Print();
//	pt2.Print();
//	pt3.Print();
//}

//----------------------------------------------------------
// [단계 9] 멤버 기본값(int x = 0) - 선언만 해도 0 으로 초기화된다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//struct Point
//{
//	int x=0;
//	int y=0;
//	void Print()
//	{
//		cout << x << ", " << y << endl;
//	}
//};
//int main()
//{
//	Point pt;
//
//	pt.Print();   // 0, 0
//}

//----------------------------------------------------------
// [단계 8] 중괄호 초기화 { 2, 3 } 과 멤버 함수
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//struct Point
//{
//	int x;
//	int y;
//	void Print()
//	{
//		cout << x << ", " << y << endl;
//	}
//};
//int main()
//{
//	Point pt = { 2, 3 };
//
//	pt.Print();
//}

//----------------------------------------------------------
// [단계 7] 생성자와 소멸자 - 객체가 태어날 때 / 사라질 때 자동 호출
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//struct Point
//{
//	int x;
//	int y;
//
//	Point(int _x, int _y)   // 생성자 : 이름이 struct 이름과 같고 반환형이 없다
//	{
//		x = _x;
//		y = _y;
//		cout << "Point()" << endl;
//	}
//	~Point()                // 소멸자 : ~ 를 붙인다. 매개변수가 없다
//	{
//		x = 0;
//		y = 0;
//		cout << "~Point()" << endl;
//	}
//	void PrintPoint()
//	{
//		cout << x << ", " << y << endl;
//	}
//};
//int main()
//{
//	{
//		Point pt(2, 3);     // 여기서 생성자 호출
//
//		pt.PrintPoint();
//	}                       // 블록을 벗어나면서 소멸자 호출
//	cout << "end main() " << endl;
//}

//----------------------------------------------------------
// [단계 6] 초기화/정리 함수를 멤버 함수로 옮기기 - this 없이 x, y 를 그냥 쓴다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//struct Point
//{
//	int x;
//	int y;
//
//	void InitPoint(int _x, int _y)
//	{
//		x = _x;
//		y = _y;
//	}
//	void UninitPoint()
//	{
//		x = 0;
//		y = 0;
//	}
//	void PrintPoint()
//	{
//		cout << x << ", " << y << endl;
//	}
//};
//int main()
//{
//	Point pt;
//
//	pt.InitPoint(2, 3);
//	pt.PrintPoint();
//	pt.UninitPoint();
//}

//----------------------------------------------------------
// [단계 5] C 스타일 : struct + 전역 함수 (Point* 를 계속 넘겨야 한다)
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//struct Point
//{
//	int x;
//	int y;
//};
//void InitPoint(Point* p, int x, int y)
//{
//	p->x = x;
//	p->y = y;
//}
//void UninitPoint(Point* p)
//{
//	p->x = 0;
//	p->y = 0;
//}
//void PrintPoint(const Point* p)
//{
//	cout << p->x << ", " << p->y << endl;
//}
//int main()
//{
//	Point pt;
//
//	InitPoint(&pt, 2, 3);
//	PrintPoint(&pt);
//	UninitPoint(&pt);
//}

//----------------------------------------------------------
// [단계 4] 구조체 배열을 동적으로 할당하기 - new[] / delete[]
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//struct Point
//{
//	int x = 0;
//	int y = 0;
//};
//int main()
//{
//	Point* p = new Point[5];
//
//	for(int i = 0 ; i < 5 ; ++i)
//		cout << p[i].x << ", " << p[i].y << endl;
//
//	delete [] p;   // 배열은 반드시 delete[] 로 해제
//}

//----------------------------------------------------------
// [단계 3] 구조체 하나를 동적으로 할당하기 - new Point / delete
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//struct Point
//{
//	int x = 0;
//	int y = 0;
//};
//int main()
//{
//	Point* p = new Point;
//
//	cout << p->x << ", " << p->y << endl;
//
//	delete p;
//}

//----------------------------------------------------------
// [단계 2c] 지역 구조체 배열
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//struct Point
//{
//	int x = 0;
//	int y = 0;
//};
//int main()
//{
//	Point ptArray[5];
//
//	for (int i = 0; i < 5; ++i)
//		cout << ptArray[i].x << ", " << ptArray[i].y << endl;
//}

//----------------------------------------------------------
// [단계 2b] 배열 동적 할당 + 초기화 목록
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//int main()
//{
//	int* pa = new int[5]{ 10,20,30,40,50 };
//
//	for (int i = 0; i < 5; ++i)
//		cout << pa[i] << endl;
//
//	delete [] pa; //** delete 가 아니라 delete[]
//}

//----------------------------------------------------------
// [단계 2a] 할당과 동시에 초기화하기
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//int main()
//{
//	//int* pn = new int(10);
//	int* pn = new int{ 10 };
//
//	cout << *pn << endl;
//
//	delete pn;
//}

//----------------------------------------------------------
// [단계 1] new / delete - C 의 malloc / free 를 대신한다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//int main()
//{
//	int* pn = new int;   // C : int* pn = (int*)malloc(sizeof(int));
//
//	*pn = 10;
//	cout << *pn << endl;
//
//	delete pn;           // C : free(pn);
//}
