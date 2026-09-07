# C++ 기초 강의 2 - ex2.cpp 코드를 통한 함수의 확장과 namespace

이 강의는 **example/src/ex2.cpp** 에 담긴 예제들을 바탕으로, C++ 에서 **함수가 어떻게 강해졌는지**와 **namespace · string · auto** 를 정리한 내용입니다. 파일 아래쪽이 기초, 위로 갈수록 발전된 내용입니다.

---

## 0. 두 번째 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- 참조 매개변수(`int&`)로 `Swap` 을 만들고, C 의 포인터 방식과 비교할 수 있다.
- **함수 오버로딩(중복)** 으로 같은 이름의 함수를 여러 개 만들 수 있다.
- **매개변수 기본값**을 지정해 호출을 간단하게 만들 수 있다.
- `const 참조 매개변수`가 왜 필요한지 설명할 수 있다.
- 매크로(`#define`)의 부작용과 `inline` 함수의 차이를 안다.
- `namespace` 를 정의하고 `::`, `using` 으로 접근할 수 있다.
- `std::string` 과 `const char*` 의 차이를 안다.
- `auto` 와 `int()` (기본값 생성)를 사용할 수 있다.

### 0.2 수업 진행 순서 제안

1. 단계 1 : 참조 매개변수 `Swap`
2. 단계 2 : 함수 오버로딩 → 기본 인자 → 임시 객체
3. 단계 3~4 : 구조체를 값 / const 참조로 전달
4. 단계 5~6 : 매크로 vs inline
5. 단계 7~9 : namespace
6. 단계 10~11 : string
7. 단계 12~13 : auto, int()

---

## 1. 참조 매개변수 - Swap 다시 만들기

### 1.1 C 의 방식 vs C++ 의 방식

```cpp
#include <iostream>
using namespace std;

void Print(int a, int b)
{
	cout << a << ',' << b << endl;
}
void Swap(int& a, int& b)   // C 에서는 void Swap(int* a, int* b)
{
	int t = a;
	a = b;
	b = t;
}
int main()
{
	int a = 10;
	int b = 20;

	Print(a, b);
	Swap(a, b);   // C 에서는 Swap(&a, &b);
	Print(a, b);
}
```

**예상 출력**
```
10,20
20,10
```

**C 로 쓴 같은 코드**

```c
void Swap(int* a, int* b)
{
	int t = *a;    // * 를 붙여야 한다
	*a = *b;
	*b = t;
}
Swap(&a, &b);      // & 를 붙여야 한다
```

| 비교 | C (포인터) | C++ (참조자) |
|------|-----------|--------------|
| 함수 정의 | `void Swap(int* a, int* b)` | `void Swap(int& a, int& b)` |
| 함수 안에서 | `*a`, `*b` 로 접근 | `a`, `b` 로 그냥 접근 |
| 호출할 때 | `Swap(&a, &b);` | `Swap(a, b);` |
| 실수 위험 | `*` 나 `&` 를 빠뜨리면 버그 | 문법적으로 빠뜨릴 수 없다 |
| `nullptr` 검사 | 필요할 수 있다 | **필요 없다** (참조는 항상 유효) |

> **핵심** : 참조 매개변수는 **원본을 직접 바꿉니다.** 호출하는 쪽 코드만 봐서는 값이 바뀌는지 알 수 없으므로, **바꾸지 않을 매개변수에는 반드시 `const` 를 붙이는 습관**이 중요합니다.

---

## 2. 함수 오버로딩(function overloading)

### 2.1 이름이 같고 매개변수가 다른 함수

```cpp
#include <iostream>
using namespace std;

//function overloading(함수 중복) <==> function overriding(함수 재정의)
void Print(int a, int b, int c)
{
	cout << a << ',' << b << ',' << c << endl;
}
void Print(const char* s)
{
	cout << s << endl;
}
int main()
{
	Print(10, 20, 30);
	Print("Hello!");
}
```

**예상 출력**
```
10,20,30
Hello!
```

- C 에서는 같은 이름의 함수를 두 개 만들 수 없어서 `PrintInt`, `PrintString` 처럼 이름을 다르게 지어야 했습니다.
- C++ 는 **매개변수의 개수나 형식이 다르면** 같은 이름을 써도 됩니다.

| 구분 | 뜻 | 언제 배우나 |
|------|-----|-------------|
| **overloading (중복)** | 같은 이름 + 다른 매개변수 | 지금 |
| **overriding (재정의)** | 부모의 가상 함수를 자식이 다시 정의 | [class9.md](class9.md) |

**오버로딩이 성립하는 조건**

| 무엇이 다르면 | 오버로딩 성립? |
|---------------|----------------|
| 매개변수 **개수** | O |
| 매개변수 **형식** | O |
| 매개변수 **순서** (형식이 다를 때) | O |
| **반환형만** 다름 | **X** (에러) |
| 매개변수 **이름만** 다름 | **X** (에러) |

---

### 2.2 매개변수 기본값(default argument)

```cpp
#include <iostream>
using namespace std;

void Print(int a=0, int b=0, int c=0)
{
	cout << a << ',' << b << ',' << c << endl;
}

int main()
{
	Print(10, 20, 30);   // 10,20,30
	Print(100);          // 100,0,0
	Print(55,66);        // 55,66,0
}
```

**예상 출력**
```
10,20,30
100,0,0
55,66,0
```

**규칙**

- 기본값은 **오른쪽부터** 채워야 합니다.
  ```cpp
  void f(int a, int b = 0, int c = 0);   // O
  void f(int a = 0, int b, int c);       // X - 에러
  ```
- 기본값은 **선언에 한 번만** 적습니다. (헤더에 선언하고 소스에 정의할 때는 헤더 쪽에만)
- 기본값을 쓰면 오버로딩 여러 개를 **함수 하나로 줄일 수** 있습니다.

---

### 2.3 임시 객체로 인자를 묶어서 넘기기

```cpp
#include <iostream>
using namespace std;

struct DataParam
{
	int a = 0;   // 멤버 기본값 (C++11)
	int b = 0;
	int c = 0;
};
void Print(DataParam d)
{
	cout << d.a << ',' << d.b << ',' << d.c << endl;
}

int main()
{
	Print(DataParam());          // 0,0,0
	Print(DataParam{});          // 0,0,0
	Print(DataParam{10,20,30});  // 10,20,30
}
```

**예상 출력**
```
0,0,0
0,0,0
10,20,30
```

- `DataParam()` 은 **이름 없는 임시 객체**를 그 자리에서 만들어 넘깁니다.
- 매개변수가 많아질 때 **구조체 하나로 묶어서 넘기는** 실무 기법입니다.
- 임시 객체의 생명 주기는 **[class8.md](class8.md)** 에서 자세히 다룹니다.

---

## 3. 구조체를 함수에 전달하는 두 가지 방법

### 3.1 값으로 전달 - 통째로 복사된다

```cpp
#include <iostream>
using namespace std;
struct Data
{
	char name[20];
	int age;
	char addr[50];
};
void Print(int data)
{
	cout << "int : " << data << endl;
}
void Print(Data data)      // 구조체 전체가 복사된다
{
	cout << "name : " << data.name << ",   " << "age:" << data.age << ','
		<< "addr : " << data.addr << endl;
}
int main()
{
	Print(10);
	Data d = { "홍길동", 25, "경기 파주시" };
	Print(d);
}
```

**예상 출력**
```
int : 10
name : 홍길동,   age:25,addr : 경기 파주시
```

- 이 `Data` 는 **74바이트**입니다. 함수를 부를 때마다 74바이트가 복사됩니다.
- 구조체가 커질수록 낭비가 커집니다.

---

### 3.2 const 참조로 전달 - 복사 없이, 수정도 막고

```cpp
void Print(const Data& data) //** 복사 비용이 없고, 원본 수정도 막힌다
{
	//data.age = 100;   // X - const 라서 수정 불가
	cout << "name : " << data.name << ",   " << "age:" << data.age << ','
		<< "addr : " << data.addr << endl;
}
```

| 전달 방식 | 복사 비용 | 원본 수정 | 언제 쓰나 |
|-----------|-----------|-----------|-----------|
| `Data data` (값) | **크다** | 불가 (사본을 고침) | 작은 자료형(`int`, `double`) |
| `Data& data` (참조) | 없음 | **가능** | 함수가 원본을 바꿔야 할 때 |
| `const Data& data` | 없음 | 불가 | **읽기만 할 때 (가장 많이 쓴다)** |
| `const Data* data` | 없음 | 불가 | C 스타일 / `nullptr` 을 허용할 때 |

> **실무 규칙**
> 구조체·클래스를 함수에 넘길 때는 **일단 `const 형식&` 를 쓴다**고 외워 두세요.
> 이 습관 하나가 성능과 안정성을 동시에 챙깁니다.

---

## 4. 매크로 vs inline 함수

### 4.1 매크로 `#define Max` 의 함정

```cpp
#include <iostream>
using namespace std;

#define Max(a,b) (( (a) > (b)) ? (a) : (b))

int main()
{
	int n1 = 10;
	int n2 = 20;
	cout << Max(++n1, ++n2) << endl;
	cout << n1 << ',' << n2 << endl;   // n2 가 두 번 증가한다!
}
```

**예상 출력**
```
22
11,22
```

**왜 이렇게 되나?** 매크로는 **단순 문자열 치환**입니다.

```cpp
Max(++n1, ++n2)
// 아래처럼 그대로 펼쳐진다
(( (++n1) > (++n2)) ? (++n1) : (++n2))
//    ↑ 11      ↑ 21      비교 후 다시 ++n2 → 22
```

- `++n2` 가 **두 번 실행**되어 n2 가 22 가 되었습니다.
- 이것을 **매크로의 부작용(side effect)** 이라고 합니다.

---

### 4.2 inline 함수로 해결

```cpp
#include <iostream>
using namespace std;

inline int Max(int a, int b)
{
	return (((a) > (b)) ? (a) : (b));
}

int main()
{
	int n1 = 10;
	int n2 = 20;
	cout << Max(++n1, ++n2) << endl;   // ++ 가 한 번씩만 실행된다
	cout << Max(++n1, ++n2) << endl;
	cout << Max(++n1, ++n2) << endl;
	cout << n1 << ',' << n2 << endl;
}
```

**예상 출력**
```
21
22
23
13,23
```

- 함수이므로 **매개변수가 딱 한 번만 평가**됩니다.
- `inline` 은 "가능하면 함수 호출 비용 없이 코드를 그 자리에 펼쳐 달라"는 **요청**입니다.

| 비교 | `#define` 매크로 | `inline` 함수 |
|------|------------------|---------------|
| 처리 시점 | 전처리기 (컴파일 전) | 컴파일러 |
| 형식 검사 | **없음** | 있다 |
| 부작용 | 인자가 여러 번 평가될 수 있다 | 없다 |
| 디버깅 | 어렵다 (원본이 안 보임) | 쉽다 |
| 속도 | 빠르다 | **같거나 더 빠르다** |

> **결론** : C++ 에서는 값을 계산하는 매크로 대신 **`inline` 함수를 쓰세요.**

---

## 5. namespace - 이름 충돌 막기

### 5.1 namespace 정의와 `::` 로 접근하기

```cpp
#include <iostream>
namespace MyName
{
	int x = 100;
	void Print(int data)
	{
		std::cout << "MyName::Print() : " << data << std::endl;
	}
}
int main()
{
	std::cout << "Hello!" << std::endl;
	std::cout << "x : " << MyName::x << std::endl;
	MyName::Print(100);
}
```

**예상 출력**
```
Hello!
x : 100
MyName::Print() : 100
```

- `namespace` 는 **이름을 담는 서랍**입니다.
- 다른 사람이 만든 `Print` 와 내 `Print` 가 부딪히지 않도록 **성(姓)을 붙여 주는 것**이라고 생각하면 쉽습니다.
- `::` 는 **범위 지정 연산자(scope resolution operator)** 입니다.

---

### 5.2 using 의 3가지 형태

```cpp
// (1) 매번 :: 로 접근            - 가장 안전, 가장 길다
MyName::Print(100);

// (2) 필요한 이름만 끌어오기      - 권장
using MyName::x;
using MyName::Print;
Print(100);

// (3) 전체를 끌어오기            - 편하지만 충돌 위험
using namespace MyName;
Print(100);
```

| 방식 | 장점 | 단점 |
|------|------|------|
| `MyName::Print()` | 어디 것인지 명확 | 매번 쓰기 번거롭다 |
| `using MyName::Print;` | 필요한 것만 짧게 | 선언을 여러 줄 써야 한다 |
| `using namespace MyName;` | 가장 짧다 | **이름 충돌 위험** |

> **`using namespace std;` 는 왜 쓰나요?**
> 수업 예제처럼 짧은 코드에서는 편해서 씁니다.
> 하지만 **헤더 파일(.h)에는 절대 쓰면 안 됩니다.** 그 헤더를 포함하는 모든 파일에 영향을 주기 때문입니다.

---

### 5.3 이 프로젝트에서의 예

`example/src/app/point/Point.h` 에서 `Point` 클래스를 `MySP` 라는 namespace 에 넣은 것을 볼 수 있습니다.

```cpp
namespace MySP
{
	class Point { ... };
}
```
```cpp
MySP::Point pt(2, 3);   // 사용할 때
```

---

## 6. std::string - C 문자열의 불편함이 사라진다

### 6.1 `const char*` 와 `std::string` 의 차이

```cpp
#include <iostream>
#include <string>
using namespace std;

int main()
{
	const char* s1 = "Hello!";
	string s2 = "Hello!";

	//s1 += "?";   // X - 포인터는 문자열을 이어 붙일 수 없다
	s2 += "?";     // O - string 은 가능
	cout << s1 << endl;
	cout << s2 << endl;
}
```

**예상 출력**
```
Hello!
Hello!?
```

---

### 6.2 string 은 비교·인덱싱·수정이 자유롭다

```cpp
#include <iostream>
#include <string>
using namespace std;

int main()
{
	string s = "Hello!";

	cout << s << endl;
	if (s == "Hello!")            // strcmp 없이 == 로 비교
		cout << "true" << endl;
	cout << s[0] << ',' << s[1] << endl;

	s = "Hi!";
	s[0] = 'Z';
	cout << s << endl;
}
```

**예상 출력**
```
Hello!
true
H,e
Zi!
```

| 작업 | C (`char[]` / `char*`) | C++ (`string`) |
|------|------------------------|----------------|
| 길이 구하기 | `strlen(s)` | `s.size()` 또는 `s.length()` |
| 복사 | `strcpy_s(dst, n, src)` | `dst = src;` |
| 이어 붙이기 | `strcat_s(dst, n, src)` | `dst += src;` |
| 비교 | `strcmp(a, b) == 0` | `a == b` |
| 크기 관리 | **직접** (버퍼 오버플로 위험) | **자동** |
| 필요한 헤더 | `<cstring>` | `<string>` |

> **`string` 은 어떻게 만들어졌나?**
> 이 수업에서는 **[class6.md](class6.md)** 와 **[class7.md](class7.md)** 에서 `string` 과 똑같이 동작하는 `String` 클래스를 **직접 만들어 봅니다.**
> 그 과정에서 복사 생성자·소멸자·대입 연산자를 배우게 됩니다.

---

## 7. auto 와 기본값 생성

### 7.1 `int()` , `double()` - 기본값을 만드는 문법

```cpp
#include <iostream>
using namespace std;

int main()
{
	cout << int() << endl;      // 0
	cout << double() << endl;   // 0

	int n = int();
	double d = double();
	cout << n << endl;
	cout << d << endl;
}
```

**예상 출력**
```
0
0
0
0
```

- `형식()` 은 그 형식의 **기본값(0 또는 빈 값)** 을 만드는 문법입니다.
- 템플릿에서 "어떤 형식이 올지 모를 때 기본값으로 초기화" 할 때 매우 유용합니다. ([class14.md](class14.md))

---

### 7.2 auto - 형식을 컴파일러가 정해 준다

```cpp
auto v = 10L;      //C언어의 auto 변수 / C++언어의 auto type
cout << v << endl;
cout << typeid(v).name() << endl;   // long

auto v2 = 5.5F;
cout << v2 << endl;
cout << typeid(v2).name() << endl;  // float
```

**예상 출력**
```
10
long
5.5
float
```

| 주의 | 설명 |
|------|------|
| `auto` 는 **반드시 초기값이 있어야** 한다 | `auto x;` 는 에러 |
| C 의 `auto` 와 뜻이 완전히 다르다 | C 에서는 "자동 저장 기간"이라는 거의 안 쓰이는 키워드였다 |
| 형식이 안 보여서 헷갈릴 수 있다 | 형식 이름이 길 때(`vector<int>::iterator`) 특히 유용하다 |

**`typeid(v).name()`** 은 그 변수의 **실제 형식 이름**을 문자열로 알려 줍니다. 형식을 확인할 때 아주 유용합니다.

---

### 7.3 임시 객체와 auto

```cpp
#include <iostream>
using namespace std;
struct Data
{
	int a = 0;
	int b = 0;
};
int main()
{
	cout << Data().a << endl;   // 임시 객체를 만들어 바로 사용
	cout << Data().b << endl;

	auto v = Data();
	cout << v.a << ',' << v.b << endl;

	auto v2 = Data{ 10,20 };
	cout << v2.a << ',' << v2.b << endl;
}
```

**예상 출력**
```
0
0
0,0
10,20
```

- `int a = 0;` 처럼 **구조체 멤버에 기본값을 적을 수 있는 것**은 C++11 부터입니다. C 에서는 불가능했습니다.

---

## 8. 완성 코드 (ex2.cpp 맨 위)

핵심 부분만 발췌합니다.

```cpp
int main()
{
	int a = 10, b = 20;
	Print(a, b);        // 10,20,0
	Swap(a, b);
	Print(a, b);        // 20,10,0
	Print("Hello!");    // 오버로딩된 다른 Print 가 호출된다

	cout << Max(++a, ++b) << endl;

	MyName::Print(MyName::x);   // namespace 로 이름 충돌을 막는다

	string s = "Hello!";
	s += "?";
	cout << s << ' ' << s[0] << endl;

	auto v = Data{ 10, 20 };    // auto : 컴파일러가 형식을 알아서 정한다
	cout << v.a << ',' << v.b << endl;
	cout << int() << ',' << double() << endl;   // 0, 0
}
```

**예상 출력**
```
10,20,0
20,10,0
Hello!
21
MyName::Print() : 100
Hello!? H
10,20
0,0
```

> **`21` 이 나오는 이유** : `Swap` 후 `a=20, b=10` 입니다. `Max(++a, ++b)` 는 `Max(21, 11)` 이므로 21 입니다.

---

## 9. 정리 표

| 주제 | 내용 |
|------|------|
| 참조 매개변수 | `void Swap(int& a, int& b)` — 호출할 때 `&` 불필요 |
| 함수 오버로딩 | 같은 이름 + **다른 매개변수**. 반환형만 다르면 에러 |
| 기본 인자 | `void f(int a, int b = 0)` — **오른쪽부터** 채운다 |
| 값 전달 | 구조체가 통째로 복사 → 크면 낭비 |
| `const 형식&` | **복사 없이 + 수정 금지**. 클래스 전달의 기본 |
| 매크로 부작용 | `Max(++n1, ++n2)` 에서 `++` 가 두 번 실행 |
| inline 함수 | 형식 검사 O, 부작용 X → 매크로 대신 사용 |
| namespace | 이름 충돌 방지용 서랍. `::` 로 접근 |
| using 3형태 | `A::x` / `using A::x;` / `using namespace A;` |
| 헤더에 using 금지 | `.h` 에 `using namespace` 를 쓰면 안 된다 |
| `std::string` | `==` 비교, `+=` 연결, `[]` 인덱싱, 크기 자동 관리 |
| `int()` | 그 형식의 기본값(0) 을 만든다 |
| `auto` | 초기값을 보고 형식을 결정. **초기값 필수** |
| `typeid(v).name()` | 변수의 실제 형식 이름을 문자열로 |

---

## 10. 연습 문제 (스스로 풀어보기)

1. `void Swap(double& a, double& b)` 를 만들어 실수 두 개를 바꿔 보세요.
2. `int Add(int, int)`, `double Add(double, double)`, `string Add(string, string)` 을 오버로딩해 보세요.
3. `void PrintLine(char c = '-', int count = 20)` 을 만들고 `PrintLine();`, `PrintLine('*');`, `PrintLine('=', 10);` 을 호출해 보세요.
4. `#define Square(x) x*x` 를 만들고 `Square(2+3)` 을 출력해 보세요. 왜 25 가 아니라 11 이 나오는지 설명하고, `inline` 함수로 고쳐 보세요.
5. `namespace Math` 안에 `PI` 와 `Area(double r)` 를 만들고, 세 가지 `using` 방식으로 각각 호출해 보세요.
6. `string` 두 개를 입력받아 이어 붙이고, 길이와 첫 글자를 출력해 보세요.
7. `auto` 로 변수 5개를 선언하고 `typeid(x).name()` 으로 각각의 형식을 확인해 보세요.

---

이 강의 자료는 **example/src/ex2.cpp** 에 있는 모든 예제를 바탕으로 구성했습니다. 이번 시간의 핵심은 **`const 형식&` 매개변수**입니다. 앞으로 클래스를 다루는 거의 모든 함수에서 이 형태를 보게 되므로, 왜 복사가 없는지·왜 수정이 막히는지 확실히 이해하고 넘어가세요.
