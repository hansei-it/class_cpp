# C++ 기초 강의 11 - ex11.cpp 코드를 통한 연산자 오버로딩

이 강의는 **example/src/ex11.cpp** 에 담긴 예제들을 바탕으로, 내가 만든 클래스에 **`+`, `==`, `+=`, `++`, `=`, `[]` 같은 연산자를 붙이는 방법**을 정리한 내용입니다.

---

## 0. 열한 번째 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- **rvalue 참조(`&&`)** 와 이동 생성자의 개념을 맛본다.
- 멤버 함수 `Add()` 를 `operator+` 로 바꿀 수 있다.
- `operator==` 를 만들고 `operator!=` 를 그것으로부터 만들 수 있다.
- `operator+=` 가 **참조를 반환해야 하는 이유**를 설명할 수 있다.
- **전위 `++x`** 와 **후위 `x++`** 를 구분해서 만들 수 있다.
- `operator[]` 를 만들고 **const / 비-const 로 오버로딩**할 수 있다.
- 만든 연산자를 `Array` 클래스에 적용할 수 있다.

### 0.2 연산자 오버로딩이란?

```cpp
int a = 2, b = 3;
int c = a + b;      // int 끼리는 + 가 된다

Point p1(2,3), p2(4,5);
Point p3 = p1 + p2; // Point 끼리도 + 를 쓰고 싶다!
```

- **연산자도 함수입니다.** 이름이 `operator+` 인 함수일 뿐입니다.
- 이 함수를 내 클래스에 만들어 주면 **`+` 를 쓸 수 있게 됩니다.**

```cpp
p1 + p2       ←→   p1.operator+(p2)      // 완전히 같은 뜻
```

---

## 1. rvalue 참조 맛보기

### 1.1 `&&` - 곧 사라질 임시 값에 붙이는 참조

```cpp
int n = 10;
int& r = n;      // lvalue 참조 : 이름이 있는 값에만 붙는다
int&& r2 = 50;   // rvalue 참조 : 임시 값(리터럴)에 붙는다
//int&& r3 = n;  // X - 에러 : n 은 이름이 있으므로 rvalue 가 아니다
```

| 용어 | 뜻 | 예 |
|------|-----|-----|
| **lvalue** | 이름이 있고 주소를 얻을 수 있는 것 | `n`, `arr[0]`, `*p` |
| **rvalue** | 임시 값. 그 문장이 끝나면 사라진다 | `50`, `a + b`, `Point(0,0)` |
| `T&` | lvalue 참조 | `int& r = n;` |
| `T&&` | **rvalue 참조** | `int&& r2 = 50;` |

---

### 1.2 오버로딩으로 구분하기

```cpp
void PrintPoint(const Point& arg)    { arg.Print(); cout << "Point&" << endl; }
void PrintPoint(const Point&& arg)   { arg.Print(); cout << "Point&&" << endl; }

int main()
{
	Point p1(2, 3);
	PrintPoint(p1);            // 이름이 있는 객체   -> Point&
	PrintPoint(Point(0, 0));   // 임시 객체(rvalue)  -> Point&&
}
```

**예상 출력**
```
2,3
Point&
0,0
Point&&
```

---

### 1.3 이동 생성자 (move constructor)

```cpp
Point(const Point& arg) {//복사 생성자
	this->x = arg.x;
	this->y = arg.y;
}
Point(Point&& arg) { //이동 생성자
	this->x = arg.x;
	this->y = arg.y;
}
```

**왜 필요한가?**

- 임시 객체는 **곧 사라집니다.** 그런데 복사 생성자는 열심히 깊은 복사를 합니다.
- "어차피 사라질 거라면 **내용을 훔쳐 오면(옮기면)** 되지 않을까?" 가 이동의 아이디어입니다.

```cpp
// String 이라면 이렇게 된다
String(String&& arg) : buf(arg.buf), length(arg.length)   // 주소만 가져오고
{
	arg.buf = nullptr;      // 원본은 빈 껍데기로 만든다
	arg.length = 0;
}
```

| | 복사 | 이동 |
|---|------|------|
| 하는 일 | 새 메모리 할당 + 내용 복사 | **주소만 옮기고** 원본을 비운다 |
| 비용 | 크다 | **거의 0** |
| 언제 | lvalue 를 받을 때 | **rvalue(임시 객체)** 를 받을 때 |

> `Point` 처럼 `int` 두 개뿐인 클래스는 이동의 이득이 없습니다.
> `String`, `vector` 처럼 **동적 메모리를 가진 클래스**에서 큰 차이가 납니다.
> 이 수업에서는 **개념만** 알고 넘어갑니다.

---

## 2. operator+ - 산술 연산자

### 2.1 먼저 보통 멤버 함수 `Add()` 로

```cpp
class Point
{
	int x, y;
public:
	Point(int x = 0, int y = 0) :x(x), y(y) {}
	void Print()const { cout << x << ',' << y << endl; }
	Point Add(const Point& arg)
	{
		return Point(x + arg.x, y + arg.y);
	}
};
int main()
{
	Point p1(2, 3);
	Point p2(4, 5);
	Point p3 = p1.Add(p2);

	p3.Print();// 6, 8
}
```

---

### 2.2 이름만 `operator+` 로 바꾸면 끝

```cpp
Point operator+(const Point& arg)const   // Add -> operator+
{
	return Point(x + arg.x, y + arg.y);
}
```
```cpp
Point p3 = p1 + p2; // p1.operator+(p2);
```

**예상 출력**
```
6,8
```

**`operator+` 의 형태를 뜯어보기**

```cpp
Point operator+(const Point& arg) const
~~~~~          ~~~~~~~~~~~~~~~~~ ~~~~~
 ①                    ②           ③
```

| 부분 | 이유 |
|------|------|
| ① 반환형 `Point` (**값**) | `p1 + p2` 는 **새로운 결과**를 만든다. p1 도 p2 도 안 바뀐다 |
| ② `const Point&` | 복사 비용 없이 받고, 오른쪽 피연산자를 안 바꾼다 |
| ③ 끝의 `const` | **왼쪽 피연산자(자기 자신)도 안 바꾼다** |

> `a + b` 는 **a 도 b 도 바꾸지 않아야** 합니다. `int` 의 `+` 가 그렇듯이요.
> 그래서 `const` 두 개가 모두 필요합니다.

---

## 3. operator== 와 operator!= - 비교 연산자

```cpp
bool operator==(const Point& arg)const
{
	return x == arg.x && y == arg.y;
}
bool operator!=(const Point& arg)const
{
	return !(*this == arg);   // == 를 재사용 -> 규칙이 어긋날 수 없다
}
```
```cpp
if (p1 != p2) //p1.operator!=(p2)
	cout << "다르다" << endl;
```

**예상 출력**
```
다르다
```

> ### 짝으로 만드는 규칙
>
> | 만들면 | 같이 만들어야 할 것 |
> |--------|---------------------|
> | `==` | `!=` |
> | `<` | `>`, `<=`, `>=` |
> | `+` | `+=` (보통) |
>
> 그리고 **`!=` 는 반드시 `==` 를 이용해서 만드세요.**
> 그래야 나중에 `==` 의 판단 기준을 바꿔도 `!=` 가 자동으로 따라옵니다.

> **C++20 의 `<=>` (우주선 연산자)**
> ```cpp
> auto operator<=>(const Point&) const = default;   // 6개를 한 번에!
> ```
> 이 프로젝트는 C++20 이므로 사용할 수 있습니다. 다만 원리를 이해하기 위해 하나씩 만들어 봅니다.

---

## 4. operator+= - 복합 대입 연산자

### 4.1 void 로 만들면

```cpp
void operator+=(const Point& arg)
{
	x += arg.x;
	y += arg.y;
}
```
```cpp
p1 += p2;          // O
p3 = p1 += p2;     // X - 에러 : void 는 대입할 수 없다
```

---

### 4.2 참조를 반환하면

```cpp
const Point& operator+=(const Point& arg)   // void 였다면 p3 = p1 += p2; 가 에러
{
	x += arg.x;
	y += arg.y;

	return *this;      // 참조를 반환해야 p3 = p1 += p2; 가 된다
}
```
```cpp
Point p1(2, 3), p2(4, 5), p3;

p3 = p1 += p2; //p1.operator+=(p2)
p1.Print();    // 6,8
p3.Print();    // 6,8
```

**예상 출력**
```
6,8
6,8
```

### 4.3 `+` 와 `+=` 의 차이

| | `operator+` | `operator+=` |
|---|-------------|--------------|
| 자기 자신을 | **안 바꾼다** | **바꾼다** |
| 함수 뒤 `const` | **붙인다** | 붙이지 않는다 |
| 반환형 | `Point` (**새 값**) | `Point&` (**자기 자신**) |
| 반환 문 | `return Point(...);` | `return *this;` |

> **`*this` 를 반환하는 이유** ([class5.md](class5.md), [class7.md](class7.md))
> `this` 는 자기 자신의 **주소**이므로, `*this` 는 **자기 자신**입니다.
> 이것을 참조로 반환하면 **결과를 다시 다른 연산에 이어 쓸 수** 있습니다.

---

## 5. operator++ - 전위와 후위 구분하기

### 5.1 전위 `++x` - 참조 반환

```cpp
const Point& operator++()      // 전위 : 참조 반환 (복사 없음)
{
	++x;
	++y;
	return *this;
}
```
```cpp
p2 = ++p1;   //p1.operator++()
p1.Print();  // 7,9
p2.Print();  // 7,9   (증가 후의 값)
```

---

### 5.2 후위 `x++` - int 더미 매개변수 + 값 반환

```cpp
Point operator++(int)          // 후위 : 값 반환 (복사가 생긴다)
{
	Point t(*this);    // 증가 전의 값을 복사해 두었다가
	++x;
	++y;
	return t;          // 그것을 반환한다 -> 그래서 후위가 더 비싸다
}
```
```cpp
p2 = p1++;   //p1.operator++(0)
p1.Print();  // 8,10  (증가됨)
p2.Print();  // 7,9   (증가 전의 값)
```

**예상 출력**
```
7,9
7,9
8,10
7,9
```

### 5.3 정리

| | 전위 `++x` | 후위 `x++` |
|---|-----------|-----------|
| 선언 | `const Point& operator++()` | `Point operator++(int)` |
| `int` 매개변수 | 없다 | **있다 (이름 없는 더미)** |
| 반환 | 참조 (증가 **후**) | 값 (증가 **전**) |
| 복사 | 없다 | **한 번 일어난다** |
| 속도 | 빠르다 | 느리다 |

> **`int` 는 왜 있나?**
> `operator++()` 두 개를 만들 수 없으니, **구분용으로 가짜 매개변수**를 넣기로 정한 것입니다.
> 실제로 값이 넘어오지도 않고 쓰이지도 않습니다. 그래서 **이름조차 쓰지 않습니다.**

> **실무 팁** : 반복문에서 결과를 안 쓴다면 **`++i` 를 쓰세요.**
> ```cpp
> for (int i = 0; i < n; ++i)      // 좋다
> for (auto it = v.begin(); it != v.end(); ++it)   // 특히 반복자에서 차이가 크다
> ```

---

## 6. operator= - 복사 대입 연산자

[class7.md](class7.md) 에서 배운 내용을 연산자 관점에서 다시 봅니다.

```cpp
class Point
{
	int x, *py;      // 포인터 멤버가 있다!
public:
	Point(int x = 0, int y = 0) :x(x), py(new int(y)) { }
	~Point() { delete py; }
	Point(const Point& arg) :x(arg.x), py(new int(*arg.py)) { }
	const Point& operator=(const Point& arg)
	{
		x = arg.x;
		*py = *arg.py;     // 주소가 아니라 "값" 을 옮긴다
		return *this;
	}
};
```

```cpp
p1 = p2; //p1.operator=(p2)
```

| 주의 | 설명 |
|------|------|
| `py = arg.py;` 로 쓰면 | **얕은 복사** → 이중 해제 ([class6.md](class6.md)) |
| `*py = *arg.py;` | 이미 할당된 메모리에 **값만** 옮긴다 → 안전 |
| 자기 대입 검사 | 이 예제는 `*py = *py` 라서 문제가 없지만, **크기가 변하는 경우엔 반드시 필요** |

---

## 7. operator[] - 첨자 연산자

### 7.1 값 반환 버전 (읽기만)

```cpp
int operator[](int idx)const   // 값 반환이라 pt[0] = 15; 는 안 된다
{
	if (idx == 0) return x;
	else if (idx == 1) return y;
	else throw "index 범위 설정 실패!";
}
```
```cpp
cout << pt[0] << endl; //pt.operator[](0)
cout << pt[1] << endl;
```

---

### 7.2 참조 반환 버전 (읽기 + 쓰기)

```cpp
int& operator[](int idx)      // <= 참조 반환
{
	if (idx == 0) return x;
	else if (idx == 1) return y;
	else throw "index 범위 설정 실패!";
}
```
```cpp
pt[0] = 15;                  // 참조 반환이라 대입이 가능하다
cout << pt[0] << endl;
```

- 원리는 [class7.md](class7.md) 의 `int& at(int)` 와 완전히 같습니다.

---

### 7.3 const / 비-const 오버로딩

```cpp
int& operator[](int idx)                            // p1[0] = 15;
{ ... }
int operator[](int idx)const                        // const 객체용 읽기 전용
{ ... }
```
```cpp
Point pt(2, 3);
pt[0] = 15;                                // 비-const 버전
cout << pt[0] << ',' << pt[1] << endl;

const Point cpt(0, 0);
cout << cpt[0] << ',' << cpt[1] << endl;   // const 버전이 호출된다
```

**예상 출력**
```
15,10
0,0
```

> `throw "index 범위 설정 실패!";` 는 **예외를 던지는 것**입니다.
> 자세한 내용은 **[class15.md](class15.md)** 에서 배웁니다. 지금은 "잘못된 인덱스면 프로그램이 여기서 멈춘다" 정도로 이해하면 됩니다.

---

## 8. Array 클래스에 적용하기

```cpp
class Array
{
	int* buf;
	size_type count;
public:
	...
	const int& operator[](int idx)const { return buf[idx]; }

	bool operator==(const Array& arg)const
	{
		return this == &arg;      // 같은 객체인지(주소 비교)
	}
	const Array& operator=(const Array& arg)
	{
		if (*this == arg)
			throw "자신의 객체를 대입할 수 없음!";

		delete[] buf;

		count = arg.count;        // count 를 빠뜨리면 값이 안 옮겨진다!
		buf = new int[100];
		for (size_type i = 0; i < count; ++i)
			buf[i] = arg.buf[i];

		return *this;
	}
};
int main()
{
	Array arr;
	arr.push_back(10); ... arr.push_back(50);

	for (Array::size_type i = 0; i < arr.size(); ++i)
		cout << arr[i] << endl;    // arr.operator[](i)

	Array arr2;
	arr2 = arr;
	...
}
```

> **`operator==` 가 주소를 비교하는 것에 주의하세요.**
> 이 `Array` 의 `==` 는 "내용이 같은가" 가 아니라 **"같은 객체인가"** 를 묻습니다.
> 자기 대입 검사용으로 만든 것이므로 이렇게 되어 있지만, **일반적인 `==` 의 의미와는 다릅니다.**
> 실제 클래스를 설계할 때는 **`==` 는 내용 비교**로 만드는 것이 보통입니다.

---

## 9. 연산자 오버로딩의 규칙

### 9.1 오버로딩할 수 있는 것 / 없는 것

| 가능 | `+ - * / % ++ -- == != < > <= >= && \|\| ! [] () -> = += -= << >> ...` |
|------|-------|
| **불가능** | `.` (멤버 접근) `.*` `::` (범위 지정) `?:` (삼항) `sizeof` `typeid` |

---

### 9.2 지켜야 할 것

| 규칙 | 설명 |
|------|------|
| **의미를 바꾸지 마라** | `+` 는 더하기여야 한다. 빼기로 만들면 아무도 못 읽는다 |
| 우선순위는 못 바꾼다 | `*` 가 `+` 보다 먼저인 것은 그대로 |
| 피연산자 개수도 못 바꾼다 | 이항 연산자는 계속 이항 |
| 최소 하나는 클래스여야 | `int + int` 의 동작은 바꿀 수 없다 |
| 기본값 인자 금지 | `operator+(const Point& a = Point())` 는 안 된다 |

---

### 9.3 반환형 선택 요령

| 연산자 | 반환형 | 이유 |
|--------|--------|------|
| `+ - * /` | **값** (`Point`) | 새 결과를 만든다 |
| `== != < >` | `bool` | 참/거짓 |
| `= += -= *=` | **참조** (`Point&`) | 자기 자신을 바꾸고 연쇄 가능하게 |
| 전위 `++ --` | **참조** | 증가 후 자기 자신 |
| 후위 `++ --` | **값** | 증가 전의 사본 |
| `[]` | **참조** (`int&`) | 대입 가능하게 |

---

## 10. 완성 코드 (ex11.cpp 맨 위)

```cpp
int main()
{
	Point p1(2, 3);
	Point p2(4, 5);
	Point p3;

	p3 = p1 + p2;        // p1.operator+(p2)
	p3.Print();          // 6,8

	if (p1 != p2)        // p1.operator!=(p2)
		cout << "다르다" << endl;

	p3 = p1 += p2;       // p1.operator+=(p2)
	p1.Print();          // 6,8
	p3.Print();          // 6,8

	p2 = ++p1;           // 전위 : 증가 후의 값
	p1.Print(); p2.Print();
	p2 = p1++;           // 후위 : 증가 전의 값
	p1.Print(); p2.Print();

	p1[0] = 15;          // p1.operator[](0) 이 int& 를 반환
	cout << p1[0] << ',' << p1[1] << endl;

	const Point cpt(0, 0);
	cout << cpt[0] << ',' << cpt[1] << endl;   // const 버전이 호출된다
}
```

**예상 출력**
```
6,8
다르다
6,8
6,8
7,9
7,9
8,10
7,9
15,10
0,0
```

---

## 10.5 종합 과제 - Matrix(행렬) 클래스

`ex11.cpp` **[단계 13]** 에는 지금까지 배운 것을 모두 쓰는 **행렬 클래스**가 들어 있습니다.

### 10.5.1 2차원을 1차원으로 펼치기

```cpp
class Matrix
{
    int* matrix;      // row * col 크기의 1차원 배열
    int row;
    int col;
public:
    // (i, j) 원소의 위치 = matrix[(col * i) + j]
    explicit Matrix(int _row = 0, int _col = 0) :matrix(nullptr), row(_row), col(_col)
    {
        matrix = new int[_row * _col];
        ...
    }
```

```
2행 3열 행렬을 1차원 배열로

  논리적 모습            실제 메모리
  ┌───┬───┬───┐
  │ a │ b │ c │   →   [ a │ b │ c │ d │ e │ f ]
  ├───┼───┼───┤          0   1   2   3   4   5
  │ d │ e │ f │
  └───┴───┴───┘        (i, j) → col * i + j
```

- `new int[row][col]` 은 **불가능**합니다. 그래서 1차원으로 펼쳐 쓰는 것이 흔한 방법입니다.
- `(1,2)` 원소는 `matrix[3 * 1 + 2]` = `matrix[5]` 입니다.

### 10.5.2 들어 있는 것

| 요소 | 관련 강의 |
|------|-----------|
| `new int[]` / `delete[]` | [class3](class3.md) |
| `explicit` 생성자 | [class13](class13.md) |
| 복사 생성자 (깊은 복사) | [class6](class6.md) |
| 소멸자 | [class4](class4.md) |
| `operator=` + 자기 대입 검사 | [class7](class7.md) |
| `operator==` / `!=` / `+` / `-` / `*` | 이번 강의 |
| `throw` 로 오류 알리기 | [class15](class15.md) |

### 10.5.3 일부러 남겨 둔 문제

이 코드는 **수업 중에 만들다 만 상태 그대로** 옮겨 두었습니다. 고치는 것이 연습 문제입니다.

| # | 문제 | 왜 문제인가 |
|---|------|-------------|
| 1 | `operator+` 가 **자기 자신을 바꾼다** | `a + b` 는 a 도 b 도 바꾸면 안 된다 (11장 2.2) |
| 2 | `operator*` 의 **몸통이 비어 있다** | 곱셈 결과가 항상 0 행렬 |
| 3 | `operator*` 에서 `else throw` 뒤에 **return 이 없다** | 조건이 거짓이면 반환값이 없다 (경고 C4715) |
| 4 | `operator==` 가 **주소만 비교**한다 | 내용 비교가 아니다 (`Array` 와 같은 사정) |

---

## 11. 정리 표

| 주제 | 내용 |
|------|------|
| 연산자 = 함수 | `p1 + p2` ←→ `p1.operator+(p2)` |
| lvalue / rvalue | 이름이 있는 값 / 곧 사라질 임시 값 |
| `T&&` | rvalue 참조. 이동 생성자의 재료 |
| `operator+` | 반환 **값**, 매개변수 `const T&`, 함수 뒤 `const` |
| `operator==` | `bool` 반환, 함수 뒤 `const` |
| `operator!=` | **`!(*this == arg)`** 로 만든다 |
| `operator+=` | **`T&` 반환**, `return *this;`, 함수 뒤 const **없음** |
| 전위 `++` | `T& operator++()` — 참조, 증가 후 |
| 후위 `++` | `T operator++(int)` — 값, 증가 전. **더 비싸다** |
| `operator=` | 자기 대입 검사 → 해제 → 복사 → `*this` 반환 |
| `operator[]` | 참조 반환하면 `arr[i] = 값;` 이 된다 |
| const 오버로딩 | 읽기 전용 버전과 수정 가능 버전을 함께 |
| 오버로딩 불가 | `.` `.*` `::` `?:` `sizeof` |
| 최우선 원칙 | **원래 의미를 지켜라** |

---

## 12. 연습 문제 (스스로 풀어보기)

1. `Point` 에 `operator-` 와 `operator-=` 를 추가하세요.
2. `Point` 에 전위/후위 `operator--` 를 추가하고 결과를 비교하세요.
3. `Point` 의 `operator*` 를 만들어 `p * 2` (스칼라 곱) 가 되게 하세요.
4. `operator<` 를 만들고 (거리 기준), `>`, `<=`, `>=` 를 `<` 와 `==` 만 이용해 만들어 보세요.
5. `operator+=` 의 반환형을 `void` 로 바꾸고 `p3 = p1 += p2;` 를 시도해 어떤 에러가 나는지 확인하세요.
6. 후위 `++` 안에 `cout << "postfix" << endl;` 을 넣고, `for (int i=0;i<3;p1++)` 와 `for (int i=0;i<3;++p1)` 에서 복사 생성자가 몇 번 불리는지 세어 보세요.
7. `class Money { long won; };` 를 만들고 `+`, `-`, `==`, `<`, `+=` 를 구현하세요.
8. `operator[]` 에서 `throw` 를 없애고 범위를 벗어난 인덱스로 접근해 보세요. 무슨 일이 생기나요?
9. `Array` 의 `operator=` 에서 `count = arg.count;` 를 지우고 실행해 보세요. 왜 값이 안 나오나요?
10. **[Matrix 과제]** `operator+` 를 자기 자신을 바꾸지 않도록 고치세요. (힌트 : `Matrix t(*this);` 로 사본을 만들어 그것을 고쳐 반환)
11. **[Matrix 과제]** `operator*` 의 주석 처리된 삼중 반복문을 완성하고, 2×3 행렬과 3×2 행렬을 곱해 보세요.
12. **[Matrix 과제]** `operator==` 를 **내용 비교**로 바꾸고, 자기 대입 검사는 `this == &arg` 로 따로 하세요.
13. **[Matrix 과제]** `SetElement(100, 5, 5)` 처럼 범위를 벗어난 호출을 `try/catch` 로 잡아 보세요. ([class15.md](class15.md))

---

이 강의 자료는 **example/src/ex11.cpp** 에 있는 모든 예제를 바탕으로 구성했습니다. 이번 시간의 핵심은 **반환형 선택**입니다. "새 값을 만드는 연산자는 값을, 자기 자신을 바꾸는 연산자는 참조를 반환한다" 는 원칙만 잡으면 나머지는 자연스럽게 따라옵니다. 다음 시간에는 `operator()` 로 만드는 **함수 객체**를 배웁니다.
