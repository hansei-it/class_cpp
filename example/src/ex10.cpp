//===========================================================
// ex10.cpp - 추상 클래스 설계와 인터페이스(interface)
//   1) Shape 추상 클래스와 Rect / Circle / Polygon
//   2) 부모가 멤버(Point start) 를 갖고, 자식이 각자의 멤버를 더한다
//   3) 인터페이스 : 순수 가상 함수만 가진 클래스 (IPoints)
//   4) 다중 상속 : class Rect : public Shape, public IPoints
//   5) 하나의 객체를 두 가지 관점(Shape / IPoints)으로 바라보기
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
#include <vector>
using namespace std;

struct IPoints    // 인터페이스 : 순수 가상 함수만 있다 (struct 라서 기본 public)
{
	virtual int GetPoints()const = 0;
};

class Point
{
	int x, y;
public:
	Point(int x = 0, int y = 0) :x(x), y(y) {	}
	int GetX()const { return x; }
	int GetY()const { return y; }
};

class Shape//추상클래스
{
protected:
	Point start;              // 모든 도형이 공통으로 갖는 시작점
public:
	Shape(const Point& pt) :start(pt) {}
	virtual void Draw()const = 0;
	virtual ~Shape() {}       // 부모 포인터로 delete 하므로 가상 소멸자 필수
};

class Rect : public Shape, public IPoints   // 다중 상속
{
	int width;
	int height;
public:
	Rect(const Point& pt = Point(), int w = 0, int h = 0) :Shape(pt), width(w), height(h) {}
	void Draw()const
	{
		cout << "Rect: " << start.GetX() << ',' << start.GetY() << " - " <<
			"w: " << width << ", " << "h: " << height << endl;
	}
	int GetPoints()const { return 4; }
};

class Circle : public Shape          // IPoints 는 상속하지 않는다 (꼭짓점이 없으므로)
{
	int radius;
public:
	Circle(const Point& pt = Point(), int r = 0) :Shape(pt), radius(r) {}
	void Draw()const
	{
		cout << "Circle: " << start.GetX() << ',' << start.GetY() << " - " <<
			"r: " << radius << endl;
	}
};

class Polygon : public Shape, public IPoints
{
	vector<Point> list;
public:
	Polygon(const Point& pt = Point(), int points = 0) :Shape(pt)
	{
		list.push_back(start);
		for (int i = 0; i < points; ++i)
			list.push_back(Point(i + 1, i + 1));
	}
	void Draw()const
	{
		cout << "Polygon: ";
		for (const auto& pt : list)
			cout << pt.GetX() << "," << pt.GetY() << " ";
		cout << endl;
	}
	int GetPoints()const { return (int)list.size(); }
};

class Fork : public IPoints    // Shape 와는 아무 관계가 없지만 IPoints 는 구현한다
{
public:
	int GetPoints()const { return 5; }
};

int main()
{
	Shape* sarr[3] = { new Rect(Point(0, 0), 100, 150),
		new Circle(Point(5, 5), 12),
		new Polygon(Point(0, 15), 5) };
	Fork f;

	for (auto shape : sarr)      // Shape 라는 관점 : "그릴 수 있는 것"
		shape->Draw();
	cout << endl;

	// 같은 객체를 IPoints 라는 다른 관점으로 모은다 : "꼭짓점을 셀 수 있는 것"
	IPoints* points[3] = { (Rect*)sarr[0], (Polygon*)sarr[2], &f };

	for (auto x : points)
		cout << "point count : " << x->GetPoints() << endl;

	for (auto shape : sarr)
		delete shape;            // 가상 소멸자 덕분에 각자의 소멸자가 호출된다
}

//----------------------------------------------------------
// [단계 3] Shape* 배열로 한꺼번에 다루고, 마지막에 delete 하기
//   * 위 완성본과 같지만 delete 를 빠뜨린 버전 -> 메모리 누수 확인용
//----------------------------------------------------------
//int main()
//{
//	Shape* sarr[3] = { new Rect(Point(0, 0), 100, 150),
//		new Circle(Point(5, 5), 12),
//		new Polygon(Point(0, 15), 5) };
//	Fork f;
//
//	for (auto shape : sarr)
//		shape->Draw();
//	cout << endl;
//
//	IPoints* points[3] = { (Rect*)sarr[0], (Polygon*)sarr[2], &f };
//
//	for (auto x : points)
//		cout << "point count : " << x->GetPoints() << endl;
//
//	// delete 를 빠뜨렸다 -> 메모리 누수!
//}

//----------------------------------------------------------
// [단계 2] IPoints 인터페이스 추가 - 상속 계보가 달라도 같은 배열에 담긴다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//struct IPoints
//{
//	virtual int GetPoints()const = 0;
//};
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) {	}
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//};
//class Shape//추상클래스
//{
//protected:
//	Point start;
//public:
//	Shape(const Point& pt) :start(pt) {}
//	virtual void Draw()const = 0;
//	virtual ~Shape() {}
//};
//class Rect : public Shape , public IPoints
//{
//	int width;
//	int height;
//public:
//	Rect(const Point& pt = Point(), int w = 0, int h = 0) :Shape(pt), width(w), height(h) {}
//	void Draw()const
//	{
//		cout << "Rect: " << start.GetX() << ',' << start.GetY() << " - " <<
//			"w: " << width << ", " << "h: " << height << endl;
//	}
//	int GetPoints()const { return 4; }
//};
//class Circle : public Shape
//{
//	int radius;
//public:
//	Circle(const Point& pt = Point(), int r = 0) :Shape(pt), radius(r) {}
//	void Draw()const
//	{
//		cout << "Circle: " << start.GetX() << ',' << start.GetY() << " - " <<
//			"r: " << radius << endl;
//	}
//};
//class Polygon : public Shape, public IPoints
//{
//	vector<Point> list;
//public:
//	Polygon(const Point& pt = Point(), int points = 0) :Shape(pt)
//	{
//		list.push_back(start);
//		for (int i = 0; i < points; ++i)
//			list.push_back(Point(i + 1, i + 1));
//	}
//	void Draw()const
//	{
//		cout << "Polygon: ";
//		for (const auto& pt : list)
//			cout << pt.GetX() << "," << pt.GetY() << " ";
//		cout << endl;
//	}
//	int GetPoints()const { return list.size(); }
//};
//class Fork : public IPoints
//{
//public:
//	int GetPoints()const { return 5; }
//};
//int main()
//{
//	Rect r(Point(0, 0), 100, 150);
//	Circle c(Point(5, 5), 12);
//	Polygon pg(Point(0, 15), 5);
//	Fork f;
//
//	IPoints* points[3] = { &r, &pg, &f };   // Circle 은 IPoints 가 아니라서 못 넣는다
//
//	for (auto x : points)
//		cout << "point count : " << x->GetPoints() << endl;
//}

//----------------------------------------------------------
// [단계 1] Shape 추상 클래스와 세 가지 도형
//   - 공통된 것(start) 은 부모에, 다른 것(width/radius/list) 은 자식에
//   - Draw() 는 도형마다 다르므로 순수 가상 함수로 둔다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) {	}
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//};
//class Shape//추상클래스
//{
//protected:
//	Point start;
//public:
//	Shape(const Point& pt) :start(pt) {}
//	virtual void Draw()const = 0;
//	virtual ~Shape() {}
//};
//class Rect : public Shape
//{
//	int width;
//	int height;
//public:
//	Rect(const Point& pt = Point(), int w = 0, int h = 0) :Shape(pt), width(w), height(h) {}
//	void Draw()const
//	{
//		cout << "Rect: " << start.GetX() << ',' << start.GetY() << " - " <<
//			"w: " << width << ", " << "h: " << height << endl;
//	}
//};
//class Circle : public Shape
//{
//	int radius;
//public:
//	Circle(const Point& pt = Point(), int r = 0) :Shape(pt), radius(r) {}
//	void Draw()const
//	{
//		cout << "Circle: " << start.GetX() << ',' << start.GetY() << " - " <<
//			"r: " << radius << endl;
//	}
//};
//class Polygon : public Shape
//{
//	vector<Point> list;
//public:
//	Polygon(const Point& pt = Point(), int points = 0) :Shape(pt)
//	{
//		list.push_back(start);
//		for (int i = 0; i < points; ++i)
//			list.push_back(Point(i + 1, i + 1));
//	}
//	void Draw()const
//	{
//		cout << "Polygon: ";
//		for (const auto& pt : list)
//			cout << pt.GetX() << "," << pt.GetY() << " ";
//		cout << endl;
//	}
//};
//int main()
//{
//	Rect r(Point(0, 0), 100, 150);
//	Circle c(Point(5, 5), 12);
//	Polygon pg(Point(0, 15), 5);
//
//	r.Draw();
//	c.Draw();
//	pg.Draw();
//}
