# C++ 기초 강의 4 - ex4.cpp 코드를 통한 클래스와 생성자·소멸자·객체의 수명

이 강의는 **example/src/ex4.cpp** 에 담긴 예제들을 바탕으로, **class** 의 정식 문법과 **생성자·소멸자**, 그리고 **객체가 언제 태어나고 언제 죽는가**를 정리한 내용입니다. 파일 아래쪽이 기초, 위로 갈수록 발전된 내용입니다.

---

## 0. 네 번째 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- `struct` 와 `class` 의 차이를 한 문장으로 설명할 수 있다.
- 생성자를 **오버로딩**하고, **기본 인자**로 하나로 합칠 수 있다.
- "생성자를 하나라도 만들면 기본 생성자가 사라진다" 는 규칙을 안다.
- 소멸자가 **생성의 역순**으로 호출되는 것을 확인할 수 있다.
- 객체의 수명 4가지(**지역 / 전역 / static / 동적**)를 구분할 수 있다.
- `new` / `delete` 가 생성자·소멸자를 호출한다는 것을 확인할 수 있다.
- `sizeof(객체)` 에 멤버 함수가 포함되지 않는 이유를 안다.

### 0.2 수업 진행 순서 제안

1. 단계 1 : `struct` → `class`
2. 단계 2 : 생성자 오버로딩 → 기본 인자
3. 단계 3~4 : 소멸자, 전역/static 객체
4. 단계 5~8 : 객체의 수명
5. 단계 9~11 : `new` / `delete` 와 객체 배열

---

## 1. struct 와 class 의 차이

### 1.1 딱 한 가지만 다르다

```cpp
#include <iostream>
using namespace std;
class Point
{
	int x, y;     // public: 을 안 썼으므로 private
public:
	void Print() {
		cout << x << ", " << y << endl;   // 초기화하지 않아 쓰레기 값이 출력된다
	}
};
int main()
{
	Point pt1;
	Point pt2(2, 3);   // X - 에러 : 생성자가 없다

	pt1.Print();
	pt2.Print();
}
```

| | `struct` | `class` |
|---|----------|---------|
| 기본 접근 지정자 | **public** | **private** |
| 기본 상속 방식 | public 상속 | private 상속 |
| 그 외 문법 | **완전히 같다** | **완전히 같다** |

**실무 관례**

| 언제 | 무엇을 쓰나 |
|------|-------------|
| 데이터만 묶어 놓은 단순한 덩어리 | `struct` |
| 데이터 + 동작 + 캡슐화가 있는 것 | `class` |
| 인터페이스 (순수 가상 함수만) | `struct` (어차피 다 public) |

> 이 프로젝트에서도 `struct IPoints`(인터페이스, [class10.md](class10.md)) 와 `struct Functor`(함수 객체, [class12.md](class12.md)) 는 struct 로, 나머지는 class 로 되어 있습니다.

---

## 2. 생성자(constructor)

### 2.1 생성자를 하나라도 만들면 기본 생성자가 사라진다

```cpp
#include <iostream>
using namespace std;
class Point
{
	int x, y;
public:
	Point(int _x, int _y) {   // 이 생성자 하나 때문에
		x = _x;
		y = _y;
	}
	void Print() {
		cout << x << ", " << y << endl;
	}
};
int main()
{
	Point pt1;         // X - 에러 : 매개변수 없는 생성자가 없다
	Point pt2(2, 3);   // O
}
```

**컴파일 에러**
```
error C2512: 'Point': 사용할 수 있는 적절한 기본 생성자가 없습니다.
```

**규칙**

| 상황 | 컴파일러의 행동 |
|------|-----------------|
| 생성자를 **하나도 안 만들었다** | **기본 생성자를 자동으로 만들어 준다** |
| 생성자를 **하나라도 만들었다** | 기본 생성자를 만들어 주지 **않는다** |

> `Point pt1;` 이 갑자기 에러가 나는 흔한 원인이 바로 이것입니다.

---

### 2.2 생성자 오버로딩

```cpp
#include <iostream>
using namespace std;
class Point
{
	int x, y;
public:
	Point() {
		x = 0;
		y = 0;
		cout << "Point()" << endl;
	}
	Point(int _x, int _y) {
		x = _x;
		y = _y;
		cout << "Point(int,int)" << endl;
	}
	void Print() {
		cout << x << ", " << y << endl;
	}
};
int main()
{
	Point pt1;         // Point()
	Point pt2(2, 3);   // Point(int,int)

	pt1.Print();
	pt2.Print();
}
```

**예상 출력**
```
Point()
Point(int,int)
0, 0
2, 3
```

- 생성자도 **일반 함수처럼 오버로딩**됩니다. ([class2.md](class2.md))

---

### 2.3 기본 인자로 하나로 합치기

```cpp
#include <iostream>
using namespace std;
class Point
{
	int x, y;
public:
	Point(int _x=0, int _y=0) {   // Point() 와 Point(int,int) 를 모두 대신한다
		x = _x;
		y = _y;
		cout << "Point(int,int)" << endl;
	}
	void Print() {
		cout << x << ", " << y << endl;
	}
};
int main()
{
	Point pt1;
	Point pt2(2, 3);

	pt1.Print();
	pt2.Print();

	cout << "pt1 : " << sizeof(pt1) << endl;   // 멤버 함수는 객체 크기에 포함되지 않는다
	cout << "pt2 : " << sizeof(pt2) << endl;
}
```

**예상 출력**
```
Point(int,int)
Point(int,int)
0, 0
2, 3
pt1 : 8
pt2 : 8
```

> **왜 sizeof 가 8 인가?**
> `int x, y;` 두 개 = 4 + 4 = **8바이트**입니다.
> **멤버 함수는 객체마다 복사되지 않고 코드 영역에 한 벌만 존재**하므로 크기에 포함되지 않습니다.
> (가상 함수가 생기면 `vptr` 때문에 8바이트가 더 붙습니다. [class9.md](class9.md))

**주의** : 기본 인자로 합치면 오버로딩과 충돌할 수 있습니다.

```cpp
Point() { }                     // 이 둘을 같이 두면
Point(int x = 0, int y = 0) { } // Point pt; 가 모호해져서 에러
```

---

## 3. 소멸자(destructor)

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
		cout << "Point(int,int)" << endl;
	}
	~Point()
	{
		cout << "~Point()" << endl;
	}
	void Print() { cout << x << ", " << y << endl; }
};
int main()
{
	Point pt1;
	Point pt2(2, 3);

	pt1.Print();
	pt2.Print();
}
```

**예상 출력**
```
Point(int,int)
Point(int,int)
0, 0
2, 3
~Point()      <- pt2 가 먼저 (생성의 역순)
~Point()      <- pt1
```

| 규칙 | 설명 |
|------|------|
| 이름 | `~클래스이름` |
| 매개변수 | **없다** (그래서 오버로딩 불가) |
| 반환형 | **없다** |
| 개수 | 클래스당 **딱 하나** |
| 호출 순서 | **생성의 역순** (나중에 만든 것이 먼저 소멸) |

---

## 4. 객체의 수명 4가지

### 4.1 지역 객체 - 블록을 벗어나면 사라진다

```cpp
int main()
{
	cout << "start main() " << endl;
	{
		Point pt1;
		Point pt2(2, 3);
		pt1.Print();
		pt2.Print();
	}                    // 여기서 pt2, pt1 소멸
	cout << "end main() " << endl;
}
```

**예상 출력**
```
start main()
Point(int,int)
Point(int,int)
0, 0
2, 3
~Point()
~Point()
end main()
```

---

### 4.2 전역 객체 - main 보다 먼저 태어난다

```cpp
Point gpt(4, 5);   // 전역 객체
int main()
{
	cout << "start main() " << endl;
	...
	cout << "end main() " << endl;
}
```

**예상 출력**
```
Point(int,int)      <- "start main()" 보다 먼저!
start main()
...
end main()
~Point()            <- 프로그램이 끝날 때
```

---

### 4.3 함수 안의 지역 객체 - 호출할 때마다 생성/소멸

```cpp
void CreatePoint()
{
	Point pt;
	pt.Print();
}
int main()
{
	CreatePoint();
	CreatePoint();
	CreatePoint();
}
```

**예상 출력**
```
Point(int,int)
0, 0
~Point()
Point(int,int)
0, 0
~Point()
Point(int,int)
0, 0
~Point()
```

---

### 4.4 static 지역 객체 - 딱 한 번만 생성된다

```cpp
void CreatePoint()
{
	static Point pt;   // 소멸자는 프로그램이 끝날 때 호출된다
	pt.Print();
}
int main()
{
	CreatePoint();
	CreatePoint();
	CreatePoint();
}
```

**예상 출력**
```
Point(int,int)     <- 딱 한 번!
0, 0
0, 0
0, 0
~Point()           <- 프로그램 종료 시
```

**static 변수 복습**

```cpp
void PrintInteger()
{
	int n = 0;
	static int sn = 0;   // 최초 1회만 초기화

	cout << ++n << ',' << ++sn << endl;
}
// 호출 결과 : 1,1 -> 1,2 -> 1,3
```

---

### 4.5 static 객체는 함수마다 따로

```cpp
void CreatePoint1() { static Point pt1; pt1.Print(); }
void CreatePoint2() { static Point pt2; pt2.Print(); }
void CreatePoint3() { static Point pt3; pt3.Print(); }
int main()
{
	CreatePoint1();   // 여기서만 pt1 생성자 호출
	CreatePoint1();
	CreatePoint1();
	CreatePoint2();   // pt2 는 여기서 처음 생성
	//CreatePoint3(); // 호출하지 않으면 pt3 는 아예 생성되지 않는다
}
```

> **핵심** : `static` 지역 객체는 **그 함수가 처음 호출될 때** 생성됩니다. 한 번도 호출하지 않으면 **아예 만들어지지 않습니다.**

---

### 4.6 객체 수명 정리

| 종류 | 선언 위치 | 생성 시점 | 소멸 시점 |
|------|-----------|-----------|-----------|
| **지역 객체** | 함수/블록 안 | 선언문을 만날 때 | 블록을 벗어날 때 |
| **전역 객체** | 함수 밖 | `main()` 시작 전 | `main()` 끝난 후 |
| **static 지역** | 함수 안 + `static` | 그 줄을 **처음** 만날 때 | 프로그램 종료 시 |
| **동적 객체** | `new` | `new` 를 실행할 때 | **`delete` 를 부를 때** |

> 동적 객체만 **직접 `delete` 를 불러야** 소멸합니다. 이것을 잊는 것이 **메모리 누수**의 원인이고, 그래서 **[class16.md](class16.md)** 에서 스마트 포인터를 배웁니다.

---

## 5. new / delete 는 생성자·소멸자까지 불러 준다

### 5.1 객체 하나

```cpp
int main()
{
	Point* p = new Point; //(Point*) malloc(sizeof(Point)); <- 이러면 생성자가 안 불린다

	p->Print();

	delete p; // free(p); <- 이러면 소멸자가 안 불린다
	cout << "end main() " << endl;
}
```

**예상 출력**
```
Point(int,int)
0, 0
~Point()
end main()
```

> ### malloc / free 를 C++ 클래스에 쓰면 안 되는 이유
>
> | | `malloc` / `free` | `new` / `delete` |
> |---|-------------------|------------------|
> | 메모리 확보 | O | O |
> | **생성자 호출** | **X** | **O** |
> | **소멸자 호출** | **X** | **O** |
>
> `malloc` 으로 객체를 만들면 **멤버가 초기화되지 않은 쓰레기 상태**가 되고,
> `free` 로 지우면 **소멸자가 정리해야 할 자원이 그대로 남습니다.**

---

### 5.2 객체 배열

```cpp
int main()
{
	Point* pa = new Point[5];   // 생성자가 5번 호출된다

	for (int i = 0; i < 5; ++i)
		pa[i].Print();

	delete [] pa;               // 소멸자가 5번 호출된다
}
```

**예상 출력**
```
Point(int,int)   <- 5번
Point(int,int)
Point(int,int)
Point(int,int)
Point(int,int)
0, 0             <- 5번
...
~Point()         <- 5번
...
```

> **`new Point[5]` 는 기본 생성자를 부릅니다.**
> 따라서 클래스에 **매개변수 없이 호출 가능한 생성자**(기본 생성자 또는 모든 매개변수에 기본값이 있는 생성자)가 없으면 **컴파일 에러**가 납니다.

---

## 6. 완성 코드 (ex4.cpp 맨 위)

```cpp
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
```

**예상 출력**
```
--- 지역 객체 ---
Point(int,int)
Point(int,int)
0, 0
2, 3
sizeof(pt2) : 8
~Point()
~Point()
--- 동적 객체 배열 ---
Point(int,int)
Point(int,int)
Point(int,int)
Point(int,int)
Point(int,int)
1, 1
2, 2
3, 3
4, 4
5, 5
~Point()
~Point()
~Point()
~Point()
~Point()
end main()
```

---

## 7. 정리 표

| 주제 | 내용 |
|------|------|
| struct vs class | **기본 접근 지정자만** 다르다 (struct=public, class=private) |
| 생성자 | 이름=클래스명, 반환형 없음, 오버로딩 가능 |
| 기본 생성자 소멸 | 생성자를 **하나라도** 만들면 컴파일러가 안 만들어 준다 |
| 기본 인자 생성자 | `Point(int x=0, int y=0)` 하나로 두 경우 처리 |
| 소멸자 | `~클래스명`, 매개변수 없음, **딱 하나**, 오버로딩 불가 |
| 소멸 순서 | **생성의 역순** |
| `sizeof(객체)` | **멤버 변수만** 계산. 멤버 함수는 포함 안 됨 |
| 지역 객체 | 블록 진입 시 생성, 탈출 시 소멸 |
| 전역 객체 | `main()` 전에 생성, 후에 소멸 |
| static 지역 객체 | **처음 그 줄을 만날 때** 1회 생성, 프로그램 종료 시 소멸 |
| 동적 객체 | `new` 로 생성, **`delete` 를 직접 불러야** 소멸 |
| `new` vs `malloc` | `new` 만 **생성자·소멸자를 호출**한다 |
| `new T[n]` | **기본 생성자**가 필요하다 |

---

## 8. 연습 문제 (스스로 풀어보기)

1. `class Account` 를 만들고 `balance` 를 private 로, `Deposit(int)`·`Withdraw(int)`·`Print()` 를 public 으로 만드세요.
2. 1번에 생성자 `Account(int initial = 0)` 와 소멸자를 추가하고 각각에서 메시지를 출력하세요.
3. `Withdraw` 에서 잔액보다 큰 금액을 빼려 하면 "잔액 부족" 을 출력하고 아무 일도 하지 않게 만드세요.
4. `Account` 객체를 (a) 지역, (b) 전역, (c) static 지역, (d) `new` 로 각각 만들고 생성/소멸 순서를 관찰하세요.
5. `Account* pa = new Account[3];` 를 시도해 보고, 생성자에 기본값이 없으면 어떤 에러가 나는지 확인하세요.
6. `sizeof(Account)` 를 출력하고, 멤버 함수를 10개 더 추가한 뒤 다시 출력해 값이 그대로인지 확인하세요.
7. `delete[]` 대신 `delete` 를 쓰면 소멸자가 몇 번 불리는지 출력으로 확인해 보세요.

---

이 강의 자료는 **example/src/ex4.cpp** 에 있는 모든 예제를 바탕으로 구성했습니다. 이번 시간의 핵심은 **"객체는 언제 태어나고 언제 죽는가"** 입니다. 특히 **생성의 역순으로 소멸한다**는 규칙과 **동적 객체만 직접 `delete` 해야 한다**는 사실은 앞으로 계속 나오므로 꼭 기억하세요.
