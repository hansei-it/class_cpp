# C++ 기초 강의 15 - ex15.cpp 코드를 통한 예외 처리와 형변환·typeid

이 강의는 **example/src/ex15.cpp** 에 담긴 예제들을 바탕으로, **오류를 처리하는 표준 방법인 예외**와 **C++ 의 형변환 4가지**, 그리고 **typeid** 를 정리한 내용입니다.

---

## 0. 열다섯 번째 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- `try` / `catch` / `throw` 로 오류를 처리할 수 있다.
- `catch(...)` 의 용도와 한계를 안다.
- **예외 클래스 계층**을 만들고 부모 형식으로 받을 수 있다.
- `std::exception` 을 상속해 `what()` 으로 통일할 수 있다.
- **catch 는 구체적인 것부터** 써야 하는 이유를 안다.
- **형변환 4가지**(`static_cast`, `const_cast`, `reinterpret_cast`, `dynamic_cast`)를 구분해 쓸 수 있다.
- `typeid` 로 실제 형식을 확인할 수 있다.
- `const` 객체와 `const` 반환값이 막아 주는 것을 정리할 수 있다.

### 0.2 왜 예외가 필요한가

**C 의 방식 — 반환값으로 오류 알리기**

```c
int result = DoSomething();
if (result == -1) { /* 오류 처리 */ }
```

| 문제 | 설명 |
|------|------|
| **무시할 수 있다** | `DoSomething();` 만 쓰면 오류가 그냥 지나간다 |
| 반환값을 못 쓴다 | 실제 결과와 오류 코드를 같이 돌려줄 수 없다 |
| **생성자는 반환값이 없다** | 생성자에서 오류가 나면 알릴 방법이 없다 |
| 코드가 지저분해진다 | 모든 호출마다 `if` 검사 |

**C++ 의 방식 — 예외**

- **무시할 수 없습니다.** 처리하지 않으면 프로그램이 종료됩니다.
- 오류 처리 코드를 **한곳에 모을 수 있습니다.**
- **생성자에서도** 쓸 수 있습니다.

---

## 1. try / catch / throw

### 1.1 첫 모습

```cpp
#include <iostream>
using namespace std;

void function(int data)
{
	throw "임의의 예외!";   // 여기서 함수가 즉시 중단되고
}
int main()
{
	try {
		function(10);       // 호출한 쪽으로 되돌아온 뒤
	}
	catch (...) //최상위 예외
	{
		cout << "예외 발생!" << endl;   // 여기로 점프한다
	}
}
```

**예상 출력**
```
예외 발생!
```

| 키워드 | 하는 일 |
|--------|---------|
| `throw` | 예외를 **던진다**. 그 즉시 함수가 중단된다 |
| `try { }` | **감시 구역**. 이 안에서 던져진 예외를 잡을 수 있다 |
| `catch` | 예외를 **받아서 처리**한다 |
| `catch (...)` | **모든 예외**를 받는다 (내용은 알 수 없다) |

**실행 흐름**

```
main()
  try {
      function(10);  ──────┐
  }                        │  throw 발생!
  catch (...) { ... }  ◀───┘  스택을 되감으며(unwinding) 여기로 점프
  // 그 다음 코드는 정상 실행
```

> **스택 되감기(stack unwinding)**
> `throw` 가 발생하면 `catch` 를 만날 때까지 **함수들을 거꾸로 빠져나옵니다.**
> 이때 **지역 객체들의 소멸자가 정상적으로 호출됩니다.**
> 이것이 RAII([class6.md](class6.md))가 강력한 이유입니다. `delete` 를 직접 쓴 코드는 예외가 나면 그냥 샙니다.

---

### 1.2 어떤 형식이든 던질 수 있다

```cpp
void function(int data)
{
	throw 100;      // int 를 던진다
}
int main()
{
	try
	{
		function(1);
	}
	catch (int ex)  // 던진 형식과 맞는 catch 가 잡는다
	{
		cout << "예외 : " << ex << endl;
	}
}
```

**예상 출력**
```
예외 : 100
```

- `int`, `const char*`, 클래스 객체 등 **무엇이든** 던질 수 있습니다.
- 하지만 **클래스를 던지는 것이 좋습니다.** 정보를 담을 수 있기 때문입니다.

---

## 2. 예외 클래스 계층

### 2.1 상황별로 다른 예외 클래스 만들기

```cpp
class Exception
{
public:
	virtual const char* what()const { return "예외"; }
	virtual ~Exception() {}
};
class InputException : public Exception
{
public:
	const char* what()const { return "입력 예외"; }
};
class OperationException : public Exception
{
public:
	const char* what()const { return "연산 예외"; }
};
class OutputException : public Exception
{
public:
	const char* what()const { return "출력 예외"; }
};
```

```
        Exception
       ╱    │    ╲
Input  Operation  Output
```

- `what()` 은 **가상 함수**입니다. ([class9.md](class9.md))
- 소멸자도 **가상 소멸자**입니다.

---

### 2.2 부모 형식으로 받으면 자식 예외도 잡힌다

```cpp
void func1(bool exFlag)
{
	if (exFlag) throw InputException();
	cout << "1. 입력" << endl;
}
void func2(bool exFlag)
{
	if (exFlag) throw OperationException();
	cout << "2. 연산" << endl;
}
void func3(bool exFlag)
{
	if (exFlag) throw OutputException();
	cout << "3. 출력" << endl;
}
int main()
{
	try
	{
		func1(true);
		func2(false);
		func3(true);
	}
	catch(const Exception& ex)   // 부모 참조로 받고 what() 은 가상 함수라 자식 것이 불린다
	{
		cout << ex.what() << endl;
	}
	catch (...)
	{
		cout << "예외 : " << endl;
	}
}
```

**예상 출력**
```
입력 예외
```

> ### 반드시 `const 부모&` 로 받으세요
>
> ```cpp
> catch (Exception ex)          // X - 값으로 받으면 객체 슬라이싱! (class8.md)
> catch (const Exception& ex)   // O - 참조로 받아야 자식의 what() 이 불린다
> ```
>
> 값으로 받으면 `InputException` 이 `Exception` 으로 **잘려서** `what()` 이 "예외" 를 반환합니다.

**중요** : `func1(true)` 에서 예외가 나면 **`func2`, `func3` 은 실행되지 않습니다.**

---

## 3. std::exception 을 상속하기

### 3.1 표준 예외와 통일하기

```cpp
#include <exception>

class Exception : public exception   // <exception> 의 표준 예외를 상속
{
public:
	virtual const char* what()const { return "예외"; }
	virtual ~Exception() {}
};
...
int main()
{
	try { ... }
	catch (const exception& ex)   // 표준 예외까지 한 번에 받을 수 있다
	{
		cout << ex.what() << endl;
	}
	catch (...)
	{
		cout << "예외 : " << endl;
	}
}
```

**왜 좋은가?**

- `vector::at()`, `new`, `dynamic_cast` 등 **표준 라이브러리가 던지는 예외**도 같이 잡힙니다.
- 모두 `what()` 이라는 **같은 인터페이스**를 갖습니다.

**표준 예외 계층**

```
std::exception
 ├─ logic_error
 │   ├─ invalid_argument
 │   ├─ out_of_range        <- vector::at() 이 던진다
 │   └─ length_error
 ├─ runtime_error
 │   ├─ overflow_error
 │   └─ underflow_error
 ├─ bad_alloc               <- new 가 실패하면
 └─ bad_cast                <- dynamic_cast<참조> 가 실패하면
```

---

### 3.2 메시지를 담아 던지기

```cpp
class Exception : public exception
{
public:
	Exception(const char* msg) :exception(msg) {}   // exception(const char*) 은 MSVC 확장
	virtual const char* what()const override { return "예외"; }
	virtual ~Exception() {}
};
```

> **주의** : `exception(const char*)` 생성자는 **Visual Studio(MSVC) 전용 확장**입니다.
> 표준 C++ 에서는 `std::runtime_error` 를 상속하는 것이 이식성 있는 방법입니다.
>
> ```cpp
> #include <stdexcept>
> class MyException : public std::runtime_error
> {
> public:
>     MyException(const std::string& msg) : std::runtime_error(msg) {}
> };
> ```

---

### 3.3 ostringstream 으로 메시지 조립하기

```cpp
#include <sstream>

class OperationException : public Exception
{
	int index;
	int value;
	string message;
public:
	OperationException(const char* msg, int idx, int val) :
		Exception(msg), index(idx), value(val)
	{
		ostringstream oss;      // 문자열 스트림으로 메시지를 조립한다
		oss << "연산 예외 : " << "index : " << index << ',' << "value : " << value;
		message = oss.str();
	}
	const char* what()const override
	{
		return message.c_str();
	}
};
```

**예상 출력**
```
연산 예외 : index : 5,value : 100
```

| 요소 | 설명 |
|------|------|
| `<sstream>` | 문자열을 스트림처럼 다루는 헤더 |
| `ostringstream oss;` | 메모리에 쓰는 출력 스트림 |
| `oss << ...` | `cout` 과 똑같이 쓴다 |
| `oss.str()` | 지금까지 쓴 내용을 `string` 으로 |

> **`message` 를 멤버로 저장하는 이유**
> `what()` 은 `const char*` 를 반환합니다. 지역 변수의 주소를 반환하면 **dangling pointer** 가 됩니다. ([class7.md](class7.md))
> 그래서 `string message` 를 **멤버로 두고** `message.c_str()` 를 반환합니다.

---

## 4. catch 의 순서 - 구체적인 것부터

```cpp
int main()
{
	try
	{
		func1(false);
		func2(true);
		func3(true);
	}
	catch (const OperationException& ex)   // 자식부터
	{
		cout << ex.what() << endl;
		cout << "Log..." << endl;
	}
	catch (const exception& ex)            // 그 다음 부모
	{
		cout << ex.what() << endl;
	}
	catch (...)                            // 마지막에 전부
	{
		cout << "예외 : " << endl;
	}
}
```

**예상 출력**
```
1. 입력
연산 예외 : index : 5,value : 100
Log...
```

> ### 순서를 거꾸로 쓰면?
>
> ```cpp
> catch (const exception& ex) { ... }             // 부모를 먼저
> catch (const OperationException& ex) { ... }    // 이 줄은 영원히 실행되지 않는다
> ```
>
> `catch` 는 **위에서부터 순서대로** 검사하고 **처음 맞는 것**을 실행합니다.
> 부모 형식이 위에 있으면 자식도 거기서 다 잡혀 버립니다.
>
> Visual Studio 는 이 경우 **경고 C4286** 을 내 줍니다.

**catch 작성 순서**

```
1. 가장 구체적인 자식 예외
2. 중간 부모 예외
3. std::exception
4. catch (...)     <- 최후의 그물
```

---

## 5. C++ 의 형변환 4가지

### 5.1 왜 C 스타일 캐스팅을 쓰지 않나

```cpp
(int)d          // C 스타일 : 무엇을 하려는 건지 알 수 없다
```

| 문제 | 설명 |
|------|------|
| 의도가 안 보인다 | 값 변환인지, const 제거인지, 포인터 재해석인지 구분 안 됨 |
| **검색이 안 된다** | 코드에서 `(int)` 를 찾기 어렵다 |
| 너무 강력하다 | 하면 안 되는 변환도 조용히 해 버린다 |

C++ 는 목적별로 **4개의 캐스팅 연산자**를 제공합니다.

---

### 5.2 `static_cast` - 가장 흔한 변환

```cpp
int n = 10;
double d = 5.5;

//n = (int)d;
n = static_cast<int>(d); //1.
cout << n << ',' << d << endl;

Shape* ps = new Rect();
//Rect* pr = (Rect*)ps;
Rect* pr = static_cast<Rect*>(ps); //1. 검사 없이 내려간다
delete ps;
```

**예상 출력**
```
5,5.5
```

| 용도 | 예 |
|------|-----|
| 숫자 형식 변환 | `static_cast<int>(3.9)` |
| `void*` → 구체 포인터 | `static_cast<int*>(pv)` |
| **업캐스팅** (자식→부모) | 안전 |
| **다운캐스팅** (부모→자식) | **검사를 하지 않는다** — 위험 |

---

### 5.3 `const_cast` - const 를 떼어낸다

```cpp
int n = 10;
const int* p = &n;

cout << *p << ',' << n << endl;

//int* p2 = (int*)p;
int* p2 = const_cast<int*>(p); //2.
*p2 = 100;
cout << *p << ',' << n << endl;   // 100, 100
```

**예상 출력**
```
10,10
100,100
```

> ### 매우 조심해서 쓰세요
>
> **원래 `const` 로 선언된 것**의 const 를 떼어내고 수정하면 **정의되지 않은 동작(UB)** 입니다.
>
> ```cpp
> const int cn = 10;                  // 진짜 const
> int* p = const_cast<int*>(&cn);
> *p = 100;                           // UB! 컴파일러가 10 을 상수로 최적화했을 수 있다
> ```
>
> **정당한 용도**는 하나뿐입니다 : **원래는 const 가 아닌데 const 로 받아 온 것**을 되돌릴 때.
> (주로 const 를 고려하지 않고 만들어진 옛날 C 라이브러리에 넘길 때)

---

### 5.4 `reinterpret_cast` - 메모리를 다른 눈으로 보기

```cpp
int n = 0x44434241;   // 'A'=0x41 'B'=0x42 'C'=0x43 'D'=0x44
//char* p = (char*)&n; //3.
char* p = reinterpret_cast<char*>(&n); //3.

cout << p[0] << ',' << p[1] << ',' << p[2] << ',' << p[3] << endl;  // A,B,C,D
```

**예상 출력**
```
A,B,C,D
```

**왜 A,B,C,D 순서인가?**

```
int n = 0x44434241;   (4바이트)

리틀 엔디언(Intel/AMD) 메모리 배치:
주소:    +0    +1    +2    +3
값:     0x41  0x42  0x43  0x44
문자:   'A'   'B'   'C'   'D'
```

- x86/x64 는 **리틀 엔디언**이라 **낮은 자리 바이트가 앞**에 저장됩니다.
- `char*` 로 보면 **한 바이트씩** 읽을 수 있습니다.

> **가장 위험한 캐스팅입니다.** 네트워크 패킷 파싱, 바이너리 파일 처리 등 **정말 필요할 때만** 쓰세요.

---

### 5.5 `dynamic_cast` - 안전한 다운캐스팅

```cpp
class Shape
{
public:
	virtual ~Shape() {}      // 가상 함수가 하나라도 있어야 dynamic_cast 가 된다
	virtual void Draw()const = 0;
};
class Rect : public Shape { ... };
class Circle : public Shape
{
	int radius;
public:
	Circle(int r = 0) :radius(r) {}
	void Draw()const { cout << "Circle" << endl; }
	void PrintRadius()const { cout << "raidus : "<< radius << endl; }
};
int main()
{
	Shape* shapeList[] = { new Rect, new Circle(10), new Rect, new Rect, new Circle(5) };

	for (auto p : shapeList)
	{
		//Circle* pc = static_cast<Circle*>(p);   // 검사를 안 한다 -> 위험
		Circle* pc = dynamic_cast<Circle*>(p);    // Circle 이 아니면 nullptr
		if (pc != nullptr)
			pc->PrintRadius();
	}

	for (auto p : shapeList)
		delete p;
}
```

**예상 출력**
```
raidus : 10
raidus : 5
```

| 조건 | 설명 |
|------|------|
| **가상 함수가 필요** | 클래스에 가상 함수가 하나도 없으면 컴파일 에러 (RTTI 정보가 없어서) |
| 실패했을 때 | **포인터** → `nullptr` 반환 / **참조** → `bad_cast` 예외 |
| 비용 | 실행 시점에 형식을 확인하므로 **약간 느리다** |

---

### 5.6 참조 버전은 예외를 던진다

```cpp
void PrintRadius(Shape& s)
{
	try
	{
		Circle& c = dynamic_cast<Circle&>(s);   // 참조는 nullptr 이 없으므로
		c.PrintRadius();
	}
	catch (const exception& ex)                 // 실패를 예외로 알려 준다
	{
		cout << ex.what() << endl;
	}
}
int main()
{
	Rect r1;
	Circle c1(10);

	PrintRadius(r1);   // bad_cast
	PrintRadius(c1);   // raidus : 10
}
```

**예상 출력**
```
Bad dynamic_cast!
raidus : 10
```

- 참조에는 "실패" 를 나타낼 `nullptr` 이 없으므로 **예외**로 알립니다.

---

### 5.7 형변환 4가지 비교표

| 캐스팅 | 용도 | 안전성 | 실행 시 검사 |
|--------|------|--------|--------------|
| `static_cast<T>` | 일반적인 형식 변환, 업캐스팅 | 보통 | X |
| `const_cast<T>` | `const`/`volatile` 제거 | **위험** | X |
| `reinterpret_cast<T>` | 비트 패턴 재해석 | **매우 위험** | X |
| `dynamic_cast<T>` | **다운캐스팅** | **안전** | **O** |

> ### 실무 지침
> 1. **되도록 캐스팅을 쓰지 않는 설계**를 먼저 고민한다 (다형성으로 해결)
> 2. 필요하면 **`static_cast`** 를 기본으로
> 3. 다운캐스팅은 **반드시 `dynamic_cast`** + `nullptr` 검사
> 4. `const_cast`, `reinterpret_cast` 는 **정말 마지막 수단**

---

## 6. typeid - 실제 형식 확인하기

```cpp
#include <iostream>
using namespace std;

class Parent
{
public :
	virtual void Print()const {}   // 가상 함수가 있어야 실제 형식을 알 수 있다
};
class Child1 : public Parent { };
class Child2 : public Parent
{
public:
	void Print_Child2()const { cout << "Child2 Method" << endl; }
};
int main()
{
	Parent* parr[] = { new Child1, new Child2, new Child1, new Child2 };

	for (Parent* x : parr)
	{
		if (typeid(Child2) == typeid(*x))     // 실제 형식이 Child2 일 때만
			((Child2*)x)->Print_Child2();
	}

	for (Parent* x : parr)
		delete x;
}
```

**예상 출력**
```
Child2 Method
Child2 Method
```

| 항목 | 설명 |
|------|------|
| 헤더 | `<typeinfo>` (보통 자동 포함됨) |
| 반환 | `const type_info&` |
| `.name()` | 형식 이름 문자열 (컴파일러마다 다름) |
| `==` `!=` | 형식 비교 |
| **가상 함수 필요** | 없으면 **포인터의 형식**을 알려 준다 (실제 객체가 아니라) |

```cpp
const type_info& ti  = typeid(s);
const type_info& ti2 = typeid(String);
if (ti == ti2) cout << "class String" << endl;
```

> ### `typeid` vs `dynamic_cast`
>
> | | `typeid` | `dynamic_cast` |
> |---|----------|----------------|
> | 비교 방식 | **정확히 그 형식인가** | **그 형식이거나 그 자식인가** |
> | `Child2` 의 자식 `GrandChild` | `typeid(Child2) == typeid(*x)` → **거짓** | `dynamic_cast<Child2*>` → **성공** |
> | 권장 | 특별한 경우 | **일반적으로 이쪽** |
>
> 대부분의 경우 `dynamic_cast` 가 더 적절합니다.
> 그리고 **둘 다 안 쓰는 설계(가상 함수로 해결)가 가장 좋습니다.**

---

## 7. const 정리

```cpp
class ConstData
{
	int arr[5] = { 1,2,3,4,5 };
public:
	ConstData() :arr{ 1, 2 } {}         // 나머지는 0 으로 채워진다
	void Print()const
	{
		//arr[0] = 100;                 // X - const 멤버 함수는 멤버를 못 바꾼다
		for (auto item : arr)
			cout << item << " ";
		cout << endl;
	}
	const int* GetArray()const          // const int* 로 반환 -> 밖에서 수정 불가
	{
		return arr;
	}
	const int& GetItem() const          // const int& 로 반환
	{
		return arr[0];
	}
	bool operator==(const ConstData& arg)const
	{
		//arg.arr[0] = 100;             // X - arg 도 const 이다
		return arr == arg.arr;
	}
	void SetData(int idx, int data)     // const 가 아니므로 const 객체는 못 부른다
	{
		arr[idx] = data;
	}
};
int main()
{
	const ConstData cd1;
	cd1.Print();                        // O
	//cd1.SetData(0, 100);              // X
	//cd1.GetArray()[0] = 100;          // X
	//cd1.GetItem() = 100;              // X

	ConstData cd2;
	cd2.SetData(0, 100);                // O
	cd2.Print();
}
```

### 7.1 const 와 포인터가 겹칠 때

`ex15.cpp` **[단계 0a]** 입니다.

```cpp
int n = 10;
int* p = &n;
int** pp = &p;        // 포인터를 가리키는 포인터

const int * const  * pp2 = pp;

cout << **pp2 << endl;   // 읽기는 가능
//**pp2 = 20;            // X - 가리키는 int 가 const
//*pp2 = nullptr;        // X - 가리키는 포인터도 const
pp2 = nullptr;           // O - pp2 자신은 const 가 아니다
```

**읽는 요령 : `*` 를 기준으로 오른쪽에서 왼쪽으로**

| 선언 | 뜻 | 막히는 것 |
|------|-----|-----------|
| `int* p` | int 를 가리키는 포인터 | 없음 |
| `const int* p` | **(const int)** 를 가리키는 포인터 | `*p = 1;` |
| `int* const p` | **(const 인)** int 포인터 | `p = &n;` |
| `const int* const p` | 둘 다 const | 둘 다 |

> `const int*` 와 `int const*` 는 **같은 뜻**입니다. `*` 의 **왼쪽**에 있으면 가리키는 대상이, **오른쪽**에 있으면 포인터 자신이 const 입니다.

---

**const 가 막아 주는 것 총정리**

| 쓰는 곳 | 예 | 막아 주는 것 |
|---------|-----|--------------|
| 변수 | `const int n = 10;` | 값 변경 |
| 포인터가 가리키는 값 | `const int* p` | `*p = 1;` |
| 포인터 자체 | `int* const p` | `p = &other;` |
| 매개변수 | `void f(const T& t)` | 함수 안에서 원본 수정 |
| **멤버 함수** | `void f() const` | **멤버 변수 수정** |
| 반환값 | `const int& Get() const` | `obj.Get() = 1;` |
| 객체 | `const T obj;` | **비-const 멤버 함수 호출** |

---

## 8. 완성 코드 (ex15.cpp 맨 위)

```cpp
int main()
{
	try
	{
		func1(false);
		func2(true);     // 여기서 예외가 던져지면
		func3(true);     // 이 줄은 실행되지 않고
	}
	catch (const OperationException& ex)   // 구체적인 것부터 먼저
	{
		cout << ex.what() << endl;
		cout << "Log..." << endl;
	}
	catch (const exception& ex)            // 그 다음 부모 형식
	{
		cout << ex.what() << endl;
	}
	catch (...)                            // 마지막에 모든 예외
	{
		cout << "알 수 없는 예외" << endl;
	}

	cout << "--- 형변환 4종 ---" << endl;
	double d = 5.5;
	int n = static_cast<int>(d);                    // 1. 일반적인 형변환
	cout << n << ',' << d << endl;

	const int cn = 10;
	const int* pc = &cn;
	int* p2 = const_cast<int*>(pc);                 // 2. const 떼어내기 (위험!)
	cout << *p2 << endl;

	int m = 0x44434241;
	char* pch = reinterpret_cast<char*>(&m);        // 3. 메모리를 다른 눈으로 보기
	cout << pch[0] << ',' << pch[1] << ',' << pch[2] << ',' << pch[3] << endl;
}
```

**예상 출력**
```
1. 입력
연산 예외 : index : 5,value : 100
Log...
--- 형변환 4종 ---
5,5.5
10
A,B,C,D
```

---

## 9. 정리 표

| 주제 | 내용 |
|------|------|
| `throw` | 예외를 던지고 그 즉시 함수 중단 |
| `try` / `catch` | 감시 구역 / 예외 처리 |
| `catch (...)` | 모든 예외를 받지만 **내용은 알 수 없다** |
| 스택 되감기 | `catch` 까지 되돌아가며 **지역 객체 소멸자가 호출된다** |
| 예외 클래스 | 정보를 담을 수 있어서 좋다 |
| **`catch (const 부모&)`** | 반드시 **참조**로. 값으로 받으면 **슬라이싱** |
| `std::exception` 상속 | 표준 예외와 함께 `what()` 으로 통일 |
| `ostringstream` | 메시지 조립. `oss.str()` 로 `string` 얻기 |
| `what()` 의 반환 | 멤버 `string` 의 `c_str()` (지역 변수 금지) |
| **catch 순서** | **자식 → 부모 → `...`** |
| `static_cast` | 일반 형변환. 다운캐스팅은 검사 안 함 |
| `const_cast` | const 제거. **원래 const 를 수정하면 UB** |
| `reinterpret_cast` | 비트 재해석. **가장 위험** |
| `dynamic_cast` | **안전한 다운캐스팅.** 가상 함수 필요 |
| dynamic_cast 실패 | 포인터 → `nullptr`, 참조 → `bad_cast` |
| `typeid` | **정확히 그 형식**인지 비교 |
| `typeid` vs `dynamic_cast` | 정확히 일치 / 자식도 포함 |

---

## 10. 연습 문제 (스스로 풀어보기)

1. `int Divide(int a, int b)` 에서 `b == 0` 이면 예외를 던지고, `main` 에서 잡아 처리하세요.
2. `vector<int> v(3); v.at(10);` 을 `try/catch` 로 감싸고 `ex.what()` 을 출력해 보세요.
3. `class FileException : public std::runtime_error` 를 만들어 파일 이름을 담아 던져 보세요.
4. `catch (Exception ex)` (값으로) 와 `catch (const Exception& ex)` (참조로) 의 `what()` 결과를 비교하세요.
5. `catch (const exception&)` 를 `catch (const OperationException&)` 보다 **위에** 놓고 경고 C4286 을 확인하세요.
6. 소멸자에서 메시지를 출력하는 클래스를 만들고, 그 객체가 있는 함수에서 예외를 던져 **소멸자가 불리는지** 확인하세요.
7. `static_cast<Circle*>` 로 `Rect` 객체를 캐스팅한 뒤 `PrintRadius()` 를 불러 보세요. 무슨 값이 나오나요?
8. 7번을 `dynamic_cast` 로 바꾸고 `nullptr` 검사를 넣어 안전하게 만드세요.
9. `Shape` 에서 `virtual` 을 모두 지우고 `dynamic_cast` 를 시도해 보세요. 어떤 에러가 나나요?
10. `int n = 0x41424344;` 로 바꾸고 `reinterpret_cast<char*>` 결과가 어떻게 달라지는지 확인하세요.

---

이 강의 자료는 **example/src/ex15.cpp** 에 있는 모든 예제를 바탕으로 구성했습니다. 이번 시간의 핵심은 **`catch` 는 구체적인 것부터 쓴다**는 규칙과 **`dynamic_cast` 로 안전하게 다운캐스팅한다**는 것입니다. 그리고 예외가 발생해도 소멸자가 정상 호출된다는 사실은, 다음 시간의 **스마트 포인터**가 왜 안전한지를 설명해 줍니다.
