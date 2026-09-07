# C++ 기초 강의 6 - ex6.cpp 코드를 통한 복사 생성자와 깊은 복사

이 강의는 **example/src/ex6.cpp** 와 **example/src/app/string/** 의 예제들을 바탕으로, **포인터 멤버를 가진 클래스**를 만들 때 반드시 알아야 할 **복사 생성자**와 **얕은 복사 / 깊은 복사**를 정리한 내용입니다.

---

## 0. 여섯 번째 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- `std::string` 과 똑같이 동작하는 `String` 클래스를 직접 만들 수 있다.
- 생성자에서 동적 할당하고 소멸자에서 해제하는 **RAII** 패턴을 이해한다.
- 컴파일러가 자동으로 만들어 주는 **복사 생성자**의 동작(얕은 복사)을 안다.
- **얕은 복사(shallow copy)** 가 왜 프로그램을 죽이는지 설명할 수 있다.
- **깊은 복사(deep copy)** 를 하는 복사 생성자를 직접 작성할 수 있다.
- 아직 준비되지 않은 기능(대입 연산자)을 **private 선언으로 막아 두는** 기법을 안다.

### 0.2 수업 진행 순서 제안

1. 단계 1 : `std::string` 을 먼저 써 본다 (목표 확인)
2. 단계 1b : 껍데기 클래스 만들기
3. 단계 2~4 : 생성자/소멸자 → assign → append
4. 단계 5 : **얕은 복사로 프로그램을 죽여 본다** (가장 중요)
5. 단계 6 : 복사 생성자로 해결
6. app/string : 파일 분리 + 대입 연산자 ([class7.md](class7.md) 예고)

---

## 1. 목표 확인 - std::string 을 먼저 써 본다

```cpp
#include <iostream>
#include <string>
using namespace std;

int main()
{
	string s("Hello!");

	for (string::size_type i = 0; i < s.size(); ++i)
		cout << s.at(i) << " ";
	cout << endl;

	s.append("12345");
	cout << s.c_str() << endl;

	s.assign("ABC");
	cout << s.c_str() << endl;
}
```

**예상 출력**
```
H e l l o ! 
Hello!12345
ABC
```

| 멤버 | 하는 일 |
|------|---------|
| `size()` | 문자열 길이 |
| `at(i)` | i 번째 문자 |
| `append(s)` | 뒤에 이어 붙이기 |
| `assign(s)` | 새 문자열로 교체 |
| `c_str()` | C 스타일 `const char*` 로 얻기 |
| `size_type` | 크기를 담는 형식 (클래스 안에 정의된 별칭) |

이제 이것과 **똑같이 동작하는 클래스를 직접 만들어 봅니다.**

---

## 2. 껍데기부터 만들기

```cpp
#include <iostream>
#include <string>
using namespace std;

class String
{
public:
	typedef unsigned int Size_type;   // 클래스 안에 형식 별칭을 정의
public:
	String(const char* s=""){}

	Size_type size() { return 0; }
	char at(Size_type i) { return 'A'; }
	void append(const char* s) { }
	void assign(const char* s) { }
	const char* c_str() { return ""; }
};
```

- 먼저 **인터페이스(함수 이름과 매개변수)를 `std::string` 과 똑같이** 맞춰 놓습니다.
- 내용은 나중에 채웁니다. 이렇게 하면 **사용하는 쪽 코드를 바꾸지 않고** 구현을 완성해 갈 수 있습니다.

**`typedef unsigned int Size_type;`**

- 클래스 **안에** 형식 별칭을 만든 것입니다.
- 사용할 때는 `String::Size_type i;` 라고 씁니다.
- `std::string::size_type`, `vector<int>::size_type` 도 모두 같은 방식입니다.

---

## 3. 생성자에서 할당, 소멸자에서 해제 (RAII)

```cpp
class String
{
public:
	typedef unsigned int Size_type;
private:
	char* buf;          // 문자열을 담을 동적 메모리
	Size_type length;
public:
	String(const char* s = ""):buf(nullptr),length(0) {
		length = (Size_type)strlen(s);
		buf = new char[length + 1];       // 생성자에서 확보
		strcpy_s(buf, length + 1, s);
	}
	~String() { delete[] buf; }           // 소멸자에서 해제
	Size_type size()const { return length; }
	char at(Size_type i)const { return buf[i]; }
	const char* c_str()const { return buf; }
};
```

**`length + 1` 인 이유**

```
"Hello!" 를 저장하려면
┌───┬───┬───┬───┬───┬───┬────┐
│ H │ e │ l │ l │ o │ ! │ \0 │   <- 널 종료 문자 자리가 하나 더 필요
└───┴───┴───┴───┴───┴───┴────┘
  0   1   2   3   4   5    6      => strlen = 6, 필요한 크기 = 7
```

**RAII (Resource Acquisition Is Initialization)**

| 원칙 | 설명 |
|------|------|
| 자원을 **생성자에서 얻는다** | `new`, 파일 열기, 잠금 획득 |
| 자원을 **소멸자에서 놓는다** | `delete`, 파일 닫기, 잠금 해제 |
| 결과 | **해제를 잊을 수가 없다.** 객체가 사라지면 자동으로 정리된다 |

- C 에서는 `malloc` 뒤에 `free` 를 **직접** 불러야 했고, 중간에 `return` 을 하면 새기 일쑤였습니다.
- C++ 는 **블록만 벗어나면 소멸자가 반드시 불립니다.** 심지어 예외가 발생해도요. ([class15.md](class15.md))

---

## 4. assign 과 append

### 4.1 assign - 새 문자열로 갈아 끼우기

```cpp
void assign(const char* s) {
	delete[] buf;                     // 1. 기존 메모리 해제

	length = (Size_type)strlen(s);    // 2. 새 길이 계산
	buf = new char[length + 1];       // 3. 새 메모리 확보
	strcpy_s(buf, length + 1, s);     // 4. 복사
}
```

> **순서가 중요합니다.** 기존 메모리를 먼저 해제하지 않으면 **메모리 누수**가 됩니다.

---

### 4.2 append - 뒤에 이어 붙이기

```cpp
void append(const char* s) {
	int tlength = length + (Size_type)strlen(s);   // 합친 길이
	char* t = new char[tlength + 1];               // 1. 더 큰 메모리를 새로 잡고
	strcpy_s(t, length + 1, buf);                  // 2. 기존 내용을 옮겨 담고
	strcat_s(t, tlength + 1, s);                   // 3. 새 내용을 이어 붙인다

	delete[] buf;                                  // 4. 예전 메모리 해제
	buf = t;                                       // 5. 새 메모리로 교체
	length = tlength;
}
```

```
before:  buf ──▶ ["Hello!\0"]           (7바이트)

step1:   t   ──▶ [.............]        (12바이트 새로 할당)
step2:   t   ──▶ ["Hello!\0.....]
step3:   t   ──▶ ["Hello!12345\0"]
step4:   buf 가 가리키던 7바이트 해제
step5:   buf ──▶ ["Hello!12345\0"]
```

- 기존 메모리를 **늘릴 수는 없기 때문에**, 항상 "더 큰 것을 새로 잡고 → 옮기고 → 예전 것 버리기" 순서입니다.
- `std::string`, `std::vector` 도 내부적으로 똑같이 동작합니다.

---

## 5. 얕은 복사 - 프로그램이 죽는 이유

### 5.1 복사 생성자가 없으면?

```cpp
class String
{
	char* buf;
	Size_type length;
public:
	String(const char* s = "") { ... new char[...] ... }
	// 복사 생성자가 없다!
	~String() { delete[] buf; }
	const char* c_str()const { return buf; }
};
int main()
{
	String s("Hello!");
	String s2 = s;          // 얕은 복사 : buf 주소만 복사된다

	cout << s.c_str() << endl;
	cout << s2.c_str() << endl;
}                           // 소멸자가 두 번 delete[] -> 크래시!
```

**실행 결과**
```
Hello!
Hello!
(그 다음 프로그램이 비정상 종료됨)
```

**무슨 일이 일어났나**

```
String s("Hello!");
    s.buf ──────────▶ ["Hello!\0"]   (힙에 할당된 메모리)

String s2 = s;   <- 컴파일러가 만든 복사 생성자가 "주소만" 복사
    s.buf  ─────┐
                ├──▶ ["Hello!\0"]    <- 두 객체가 같은 메모리를 가리킨다!
    s2.buf ─────┘

} 블록 끝
    ~String()  s2.buf 를 delete[]  ──▶ 메모리 해제됨
    ~String()  s.buf  를 delete[]  ──▶ 이미 해제된 메모리를 또 해제! (크래시)
```

| 문제 이름 | 설명 |
|-----------|------|
| **이중 해제 (double free)** | 같은 메모리를 두 번 `delete` → 프로그램 중단 |
| **dangling pointer** | 한쪽이 먼저 소멸하면 다른 쪽은 **사라진 메모리**를 가리킨다 |
| **의도치 않은 공유** | `s2` 를 고치면 `s` 도 같이 바뀐다 |

> ### 컴파일러가 자동으로 만들어 주는 것들
>
> 클래스에 아무것도 안 써도 컴파일러는 다음을 만들어 줍니다.
>
> | 함수 | 자동 생성 동작 |
> |------|----------------|
> | 기본 생성자 | 아무것도 안 함 (다른 생성자가 없을 때만 생성) |
> | **복사 생성자** | **멤버를 하나씩 그대로 복사 (얕은 복사)** |
> | **복사 대입 연산자** | **멤버를 하나씩 그대로 복사 (얕은 복사)** |
> | 소멸자 | 아무것도 안 함 |
>
> 멤버가 `int`, `double` 뿐이면 얕은 복사로 충분합니다.
> **포인터 멤버가 있으면 반드시 직접 만들어야 합니다.**

---

## 6. 깊은 복사 - 복사 생성자 만들기

### 6.1 복사 생성자의 모양

```cpp
String(const String& arg) :buf(nullptr), length(arg.length)
{
	buf = new char[length + 1];              // 새 메모리를 따로 확보 -> 깊은 복사
	strcpy_s(buf, length + 1, arg.buf);
}
```

**복사 생성자의 형태**

```cpp
클래스이름(const 클래스이름& arg)
```

| 부분 | 이유 |
|------|------|
| `const` | 원본을 바꾸지 않겠다는 약속 |
| `&` (참조) | **값으로 받으면 복사 생성자를 부르려고 또 복사 생성자를 부르는 무한 재귀**가 된다 |

---

### 6.2 깊은 복사의 그림

```
String s("ABC");
    s.buf  ──────▶ ["ABC\0"]

String s2 = s;   <- 직접 만든 복사 생성자
    s.buf  ──────▶ ["ABC\0"]    <- 각자 자기 메모리
    s2.buf ──────▶ ["ABC\0"]

} 블록 끝
    ~String()  s2.buf 해제  ──▶ 자기 것만 해제 (안전)
    ~String()  s.buf  해제  ──▶ 자기 것만 해제 (안전)
```

---

### 6.3 복사 생성자가 호출되는 3가지 경우

`ex6.cpp` **[단계 7]** 을 실행하면 `"copy Point()"` 가 언제 찍히는지 직접 셀 수 있습니다.

```cpp
class Point
{
	int x, y;
public:
	Point(int x = 0, int y = 0) :x(x), y(y) { cout << "Point() " << endl; }
	Point(const Point& arg) :x(arg.x), y(arg.y) { cout << "copy Point() " << endl; }
	void Print()const { cout << x << ',' << y << endl; }
};
void PrintPoint(Point arg)     // 2. 함수의 인수(값 객체)로 전달
{
	arg.Print();
}
Point pt;
Point CreatePoint()
{
	return pt;                 // 3. 객체가 리턴(값 객체)
}
int main()
{
	Point pt1(2, 3);
	Point pt2 = pt1;   //1. 경우 : 다른 객체로 초기화

	PrintPoint(pt1);   //2. 함수의 인수(값 객체)로 전달

	CreatePoint();     //3. 객체가 리턴(값 객체)
}
```

**예상 출력**
```
Point()            <- 전역 pt
Point()            <- pt1
copy Point()       <- (1) pt2 = pt1
copy Point()       <- (2) PrintPoint 의 매개변수 arg
2,3
copy Point()       <- (3) CreatePoint 의 반환값
```

> 마지막 `copy Point()` 는 컴파일러 최적화(RVO)로 **사라질 수도 있습니다.** 전역 `pt` 를 복사해 돌려주는 형태라 최적화가 제한적이지만, 출력이 다르더라도 정상입니다.

---

### 6.4 참조로 바꾸면 복사가 사라진다

`ex6.cpp` **[단계 6b]** 는 위 코드에서 **값을 참조로만** 바꾼 것입니다.

```cpp
void PrintPoint(Point& arg)    // 값 -> 참조 : 복사가 일어나지 않는다
{
	arg.Print();
}
Point pt;
Point& CreatePoint()           // 값 -> 참조 : 복사가 일어나지 않는다
{
	return pt;                 // 전역 객체라서 참조 반환이 안전하다 (class7.md)
}
int main()
{
	Point pt1(2, 3);
	Point& pt2 = pt1;  //1. 참조로 받으면 복사 생성자가 안 불린다

	PrintPoint(pt1);   //2. 참조 매개변수

	CreatePoint() = Point(0, 0);   // 참조 반환이므로 왼쪽 값이 된다
	pt.Print();
}
```

**예상 출력**
```
Point()
Point()
2,3
Point()
0,0
```

- `"copy Point()"` 가 **한 번도 찍히지 않습니다.**
- 이것이 [class2.md](class2.md) 에서 배운 **`const 형식&` 매개변수를 쓰라**는 조언의 실제 근거입니다.
- `CreatePoint() = Point(0,0);` 은 **참조를 반환했기 때문에** 왼쪽 값이 됩니다. 자세한 원리는 [class7.md](class7.md) 에서 다룹니다.

> **주의** : `String s2 = s;` 는 **대입이 아니라 초기화**입니다. **복사 생성자**가 불립니다.
> ```cpp
> String s2 = s;    // 복사 생성자 (s2 를 새로 만드는 중)
> String s3;
> s3 = s;           // 복사 대입 연산자 (s3 는 이미 존재)  <- class7.md
> ```
> 이 둘을 구분하는 것이 매우 중요합니다.

---

## 7. 아직 안 만든 기능 막아 두기

```cpp
class String
{
	...
private:
	const String& operator=(const String& arg);   // 선언만 하고 정의 안 함 -> 대입 금지
public:
	...
};
```

```cpp
String s2 = s;    // O : 복사 생성자
s2 = s;           // X : 에러 - operator= 가 private
```

**왜 이렇게 하나?**

- 대입 연산자를 안 만들면 컴파일러가 **얕은 복사 버전을 자동으로 만들어 버립니다.**
- 그러면 `s2 = s;` 가 조용히 크래시를 일으킵니다.
- **아예 못 쓰게 막아 두는 것**이 훨씬 안전합니다.

| 방법 | 문법 | 비고 |
|------|------|------|
| 전통적 방법 | `private:` 에 **선언만** | 이 수업 예제 방식 |
| C++11 방식 | `String& operator=(const String&) = delete;` | 에러 메시지가 훨씬 명확 |

> 대입 연산자는 **[class7.md](class7.md)** 에서 직접 만듭니다.

---

## 8. 완성 코드 (ex6.cpp 맨 위)

```cpp
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

class String
{
public:
	typedef unsigned int Size_type;
private:
	char* buf;
	Size_type length;
	const String& operator=(const String& arg);   // 대입 금지
public:
	String(const char* s = "") :buf(nullptr), length(0) {
		length = (Size_type)strlen(s);
		buf = new char[length + 1];       // +1 은 널 종료 문자('\0') 자리
		strcpy_s(buf, length + 1, s);
	}
	String(const String& arg) :buf(nullptr), length(arg.length)   // 복사 생성자
	{
		buf = new char[length + 1];       // 새 메모리를 따로 확보 -> 깊은 복사
		strcpy_s(buf, length + 1, arg.buf);
	}
	~String() { delete[] buf; }
	Size_type size()const { return length; }
	char at(Size_type i)const { return buf[i]; }
	void append(const char* s) { ... }
	void assign(const char* s) { ... }
	const char* c_str()const { return buf; }
};

int main()
{
	String s("Hello!");

	for (String::Size_type i = 0; i < s.size(); ++i)
		cout << s.at(i) << " ";
	cout << endl;

	s.append("12345");
	cout << s.c_str() << endl;      // Hello!12345

	s.assign("ABC");
	cout << s.c_str() << endl;      // ABC

	String s2 = s;                  // 복사 생성자 호출 -> 깊은 복사
	cout << s2.c_str() << endl;     // ABC
	//s2 = s;                       // X - 에러 : operator= 를 private 으로 막아 두었다
}
```

**예상 출력**
```
H e l l o ! 
Hello!12345
ABC
ABC
```

---

## 9. 파일 분리 버전 — app/string

`example/src/app/string/String.h` 와 `String.cpp` 에는 같은 클래스가 **헤더/소스로 분리**되어 있고, **대입 연산자까지 구현**되어 있습니다.

```cpp
// String.h - 선언
#pragma once
class String
{
public:
	typedef unsigned int Size_type;
private:
	char* buf;
	Size_type length;
public:
	String(const char* s = "");
	String(const String& arg);
	~String();
	Size_type size()const;
	char at(Size_type i)const;
	void append(const char* s);
	void assign(const char* s);
	const char* c_str()const;
	const String& operator=(const String& arg);
};
```

> ### 3의 법칙 (Rule of Three)
> **소멸자 / 복사 생성자 / 복사 대입 연산자** 중 **하나라도 직접 만들어야 한다면, 나머지 둘도 만들어야 한다.**
>
> 이유는 간단합니다. 셋 중 하나가 필요하다는 것은 **클래스가 직접 관리하는 자원(동적 메모리 등)이 있다**는 뜻이고, 그러면 나머지 둘의 자동 생성 버전(얕은 복사)은 반드시 문제를 일으키기 때문입니다.
>
> (C++11 이후로는 이동 생성자·이동 대입까지 포함해 **5의 법칙**이라고도 합니다.)

빌드 방법은 **[class5.md](class5.md) 6.4절** 과 같습니다.

---

## 10. 정리 표

| 주제 | 내용 |
|------|------|
| RAII | 생성자에서 자원 획득, 소멸자에서 해제 → 잊을 수 없다 |
| `length + 1` | 널 종료 문자 `'\0'` 자리 |
| `assign` | 해제 → 새로 할당 → 복사 |
| `append` | 더 큰 것 할당 → 옮김 → 예전 것 해제 → 교체 |
| 자동 생성 복사 생성자 | **멤버를 그대로 복사 (얕은 복사)** |
| 얕은 복사의 문제 | 같은 메모리를 두 객체가 가리킴 → **이중 해제로 크래시** |
| 깊은 복사 | 새 메모리를 할당하고 **내용**을 복사 |
| 복사 생성자 형태 | `String(const String& arg)` — `const` 와 `&` 둘 다 필수 |
| `&` 가 필요한 이유 | 값으로 받으면 복사 생성자의 **무한 재귀** |
| 복사 생성자 호출 3경우 | 초기화 / 값 전달 / 값 반환 |
| `String s2 = s;` | **복사 생성자** (대입 아님!) |
| 기능 막기 | `private` 에 선언만 (또는 C++11 의 `= delete`) |
| **3의 법칙** | 소멸자·복사생성자·복사대입 중 하나가 필요하면 셋 다 만든다 |

---

## 11. 연습 문제 (스스로 풀어보기)

1. `String s("Hi");` 를 만들고 `s.size()`, `s.at(0)`, `s.c_str()` 를 출력해 보세요.
2. `append` 를 세 번 연달아 호출하고 결과와 길이를 출력해 보세요.
3. 복사 생성자를 **주석 처리**하고 `String s2 = s;` 를 실행해 보세요. (디버그 모드에서 어떤 창이 뜨는지 확인)
4. 복사 생성자 안에 `cout << "copy String()" << endl;` 을 넣고, 복사 생성자가 호출되는 **3가지 경우**를 각각 재현해 보세요.
5. `void Print(String s)` (값 전달) 과 `void Print(const String& s)` (참조 전달) 을 만들고, 복사 생성자가 몇 번 불리는지 비교하세요.
6. `int* data` 를 멤버로 갖는 `class IntArray` 를 만들고, 3의 법칙을 지켜 완성해 보세요.
7. `operator=` 의 `private` 선언을 지우고 `s2 = s;` 를 실행해 보세요. 컴파일은 되지만 실행 시 무슨 일이 생기나요?

---

이 강의 자료는 **example/src/ex6.cpp** 와 **example/src/app/string/** 예제를 바탕으로 구성했습니다. 이번 시간의 핵심은 **"포인터 멤버가 있으면 복사 생성자를 직접 만들어야 한다"** 는 것 하나입니다. 단계 5의 크래시를 **직접 실행해서 눈으로 확인**하고 넘어가면, 이 규칙을 평생 잊지 않게 됩니다.
