# C++ 기초 강의 5 - ex5.cpp 코드를 통한 접근자·const·this·파일 분리

이 강의는 **example/src/ex5.cpp** 와 **example/src/app/point/** 의 예제들을 바탕으로, **Get/Set 접근자**, **const 멤버 함수**, **멤버 초기화 리스트**, **`this` 포인터**, 그리고 **헤더와 소스의 분리**를 정리한 내용입니다.

---

## 0. 다섯 번째 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- Get / Set 접근자로 private 멤버를 안전하게 읽고 쓸 수 있다.
- **const 멤버 함수**가 왜 필요한지 설명하고 직접 붙일 수 있다.
- `const 객체` 와 `const 참조 매개변수`가 const 멤버 함수만 부를 수 있는 이유를 안다.
- **멤버 초기화 리스트**(`:x(x), y(y)`)와 생성자 몸통 대입의 차이를 안다.
- `this` 포인터의 정체를 이해한다.
- 클래스 **선언과 정의를 분리**하고, 나아가 **헤더 파일(.h)과 소스 파일(.cpp)** 로 나눌 수 있다.

### 0.2 수업 진행 순서 제안

1. 단계 1 : Get / Set 접근자
2. 단계 2~4 : const 멤버 함수 (에러 → 해결)
3. 단계 5~6 : 초기화 리스트, const 객체
4. 단계 7~8 : `this`
5. 단계 9 : 클래스 안에서 선언/정의 분리
6. **app/point** : 파일 단위 분리 실습

---

## 1. Get / Set 접근자

```cpp
#include <iostream>
using namespace std;
class Point
{
	int x, y;
public:
	Point(int _x = 0, int _y = 0) {
		x = _x;
		y = _y;
	}
	void Print() {
		cout << x << ", " << y << endl;
	}
	void SetX(int _x) {   // setter : 값을 쓴다
		x = _x;
	}
	void SetY(int _y) {
		y = _y;
	}
	int GetX() { return x; }   // getter : 값을 읽는다
	int GetY() { return y; }
};
int main()
{
	Point pt(2, 3);

	pt.Print();

	pt.SetX(pt.GetX() + 10);
	pt.SetY(pt.GetY() + 10);
	pt.Print();
}
```

**예상 출력**
```
2, 3
12, 13
```

| 이름 | 역할 | 관례 |
|------|------|------|
| **getter** | private 멤버 값을 **읽어서 반환** | `GetX()`, `getX()`, `x()` |
| **setter** | private 멤버 값을 **검사 후 저장** | `SetX(int)`, `setX(int)` |

**왜 그냥 public 으로 열지 않고 접근자를 쓰나?**

```cpp
void SetX(int _x) {
	if (_x < 0) _x = 0;        // 값 검사를 여기 한 곳에서만 하면 된다
	x = _x;
}
```

- 값 검사, 로그 기록, 다른 멤버와의 동기화를 **한 곳에서** 할 수 있습니다.
- 나중에 `x` 를 `double` 로 바꾸거나 계산으로 대체해도 **사용하는 쪽 코드는 안 바뀝니다.**

> 물론 **모든 멤버에 기계적으로 Get/Set 을 만드는 것은 좋은 설계가 아닙니다.**
> "이 값을 밖에서 정말 읽어야 하나 / 바꿔야 하나" 를 먼저 생각하세요.

---

## 2. const 멤버 함수 - 이 수업의 가장 중요한 개념

### 2.1 문제 상황 : const 참조로 받으면 함수를 못 부른다

```cpp
#include <iostream>
using namespace std;
class Point
{
	int x, y;
public:
	Point(int _x = 0, int _y = 0) { x = _x; y = _y; }
	void Print() {          // const 가 없다
		cout << x << ", " << y << endl;
	}
	int GetX() { return x; }
	int GetY() { return y; }
};
void PrintPoint(const Point& arg)
{
	arg.Print();   // X - 에러 : const 객체에서 비-const 함수 호출 불가
}
int main()
{
	Point pt(2, 3);
	PrintPoint(pt);
}
```

**컴파일 에러**
```
error C2662: 'void Point::Print(void)': 'this' 포인터를 'const Point'에서
'Point &'(으)로 변환할 수 없습니다.
```

**왜 에러인가?**

- `arg` 는 `const Point&` 입니다. 즉 **"이 객체를 바꾸지 않겠다"** 는 약속입니다.
- 그런데 `Print()` 에는 아무 표시가 없습니다. 컴파일러 입장에서는 **`Print()` 가 멤버를 바꿀지도 모른다**고 봅니다.
- 약속을 어길 수 있으니 **아예 호출을 막습니다.**

---

### 2.2 해결 : 함수 뒤에 `const` 붙이기

```cpp
void Print() const {   // <= const 를 붙였다
	cout << x << ", " << y << endl;
}
```

```cpp
void PrintPoint(const Point& arg)
{
	arg.Print();   // 이제 호출된다
}
```

**예상 출력**
```
2, 3
```

**`const` 의 위치에 주의하세요.**

```cpp
const int GetX();          // 반환값이 const (거의 의미 없다)
int GetX() const;          // <- 이것! "이 함수는 멤버를 바꾸지 않는다"
const int& GetX() const;   // 반환값도 const, 함수도 const
```

---

### 2.3 const 멤버 함수 안에서 멤버를 바꾸면?

```cpp
void Print() const {
	x = 100;      // X - 에러 : const 멤버 함수는 멤버를 수정할 수 없다
	cout << x << ", " << y << endl;
}
```

**컴파일 에러**
```
error C3490: 'x'은(는) const 개체를 통해 액세스되고 있으므로 수정할 수 없습니다.
```

- 컴파일러가 **약속을 지키는지 검사해 줍니다.**
- 즉 `const` 는 프로그래머끼리의 약속이 아니라 **컴파일러가 강제하는 규칙**입니다.

---

### 2.4 const 객체

```cpp
int main()
{
	Point pt(2, 3);
	pt.Print();       // O
	pt.SetX(10);      // O

	const Point cpt(4, 5);
	cpt.Print();      // O : Print() 가 const 라서 가능
	//cpt.SetX(10);   // X : SetX() 는 const 가 아니라서 에러
}
```

**정리 표**

| 객체의 종류 | 부를 수 있는 함수 |
|-------------|-------------------|
| `Point pt;` (일반) | const 함수 + 비-const 함수 (**전부**) |
| `const Point cpt;` | **const 함수만** |
| `const Point& ref` | **const 함수만** |
| `const Point* p` | **const 함수만** |

> ### 실무 규칙
> **"멤버를 바꾸지 않는 모든 멤버 함수에는 `const` 를 붙인다."**
>
> 처음에 안 붙였다가 나중에 `const Point&` 로 받는 함수를 만들면 **줄줄이 에러가 납니다.**
> 처음부터 붙이는 습관이 훨씬 편합니다. 이것을 **const 정확성(const-correctness)** 이라고 합니다.

---

## 3. 멤버 초기화 리스트

### 3.1 대입과 초기화는 다르다

```cpp
class Point
{
	int x, y;
public:
	// (A) 생성자 몸통에서 대입
	Point(int _x = 0, int _y = 0) {
		x = _x;      // 먼저 x 가 만들어지고, 그 다음 대입
		y = _y;
	}

	// (B) 멤버 초기화 리스트
	Point(int _x = 0, int _y = 0) :x(_x), y(_y) {   // 만들어질 때 바로 값이 들어간다
	}
};
```

**비교해서 이해하기**

```cpp
int a;          // 선언 후
a = 10;         // 대입     <- (A) 생성자 몸통에서 x = _x;

int b = 20;     // 선언과 동시에 초기화 <- (B) 초기화 리스트 :x(_x)
```

| | 생성자 몸통 대입 (A) | 초기화 리스트 (B) |
|---|----------------------|-------------------|
| 동작 | 기본 초기화 → 대입 (**2단계**) | 처음부터 그 값으로 (**1단계**) |
| 성능 | 클래스 멤버일 때 손해 | 유리 |
| `const` 멤버 | **불가능** | 가능 |
| 참조(`&`) 멤버 | **불가능** | 가능 |
| 부모 생성자 호출 | 불가능 | 가능 ([class8.md](class8.md)) |

> **결론** : **초기화 리스트를 기본으로 쓰세요.** 특히 멤버가 클래스(`string`, `vector` 등)일 때 차이가 큽니다.

---

### 3.2 주의 : 초기화 순서는 선언 순서를 따른다

```cpp
class Test
{
	int a;
	int b;
public:
	Test() : b(10), a(b) { }   // 위험! a 가 b 보다 먼저 초기화된다
};
```

- 초기화 리스트에 쓴 **순서와 무관하게**, **멤버가 선언된 순서대로** 초기화됩니다.
- 위 코드는 `a` 를 먼저 초기화하는데 그때 `b` 는 아직 쓰레기 값입니다.
- **선언 순서와 초기화 리스트 순서를 항상 일치**시키세요.

---

## 4. this 포인터

### 4.1 멤버 함수는 "어느 객체인지" 를 숨은 인자로 받는다

```cpp
#include <iostream>
using namespace std;
class Point
{
	int x, y;
public:
	Point(int x=0, int y=0) {
		this->x = x;      // 매개변수 이름과 멤버 이름이 같을 때 this-> 로 구분
		(*this).y = y;    // this-> 와 (*this). 은 같은 뜻
	}
	void Print() const {
		cout << this->x << ", " << this->y << endl;
	}
	const Point* GetThis() const {
		return this;      // 자기 자신의 주소
	}
};
int main()
{
	Point pt(2, 3);
	pt.Print();
	cout << &pt << ',' << pt.GetThis() << endl;    // 같은 주소

	Point pt2(4, 5);
	pt2.Print();
	cout << &pt2 << ',' << pt2.GetThis() << endl;  // pt 와는 다른 주소
}
```

**예상 출력** (주소는 실행할 때마다 달라집니다)
```
2, 3
000000B47CCFF7B0,000000B47CCFF7B0
4, 5
000000B47CCFF7D0,000000B47CCFF7D0
```

**`this` 란?**

| 항목 | 설명 |
|------|------|
| 정체 | **자기 자신을 가리키는 포인터** |
| 형식 | 비-const 함수에서는 `Point*`, const 함수에서는 `const Point*` |
| 누가 넘기나 | **컴파일러가 자동으로** (숨은 첫 번째 매개변수) |
| static 함수 | `this` 가 **없다** ([class14.md](class14.md)) |

**내부적으로 무슨 일이 일어나나**

```cpp
pt.Print();
// 컴파일러가 대략 이렇게 바꾼다
Point::Print(&pt);   // 이 &pt 가 this 가 된다
```

```cpp
void Print() const {
	cout << x << endl;        // 이 x 는 사실
	cout << this->x << endl;  // 이것과 같다
}
```

**`this` 를 명시적으로 써야 하는 경우**

1. **매개변수와 멤버 이름이 같을 때**
   ```cpp
   Point(int x, int y) { this->x = x; this->y = y; }
   ```
2. **자기 자신을 반환할 때** — 연산자 오버로딩에서 매우 자주 쓴다 ([class11.md](class11.md))
   ```cpp
   const Point& operator=(const Point& arg) { ...; return *this; }
   ```
3. **자기 대입인지 검사할 때**
   ```cpp
   if (this == &arg) return *this;
   ```

---

## 5. 선언과 정의의 분리

### 5.1 클래스 안에서 분리하기

```cpp
#include <iostream>
using namespace std;

//------------------- 클래스 선언(무엇이 있는가) -------------------
class Point
{
	int x, y;
public:
	Point(int x = 0, int y = 0);     // 선언만
	void Print() const;
	void SetX(int _x);
	void SetY(int _y);
	int GetX() const;
	int GetY() const;
	const Point* GetThis() const;
};

//------------------- 멤버 함수 정의(어떻게 동작하는가) -------------------
Point::Point(int x, int y) :x(x), y(y) {   // 클래스이름:: 을 붙여 정의
}
void Point::Print() const {
	cout << this->x << ", " << this->y << endl;
}
void Point::SetX(int _x) { x = _x; }
void Point::SetY(int _y) { y = _y; }
int Point::GetX()const { return x; }
int Point::GetY()const { return y; }
const Point* Point::GetThis()const { return this; }
```

**규칙**

| 항목 | 선언(클래스 안) | 정의(클래스 밖) |
|------|-----------------|-----------------|
| 이름 앞 | 그냥 이름 | **`클래스이름::`** 을 붙인다 |
| 기본 인자 | **여기에 쓴다** | 쓰면 **에러** |
| `const` | 쓴다 | **똑같이 써야 한다** |
| `static`, `virtual` | 쓴다 | 쓰면 **에러** |

> **자주 하는 실수** : 정의 쪽에 기본 인자를 또 쓰거나, `const` 를 빠뜨리는 것입니다.
> `const` 를 빠뜨리면 "선언한 함수를 못 찾겠다" 는 에러가 납니다.

**왜 분리하나?**

- 클래스 선언만 보면 **이 클래스가 무엇을 할 수 있는지 한눈에** 보입니다.
- 구현이 길어져도 클래스 선언이 지저분해지지 않습니다.
- 그리고 이 분리가 다음 단계인 **파일 분리**로 이어집니다.

---

## 6. 파일 분리 실습 — app/point

실제 프로젝트에서는 클래스 선언을 **헤더 파일(.h)** 에, 정의를 **소스 파일(.cpp)** 에 둡니다.

### 6.1 Point.h — 선언

```cpp
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
```

| 요소 | 설명 |
|------|------|
| `#pragma once` | 이 헤더가 여러 번 `#include` 되어도 **한 번만** 처리한다 |
| `namespace MySP` | 다른 라이브러리의 `Point` 와 이름이 겹치지 않게 ([class2.md](class2.md)) |

> **`#pragma once` 대신 쓰는 전통적 방법 (인클루드 가드)**
> ```cpp
> #ifndef POINT_H
> #define POINT_H
> ... 내용 ...
> #endif
> ```
> 요즘은 대부분의 컴파일러가 `#pragma once` 를 지원하므로 이쪽이 더 간단합니다.

---

### 6.2 Point.cpp — 정의

```cpp
#include <iostream>
#include "Point.h"
using namespace std;

//Point 멤버 함수 정의 : 이름 앞에 namespace 와 클래스 이름을 모두 붙인다
MySP::Point::Point(int x, int y) :x(x), y(y) {
}
void MySP::Point::Print() const {
	cout << this->x << ", " << this->y << endl;
}
void MySP::Point::SetX(int _x) { x = _x; }
void MySP::Point::SetY(int _y) { y = _y; }
int MySP::Point::GetX()const { return x; }
int MySP::Point::GetY()const { return y; }
const MySP::Point* MySP::Point::GetThis()const { return this; }
```

**더 편한 방법 : namespace 블록으로 감싸기**

```cpp
namespace MySP
{
	Point::Point(int x, int y) :x(x), y(y) { }   // MySP:: 를 안 써도 된다
	void Point::Print() const { ... }
	...
} //namespace MySP
```

> `#include <iostream>` 은 꺾쇠(`< >`), `#include "Point.h"` 는 큰따옴표(`" "`) 를 씁니다.
>
> | 표기 | 찾는 곳 |
> |------|---------|
> | `< >` | 컴파일러 표준 라이브러리 폴더 |
> | `" "` | **현재 파일이 있는 폴더 먼저**, 없으면 표준 폴더 |

---

### 6.3 PointMain.cpp — 사용

```cpp
#include "Point.h"

int main()
{
	MySP::Point pt(2, 3);

	pt.Print();

	pt.SetX(10);
	pt.SetY(20);
	pt.Print();
}
```

**예상 출력**
```
2, 3
10, 20
```

> **핵심** : `PointMain.cpp` 는 **`Point.cpp` 를 전혀 모릅니다.** `Point.h` 만 보고도 사용할 수 있습니다.
> 이것이 **인터페이스와 구현의 분리**이며, 큰 프로그램을 만드는 기본 원칙입니다.

---

### 6.4 빌드 방법

1. 솔루션 탐색기에서 `ex1.cpp` ~ `ex16.cpp` 를 **모두 "빌드에서 제외 : 예"** 로 둡니다.
2. `app/point` 폴더의 **`Point.cpp`, `PointMain.cpp`** 만 "빌드에서 제외 : **아니오**" 로 바꿉니다.
3. **Ctrl + F5** 로 실행합니다.

| 파일 | 컴파일 대상? | 이유 |
|------|--------------|------|
| `Point.h` | **아니오** | 헤더는 `#include` 로 소스에 붙여 넣어질 뿐이다 |
| `Point.cpp` | 예 | 멤버 함수의 실제 코드가 들어 있다 |
| `PointMain.cpp` | 예 | `main()` 이 들어 있다 |

---

## 7. 완성 코드 (ex5.cpp 맨 위)

```cpp
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
```

**예상 출력**
```
2, 3
000000B47CCFF7B0,000000B47CCFF7B0
12, 13
4, 5
4, 5
```

---

## 8. 정리 표

| 주제 | 내용 |
|------|------|
| getter / setter | private 멤버를 읽고 쓰는 public 함수. 검사를 한곳에 모을 수 있다 |
| const 멤버 함수 | `void Print() const` — "이 함수는 멤버를 안 바꾼다" |
| const 객체 | **const 멤버 함수만** 호출 가능 |
| `const 형식&` 매개변수 | 받은 객체를 const 로 취급 → const 함수만 호출 가능 |
| const 정확성 | 안 바꾸는 함수엔 **처음부터** const 를 붙인다 |
| 초기화 리스트 | `Point(int x, int y) :x(x), y(y) { }` — 1단계로 초기화 |
| 초기화 리스트 필수 | `const` 멤버, 참조 멤버, 부모 생성자 호출 |
| 초기화 순서 | 리스트 순서가 아니라 **멤버 선언 순서** |
| `this` | 자기 자신을 가리키는 숨은 포인터 |
| `this` 용도 | 이름 충돌 해결 / 자기 반환 / 자기 대입 검사 |
| 클래스 밖 정의 | `void Point::Print() const { }` — `const` 를 똑같이 |
| 기본 인자 위치 | **선언 쪽에만** 쓴다 |
| `#pragma once` | 헤더 중복 포함 방지 |
| `< >` vs `" "` | 표준 라이브러리 / 내 프로젝트 파일 |

---

## 9. 연습 문제 (스스로 풀어보기)

1. `class Rect` 를 만들고 `width`, `height` 를 private, `GetArea() const` 를 public 으로 만드세요.
2. 1번의 모든 getter 에 `const` 를 붙이고, `void PrintRect(const Rect& r)` 함수에서 호출해 보세요.
3. `const` 를 하나 빼 보고 어떤 에러 메시지가 나오는지 읽어 보세요.
4. `Rect` 의 생성자를 **초기화 리스트**로 바꾸어 보세요.
5. `class Config { const int id; public: Config(int i) : id(i) {} };` 를 만들고, 초기화 리스트 대신 몸통에서 `id = i;` 를 했을 때 어떤 에러가 나는지 확인하세요.
6. `Rect` 에 `const Rect* GetThis() const` 를 추가하고 `&obj` 와 값이 같은지 출력해 보세요.
7. `Rect` 를 `Rect.h` / `Rect.cpp` / `RectMain.cpp` 세 파일로 나누어 빌드해 보세요.
8. `Rect` 를 `MyShape` 라는 namespace 안에 넣고 `MyShape::Rect r(3,4);` 로 사용해 보세요.

---

이 강의 자료는 **example/src/ex5.cpp** 와 **example/src/app/point/** 예제를 바탕으로 구성했습니다. 이번 시간의 핵심은 단연 **const 멤버 함수**입니다. "바꾸지 않는 함수에는 const 를 붙인다" 는 습관 하나로 앞으로의 에러가 크게 줄어듭니다. 그리고 **파일 분리**는 다음 강의들의 `app/` 예제에서 계속 쓰이니 빌드 방법을 꼭 익혀 두세요.
