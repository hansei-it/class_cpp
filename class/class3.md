# C++ 기초 강의 3 - ex3.cpp 코드를 통한 동적 메모리와 struct 에서 class 로

이 강의는 **example/src/ex3.cpp** 에 담긴 예제들을 바탕으로, C 의 `malloc/free` 를 대신하는 **`new`/`delete`** 와, C 의 구조체가 어떻게 **클래스**로 발전하는지를 정리한 내용입니다. 파일 아래쪽이 기초, 위로 갈수록 발전된 내용입니다.

---

## 0. 세 번째 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- `new` / `delete`, `new[]` / `delete[]` 를 사용할 수 있다.
- `malloc/free` 와 `new/delete` 의 결정적 차이를 설명할 수 있다.
- 구조체 멤버에 **기본값**을 줄 수 있다.
- C 스타일(구조체 + 전역 함수) 코드를 **멤버 함수**로 옮길 수 있다.
- **생성자**와 **소멸자**가 언제 호출되는지 안다.
- `private` / `public` 접근 지정자로 **캡슐화**를 시작할 수 있다.

### 0.2 이번 강의의 큰 그림

이번 시간은 **"구조체가 클래스가 되어 가는 과정"** 을 따라갑니다.

```
[1] struct Point { int x, y; };                 <- C 의 구조체
        ↓ 함수를 만들어 붙인다
[2] void InitPoint(Point* p, int x, int y);     <- C 스타일 (전역 함수 + 포인터)
        ↓ 함수를 구조체 "안"으로 넣는다
[3] struct Point { int x,y; void InitPoint(...); };
        ↓ 초기화/정리를 자동으로
[4] struct Point { Point(int,int); ~Point(); }; <- 생성자 / 소멸자
        ↓ 아무나 못 건드리게
[5] struct Point { private: int x,y; public: ... };  <- 캡슐화
        ↓ 기본이 private 인 것으로 바꾸면
[6] class Point { ... };                        <- 클래스 (class4.md)
```

---

## 1. new 와 delete

### 1.1 malloc / free 를 대신한다

```cpp
#include <iostream>
using namespace std;

int main()
{
	int* pn = new int;   // C : int* pn = (int*)malloc(sizeof(int));

	*pn = 10;
	cout << *pn << endl;

	delete pn;           // C : free(pn);
}
```

**예상 출력**
```
10
```

| 비교 | C (`malloc`/`free`) | C++ (`new`/`delete`) |
|------|---------------------|----------------------|
| 문법 | `(int*)malloc(sizeof(int))` | `new int` |
| 형변환 | **필요** (`(int*)`) | 필요 없다 |
| 크기 계산 | **직접** (`sizeof(int)`) | 자동 |
| 초기화 | 불가 (별도로 해야 함) | `new int(10)` 으로 가능 |
| 해제 | `free(p)` | `delete p` |
| **생성자/소멸자** | **호출 안 됨** | **자동 호출** |
| 필요한 헤더 | `<cstdlib>` | 없음 (언어 기능) |

> 마지막 줄이 가장 중요합니다. 클래스를 배우면 `malloc` 을 쓰면 안 되는 이유가 분명해집니다. ([class4.md](class4.md))

---

### 1.2 할당과 동시에 초기화하기

```cpp
int* pn = new int(10);    // 괄호 초기화
int* pn = new int{ 10 };  // 중괄호 초기화 (C++11)

cout << *pn << endl;      // 10
delete pn;
```

---

### 1.3 배열 - `new[]` 와 `delete[]`

```cpp
#include <iostream>
using namespace std;

int main()
{
	int* pa = new int[5]{ 10,20,30,40,50 };

	for (int i = 0; i < 5; ++i)
		cout << pa[i] << endl;

	delete [] pa; //** delete 가 아니라 delete[]
}
```

**예상 출력**
```
10
20
30
40
50
```

> ### 반드시 기억할 규칙
>
> | 할당 | 해제 |
> |------|------|
> | `new` | `delete` |
> | `new[]` | `delete[]` |
>
> **짝을 맞추지 않으면 프로그램이 이상하게 동작하거나 죽습니다.**
> 특히 `new[]` 로 만든 객체 배열을 `delete` 로 지우면 **첫 번째 객체의 소멸자만 호출**되고 나머지는 정리되지 않습니다.

---

## 2. 구조체와 동적 할당

### 2.1 멤버 기본값 (C++11)

```cpp
#include <iostream>
using namespace std;

struct Point
{
	int x = 0;   // C 에서는 불가능했던 문법
	int y = 0;
};
int main()
{
	Point pt;

	cout << pt.x << ',' << pt.y << endl;   // 0,0
}
```

- C 에서는 `Point pt;` 를 선언하면 `x`, `y` 에 **쓰레기 값**이 들어 있었습니다.
- C++11 부터는 **멤버 선언 자리에 기본값을 적을 수 있습니다.**

---

### 2.2 지역 배열 / 동적 배열

```cpp
struct Point { int x = 0; int y = 0; };

// (1) 지역 배열
Point ptArray[5];
for (int i = 0; i < 5; ++i)
	cout << ptArray[i].x << ", " << ptArray[i].y << endl;

// (2) 동적 객체 하나
Point* p = new Point;
cout << p->x << ", " << p->y << endl;
delete p;

// (3) 동적 배열
Point* pa = new Point[5];
for (int i = 0; i < 5; ++i)
	cout << pa[i].x << ", " << pa[i].y << endl;
delete [] pa;   // 배열은 반드시 delete[]
```

**예상 출력** (세 경우 모두)
```
0, 0
0, 0
0, 0
0, 0
0, 0
```

- `new Point[5]` 로 만들면 **각 원소의 기본값이 자동으로 적용**됩니다.
- 동적 배열도 `pa[i]` 로 접근할 수 있습니다. (포인터와 배열의 관계는 C 와 같습니다)

---

## 3. C 스타일 : 구조체 + 전역 함수

```cpp
#include <iostream>
using namespace std;
struct Point
{
	int x;
	int y;
};
void InitPoint(Point* p, int x, int y)
{
	p->x = x;
	p->y = y;
}
void UninitPoint(Point* p)
{
	p->x = 0;
	p->y = 0;
}
void PrintPoint(const Point* p)
{
	cout << p->x << ", " << p->y << endl;
}
int main()
{
	Point pt;

	InitPoint(&pt, 2, 3);
	PrintPoint(&pt);
	UninitPoint(&pt);
}
```

**예상 출력**
```
2, 3
```

**이 방식의 불편한 점**

| 문제 | 설명 |
|------|------|
| `&pt` 를 매번 넘겨야 한다 | 함수마다 첫 번째 인자가 항상 `Point*` |
| `p->` 를 매번 써야 한다 | 함수 안에서 `x` 대신 `p->x` |
| **묶여 있지 않다** | `InitPoint` 가 `Point` 전용인지 이름만 보고 짐작해야 한다 |
| 이름 충돌 | 다른 구조체용 `Init` 함수와 이름이 겹칠 수 있다 |
| **아무나 고칠 수 있다** | `pt.x = -9999;` 를 막을 방법이 없다 |

---

## 4. 함수를 구조체 안으로 옮기기

### 4.1 멤버 함수(member function)

```cpp
#include <iostream>
using namespace std;
struct Point
{
	int x;
	int y;

	void InitPoint(int _x, int _y)   // Point* 매개변수가 사라졌다
	{
		x = _x;                      // p-> 없이 그냥 x
		y = _y;
	}
	void UninitPoint()
	{
		x = 0;
		y = 0;
	}
	void PrintPoint()
	{
		cout << x << ", " << y << endl;
	}
};
int main()
{
	Point pt;

	pt.InitPoint(2, 3);   // InitPoint(&pt, 2, 3); 대신
	pt.PrintPoint();
	pt.UninitPoint();
}
```

**예상 출력**
```
2, 3
```

**무엇이 달라졌나**

| C 스타일 | C++ 멤버 함수 |
|----------|---------------|
| `InitPoint(&pt, 2, 3);` | `pt.InitPoint(2, 3);` |
| 함수 안에서 `p->x` | 함수 안에서 `x` |
| 함수와 구조체가 따로 논다 | **함수가 구조체 안에 들어 있다** |

> **어떻게 `Point*` 없이 `x` 를 알 수 있나?**
> 사실 컴파일러가 **숨은 매개변수 `this`** 를 몰래 넘겨 줍니다.
> `x = _x;` 는 실제로는 `this->x = _x;` 입니다. `this` 는 **[class5.md](class5.md)** 에서 자세히 다룹니다.

---

## 5. 생성자와 소멸자

### 5.1 자동으로 불리는 초기화·정리 함수

```cpp
#include <iostream>
using namespace std;
struct Point
{
	int x;
	int y;

	Point(int _x, int _y)   // 생성자 : 이름이 struct 이름과 같고 반환형이 없다
	{
		x = _x;
		y = _y;
		cout << "Point()" << endl;
	}
	~Point()                // 소멸자 : ~ 를 붙인다. 매개변수가 없다
	{
		x = 0;
		y = 0;
		cout << "~Point()" << endl;
	}
	void PrintPoint()
	{
		cout << x << ", " << y << endl;
	}
};
int main()
{
	{
		Point pt(2, 3);     // 여기서 생성자 호출

		pt.PrintPoint();
	}                       // 블록을 벗어나면서 소멸자 호출
	cout << "end main() " << endl;
}
```

**예상 출력**
```
Point()
2, 3
~Point()
end main()
```

| 함수 | 이름 규칙 | 반환형 | 매개변수 | 언제 호출되나 |
|------|-----------|--------|----------|---------------|
| **생성자** | 클래스 이름과 같다 | **없다** | 있어도 된다 | 객체가 만들어질 때 |
| **소멸자** | `~클래스이름` | **없다** | **없다** | 객체가 사라질 때 |

**왜 좋은가?**

- C 에서는 `InitPoint` 를 **부르는 걸 잊으면** 쓰레기 값이 남았습니다.
- C 에서는 `free` 를 **부르는 걸 잊으면** 메모리가 샜습니다.
- 생성자/소멸자는 **잊을 수가 없습니다.** 언어가 자동으로 불러 줍니다.

> 이 원리가 C++ 의 가장 강력한 특징인 **RAII** 로 이어집니다.
> "자원을 생성자에서 얻고 소멸자에서 놓는다" → **[class6.md](class6.md)**, **[class16.md](class16.md)**

---

### 5.2 중괄호 초기화 `{ 2, 3 }`

```cpp
struct Point
{
	int x;
	int y;
	void Print() { cout << x << ", " << y << endl; }
};
int main()
{
	Point pt = { 2, 3 };   // 생성자가 없을 때만 가능한 방식

	pt.Print();   // 2, 3
}
```

- 생성자를 **하나도 만들지 않았을 때**는 이렇게 중괄호로 멤버를 순서대로 채울 수 있습니다.
- 생성자를 만드는 순간 이 방식은 쓸 수 없게 됩니다.

---

## 6. 캡슐화의 시작 - private 와 public

### 6.1 아무나 고칠 수 있는 문제

```cpp
struct Point
{
	int x = 0;
	int y = 0;
	void Print() { cout << x << ", " << y << endl; }
	void SetX(int _x) { x = _x; }
};
int main()
{
	Point pt;

	pt.y = 100;    // 누구나 직접 수정 가능 -> 위험하다
	pt.y = -99999; // 이런 것도 막을 수 없다

	pt.SetX(10);
	pt.Print();
}
```

- `SetX` 라는 안전한 통로를 만들어 놨는데도 **`pt.y = ...` 로 우회**할 수 있습니다.
- 값의 범위를 검사하거나 로그를 남기고 싶어도 방법이 없습니다.

---

### 6.2 private 로 막기

```cpp
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
int main()
{
	Point pt;

	//if (pt.y >= 0)   // X - 에러 : y 는 private 이라 접근할 수 없다
	//	pt.y = 100;

	pt.SetX(10);   // 값 변경은 반드시 public 함수를 통해서
	pt.Print();
}
```

**주석을 풀면 나는 에러**
```
error C2248: 'Point::y': private 멤버('Point' 클래스에서 선언)에 액세스할 수 없습니다.
```

**접근 지정자 3가지**

| 지정자 | 클래스 안 | 자식 클래스 | 클래스 밖 | 언제 쓰나 |
|--------|-----------|-------------|-----------|-----------|
| `public` | O | O | **O** | 사용자가 써야 하는 함수 |
| `protected` | O | **O** | X | 자식에게만 물려줄 멤버 ([class8.md](class8.md)) |
| `private` | **O** | X | X | 내부 데이터, 내부 도우미 함수 |

**캡슐화(encapsulation) 란?**

- **데이터를 숨기고, 정해진 통로(public 함수)로만 접근하게 하는 것**입니다.
- 장점 :
  1. 값 검사를 한곳에서 할 수 있다 (`SetX` 안에서 범위 체크)
  2. 내부 구현을 바꿔도 사용하는 쪽 코드는 안 바뀐다
  3. 잘못된 상태가 될 수 없다

> `_mark()` 처럼 **함수도 private 로 만들 수 있습니다.** 밖에서 부를 이유가 없는 내부 도우미 함수는 private 로 두는 것이 좋습니다.
> 이름 앞에 `_` 를 붙이는 것은 "내부용" 이라는 관례적 표시입니다.

---

## 7. 완성 코드 (ex3.cpp 맨 위)

```cpp
#include <iostream>
using namespace std;

struct Point
{
private:
	int x = 0;
	int y = 0;
	char _mark() { return ','; }
public:
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

	pt.SetX(10);
	pt.Print();
	pt2.SetX(20);
	pt2.Print();
	pt3.SetX(30);
	pt3.Print();
}
```

**예상 출력**
```
10,0
20,0
30,0
```

- 객체 세 개가 **각자의 `x`, `y` 를 따로 가지고 있다**는 점을 확인하세요.
- 멤버 함수 `Print`, `SetX` 는 **코드가 한 벌만 존재**하고, 모든 객체가 공유합니다.

---

## 8. 정리 표

| 주제 | 내용 |
|------|------|
| `new` | `new int` / `new int(10)` / `new int{10}` — 형변환·크기계산 불필요 |
| `delete` | `new` 와 짝. `new[]` 는 반드시 `delete[]` |
| malloc 과의 차이 | `new/delete` 는 **생성자·소멸자를 호출**한다 |
| 멤버 기본값 | `int x = 0;` (C++11) — C 에서는 불가능 |
| 멤버 함수 | 구조체 안에 함수를 넣으면 `p->` 없이 멤버를 그냥 쓴다 |
| 생성자 | 이름 = 클래스 이름, 반환형 없음, 객체 생성 시 자동 호출 |
| 소멸자 | `~클래스이름`, 매개변수 없음, 객체 소멸 시 자동 호출 |
| 소멸 순서 | 블록을 벗어날 때 **생성의 역순**으로 |
| `private` | 클래스 안에서만 접근 가능. 데이터는 여기에 |
| `public` | 어디서나 접근 가능. 사용자용 함수는 여기에 |
| 캡슐화 | 데이터를 숨기고 함수로만 접근 → 안전 + 유지보수 용이 |
| struct vs class | 기본 접근 지정자만 다르다 (struct=public, class=private) |

---

## 9. 연습 문제 (스스로 풀어보기)

1. `int* p = new int[10];` 으로 배열을 만들고 1~10 을 채운 뒤 출력하고 `delete[]` 하세요.
2. `struct Rect { int w = 0; int h = 0; };` 를 만들고 넓이를 구하는 멤버 함수 `Area()` 를 추가하세요.
3. 2번의 `Rect` 에 생성자 `Rect(int w, int h)` 와 소멸자를 추가하고, 각각에서 메시지를 출력해 언제 불리는지 확인하세요.
4. 3번의 `Rect` 객체를 `{ }` 블록 안에 만들고, 블록 밖에 `cout << "end" << endl;` 을 두어 소멸 시점을 확인하세요.
5. `Rect` 의 `w`, `h` 를 `private` 로 바꾸고 `SetSize(int w, int h)` 를 만들되, **0 이하 값이 들어오면 1 로 바꾸는 검사**를 넣어 보세요.
6. `Rect* pa = new Rect[3];` 로 배열을 만들고 각각 크기를 지정한 뒤 넓이를 출력하고 `delete[]` 하세요.
7. `delete[]` 대신 `delete` 를 써 보고 무슨 일이 일어나는지 관찰해 보세요. (디버그 모드에서 실행)

---

이 강의 자료는 **example/src/ex3.cpp** 에 있는 모든 예제를 바탕으로 구성했습니다. 이번 시간의 핵심은 **"데이터와 그 데이터를 다루는 함수를 하나로 묶는다"** 는 생각입니다. 이것이 객체 지향 프로그래밍의 출발점이며, 다음 시간부터는 `struct` 대신 `class` 라는 이름으로 같은 이야기를 이어 갑니다.
