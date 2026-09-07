# C++ 기초 강의 12 - ex12.cpp 코드를 통한 함수 객체와 람다, STL 알고리즘

이 강의는 **example/src/ex12.cpp** 에 담긴 예제들을 바탕으로, **동작(함수)을 다른 함수에 넘겨 주는 방법**을 정리한 내용입니다. 함수 포인터 → 함수 객체(Functor) → 람다로 발전해 가는 과정을 따라갑니다.

---

## 0. 열두 번째 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- `sort` 에 **정렬 기준**을 넘겨 오름차순/내림차순을 바꿀 수 있다.
- `operator()` 를 가진 클래스가 **함수처럼 호출**된다는 것을 안다.
- **함수 포인터**와 **함수 객체(Functor)** 의 차이를 설명할 수 있다.
- 함수 객체가 **상태를 가질 수 있다**는 장점을 이해한다.
- `template <typename F>` 로 **어떤 함수 객체든 받는** 함수를 만들 수 있다.
- **람다(lambda)** 를 작성하고 **캡처 `[ ]`, `[=]`, `[&]`** 를 구분할 수 있다.
- STL 알고리즘 `sort`, `for_each` 를 사용할 수 있다.

### 0.2 이번 강의의 한 문장

> **"값만 넘기는 게 아니라, 동작(함수)도 넘길 수 있다."**

---

## 1. sort 에 정렬 기준 넘기기

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
	int arr[] = { 16,90,30,67,25 };

	for (auto x : arr)
		cout << x << " ";
	cout << endl;

	sort(arr, arr + 5);                     // 기본 : 오름차순
	for (auto x : arr)
		cout << x << " ";
	cout << endl;

	sort(arr, arr + 5, greater<int>() );    // 기준을 바꾸면 내림차순
	for (auto x : arr)
		cout << x << " ";
	cout << endl;
}
```

**예상 출력**
```
16 90 30 67 25 
16 25 30 67 90 
90 67 30 25 16 
```

| 요소 | 설명 |
|------|------|
| `<algorithm>` | `sort`, `for_each`, `find_if` 등이 들어 있는 헤더 |
| `sort(begin, end)` | `[begin, end)` 구간을 오름차순 정렬 |
| `sort(begin, end, cmp)` | **`cmp` 라는 기준**으로 정렬 |
| `greater<int>()` | "왼쪽이 더 크면 참" 이라는 **기준 객체** |

> **`arr + 5` 는 마지막 원소의 "다음" 을 가리킵니다.**
> STL 은 언제나 **`[시작, 끝)`** (끝은 포함하지 않음) 방식을 씁니다.
> `end - begin` 이 곧 원소 개수가 되어 계산이 편합니다.

---

### 1.1 `greater<int>` 의 정체

```cpp
int main()
{
	greater<int> g;      // 객체를 하나 만들고

	cout << g(1, 2) << endl;      // 0 (1 > 2 가 거짓)
	cout << g(15, 2) << endl;     // 1
	cout << g(100, 20) << endl;   // 1
}
```

**예상 출력**
```
0
1
1
```

- `g` 는 **함수가 아니라 객체**입니다. 그런데 `g(1, 2)` 처럼 **함수처럼 호출**됩니다.
- 이것이 바로 **함수 객체(Function Object, Functor)** 입니다.

---

## 2. 함수 객체(Functor) 만들기

### 2.1 `operator()` 오버로딩

```cpp
#include <iostream>
using namespace std;

struct Functor // Function Object(Functor)
{
	void operator()(int data)const
	{
		cout << "data: " << data << endl;
	}
};
int main()
{
	Functor f1;
	f1(10);            //f1.operator()(10)
	f1(15);
	Functor()(100);    // 임시 객체를 만들어 바로 호출
}
```

**예상 출력**
```
data: 10
data: 15
data: 100
```

**`operator()` 의 특징**

| 항목 | 설명 |
|------|------|
| 이름 | `operator()` — **호출 연산자** |
| 매개변수 | **몇 개든 가능** (다른 연산자와 달리 자유롭다) |
| 반환형 | 자유 |
| 오버로딩 | 여러 개 만들 수 있다 |

```cpp
Functor()(100);
//     ↑  ↑
//     │  └─ operator() 호출
//     └──── 임시 객체 생성
```

---

### 2.2 STL 알고리즘에 넘기기

```cpp
#include <algorithm>

struct Functor
{
	void operator()(int data)const
	{
		cout << "data: " << data << endl;
	}
};
int main()
{
	int arr[] = { 57,12,35,96,82 };

	for_each(arr, arr + 5, Functor());   // <algorithm> 의 for_each
}
```

**예상 출력**
```
data: 57
data: 12
data: 35
data: 96
data: 82
```

---

## 3. for_each 를 직접 만들어 보기

### 3.1 알고리즘의 정체는 그냥 반복문이다

```cpp
void For_each(int* begin, int* end, Functor f)
{
	for (; begin != end; ++begin)
		f(*begin);              // 여기서 f.operator()(*begin) 이 불린다
}
int main()
{
	int arr[] = { 57,12,35,96,82 };

	For_each(arr, arr + 5, Functor());
}
```

- STL 알고리즘은 **마법이 아닙니다.** 이렇게 단순한 반복문입니다.
- 중요한 것은 **"무엇을 할지" 를 밖에서 정해 준다**는 구조입니다.

---

### 3.2 문제 : Functor2 를 넘기려면 함수를 또 만들어야 한다

```cpp
struct Functor2
{
	~Functor2() { cout << endl; }      // 다 쓰고 사라질 때 개행
	void operator()(int data)const
	{
		cout << data << " ";
	}
};
void For_each(int* begin, int* end, Functor2 f)   // 또 만들어야 한다!
{ ... }
```

---

### 3.3 해결 : `template <typename F>`

```cpp
template <typename F>
void For_each(int* begin, int* end, F f)   // F 가 Functor 든 Functor2 든 상관없다
{
	for (; begin != end; ++begin)
		f(*begin);
}
int main()
{
	int arr[] = { 57,12,35,96,82 };

	For_each(arr, arr + 5, Functor());
	For_each(arr, arr + 5, Functor2());
}
```

**예상 출력**
```
data: 57
data: 12
data: 35
data: 96
data: 82
57 12 35 96 82 
```

- `F` 자리에 **함수 객체든, 람다든, 함수 포인터든** 무엇이든 올 수 있습니다.
- "호출할 수만 있으면 된다" 는 것이 유일한 조건입니다.
- 템플릿의 자세한 문법은 **[class14.md](class14.md)** 에서 다룹니다.

> **`Functor2` 의 소멸자 트릭**
> `~Functor2() { cout << endl; }` 는 **"다 쓰고 나면 줄바꿈" ** 을 자동으로 해 줍니다.
> 함수 객체는 **객체이므로 소멸자를 가질 수 있다**는 것을 보여 주는 예입니다.

---

## 4. 함수 포인터 vs 함수 객체

### 4.1 함수 포인터 방식

```cpp
void For_each(int* begin, int* end, void (*f)(int) )   // 함수 포인터 매개변수
{
	for (; begin != end; ++begin)
		f(*begin);
}
void PrintInt(int data)
{
	cout << "data : " << data << endl;   // 접두어를 바꾸려면 함수를 새로 만들어야 한다
}
int main()
{
	int arr[] = { 57,12,35,96,82 };

	For_each(arr, arr + 5, PrintInt);
}
```

**`void (*f)(int)` 읽는 법**

```
void (*f)(int)
 ↑    ↑    ↑
반환형 이름  매개변수
     "int 하나를 받고 void 를 반환하는 함수를 가리키는 포인터"
```

**한계** : 접두어를 `"integer: "` 로 바꾸려면?

```cpp
void PrintInt2(int data) { cout << "integer: " << data << endl; }   // 함수를 또 만들어야
void PrintInt3(int data) { cout << "mark: "    << data << endl; }   // 또 만들어야...
```

- 전역 변수를 쓰면 되지만 **지저분하고 위험합니다.**
- 함수는 **상태를 가질 수 없습니다.**

---

### 4.2 함수 객체는 상태를 가진다

```cpp
struct Functor      // 생성자로 상태(prestring) 를 받는 함수 객체
{
	string prestring;

	Functor(const string& msg = "data: ") :prestring(msg) {}
	void operator()(int data)
	{
		cout << prestring << data << endl;
	}
};
int main()
{
	int arr[] = { 57,12,35,96,82 };

	For_each(arr, arr + 5, Functor());               // data: 57 ...
	For_each(arr, arr + 5, Functor("integer: "));    // integer: 57 ...
	For_each(arr, arr + 5, Functor("mark: "));       // mark: 57 ...
}
```

**예상 출력**
```
data: 57
data: 12
data: 35
data: 96
data: 82
integer: 57
integer: 12
integer: 35
integer: 96
integer: 82
mark: 57
mark: 12
mark: 35
mark: 96
mark: 82
```

> **함수를 하나도 더 만들지 않고** 동작을 바꿨습니다. **생성자로 설정을 주입**한 것입니다.

---

### 4.3 결과를 누적하는 함수 객체

```cpp
struct Functor3 // Function Object(Functor)
{
	int sum;
	Functor3(int init):sum(init) { }
	void operator()(int data)
	{
		sum += data;      // 호출될 때마다 자기 안에 값을 쌓는다
	}
};

void For_each(int* begin, int* end, Functor3& f)   // 값이 아니라 참조로 받는다
{
	for (; begin != end; ++begin)
		f(*begin);
}
int main()
{
	int arr[] = { 57,12,35,96,82 };

	Functor3 f(0);
	For_each(arr, arr + 5, f);
	cout << "sum : " << f.sum << endl;   // 282
}
```

**예상 출력**
```
sum : 282
```

> ### 반드시 `참조(&)` 로 받아야 하는 이유
>
> ```cpp
> void For_each(int* begin, int* end, Functor3 f)   // 값 전달이면?
> ```
> **사본** 안에 합계가 쌓이고, 함수가 끝나면 그 사본은 사라집니다.
> 원본 `f.sum` 은 **여전히 0** 입니다.
>
> (참고로 STL 의 `for_each` 는 값으로 받되 **함수 객체를 반환**합니다.
> `Functor3 result = for_each(arr, arr+5, Functor3(0));` 처럼 쓸 수 있습니다.)

---

### 4.4 비교표

| | 함수 포인터 | **함수 객체** | 람다 |
|---|-------------|---------------|------|
| 상태(멤버) | **없다** | **있다** | 캡처로 가진다 |
| 생성자로 설정 | 불가 | **가능** | 캡처로 가능 |
| 인라인 최적화 | 어렵다 | **쉽다 (빠르다)** | 쉽다 |
| 문법 길이 | 짧다 | **길다** | **가장 짧다** |
| 정의 위치 | 함수 밖 | 함수 밖(클래스) | **쓰는 자리** |

---

## 5. 람다(lambda) - 이름 없는 함수 객체

### 5.1 기본 문법

```cpp
[캡처](매개변수) -> 반환형 { 몸통 }
```

```cpp
//lambda : 변수에 담아 두었다가 나중에 호출
auto f = [](int a, int b) -> int { 	return a + b; };
cout << f(10, 20) << endl;

//정의하자마자 바로 호출
cout << [](int a, int b) -> int {
			cout << a << ',' << b << endl;
			return a + b;
		}(10,20) << endl;
```

**예상 출력**
```
30
10,20
30
```

| 부분 | 설명 |
|------|------|
| `[ ]` | **캡처 목록** — 바깥 변수를 어떻게 가져올지 |
| `(int a, int b)` | 매개변수 (없으면 생략 가능) |
| `-> int` | 반환형 (**대부분 생략 가능** — 컴파일러가 추론) |
| `{ ... }` | 함수 몸통 |

```cpp
auto f = [](int a, int b) { return a + b; };   // -> int 를 생략해도 된다
```

> **람다의 정체**
> 컴파일러는 람다를 만나면 **이름 없는 함수 객체 클래스를 자동으로 만들어 줍니다.**
> ```cpp
> [](int a, int b) { return a + b; }
> // 컴파일러가 대략 이렇게 만든다
> struct 이름없는클래스 {
>     int operator()(int a, int b) const { return a + b; }
> };
> ```
> 그래서 `auto` 로 받아야 합니다. 형식에 **이름이 없기 때문**입니다.

---

### 5.2 캡처 - 바깥 변수 가져오기

**(1) `[s]` — 특정 변수를 값으로**

```cpp
string s = "Hello";

cout << [s](int a, int b) -> int {   // [s] : s 만 값으로 캡처
	cout << s << " : " << a << ',' << b << endl;
	return a + b;
}(10, 20) << endl;
```

**(2) `[=]` — 쓰이는 모든 바깥 변수를 값으로**

```cpp
cout << [=](int a, int b) -> int {   // [=] : 쓰이는 바깥 변수를 전부 값으로 캡처
	cout << s << " : " << a << ',' << b << endl;
	return a + b;
}(10, 20) << endl;
```

**(3) `[&]` — 참조로 (수정 가능)**

```cpp
string s = "Hello";

cout << [&](int a, int b) -> int {   // [&] : 참조 캡처
	cout << s << " : " << a << ',' << b << endl;
	s = "Done";                      // 바깥 s 가 실제로 바뀐다
	return a + b;
}(10, 20) << endl;

cout << s << endl;   // Done
```

**예상 출력**
```
Hello : 10,20
30
Done
```

**캡처 방식 정리**

| 캡처 | 뜻 | 바깥 변수 수정 |
|------|-----|----------------|
| `[ ]` | 아무것도 안 가져온다 | — |
| `[x]` | `x` 만 **값**으로 | X |
| `[&x]` | `x` 만 **참조**로 | O |
| `[=]` | 쓰이는 것 전부 **값**으로 | X |
| `[&]` | 쓰이는 것 전부 **참조**로 | O |
| `[=, &x]` | 기본은 값, `x` 만 참조 | `x` 만 O |
| `[this]` | 멤버 함수 안에서 자기 객체를 | O |

> ### `[&]` 사용 시 주의
> 참조 캡처는 **원본이 살아 있어야** 합니다.
> 람다를 변수에 담아 두었다가 원본이 사라진 뒤 호출하면 **dangling reference** 가 됩니다.
> ```cpp
> auto MakeBad() {
>     int local = 10;
>     return [&]() { return local; };   // X - local 이 사라진다!
> }
> ```
> **함수를 벗어나 살아남는 람다에는 `[=]` 를 쓰세요.**

---

### 5.3 STL 알고리즘 + 람다

```cpp
#include <algorithm>

int main()
{
	int arr[] = { 15,16,29,30,56,20,40 };
	string s = "string";

	sort(arr, arr + 7, [s](int a, int b)->bool {
		cout << s << endl;
		return a > b; });
	for_each(arr, arr + 7, [](int n)->void { cout << n << endl; });
	for_each(arr, arr + 7, [](int n)->void { cout << n << ", "; });
}
```

- 정렬 기준을 **그 자리에서 즉석으로** 만들어 넘깁니다.
- 함수 객체를 위해 클래스를 따로 정의할 필요가 없어졌습니다.

**`sort` 의 비교 함수 규칙**

```cpp
bool cmp(const T& a, const T& b);   // "a 가 b 보다 앞에 와야 하면 true"
```

| 원하는 결과 | 비교 함수 |
|-------------|-----------|
| 오름차순 | `return a < b;` |
| 내림차순 | `return a > b;` |
| 절댓값 오름차순 | `return abs(a) < abs(b);` |

> **주의** : `<=` 를 쓰면 안 됩니다. `sort` 는 **엄격한 약순서**를 요구하므로 `<=` 를 쓰면 프로그램이 죽을 수 있습니다.

---

## 6. 완성 코드 (ex12.cpp 맨 위)

```cpp
struct Functor      // 생성자로 상태(prestring) 를 받는 함수 객체
{
	string prestring;

	Functor(const string& msg = "data: ") :prestring(msg) {}
	void operator()(int data)
	{
		cout << prestring << data << endl;
	}
};

template <typename F>            // F 자리에 함수 객체든 람다든 함수 포인터든 들어간다
void For_each(int* begin, int* end, F f)
{
	for (; begin != end; ++begin)
		f(*begin);
}

int main()
{
	int arr[] = { 57,12,35,96,82 };

	For_each(arr, arr + 5, Functor());               // data: 57 ...
	For_each(arr, arr + 5, Functor("integer: "));    // integer: 57 ...
	For_each(arr, arr + 5, Functor("mark: "));       // mark: 57 ...

	cout << "--- 람다 ---" << endl;
	For_each(arr, arr + 5, [](int n) { cout << n << ", "; });
	cout << endl;

	cout << "--- STL 알고리즘 ---" << endl;
	sort(arr, arr + 5);                              // 오름차순
	for_each(arr, arr + 5, [](int n) { cout << n << " "; });
	cout << endl;

	sort(arr, arr + 5, greater<int>());              // 내림차순 (함수 객체)
	for_each(arr, arr + 5, [](int n) { cout << n << " "; });
	cout << endl;

	sort(arr, arr + 5, [](int a, int b) { return a < b; });   // 람다로 같은 일을
	for_each(arr, arr + 5, [](int n) { cout << n << " "; });
	cout << endl;
}
```

**예상 출력**
```
data: 57
data: 12
data: 35
data: 96
data: 82
integer: 57
integer: 12
integer: 35
integer: 96
integer: 82
mark: 57
mark: 12
mark: 35
mark: 96
mark: 82
--- 람다 ---
57, 12, 35, 96, 82, 
--- STL 알고리즘 ---
12 35 57 82 96 
96 82 57 35 12 
12 35 57 82 96 
```

---

## 7. 정리 표

| 주제 | 내용 |
|------|------|
| `<algorithm>` | `sort`, `for_each`, `find_if` 등 |
| `[begin, end)` | 끝은 **포함하지 않는다** |
| `sort(b, e, cmp)` | `cmp` 는 "a 가 앞에 오면 true" |
| `greater<int>()` | 표준 함수 객체 (내림차순) |
| 함수 객체(Functor) | **`operator()` 를 가진 클래스** |
| `Functor()(100)` | 임시 객체를 만들어 즉시 호출 |
| 함수 포인터 | `void (*f)(int)` — **상태를 가질 수 없다** |
| 함수 객체의 장점 | **상태를 가진다**, 생성자로 설정 주입, 인라인 최적화 |
| 상태 누적 | 함수 객체를 **참조(`&`)로 받아야** 결과가 남는다 |
| `template <typename F>` | 어떤 호출 가능한 것이든 받는다 |
| 람다 문법 | `[캡처](매개변수) -> 반환형 { 몸통 }` |
| 람다의 정체 | 컴파일러가 만든 **이름 없는 함수 객체** |
| `auto` 필수 | 람다는 형식에 이름이 없다 |
| 캡처 `[=]` | 값으로 (안전, 수정 불가) |
| 캡처 `[&]` | 참조로 (수정 가능, **수명 주의**) |

---

## 8. 연습 문제 (스스로 풀어보기)

1. `vector<int>` 를 만들고 `sort` 로 오름차순, 내림차순으로 각각 정렬해 출력하세요.
2. `struct Multiply { int factor; Multiply(int f); void operator()(int&); };` 를 만들어 배열의 모든 원소에 `factor` 를 곱하세요.
3. 2번을 람다로 다시 작성하세요. (힌트 : `for_each(v.begin(), v.end(), [f](int& n){ n *= f; });`)
4. 배열의 **최댓값**을 찾는 함수 객체를 만들어 `For_each` 로 구해 보세요.
5. `vector<string>` 을 **길이순**으로 정렬하는 람다를 작성하세요.
6. `[=]` 와 `[&]` 를 각각 써서 바깥 변수를 100 증가시켜 보고, 어느 쪽이 실제로 바뀌는지 확인하세요.
7. `Functor3` 을 `For_each(arr, arr+5, f)` 에서 **값 전달**로 바꾸고 `f.sum` 을 출력해 보세요. 왜 0 인가요?
8. `count_if(v.begin(), v.end(), [](int n){ return n % 2 == 0; })` 로 짝수 개수를 세어 보세요.
9. `find_if` 로 50 보다 큰 첫 원소를 찾아 보세요.

---

이 강의 자료는 **example/src/ex12.cpp** 에 있는 모든 예제를 바탕으로 구성했습니다. 이번 시간의 핵심은 **"동작을 값처럼 넘긴다"** 는 발상입니다. 함수 포인터의 한계 → 함수 객체 → 람다로 이어지는 흐름을 이해하면, STL 알고리즘 대부분을 자유롭게 쓸 수 있게 됩니다.
