# C++ 기초 강의 7 - ex7.cpp 코드를 통한 반환값의 형태와 대입 연산자

이 강의는 **example/src/ex7.cpp** 와 **example/src/app/string/** 의 예제들을 바탕으로, **함수가 값·참조·포인터 중 무엇을 반환하느냐**에 따라 무엇이 달라지는지, 그리고 **복사 대입 연산자**를 직접 만드는 방법을 정리한 내용입니다.

---

## 0. 일곱 번째 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- `std::vector` 를 사용하고, 같은 것을 흉내 낸 `Array` 클래스를 만들 수 있다.
- 함수가 **값 / 참조 / 포인터** 를 반환할 때의 차이를 설명할 수 있다.
- `at()` 이 **참조를 반환하면** `arr.at(0) = 100;` 이 가능해지는 이유를 안다.
- 같은 함수를 **const / 비-const 로 오버로딩**할 수 있다.
- **복사 대입 연산자 `operator=`** 를 직접 만들고 **자기 대입**을 막을 수 있다.
- 복사 생성자와 대입 연산자의 차이를 구분할 수 있다.

### 0.2 수업 진행 순서 제안

1. 단계 1 : `vector` 사용해 보기
2. 단계 2~3 : `Array` 클래스 만들기 + 복사 생성자
3. 단계 4~6 : 값 / 참조 / 포인터 반환
4. 단계 7~8 : `at()` 의 참조 반환, const 오버로딩
5. 단계 9 + app/string : `operator=` 구현

---

## 1. std::vector - 크기가 자동으로 늘어나는 배열

### 1.1 기본 사용

```cpp
#include <iostream>
#include <vector>
using namespace std;
typedef vector<int>	Array;    // 긴 이름을 짧게

int main()
{
	Array arr;

	arr.push_back(10);   // 뒤에 하나씩 추가
	arr.push_back(20);
	arr.push_back(30);
	arr.push_back(40);
	arr.push_back(50);

	for (Array::size_type i = 0; i < arr.size(); ++i)
		cout << arr.at(i) << " ";
	cout << endl;
}
```

**예상 출력**
```
10 20 30 40 50 
```

| 멤버 | 하는 일 |
|------|---------|
| `push_back(v)` | 맨 뒤에 원소 추가 (필요하면 메모리를 알아서 늘린다) |
| `size()` | 원소 개수 |
| `at(i)` | i 번째 원소 (**범위를 벗어나면 예외**) |
| `operator[](i)` | i 번째 원소 (범위 검사 **없음**, 더 빠름) |
| `size_type` | 크기를 담는 형식 |

---

### 1.2 vector 는 복사해도 안전하다

```cpp
Array arr2 = arr;
arr2.at(0) = 100;   // arr2 만 바뀐다

for (auto x : arr)  cout << x << " ";   // 10 20 30 40 50
cout << endl;
for (auto x : arr2) cout << x << " ";   // 100 20 30 40 50
cout << endl;
```

- `vector` 는 **깊은 복사**가 이미 구현되어 있습니다. ([class6.md](class6.md))
- 우리가 만들 `Array` 도 이렇게 동작하게 만드는 것이 목표입니다.

---

## 2. Array 클래스 만들기

### 2.1 첫 버전 - 복사 생성자가 없다

```cpp
#include <iostream>
#include <vector>
using namespace std;
class Array
{
public:
	typedef unsigned int size_type;
private:
	int* buf;
	int count;
public:
	Array() :buf(nullptr), count(0)
	{
		buf = new int[100];    // 일단 100개 고정 (나중에 늘리는 것은 연습 문제)
	}
	~Array() { delete[] buf; }
	void push_back(int data)
	{
		buf[count++] = data;
	}
	int at(int idx)const  { return buf[idx]; }
	int size() const { return count; }
};
int main()
{
	Array arr;
	arr.push_back(10);
	...
	Array arr2 = arr;   // 얕은 복사! 소멸자가 두 번 delete[] -> 크래시
}
```

- `int* buf` 라는 **포인터 멤버**가 있으므로, [class6.md](class6.md) 에서 배운 문제가 그대로 발생합니다.

---

### 2.2 복사 생성자 추가

```cpp
Array(const Array& arg) :buf(nullptr), count(arg.count)
{
	buf = new int[100];                       // 새 메모리
	for (size_type i = 0; i < count; ++i)
		buf[i] = arg.buf[i];                  // 값 하나씩 복사
}
```

```cpp
Array arr2 = arr;   // 이제 안전하다
```

- 대입 연산자는 아직 `private` 로 막아 둡니다.
  ```cpp
  private:
      const Array& operator=(const Array&);   // 선언만
  ```

---

## 3. 함수는 무엇을 반환할 수 있나

### 3.1 값 / 참조 / 포인터 반환 비교

```cpp
#include <iostream>
using namespace std;

int data = 100;
int* GetAddress()      // 주소 반환
{
	return &::data;    // :: 는 전역 범위 지정 연산자
}
int& GetReference()    // 참조 반환
{
	return ::data;
}
int GetData()          // 값(복사본) 반환
{
	return ::data;
}
int main()
{
	*GetAddress() = 200;    // O : 원본 data 가 200 이 된다
	//GetReference() = 200; // O : 원본 data 가 200 이 된다
	//GetData() = 200;      // X : 복사본이라 대입 자체가 에러
	cout << *GetAddress() << endl;
	cout << GetReference() << endl;
	cout << GetData() << endl;
}
```

**예상 출력**
```
200
200
200
```

| 반환 형태 | 무엇이 돌아오나 | 대입 가능? | 위험 |
|-----------|-----------------|-----------|------|
| `int GetData()` | **복사본** | X (`GetData() = 1;` 은 에러) | 없음 (안전) |
| `int& GetReference()` | **원본 그 자체** | **O** (원본이 바뀐다) | 지역 변수를 반환하면 **치명적** |
| `int* GetAddress()` | **원본의 주소** | O (`*p = 1;`) | `nullptr` 검사 필요 |

> **`::data`**
> `::` 앞에 아무것도 없으면 **전역 범위**를 뜻합니다.
> 지역 변수 `data` 가 있을 때 전역 `data` 를 가리키려면 `::data` 라고 씁니다.

---

### 3.2 절대 하면 안 되는 것 : 지역 변수의 참조/주소 반환

```cpp
int& Bad()
{
	int local = 10;
	return local;      // X - local 은 함수가 끝나면 사라진다!
}
int main()
{
	int& r = Bad();
	cout << r << endl;   // 이미 사라진 메모리를 읽는다 (dangling reference)
}
```

**컴파일 경고**
```
warning C4172: 지역 변수 또는 임시 항목의 주소를 반환하고 있습니다.
```

> **규칙** : 참조나 포인터로 반환해도 되는 것은 **함수보다 오래 사는 것**뿐입니다.
> - 전역 변수, static 변수 → OK
> - **멤버 변수** (`this` 가 살아 있는 한) → OK ← **가장 흔한 용도**
> - 동적 할당한 것 (`new`) → OK (해제 책임은 누가?)
> - **지역 변수** → **절대 금지**

---

### 3.3 const 를 붙이면 수정을 막을 수 있다

```cpp
const int* GetAddress()   // const int* -> 가리키는 값을 바꿀 수 없다
{
	return &::data;
}
int main()
{
	*GetAddress() = 200;   // X - 에러 : const 라서 대입 불가
}
```

```cpp
const int& GetItem() const { return arr[0]; }
// obj.GetItem() = 100;   <- X, 읽기만 가능
```

---

### 3.4 구조체로도 확인해 보기

```cpp
struct Data { int item; };
Data data = { 0 };
Data* GetAddress()   { return &::data; }
Data& GetReference() { return ::data; }
Data GetData()       { return ::data; }

int main()
{
	Data d2 = { 1 };
	//*GetAddress() = d2;   // 원본이 바뀐다
	//GetReference() = d2;  // 원본이 바뀐다
	GetData() = d2;         // 임시 복사본만 바뀌고 사라진다 (의미 없음)
	cout << GetAddress()->item << endl;   // 0
}
```

- `GetData() = d2;` 는 **컴파일은 되지만 아무 효과가 없습니다.**
  (`int` 같은 기본 자료형은 에러지만, 클래스는 `operator=` 가 있어서 컴파일이 됩니다)
- 이런 실수를 막으려면 반환형에 `const` 를 붙입니다 : `const Data GetData()`

---

## 4. at() 이 참조를 반환하면 무엇이 달라지나

### 4.1 값 반환 → 읽기만 가능

```cpp
int at(int idx)const { return buf[idx]; }   // 값 반환
```
```cpp
cout << arr.at(0) << endl;   // O
arr.at(0) = 100;             // X - 에러 : 복사본에 대입할 수 없다
```

---

### 4.2 참조 반환 → 왼쪽 값(lvalue)이 된다

```cpp
int& at(int idx) { return buf[idx]; }   // <= 참조 반환
```
```cpp
arr.at(0) = 100;             // O ! 참조 반환이라 대입이 가능하다
```

**왜 되는가?**

```cpp
arr.at(0) = 100;
// at(0) 이 buf[0] 자체(원본)를 돌려주므로
buf[0] = 100;      // 와 완전히 같은 뜻이 된다
```

> **lvalue / rvalue**
> - **lvalue** : 대입문의 **왼쪽에 올 수 있는 것** (이름이 있고, 주소를 얻을 수 있다)
> - **rvalue** : 오른쪽에만 올 수 있는 것 (임시 값, 리터럴)
>
> **참조를 반환하면 lvalue 가 됩니다.** 이것이 `vector` 의 `arr[i] = 값;` 이 되는 원리입니다.

---

### 4.3 const / 비-const 오버로딩

```cpp
int& at(int idx) { return buf[idx]; }                  // 수정 가능 버전
const int& at(int idx)const { return buf[idx]; }       // 읽기 전용 버전
```

| 어떤 객체로 부르나 | 어느 함수가 불리나 | 대입 가능? |
|--------------------|--------------------|-----------|
| `Array arr;` → `arr.at(0)` | `int& at(int)` | **O** |
| `const Array& arr` → `arr.at(0)` | `const int& at(int) const` | X |

```cpp
void PrintArray(const Array& arr)   // const 객체 -> const at() 이 호출된다
{
	for (Array::size_type i = 0; i < arr.size(); ++i)
		cout << arr.at(i) << " ";
	cout << endl;
}
```

> **함수 이름과 매개변수가 같은데 오버로딩이 되나요?**
> 됩니다. **`const` 유무도 오버로딩의 조건**입니다.
> 이것은 멤버 함수에서만 가능한 특별한 규칙이며, `vector`, `string` 등 모든 STL 컨테이너가 이 방식을 씁니다.

---

## 5. 복사 대입 연산자 operator=

### 5.1 복사 생성자와 무엇이 다른가

```cpp
String s2 = s;    // 복사 생성자    : s2 를 "새로 만드는" 중
String s3;
s3 = s;           // 복사 대입 연산자 : s3 는 "이미 존재"
```

| | 복사 생성자 | 복사 대입 연산자 |
|---|-------------|------------------|
| 언제 | 객체를 **새로 만들 때** | **이미 있는** 객체에 대입할 때 |
| 왼쪽 객체의 기존 자원 | 없음 (새 객체) | **있다 → 먼저 정리해야 한다** |
| 자기 대입(`s = s`) | 일어날 수 없음 | **일어날 수 있음 → 검사 필요** |
| 반환값 | 없음 (생성자) | 보통 `클래스&` 또는 `const 클래스&` |

---

### 5.2 구현 4단계

```cpp
const Array& operator=(const Array& arg)   // 복사 대입 연산자
{
	if (this == &arg)        // 0. 자기 대입 방지 : arr = arr;
		return *this;

	delete[] buf;            // 1. 기존 메모리 해제
	buf = new int[100];      // 2. 새 메모리 확보
	count = arg.count;
	for (size_type i = 0; i < count; ++i)   // 3. 값 복사
		buf[i] = arg.buf[i];

	return *this;            // 4. 자기 자신을 반환 -> a = b = c; 가 된다
}
```

**단계별 이유**

| 단계 | 왜 필요한가 |
|------|-------------|
| 0. 자기 대입 검사 | `s = s;` 일 때 1번에서 자기 메모리를 지워 버리면 3번에서 **쓰레기를 복사**한다 |
| 1. 기존 해제 | 안 하면 **메모리 누수** |
| 2~3. 새로 할당 후 복사 | 깊은 복사 |
| 4. `*this` 반환 | `a = b = c;` 처럼 **연쇄 대입**이 가능해진다 |

---

### 5.3 자기 대입이 왜 위험한가

```cpp
String s("Hello");
s = s;             // 이런 코드를 누가 쓰냐고요? 아래처럼 숨어 있습니다.
arr[i] = arr[j];   // i == j 일 때 자기 대입!
*p1 = *p2;         // p1 == p2 일 때 자기 대입!
```

**검사가 없으면**

```cpp
const String& operator=(const String& arg)
{
	delete[] buf;                      // 1. 내 메모리를 지웠는데
	length = strlen(arg.buf);          //    arg.buf 는 방금 지운 그 메모리!
	buf = new char[length + 1];
	strcpy_s(buf, length + 1, arg.buf);  // 이미 해제된 메모리를 읽는다 -> 쓰레기
	return *this;
}
```

**검사 방법 2가지**

```cpp
if (this == &arg) return *this;   // (1) 주소 비교 - 가장 일반적
```
```cpp
bool operator==(const Array& arg)const { return this == &arg; }
...
if (*this == arg) throw "자신의 객체를 대입할 수 없음!";   // (2) 예외를 던지는 방식
```

> 두 번째 방식은 `ex11.cpp` 의 `Array` 에서 볼 수 있습니다. 예외는 **[class15.md](class15.md)** 에서 배웁니다.

---

### 5.4 app/string 의 String::operator=

```cpp
//복사 대입 연산자 : 기존 메모리를 지우고 새로 확보한 뒤 값을 옮긴다
const String& String::operator=(const String& arg)
{
	if (this == &arg)      // 자기 대입 방지 : s = s;
		return *this;

	delete[] buf;

	length = (Size_type)strlen(arg.buf);
	buf = new char[length + 1];
	strcpy_s(buf, length + 1, arg.buf);

	return *this;
}
```

**더 깔끔한 방법** — 이미 만든 함수 재사용

```cpp
const String& String::operator=(const String& arg)
{
	if (this == &arg) return *this;
	assign(arg.buf);      // assign 이 이미 해제+할당+복사를 다 한다
	return *this;
}
```

---

### 5.5 StrArray - String 을 담는 배열

`app/string/StringMain.cpp` 에서는 `String` 을 값으로 담는 배열을 만듭니다.

```cpp
class StrArray
{
	String* buf;
	size_type count;
public:
	StrArray() :buf(nullptr), count(0)
	{
		buf = new String[100];      // String 의 기본 생성자가 100번 호출된다
	}
	void push_back(const String& data)
	{
		buf[count++] = data;        // String::operator= 가 호출된다
	}
	String& at(int idx) { return buf[idx]; }
	const String& at(int idx)const { return buf[idx]; }
	...
};
int main()
{
	StrArray arr;
	arr.push_back(String("10"));
	...
	arr.at(0) = String("100");   // at() 이 참조를 반환 -> operator= 로 대입
	PrintArray(arr);             // 100 20 30 40 50
}
```

**예상 출력**
```
100 20 30 40 50 
```

> **핵심** : `String` 이 **복사 생성자와 `operator=` 를 제대로 갖췄기 때문에** 이렇게 값으로 담을 수 있습니다.
> 이것이 [class6.md](class6.md) 의 **3의 법칙**이 중요한 실질적 이유입니다.

---

## 6. 완성 코드 (ex7.cpp 맨 위)

```cpp
class Array
{
public:
	typedef unsigned int size_type;
private:
	int* buf;
	size_type count;
public:
	Array() :buf(nullptr), count(0) { buf = new int[100]; }
	Array(const Array& arg) :buf(nullptr), count(arg.count)   // 복사 생성자
	{
		buf = new int[100];
		for (size_type i = 0; i < count; ++i)
			buf[i] = arg.buf[i];
	}
	~Array() { delete[] buf; }

	const Array& operator=(const Array& arg)   // 복사 대입 연산자
	{
		if (this == &arg) return *this;        // 자기 대입 방지

		delete[] buf;
		buf = new int[100];
		count = arg.count;
		for (size_type i = 0; i < count; ++i)
			buf[i] = arg.buf[i];

		return *this;
	}

	void push_back(int data) { buf[count++] = data; }
	int& at(int idx) { return buf[idx]; }                  // 수정 가능 버전
	const int& at(int idx)const { return buf[idx]; }       // 읽기 전용 버전
	size_type size() const { return count; }
};

int main()
{
	Array arr;
	arr.push_back(10); ... arr.push_back(50);

	arr.at(0) = 100;    // at() 이 int& 를 반환하므로 대입이 된다
	PrintArray(arr);    // 100 20 30 40 50

	Array arr2 = arr;   // 복사 생성자
	Array arr3;
	arr3 = arr;         // 복사 대입 연산자
	PrintArray(arr2);
	PrintArray(arr3);
}
```

**예상 출력**
```
100 20 30 40 50 
100 20 30 40 50 
100 20 30 40 50 
```

---

## 7. 정리 표

| 주제 | 내용 |
|------|------|
| `vector` | 크기가 자동으로 늘어나는 배열. `push_back` / `size` / `at` |
| `at` vs `[]` | `at` 은 범위 검사 O(예외), `[]` 는 검사 X(빠름) |
| 값 반환 | 복사본 → 대입 불가, 안전 |
| 참조 반환 | 원본 → **대입 가능**, 지역 변수는 절대 금지 |
| 포인터 반환 | 주소 → 대입 가능, `nullptr` 검사 필요 |
| `::data` | 전역 범위 지정 연산자 |
| 지역 변수 참조 반환 | **dangling reference** — 절대 금지 |
| 참조 반환 = lvalue | `arr.at(0) = 100;` 이 되는 원리 |
| const 오버로딩 | `int& at(int)` 와 `const int& at(int) const` 를 함께 |
| 복사 생성자 vs `operator=` | 새로 만들 때 / 이미 있는 것에 대입할 때 |
| `operator=` 4단계 | 자기 대입 검사 → 해제 → 할당·복사 → `*this` 반환 |
| `*this` 반환 이유 | `a = b = c;` 연쇄 대입 |
| 자기 대입 위험 | `arr[i] = arr[j];` 처럼 **숨어 있다** |

---

## 8. 연습 문제 (스스로 풀어보기)

1. `vector<string>` 을 만들고 이름 5개를 `push_back` 한 뒤 출력해 보세요.
2. `Array` 의 `push_back` 에 **100개를 넘으면 아무것도 하지 않는** 검사를 추가하세요.
3. `Array` 의 고정 크기 100 을 없애고, 꽉 차면 **2배로 늘리는** `_grow()` 함수를 만들어 보세요.
4. `int& GetFirst()` 를 `Array` 에 추가하고 `arr.GetFirst() = 999;` 가 되는지 확인하세요.
5. 지역 변수를 참조로 반환하는 함수를 만들어 컴파일 경고(C4172)를 직접 확인해 보세요.
6. `Array::operator=` 에서 **자기 대입 검사를 지우고** `arr = arr;` 을 실행해 보세요. 무슨 일이 생기나요?
7. `Array a, b, c;` 를 만들고 `a = b = c;` 가 동작하는지 확인한 뒤, `operator=` 의 반환형을 `void` 로 바꾸면 어떤 에러가 나는지 보세요.
8. `at()` 의 const 버전만 남기고 비-const 버전을 지운 뒤, `arr.at(0) = 100;` 이 왜 에러인지 설명해 보세요.

---

이 강의 자료는 **example/src/ex7.cpp** 와 **example/src/app/string/** 예제를 바탕으로 구성했습니다. 이번 시간의 핵심은 **"참조를 반환하면 대입이 가능해진다"** 는 것과 **`operator=` 의 4단계**입니다. 이 두 가지는 다음 강의인 [class11.md](class11.md) 의 연산자 오버로딩에서 그대로 이어집니다.
