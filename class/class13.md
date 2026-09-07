# C++ 기초 강의 13 - ex13.cpp 코드를 통한 전역 연산자·friend·형변환 연산자

이 강의는 **example/src/ex13.cpp** 에 담긴 예제들을 바탕으로, **멤버 함수로는 만들 수 없는 연산자**를 전역 함수로 만드는 방법, `friend`, 그리고 **형변환 연산자**와 `explicit` 를 정리한 내용입니다.

---

## 0. 열세 번째 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- 멤버 `operator+` 로는 `p1 + 10` 만 되고 `10 + p1` 은 안 되는 이유를 안다.
- **전역 함수 연산자**를 만들 수 있다.
- `cout << p1;` 을 가능하게 하는 `ostream& operator<<` 를 만들 수 있다.
- **`friend`** 로 전역 함수에게 private 접근을 허락할 수 있다.
- `cin >> p1;` 을 가능하게 하는 `istream& operator>>` 를 만들 수 있다.
- **형변환 연산자** `operator int()`, `operator const char*()` 를 만들 수 있다.
- **`explicit`** 로 원치 않는 자동 형변환을 막을 수 있다.

---

## 1. 멤버 연산자의 한계

### 1.1 `p1 + 10` 은 되지만

```cpp
class Point
{
	int x, y;
public:
	Point(int x = 0, int y = 0) :x(x), y(y) {}
	void Print()const { cout << x << ',' << y << endl; }
	int GetX()const { return x; }
	int GetY()const { return y; }
	Point operator+(int n)const
	{
		return Point(x + n, y + n);
	}
};
int main()
{
	Point p1(2, 3);
	Point p2;

	p2 = p1 + 10; //p1.operator+(int n);
	p2.Print();   // 12,13

	//p2 = 10 + p1;   // X - 에러 : int 에는 이런 operator+ 가 없다
}
```

**예상 출력**
```
12,13
```

---

### 1.2 왜 `10 + p1` 은 안 되나

```cpp
p1 + 10   ←→   p1.operator+(10)     // 왼쪽 p1 이 자기 자신
10 + p1   ←→   10.operator+(p1)     // ???  int 에 멤버 함수를 추가할 수 없다!
```

> ### 멤버 연산자의 근본 한계
> **멤버 함수로 만든 연산자는 왼쪽 피연산자가 반드시 자기 클래스여야 합니다.**
>
> 그런데 `int` 같은 기본 자료형에는 멤버 함수를 추가할 수 없습니다.
> 그래서 **전역 함수**로 만들어야 합니다.

---

## 2. 전역 함수 연산자

### 2.1 `operator+(int, const Point&)`

```cpp
Point operator+(int n, const Point& pt)   // 왼쪽 피연산자가 int 라서 멤버로는 못 만든다
{
	return Point(n + pt.GetX(), n + pt.GetY());
}
int main()
{
	Point p1(2, 3);
	Point p2;

	p2 = 10 + p1; //operator+( 10, p1 );
	p2.Print();   // 12,13
}
```

**예상 출력**
```
12,13
```

**멤버 vs 전역 - 매개변수 개수가 다르다**

| | 멤버 함수 | 전역 함수 |
|---|-----------|-----------|
| 선언 | `Point operator+(int n) const` | `Point operator+(int n, const Point& pt)` |
| 매개변수 | **1개** (왼쪽은 `this`) | **2개** (왼쪽, 오른쪽) |
| 왼쪽 피연산자 | 반드시 자기 클래스 | **아무것이나** |
| private 접근 | 가능 | 불가 (**`friend` 필요**) |

**어느 쪽으로 만들까?**

| 연산자 | 권장 |
|--------|------|
| `= [] () ->` | **반드시 멤버** (문법 규칙) |
| `+= -= *= ++ --` | 멤버 (자기 자신을 바꾸므로) |
| `+ - * / == != < >` | **전역** (양쪽을 대칭으로 다룰 수 있다) |
| `<< >>` | **반드시 전역** (왼쪽이 `ostream`/`istream`) |

---

## 3. `cout << p1;` 만들기

### 3.1 `cout << "Hello!"` 의 정체

```cpp
cout << "Hello!" << endl;
// 이것과 완전히 같은 뜻
operator <<(cout , "Hello!").operator <<(endl);
```

- `cout` 은 `ostream` 클래스의 **객체**입니다.
- `<<` 는 `ostream` 을 왼쪽에 두는 **연산자 함수**입니다.
- 그래서 `Point` 를 출력하려면 **왼쪽이 `ostream` 인 전역 함수**를 만들어야 합니다.

---

### 3.2 `ostream& operator<<` 만들기

```cpp
ostream& operator<<(ostream& out, const Point& pt)
{
	out << pt.GetX() << ',' << pt.GetY();
	return out;    // 반환해야 << 를 이어 붙일 수 있다
}
int main()
{
	Point p1(2, 3);

	cout << "Hello!" << endl;
	operator <<(cout , "Hello!").operator <<(endl);   // 위 줄과 완전히 같은 뜻
	cout << p1 << endl;
	operator <<(cout, p1).operator <<(endl);
}
```

**함수 시그니처 뜯어보기**

```cpp
ostream& operator<<(ostream& out, const Point& pt)
~~~~~~~~            ~~~~~~~~~~~~  ~~~~~~~~~~~~~~
   ①                    ②              ③
```

| 부분 | 이유 |
|------|------|
| ① `ostream&` 반환 | **`cout << a << b` 처럼 이어 쓰기** 위해. 값으로 반환하면 복사 불가라 에러 |
| ② `ostream& out` | **`ostream` 은 복사할 수 없으므로 반드시 참조**로 받는다 |
| ③ `const Point&` | 출력만 하므로 const, 복사 비용 없게 참조 |

**왜 `ostream&` 를 반환해야 하나**

```cpp
cout << p1 << endl;
// 왼쪽부터 계산된다
(cout << p1) << endl;
//  ↑ 이 결과가 ostream& 이어야 다음 << 를 쓸 수 있다
```

> **`ostream out` 처럼 값으로 받으면?**
> ```
> error C2280: 'std::basic_ostream<char>::basic_ostream(const ...&)':
>              삭제된 함수를 참조하려고 합니다.
> ```
> 스트림은 복사가 **금지**되어 있습니다. 반드시 `ostream&` 입니다.

---

## 4. friend - private 접근 허락하기

### 4.1 문제 : Get 함수를 계속 써야 한다

```cpp
ostream& operator<<(ostream& out, const Point& pt)
{
	out << pt.GetX() << ',' << pt.GetY();   // pt.x 는 private 이라 못 쓴다
	return out;
}
```

---

### 4.2 friend 선언

```cpp
class Point
{
	int x, y;
public:
	...
	// 전역 함수에게 private 멤버 접근을 허락한다
	friend Point operator+(int n, const Point& pt);
	friend ostream& operator<<(ostream& out, const Point& pt);
	friend istream& operator>>(istream& in, Point& pt);
};

ostream& operator<<(ostream& out, const Point& pt)
{
	//out << pt.GetX() << ',' << pt.GetY();
	out << pt.x << ',' << pt.y;   // friend 라서 private 에 직접 접근 가능
	return out;
}
```

**friend 의 성질**

| 항목 | 설명 |
|------|------|
| 위치 | 클래스 안 어디든 (`public:`/`private:` 과 무관) |
| 무엇이 될 수 있나 | 전역 함수, 다른 클래스의 멤버 함수, **클래스 전체** |
| 방향 | **주는 쪽이 정한다.** 밖에서 "나 friend 할래" 는 불가 |
| 상속 | **상속되지 않는다** |
| 상호성 | A 가 B 의 friend 라고 B 가 A 의 friend 인 것은 아니다 |

```cpp
friend class Manager;   // 클래스 전체를 friend 로
```

> ### friend 는 캡슐화를 깨는 것 아닌가?
>
> 반은 맞고 반은 틀립니다.
>
> - **깨는 면** : private 을 밖에서 볼 수 있게 된다
> - **지키는 면** : **누가 볼 수 있는지 클래스가 명시적으로 정한다**
>
> 아무나 볼 수 있게 `public` 으로 여는 것보다 **friend 하나를 지정하는 편이 낫습니다.**
> 다만 **꼭 필요할 때만** 쓰세요. `operator<<`, `operator>>` 가 대표적인 정당한 용도입니다.

---

## 5. `cin >> p1;` 만들기

```cpp
istream& operator>>(istream& in, Point& pt)   // 입력은 원본을 바꿔야 하므로 const 없음
{
	cout << "pt(x,y) 입력: ";
	in >> pt.x >> pt.y;

	return in;
}
int main()
{
	Point p1(2, 3);
	Point p2(3, 4);

	cin >> p1 >> p2;
	cout << p1 << " " << p2 << "  " << "Hello!" << endl;
}
```

**실행 예 (1 2 / 3 4 입력)**
```
pt(x,y) 입력: 1 2
pt(x,y) 입력: 3 4
1,2 3,4  Hello!
```

| `operator<<` 와 다른 점 | 이유 |
|-------------------------|------|
| `istream&` | 입력 스트림 |
| `Point& pt` — **`const` 가 없다** | **원본에 값을 넣어야** 하므로 |

---

## 6. 형변환 연산자

### 6.1 클래스를 다른 형식처럼 쓰이게 하기

```cpp
class Point
{
	int x, y;
public:
	Point(int x = 0, int y = 0) :x(x), y(y) {}
	...
	operator int()const            // 반환형을 쓰지 않는다 (이름이 곧 반환형)
	{
		return x;
	}
	operator const char*()const
	{
		return "class Point";
	}
};
int main()
{
	Point p1(2, 3);

	int n = p1;                              // 자동으로 operator int() 호출
	cout << "n: " << n << endl;              // 2
	const char* name = (const char*)p1;
	cout << "p1: " << name << endl;          // class Point
}
```

**예상 출력**
```
n: 2
p1: class Point
```

**형변환 연산자의 문법**

```cpp
operator 변환할형식() const
~~~~~~~~ ~~~~~~~~~~~
   │          └─ 반환형 자리에 아무것도 안 쓴다 (이름이 곧 반환형)
   └─ 키워드
```

| 규칙 | 설명 |
|------|------|
| 반환형 | **쓰지 않는다** (이름이 곧 반환형) |
| 매개변수 | **없다** |
| `const` | 보통 붙인다 |
| 호출 시점 | **필요하면 자동으로** 불린다 |

**어디에 쓰나?**

```cpp
// app/string 의 String 클래스에서
operator const char* ()const { return buf; }
```
```cpp
String s("Hello");
const char* p = s;              // 자동 변환
printf("%s\n", (const char*)s); // C 함수에 넘길 때 편하다
```

> ### 형변환 연산자는 조심해서 쓰세요
>
> **자동으로** 불리기 때문에 **의도치 않은 변환**이 생길 수 있습니다.
>
> ```cpp
> Point p1(2, 3), p2(5, 5);
> if (p1 < p2)   // operator< 가 없어도... int 로 변환되어 2 < 5 로 비교된다!
> ```
>
> C++11 부터는 **`explicit operator int()`** 로 만들어 명시적 캐스팅만 허용할 수 있습니다.

---

## 7. explicit - 원치 않는 형변환 막기

### 7.1 매개변수 하나짜리 생성자는 형변환 통로가 된다

```cpp
class Point
{
	int x, y;
public:
	Point(int x) :x(x), y(0) {}    // explicit 가 없다
	...
};
int main()
{
	Point p = 10;     // ??? Point(10) 이 자동으로 불린다
	void f(Point p);
	f(100);           // ??? 이것도 된다
}
```

- 편해 보이지만 **의도하지 않은 변환**이 조용히 일어납니다.
- `Point p = 10;` 는 읽는 사람을 혼란스럽게 합니다.

---

### 7.2 explicit 를 붙이면

```cpp
class Point
{
	int x, y;
public:
	Point() :x(0), y(0) {}
	explicit Point(int x) :x(x), y(0) {}   // explicit : Point p = 10; 을 막는다
	Point(int x, int y) :x(x), y(y) {}
	...
};
int main()
{
	Point p3;
	int n = 100;

	//p3 = n;        // X - explicit 때문에 막힌다
	p3 = (Point)n;   // O - Point(n) 을 명시적으로 호출
	p3.Print();
}
```

**예상 출력**
```
100,0
```

| | `explicit` 없음 | `explicit` 있음 |
|---|-----------------|-----------------|
| `Point p = 10;` | O (암시적 변환) | **X** |
| `Point p(10);` | O | **O** |
| `Point p = Point(10);` | O | **O** |
| `f(10)` (매개변수가 `Point`) | O | **X** |

> ### 실무 규칙
> **"매개변수 하나로 호출될 수 있는 생성자에는 `explicit` 를 붙인다."**
>
> ```cpp
> explicit Point(int x = 0, int y = 0);   // 이것도 x 하나만으로 호출 가능하므로
> ```
>
> 예외 : 복사 생성자, 그리고 **정말로 자연스러운 변환**일 때
> (`std::string(const char*)` 은 일부러 explicit 가 아닙니다)

---

## 8. String 클래스 종합

`ex13.cpp` 의 [단계 8] 은 지금까지 배운 것을 `String` 클래스에 모두 적용한 예제입니다.

```cpp
class String
{
	char* buf;
	Size_type length;
public:
	String(const char* s = "") { ... }
	String(const String& arg) { ... }          // 복사 생성자   (class6)
	~String() { delete[] buf; }                // 소멸자        (class6)
	void append(const char* s) { ... }
	void assign(const char* s) { ... }
	const char* c_str()const { return buf; }

	const String& operator=(const String& arg)  // 복사 대입     (class7)
	{
		if (this == &arg)      // 자기 대입 방지
			return *this;
		assign(arg.buf);       // 이미 만든 assign 을 재사용
		return *this;
	}
	const String& operator+=(const String& arg) // 복합 대입     (class11)
	{
		if (this == &arg)
			return *this;
		append(arg.buf);
		return *this;
	}
	operator const char* ()const   // 형변환 연산자             (class13)
	{
		return buf;
	}
};
ostream& operator<<(ostream& out, const String& s)   // 출력 연산자 (class13)
{
	out << s.c_str();
	return out;
}
int main()
{
	String s("hello!");
	cout << s << endl;         // operator<<

	s = "ABC";       // const char* -> String 임시객체 -> operator=
	cout << s << endl;

	s += "DEF";
	s += s;          // 자기 대입 검사 덕분에 안전하다
	cout << s << endl;

	const char* str = s.operator const char *();
	cout << str << endl;
}
```

**예상 출력**
```
hello!
ABC
ABCDEFABCDEF
ABCDEFABCDEF
```

> **`s = "ABC";` 가 되는 이유**
> 1. `"ABC"` 는 `const char*` 입니다.
> 2. `String(const char*)` 생성자가 **explicit 가 아니므로** 임시 `String` 이 만들어집니다.
> 3. 그 임시 객체로 `operator=` 가 호출됩니다.
>
> `s += s;` 는 **자기 대입**입니다. `if (this == &arg) return *this;` 가 없으면
> `append` 중에 자기 버퍼를 해제해 버려서 문제가 생깁니다.
>
> (엄밀히 말하면 이 구현은 `s += s` 에서 아무 일도 하지 않습니다.
> 진짜로 두 배로 만들려면 임시 사본을 만들어야 합니다. 연습 문제 7번을 보세요.)

---

## 9. 완성 코드 (ex13.cpp 맨 위)

```cpp
class Point
{
	int x, y;
public:
	Point() :x(0), y(0) {}
	explicit Point(int x) :x(x), y(0) {}   // explicit : Point p = 10; 을 막는다
	Point(int x, int y) :x(x), y(y) {}

	void Print()const { cout << x << ',' << y << endl; }
	int GetX()const { return x; }
	int GetY()const { return y; }

	Point operator+(int n)const { return Point(x + n, y + n); }   // p1 + 10
	operator int()const { return x; }                             // int n = p1;
	operator const char* ()const { return "class Point"; }

	friend Point operator+(int n, const Point& pt);
	friend ostream& operator<<(ostream& out, const Point& pt);
	friend istream& operator>>(istream& in, Point& pt);
};

Point operator+(int n, const Point& pt)     // 10 + p1
{
	return Point(n + pt.x, n + pt.y);
}
ostream& operator<<(ostream& out, const Point& pt)   // cout << p1
{
	out << pt.x << ',' << pt.y;
	return out;
}
istream& operator>>(istream& in, Point& pt)          // cin >> p1
{
	cout << "pt(x,y) 입력: ";
	in >> pt.x >> pt.y;
	return in;
}

int main()
{
	Point p1(2, 3);
	Point p2;

	p2 = p1 + 10;      // p1.operator+(10)
	p2.Print();        // 12,13

	p2 = 10 + p1;      // operator+(10, p1)
	p2.Print();        // 12,13

	cout << p1 << " " << p2 << "  " << "Hello!" << endl;

	int n = p1;                              // operator int()
	const char* name = (const char*)p1;      // operator const char*()
	cout << n << " / " << name << endl;

	//Point p3 = 100;      // X - explicit 때문에 막힌다
	Point p3 = Point(100); // O - 명시적으로 써야 한다
	p3.Print();
}
```

**예상 출력**
```
12,13
12,13
2,3 12,13  Hello!
2 / class Point
100,0
```

---

## 10. 정리 표

| 주제 | 내용 |
|------|------|
| 멤버 연산자의 한계 | 왼쪽 피연산자가 **반드시 자기 클래스** |
| 전역 연산자 | 매개변수가 **2개**, 왼쪽에 아무 형식이나 가능 |
| 멤버로 만들 것 | `= [] () ->` (필수), `+= ++ --` (권장) |
| 전역으로 만들 것 | `+ - == <`, **`<< >>` (필수)** |
| `operator<<` | `ostream& operator<<(ostream& out, const T& t)` |
| `ostream&` 반환 이유 | `cout << a << b` 이어 쓰기 |
| `ostream&` 매개변수 이유 | 스트림은 **복사 불가** |
| `operator>>` | 두 번째 매개변수는 **`const` 없이** (원본을 바꿔야) |
| `friend` | 전역 함수/다른 클래스에게 private 접근 허락 |
| friend 성질 | 상속 안 됨, 상호적이지 않음, **주는 쪽이 정한다** |
| 형변환 연산자 | `operator int() const` — 반환형·매개변수 **없음** |
| 형변환 연산자 위험 | **자동으로** 불려서 의도치 않은 변환 |
| `explicit` | 매개변수 1개 생성자의 **암시적 변환 금지** |
| explicit 규칙 | 1개로 호출 가능한 생성자에는 **일단 붙인다** |

---

## 11. 연습 문제 (스스로 풀어보기)

1. `Point` 에 `operator-` 를 멤버로, `operator-(int, const Point&)` 를 전역으로 만들어 보세요.
2. `ostream& operator<<` 에서 `return out;` 을 지우고 `cout << p1 << endl;` 을 시도해 보세요. 어떤 에러가 나나요?
3. `ostream& out` 을 `ostream out` 으로 바꿔 보고 에러 메시지를 읽어 보세요.
4. `friend` 선언을 지우고 `pt.x` 를 써 보세요. 어떤 에러가 나나요?
5. `class Money { long won; };` 에 `operator<<`, `operator>>` 를 friend 로 만들고 `cin >> m; cout << m;` 을 해 보세요.
6. `Money` 에 `explicit Money(long)` 을 만들고 `Money m = 1000;` 을 시도해 보세요. 그 다음 `explicit` 를 지우고 다시 해 보세요.
7. `String::operator+=` 를 고쳐 `s += s;` 가 실제로 문자열을 두 배로 만들도록 해 보세요. (힌트 : `String t(arg); append(t.c_str());`)
8. `Point` 의 `operator int()` 를 `explicit operator int()` 로 바꾸고, `int n = p1;` 이 왜 에러가 되는지 확인하세요.
9. `class Fraction { int num, den; };` 을 만들고 `operator double()` 을 구현해 `double d = f;` 가 되게 하세요.

---

이 강의 자료는 **example/src/ex13.cpp** 에 있는 모든 예제를 바탕으로 구성했습니다. 이번 시간의 핵심은 **`cout << 내객체;` 를 만드는 방법**입니다. 이것 하나만 만들어 두면 디버깅이 훨씬 편해지므로, 앞으로 클래스를 만들 때마다 `operator<<` 를 함께 만드는 습관을 들이세요.
