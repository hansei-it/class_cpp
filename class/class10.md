# C++ 기초 강의 10 - ex10.cpp 코드를 통한 추상 클래스 설계와 인터페이스

이 강의는 **example/src/ex10.cpp** 에 담긴 예제들을 바탕으로, **추상 클래스로 도형 계층을 설계**하고 **인터페이스**와 **다중 상속**으로 "하나의 객체를 여러 관점으로 바라보는" 방법을 정리한 내용입니다.

---

## 0. 열 번째 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- 공통된 것은 부모에, 다른 것은 자식에 두는 **클래스 계층**을 설계할 수 있다.
- 부모가 **멤버 변수**를 갖고 자식이 각자의 멤버를 더하는 구조를 만들 수 있다.
- **인터페이스**(순수 가상 함수만 있는 클래스)의 개념을 이해한다.
- **다중 상속** 문법을 쓸 수 있다.
- 같은 객체를 **두 가지 형식의 배열**에 담아 다른 관점으로 다룰 수 있다.
- 왜 `struct` 로 인터페이스를 만드는지 안다.

### 0.2 이번 강의의 큰 그림

```
        Shape (추상)                IPoints (인터페이스)
      + Point start              + GetPoints() = 0
      + Draw() = 0
      + virtual ~Shape()
           ▲                              ▲
     ┌─────┼─────┐                 ┌──────┼──────┐
   Rect  Circle Polygon           Rect  Polygon  Fork
   (+w,h) (+r)  (+list)

  -> Rect 와 Polygon 은 Shape 이면서 동시에 IPoints 이다 (다중 상속)
  -> Circle 은 Shape 이지만 IPoints 는 아니다 (꼭짓점이 없으므로)
  -> Fork 는 Shape 은 아니지만 IPoints 이다 (도형이 아니지만 꼭짓점은 있다)
```

---

## 1. Shape 추상 클래스로 도형 설계하기

### 1.1 공통된 것을 찾아 부모로

세 도형(사각형, 원, 다각형)의 공통점과 차이점을 정리해 봅니다.

| | Rect | Circle | Polygon |
|---|------|--------|---------|
| **시작점** | 있다 | 있다 | 있다 |
| **그릴 수 있다** | 그린다 | 그린다 | 그린다 |
| 고유 데이터 | 너비, 높이 | 반지름 | 점 목록 |
| **그리는 방법** | **다르다** | **다르다** | **다르다** |

- **공통 데이터(시작점)** → 부모 `Shape` 의 멤버로
- **공통 동작(그리기)** 이지만 **방법이 다르다** → **순수 가상 함수**로
- **고유 데이터** → 각 자식의 멤버로

---

### 1.2 코드

```cpp
#include <iostream>
#include <vector>
using namespace std;

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

class Rect : public Shape
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
};

class Circle : public Shape
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

class Polygon : public Shape
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
};

int main()
{
	Rect r(Point(0, 0), 100, 150);
	Circle c(Point(5, 5), 12);
	Polygon pg(Point(0, 15), 5);

	r.Draw();
	c.Draw();
	pg.Draw();
}
```

**예상 출력**
```
Rect: 0,0 - w: 100, h: 150
Circle: 5,5 - r: 12
Polygon: 0,15 1,1 2,2 3,3 4,4 5,5 
```

---

### 1.3 눈여겨볼 점

**(1) 부모 멤버를 초기화 리스트로 넘긴다**

```cpp
Rect(const Point& pt = Point(), int w = 0, int h = 0)
	: Shape(pt),          // 부모의 start 를 초기화
	  width(w), height(h) // 자기 멤버
{}
```

**(2) 기본 인자에 임시 객체를 쓸 수 있다**

```cpp
Rect(const Point& pt = Point(), ...)   // Point() 는 (0,0)
```
```cpp
Rect r;                        // Point(0,0), 0, 0
Rect r2(Point(1,1), 10, 20);
```

**(3) `protected: Point start;`**

- 자식들이 `Draw()` 안에서 `start.GetX()` 로 직접 씁니다. `private` 이면 불가능합니다.

**(4) 멤버로 다른 클래스를 갖는다 (포함 관계)**

```cpp
class Shape
{
protected:
	Point start;      // Shape "은(는)" Point "를 가지고 있다" (has-a)
};
class Rect : public Shape
{                     // Rect "은(는)" Shape "이다"             (is-a)
};
```

| 관계 | 문법 | 우리말 |
|------|------|--------|
| **is-a (상속)** | `class Rect : public Shape` | 사각형은 **도형이다** |
| **has-a (포함)** | `Point start;` (멤버) | 도형은 시작점을 **가지고 있다** |

> **설계 요령** : "A 는 B 이다" 가 자연스러우면 상속, "A 는 B 를 가지고 있다" 가 자연스러우면 멤버로 두세요.

**(5) `vector<Point> list;`**

- `Polygon` 은 점을 여러 개 가지므로 `vector` 를 씁니다.
- `for (const auto& pt : list)` — 원소가 클래스이므로 **`const auto&`** 로 받았습니다. ([class9.md](class9.md))

---

## 2. 인터페이스(interface)

### 2.1 "꼭짓점을 셀 수 있다" 는 능력만 따로 떼어내기

```cpp
struct IPoints    // 인터페이스 : 순수 가상 함수만 있다 (struct 라서 기본 public)
{
	virtual int GetPoints()const = 0;
};
```

| 특징 | 설명 |
|------|------|
| 멤버 변수 | **없다** |
| 함수 | **순수 가상 함수만** |
| 생성자 | 보통 없다 |
| `struct` 를 쓰는 이유 | 어차피 전부 public 이므로 `public:` 을 안 써도 된다 |
| 이름 관례 | 앞에 `I` 를 붙인다 (`IPoints`, `IDrawable`, `IComparable`) |

**인터페이스가 뜻하는 것**

> **"이 클래스는 `GetPoints()` 를 할 수 있다는 것만 보장한다."**
>
> 어떻게 세는지, 어떤 데이터를 갖는지는 **전혀 관여하지 않습니다.**

---

### 2.2 다중 상속

```cpp
class Rect : public Shape, public IPoints   // 다중 상속
{
	int width;
	int height;
public:
	Rect(const Point& pt = Point(), int w = 0, int h = 0) :Shape(pt), width(w), height(h) {}
	void Draw()const { ... }
	int GetPoints()const { return 4; }       // IPoints 구현
};
```

```cpp
class 자식 : public 부모1, public 부모2, ...
```

- 쉼표로 여러 부모를 나열합니다.
- **각 부모의 순수 가상 함수를 모두 구현**해야 객체를 만들 수 있습니다.

---

### 2.3 상속 계보가 달라도 같은 배열에 담긴다

```cpp
class Circle : public Shape          // IPoints 는 상속하지 않는다 (꼭짓점이 없으므로)
{ ... };

class Polygon : public Shape, public IPoints
{
	...
	int GetPoints()const { return (int)list.size(); }
};

class Fork : public IPoints    // Shape 와는 아무 관계가 없지만 IPoints 는 구현한다
{
public:
	int GetPoints()const { return 5; }
};

int main()
{
	Rect r(Point(0, 0), 100, 150);
	Circle c(Point(5, 5), 12);
	Polygon pg(Point(0, 15), 5);
	Fork f;

	IPoints* points[3] = { &r, &pg, &f };   // Circle 은 IPoints 가 아니라서 못 넣는다

	for (auto x : points)
		cout << "point count : " << x->GetPoints() << endl;
}
```

**예상 출력**
```
point count : 4
point count : 6
point count : 5
```

> ### 여기가 이번 강의의 핵심입니다
>
> `Rect`, `Polygon`, `Fork` 는 **공통 부모가 `IPoints` 뿐**입니다.
> - `Rect` 와 `Polygon` 은 도형이지만 `Fork`(포크)는 도형이 아닙니다.
> - 그런데도 **"꼭짓점을 셀 수 있다"** 는 공통점 하나로 같은 배열에 담을 수 있습니다.
>
> 상속을 **"분류 체계"** 로만 쓰면 이런 것을 표현할 수 없습니다.
> 인터페이스는 **"능력(할 수 있는 것)"** 을 표현하는 도구입니다.

---

## 3. 하나의 객체를 두 관점으로 보기

```cpp
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
```

**예상 출력**
```
Rect: 0,0 - w: 100, h: 150
Circle: 5,5 - r: 12
Polygon: 0,15 1,1 2,2 3,3 4,4 5,5 

point count : 4
point count : 6
point count : 5
```

**같은 객체, 다른 관점**

```
                실제 객체 (Rect)
              ┌──────────────────┐
Shape*  ────▶ │ [Shape 부분]     │
              │   start          │
              ├──────────────────┤
IPoints* ───▶ │ [IPoints 부분]   │
              ├──────────────────┤
              │ width, height    │
              └──────────────────┘
```

- 하나의 객체 안에 **`Shape` 부분과 `IPoints` 부분이 함께** 들어 있습니다.
- 어느 포인터로 보느냐에 따라 **보이는 함수가 달라집니다.**

---

### 3.1 왜 `(Rect*)` 캐스팅이 필요한가

```cpp
IPoints* points[3] = { (Rect*)sarr[0], (Polygon*)sarr[2], &f };
//                     ~~~~~~~~
```

- `sarr[0]` 의 형식은 **`Shape*`** 입니다.
- `Shape` 는 `IPoints` 와 **아무 관계가 없으므로** 컴파일러가 자동으로 변환할 수 없습니다.
- 그래서 먼저 **실제 형식인 `Rect*` 로 되돌린 뒤**, `Rect` → `IPoints` 로 자동 업캐스팅되게 한 것입니다.

```
Shape*  --(Rect*) 로 다운캐스팅)-->  Rect*  --(자동 업캐스팅)-->  IPoints*
```

> **주의** : 이 C 스타일 캐스팅은 **검사를 전혀 하지 않습니다.**
> `sarr[0]` 이 사실 `Circle` 이었다면 프로그램이 이상하게 동작합니다.
> 안전한 방법은 **`dynamic_cast`** 이며, **[class15.md](class15.md)** 에서 배웁니다.
>
> ```cpp
> IPoints* ip = dynamic_cast<IPoints*>(sarr[0]);   // 안 되면 nullptr
> if (ip != nullptr) cout << ip->GetPoints();
> ```

---

### 3.2 delete 를 잊으면

```cpp
for (auto shape : sarr)
	delete shape;            // <- 이것을 빼먹으면 메모리 누수
```

- `new` 로 만든 것은 **반드시 `delete`** 해야 합니다.
- `Shape` 에 **가상 소멸자**가 있으므로 `~Rect()`, `~Circle()`, `~Polygon()` 이 제대로 호출됩니다. ([class9.md](class9.md))
- 이 귀찮고 위험한 일을 없애 주는 것이 **스마트 포인터**입니다. ([class16.md](class16.md))

---

## 4. 다중 상속의 주의점

### 4.1 죽음의 다이아몬드 (diamond problem)

```
      Base
     ╱     ╲
  Left     Right        <- 둘 다 Base 를 상속
     ╲     ╱
      Derived           <- Base 부분이 두 개가 된다!
```

```cpp
class Base { public: int data; };
class Left : public Base {};
class Right : public Base {};
class Derived : public Left, public Right {};

Derived d;
d.data = 10;        // X - 에러 : Left::data 인가 Right::data 인가?
d.Left::data = 10;  // 이렇게 명시해야 한다
```

**해결책** : 가상 상속 (`class Left : virtual public Base`)

> **실무 규칙**
> **"구현이 있는 클래스는 하나만 상속하고, 나머지는 인터페이스로 상속한다."**
>
> ```cpp
> class Rect : public Shape,      // 구현이 있는 부모 : 하나만
>              public IPoints,    // 인터페이스 : 여러 개 가능
>              public ISerializable
> ```
>
> Java, C# 은 아예 이것을 **언어 차원에서 강제**합니다 (클래스는 하나만 상속, 인터페이스는 여러 개).
> C++ 는 자유롭지만, **같은 규칙을 지키는 것이 안전합니다.**

---

## 5. 설계 정리 - 언제 무엇을 쓰나

| 상황 | 무엇을 쓰나 | 예 |
|------|-------------|-----|
| 공통 **데이터 + 구현**이 있다 | **추상 클래스** | `Shape` (start 를 가짐) |
| 공통 **능력(약속)** 만 있다 | **인터페이스** | `IPoints` |
| 공통점이 전혀 없다 | 상속하지 않는다 | `Fork` 와 `Shape` |
| "A 는 B 이다" | 상속 | `Rect` 는 `Shape` 이다 |
| "A 는 B 를 가진다" | 멤버 변수 | `Shape` 는 `Point` 를 가진다 |

---

## 6. 완성 코드 (ex10.cpp 맨 위)

전체 코드는 `example/src/ex10.cpp` 를 참고하세요. 구조만 다시 정리하면 다음과 같습니다.

```cpp
struct IPoints                       // 인터페이스
{
	virtual int GetPoints()const = 0;
};

class Point { int x, y; ... };       // 값 클래스

class Shape                          // 추상 클래스
{
protected:
	Point start;
public:
	Shape(const Point& pt) :start(pt) {}
	virtual void Draw()const = 0;
	virtual ~Shape() {}
};

class Rect    : public Shape, public IPoints { int width, height; ... };
class Circle  : public Shape                { int radius; ... };
class Polygon : public Shape, public IPoints { vector<Point> list; ... };
class Fork    : public IPoints              { ... };
```

**예상 출력**
```
Rect: 0,0 - w: 100, h: 150
Circle: 5,5 - r: 12
Polygon: 0,15 1,1 2,2 3,3 4,4 5,5 

point count : 4
point count : 6
point count : 5
```

---

## 7. 정리 표

| 주제 | 내용 |
|------|------|
| 추상 클래스 | 순수 가상 함수가 있는 클래스. **멤버 변수를 가질 수 있다** |
| 인터페이스 | **순수 가상 함수만** 있는 클래스. 멤버 변수 없음 |
| `struct` 로 만드는 이유 | 기본이 public 이라 `public:` 을 안 써도 된다 |
| 이름 관례 | 인터페이스는 앞에 `I` (`IPoints`) |
| 다중 상속 | `class A : public B, public C` |
| 다중 상속 규칙 | **모든 부모의 순수 가상 함수를 구현**해야 한다 |
| is-a 관계 | 상속 (`Rect` 는 `Shape` 이다) |
| has-a 관계 | 멤버 변수 (`Shape` 는 `Point` 를 가진다) |
| 다른 관점으로 보기 | 같은 객체를 `Shape*` 배열과 `IPoints*` 배열에 함께 담는다 |
| 형제 관계 캐스팅 | `Shape*` → `IPoints*` 는 **자동 변환 안 됨** (실제 형식을 거쳐야) |
| 안전한 캐스팅 | `dynamic_cast` ([class15.md](class15.md)) |
| 다이아몬드 문제 | 두 부모가 같은 조상을 가지면 조상 부분이 두 개 |
| 실무 규칙 | **구현 클래스는 하나만, 인터페이스는 여러 개** |

---

## 8. 연습 문제 (스스로 풀어보기)

1. `Shape` 에 `virtual double Area() const = 0;` 를 추가하고 `Rect`, `Circle` 에서 구현하세요.
2. `Triangle` 클래스를 추가하고 `sarr` 배열에 넣어 보세요. **`main` 의 for 문을 고쳐야 하나요?**
3. `IPoints` 외에 `struct IResizable { virtual void Resize(double f) = 0; };` 를 만들고 `Rect` 에 다중 상속시켜 보세요.
4. `Circle` 을 `IPoints* points[]` 배열에 넣어 보고 어떤 에러가 나는지 확인하세요.
5. `Shape` 에서 `virtual ~Shape() {}` 를 지우고, `Polygon` 소멸자에 메시지를 넣은 뒤 `delete` 해 보세요. `vector<Point> list` 는 어떻게 되나요?
6. `(Rect*)sarr[0]` 대신 `(Rect*)sarr[1]` (실제로는 `Circle`)을 넣어 보고 어떤 값이 출력되는지 확인하세요. 왜 위험한가요?
7. `dynamic_cast<IPoints*>(sarr[i])` 로 바꾸고, `nullptr` 검사를 넣어 배열 전체를 안전하게 순회해 보세요.
8. 다이아몬드 상속을 직접 만들어 `d.data` 가 왜 모호한지 확인하고, `virtual public` 을 붙여 해결해 보세요.

---

이 강의 자료는 **example/src/ex10.cpp** 에 있는 모든 예제를 바탕으로 구성했습니다. 이번 시간의 핵심은 **"같은 객체를 여러 관점으로 바라볼 수 있다"** 는 것입니다. 상속을 단순히 "코드 재사용" 으로만 보지 말고, **"어떤 능력을 가진 것들의 집합"** 을 만드는 도구로 생각해 보세요. 여기까지가 객체 지향의 기본기이며, 다음 시간부터는 C++ 고유의 문법들로 넘어갑니다.
