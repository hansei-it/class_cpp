# C++ 기초 강의 14 - ex14.cpp 코드를 통한 static 멤버와 템플릿, STL

이 강의는 **example/src/ex14.cpp** 에 담긴 예제들을 바탕으로, **객체 없이 쓰는 static 멤버**와 **형식을 나중에 정하는 템플릿**, 그리고 **STL 컨테이너와 반복자**를 정리한 내용입니다.

---

## 0. 열네 번째 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- **static 멤버 함수**를 만들고 객체 없이 호출할 수 있다.
- **static 멤버 변수**를 선언하고 **클래스 밖에서 정의**할 수 있다.
- **함수 템플릿**을 만들고 인자 추론과 명시적 지정을 구분할 수 있다.
- **클래스 템플릿**(`Stack<int>`, `Stack<double>`)을 만들 수 있다.
- **비타입 템플릿 인자**(`Stack<int, 100>`)와 **기본 템플릿 인자**를 쓸 수 있다.
- **컨테이너 자체를 템플릿 인자**로 받을 수 있다.
- **반복자(iterator)** 로 STL 알고리즘을 사용할 수 있다.

---

## 1. static 멤버

### 1.1 static 멤버 함수 - 객체 없이 부르는 함수

```cpp
class Math
{
public:
	int Add(int a, int b) { return a + b; }    // 일반 멤버 함수
	int Sub(int a, int b) { return a - b; }
};
int main()
{
	Math m;                          // 객체를 만들어야만 부를 수 있다
	cout << m.Add(10, 20) << endl;
}
```

- `Add` 는 **멤버 변수를 하나도 안 씁니다.** 그런데도 객체를 만들어야 합니다. 낭비입니다.

```cpp
class Math
{
public:
	static int Add(int a, int b) { return a + b; }
	static int Sub(int a, int b) { return a - b; }
};
int main()
{
	int n1 = 10;
	int n2 = 20;

	// Math m; m.Add(...) 처럼 객체를 만들 필요가 없다
	cout << " + : " << Math::Add(n1, n2) << endl;
	cout << " - : " << Math::Sub(n1, n2) << endl;
}
```

**예상 출력**
```
 + : 30
 - : -10
```

**static 멤버 함수의 규칙**

| 항목 | 설명 |
|------|------|
| 호출 | `클래스이름::함수()` (객체로도 가능하지만 권장 안 함) |
| **`this`** | **없다** |
| 멤버 변수 접근 | **일반 멤버는 못 쓴다** (static 멤버만 가능) |
| `const` | **붙일 수 없다** (바꿀 `this` 자체가 없으므로) |
| `virtual` | **붙일 수 없다** |
| 정의 위치 | 클래스 밖에서 정의할 때는 `static` 을 **다시 쓰지 않는다** |

```cpp
// 선언 (헤더)
class Util { public: static void Menu(); };

// 정의 (소스)
void Util::Menu() //static (정의할 때는 static 을 다시 쓰지 않는다)
{ ... }
```

---

### 1.2 static 멤버 변수 - 모든 객체가 함께 쓰는 하나뿐인 변수

```cpp
class Unit
{
	static int count;
public:
	static int CreateUnit()
	{
		//Unit 생성;
		return ++count;
	}
	static int GetCount() { return count; }
private:
	Unit();            // 생성자를 private 로 -> 객체를 만들 수 없다
};
int Unit::count = 0;   // static 멤버 변수는 클래스 밖에서 반드시 정의해야 한다
int main()
{
	cout << Unit::GetCount() << endl;
	//Unit u;              // X
	//Unit* p = new Unit;  // X
	cout << Unit::CreateUnit() << endl;
	cout << Unit::CreateUnit() << endl;
	cout << Unit::CreateUnit() << endl;
	cout << Unit::CreateUnit() << endl;

	cout << "unit count : " << Unit::GetCount() << endl;
}
```

**예상 출력**
```
0
1
2
3
4
unit count : 4
```

> ### 반드시 기억할 것 : 클래스 밖 정의
>
> ```cpp
> class Unit { static int count; };   // 이것은 "선언" 일 뿐
> int Unit::count = 0;                // 이것이 진짜 "정의" (메모리 확보)
> ```
>
> 이것을 빼먹으면 **링크 에러**가 납니다.
> ```
> error LNK2001: 확인할 수 없는 외부 기호 "private: static int Unit::count"
> ```
>
> **어디에 쓰나?**
> - 헤더에는 선언만, **`.cpp` 파일에 정의**를 씁니다.
> - 헤더에 정의를 쓰면 여러 번 포함될 때 **중복 정의** 에러가 납니다.
> - (C++17 부터는 `inline static int count = 0;` 으로 헤더에서 한 번에 가능)

**일반 멤버 vs static 멤버**

```
        일반 멤버 변수                    static 멤버 변수
    obj1 ┌──────┐  obj2 ┌──────┐          클래스에 딱 하나
         │ x=1  │       │ x=2  │       ┌──────────────┐
         └──────┘       └──────┘       │  count = 4   │  ← 모두가 공유
                                        └──────────────┘
```

**생성자를 private 로 만드는 이유**

```cpp
private:
	Unit();   // 선언만 하고 정의 안 함
```
- **객체를 아예 못 만들게** 막습니다.
- `Unit` 은 static 함수만 쓰는 **유틸리티 클래스**이므로 객체가 필요 없습니다.

---

### 1.3 유틸리티 클래스

```cpp
struct Util
{
	static void Menu()
	{
		cout << "1. 메뉴1" << endl;
		cout << "2. 메뉴2" << endl;
		cout << "3. 메뉴3" << endl;
	}
	static void SubMenu()
	{
		cout << "1. 서브 메뉴1" << endl;
		...
	}
};
int main()
{
	Util::Menu();       // 객체를 만들 필요가 없다
	Util::SubMenu();
}
```

- 관련된 함수들을 **하나의 이름 아래 묶는** 용도입니다.
- `namespace` 로도 같은 일을 할 수 있습니다. (요즘은 namespace 를 더 선호)
- 이 프로젝트의 `app/telphonebook/Util.h` 가 이 패턴입니다.

---

## 2. 함수 템플릿

### 2.1 형식만 다르고 하는 일이 같은 함수

```cpp
void Print(int data)         { cout << data << endl; }
void Print(double data)      { cout << data << endl; }
void Print(const char* data) { cout << data << endl; }
// 형식이 늘어날 때마다 계속 만들어야 한다...
```

---

### 2.2 template 으로 하나만 만들기

```cpp
#include <iostream>
using namespace std;

template <typename T>
void Print(T data)
{
	cout << typeid(T).name() << " : " << data << endl;
}
int main()
{
	Print<int>(10);
	Print<const char*>("Hello!");
	Print<double>(5.5);
}
```

**예상 출력**
```
int : 10
char const * : Hello!
double : 5.5
```

**문법**

```cpp
template <typename T>       // T 는 "아직 정해지지 않은 형식" 이라는 뜻
void Print(T data) { ... }
```

| 표기 | 설명 |
|------|------|
| `template <typename T>` | 표준 표기 (권장) |
| `template <class T>` | 같은 뜻 (옛날 표기, `class` 가 아니어도 됨) |
| `T` | 관례적 이름. `K`, `V`, `Elem` 등 아무거나 가능 |

> **템플릿은 "틀" 입니다.**
> `Print<int>(10)` 을 쓰는 순간 컴파일러가 `T` 자리에 `int` 를 넣어 **진짜 함수를 만들어 냅니다.**
> 이것을 **인스턴스화(instantiation)** 라고 합니다.
> 그래서 쓰지 않은 형식의 코드는 **아예 만들어지지 않습니다.**

---

### 2.3 인자 추론과 명시적 지정

```cpp
Print(10);              // T 를 int 로 추론
Print("Hello!");        // T 를 const char* 로 추론
Print(5.5);             // T 를 double 로 추론
Print<int>(5.5);        // 강제로 int 로 지정 -> 5 가 출력된다
```

**예상 출력**
```
int : 10
char const * : Hello!
double : 5.5
int : 5
```

| 방식 | 설명 |
|------|------|
| **추론** | `Print(10)` — 인자를 보고 컴파일러가 `T` 를 결정 |
| **명시** | `Print<int>(5.5)` — 내가 직접 지정 (변환이 일어난다) |

- 대부분은 추론에 맡기면 됩니다.
- 추론이 안 되는 경우(반환형만 템플릿일 때 등)에만 명시합니다.

---

### 2.4 배열을 받는 템플릿 — 세 단계로 발전시키기

`ex14.cpp` 의 **[단계 3] → [단계 3a] → [단계 3b] → [단계 3c]** 를 차례로 실행해 보세요.

**[단계 3] 시작점 : int 전용 함수**

```cpp
void PrintArray(int* arr, int size)    // int 배열만 출력할 수 있다
{
	for (int i = 0; i < size; ++i)
		cout << arr[i] << endl;
}
```

**[단계 3a] `K` 로만 받으면 — 무엇이든 통과한다**

```cpp
template <typename K>
void PrintArray(K arr, int size)      // 포인터인지 아닌지 검사할 방법이 없다
{
	cout << typeid(K).name() << endl;
	for (int i = 0; i < size; ++i)
		cout << arr[i] << endl;
}
```

- 배열은 함수에 넘어갈 때 **포인터로 변하므로** `K` 가 `int*` 로 추론되어 잘 동작합니다.
- 하지만 `PrintArray(n, 10)` 처럼 **정수를 넘겨도** `K` 가 `int` 로 추론되고, `arr[i]` 에서야 에러가 납니다.

**[단계 3b] `K*` 로 받기**

```cpp
template <typename K>
void PrintArray(K* arr, int size)     // const 가 없다
```

- `PrintArray(n, 10)` 이 **호출 단계에서** 막힙니다. 훨씬 낫습니다.
- 다만 함수가 배열을 **수정할 수 있습니다.**

**[단계 3c] `const K*` 로 받기 — 최종형**

```cpp
template <typename K>
void PrintArray(const K* arr, int size)   // K* 가 아니라 const K*
{
	cout << typeid(K).name() << endl;
	for (int i = 0; i < size; ++i)
		cout << arr[i] << endl;
	cout << endl;
}
int main()
{
	int arr[10] = { 1,2,3,4,5,6,7 };
	PrintArray<int>(arr, 10);        // 명시적으로 지정

	string sarr[] = { "abc","defg","12345" };
	PrintArray<string>(sarr, 3);

	//int n = 10;
	//PrintArray(n, 10);    // X - 포인터가 아니라서 에러가 난다 (다행이다!)
}
```

**정리**

| 매개변수 | 배열이 아닌 값 | 원본 수정 | 평가 |
|----------|----------------|-----------|------|
| `int* arr` | 막힌다 | 가능 | int 전용이라 재사용 불가 |
| `K arr` | **통과해 버린다** | 가능 | 위험 |
| `K* arr` | 막힌다 | 가능 | 보통 |
| **`const K* arr`** | 막힌다 | **불가** | **가장 안전** |

> **읽기만 하는 함수는 `const K*` 로 받으세요.**
> 매개변수에 `const` 를 붙이는 습관은 [class2.md](class2.md) 의 `const 형식&` 와 같은 이야기입니다.

---

### 2.5 템플릿 인자를 명시적으로 지정하기

`ex14.cpp` **[단계 4b]** 입니다.

```cpp
template <typename T, typename F>
int Find(T* begin, T* end, F pred) { ... }

int idx = Find<int, Pred>(arr, arr + 5, Pred());    // 둘 다 명시

double darr[] = { 1.1, 4.5, 7.7, 8.8, 9.1 };
idx = Find<double>(darr, darr + 5, [](double data)->bool { return 8.8 == data; });
//    ^ T 만 명시하고 F(람다의 형식)는 추론에 맡긴다
```

| 방식 | 언제 |
|------|------|
| 전부 추론 | 대부분의 경우 |
| 전부 명시 | 변환을 강제하고 싶을 때 |
| **앞쪽만 명시** | 뒤쪽이 **이름을 쓸 수 없는 형식**(람다)일 때 |

> **람다는 형식에 이름이 없습니다.** ([class12.md](class12.md))
> 그래서 `Find<double, ???>` 라고 쓸 방법이 없고, **앞에서부터 필요한 만큼만 지정**하고 나머지는 추론에 맡깁니다.

---

## 3. 클래스 템플릿

### 3.1 먼저 int 전용 Stack 을 만들어 본다

```cpp
class Stack
{
	int buf[100];    // int 만 담을 수 있다 -> double 용은 또 만들어야 한다
	int top;
public:
	Stack() :top(0) {}
	void Push(int data) { buf[top++] = data; }
	void Pop()          { --top; }
	int Top()const      { return buf[top-1]; }
};
int main()
{
	Stack s;
	s.Push(10); s.Push(20); s.Push(30);

	cout << s.Top() << endl;   // 30
	s.Pop();
	cout << s.Top() << endl;   // 20
	s.Pop();
	cout << s.Top() << endl;   // 10
	s.Pop();
}
```

**예상 출력**
```
30
20
10
```

---

### 3.2 클래스 템플릿으로 바꾸기

```cpp
template <typename T>
class Stack
{
	T buf[100];
	int top;
public:
	Stack() :top(0) {}
	void Push(const T& data)   // 복사 비용을 줄이려면 const T&
	{
		buf[top++] = data;
	}
	void Pop() { --top; }
	const T& Top()const { return buf[top - 1]; }
};
int main()
{
	Stack<int> s;              // <int> 를 반드시 써야 한다

	cout << typeid(s).name() << endl;
	s.Push(10); s.Push(20); s.Push(30);
	cout << s.Top() << endl;

	Stack<double> ds;          // 완전히 다른 클래스가 만들어진다
	ds.Push(1.5);
	cout << ds.Top() << endl;
}
```

> ### 함수 템플릿과의 차이
>
> | | 함수 템플릿 | 클래스 템플릿 |
> |---|-------------|---------------|
> | 인자 추론 | **된다** (`Print(10)`) | **안 된다** (C++17 이전) |
> | 사용법 | `Print(10)` 또는 `Print<int>(10)` | **`Stack<int> s;`** (반드시 명시) |
>
> `Stack s;` 는 에러입니다. **`Stack<int> s;` 라고 써야 합니다.**

**`void Push(T data)` 와 `void Push(const T& data)` 의 차이**

- `T` 가 `int` 면 별 차이 없습니다.
- `T` 가 `string` 이나 큰 클래스면 **복사 비용이 큽니다.**
- 그래서 템플릿에서는 **`const T&` 를 기본**으로 씁니다. ([class2.md](class2.md))

---

## 4. 비타입 템플릿 인자와 기본 인자

### 4.1 값도 템플릿 인자가 될 수 있다

```cpp
template <typename T, int MAXSIZE>
class Stack
{
	T buf[MAXSIZE];       // 배열 크기를 사용하는 쪽이 정한다
	int top;
public:
	Stack() :top(0) {}
	...
};
int main()
{
	Stack<int,100> s;
	Stack<double, 50> ds;
}
```

| 템플릿 인자 종류 | 예 |
|------------------|-----|
| **형식** | `typename T` → `int`, `string`, ... |
| **값(비타입)** | `int MAXSIZE` → `100`, `50` |
| 조건 | 값은 **컴파일 시점에 정해지는 상수**여야 한다 |

```cpp
int n = 100;
Stack<int, n> s;    // X - 에러 : n 은 실행 중에 바뀔 수 있다
const int N = 100;
Stack<int, N> s;    // O
```

> `std::array<int, 5>` 가 바로 이 방식입니다.

---

### 4.2 기본 템플릿 인자

```cpp
template <typename T=int, int MAXSIZE=100>
class Stack
{
	T buf[MAXSIZE];
	int top;
public:
	Stack() :top(0)
	{
		for (auto& x : buf)
			x = T();          // 어떤 형식이든 기본값으로 초기화
	}
	...
};
int main()
{
	Stack<> s;              // <> 는 생략할 수 없다 (Stack s; 는 에러)
	Stack<double> ds;       // MAXSIZE 만 기본값 사용
}
```

> **`Stack<>` 의 `<>` 는 생략할 수 없습니다.** (C++17 부터는 가능)
>
> **`x = T();`** 는 [class2.md](class2.md) 에서 배운 **기본값 생성**입니다.
> `T` 가 `int` 면 0, `double` 이면 0.0, `string` 이면 빈 문자열이 됩니다.
> **어떤 형식이 올지 몰라도 초기화할 수 있는** 방법입니다.

---

## 5. 컨테이너를 템플릿 인자로

### 5.1 고정 배열 대신 vector 쓰기

```cpp
template <typename T>
class Stack
{
	vector<T> buf;     // T buf[100]; 대신
public:
	Stack(){ }
	void Push(const T& data) { buf.push_back(data); }
	void Pop()               { buf.pop_back(); }
	const T& Top()const      { return buf.back(); }
};
```

- 크기 걱정이 사라집니다. `top` 변수도 필요 없습니다.
- `vector` 가 이미 잘 만들어져 있는데 굳이 배열을 직접 관리할 이유가 없습니다.

---

### 5.2 그릇까지 바꿔 끼우기

```cpp
template <typename T, typename Cont = deque<T> >   // 담을 그릇까지 바꿔 끼울 수 있다
class Stack
{
	Cont buf;
public:
	Stack() { }
	void Push(const T& data) { buf.push_back(data); }
	void Pop()               { buf.pop_back(); }
	const T& Top()const      { return buf.back(); }
	bool Empty()const        { return buf.empty(); }
};
int main()
{
	Stack<int> s;                        // Cont 는 기본값 deque<int>
	cout << typeid(s).name() << endl;

	Stack<double, vector<double> > ds;    // 그릇을 vector 로 바꿔 끼운다
}
```

**예상 출력**
```
class Stack<int,class std::deque<int,class std::allocator<int> > >
```

> `Cont` 에는 **`push_back`, `pop_back`, `back`, `empty` 만 있으면** 무엇이든 올 수 있습니다.
> `vector`, `deque`, `list` 모두 가능합니다.
>
> 실제 표준 라이브러리의 `std::stack` 이 **정확히 이 구조**입니다.
> ```cpp
> template <class T, class Container = std::deque<T>> class stack;
> ```

> ### `> >` 사이에 공백을 주는 이유
> ```cpp
> Stack<double, vector<double> > ds;   // O
> Stack<double, vector<double>> ds;    // C++11 부터 O, 그 전에는 에러
> ```
> 옛날 컴파일러는 `>>` 를 **오른쪽 시프트 연산자**로 읽었습니다.
> C++11 부터는 붙여 써도 되지만, 오래된 코드에서 공백을 자주 봅니다.

---

## 6. STL 컨테이너와 반복자

### 6.1 주요 컨테이너

| 컨테이너 | 헤더 | 특징 | 잘하는 것 |
|----------|------|------|-----------|
| `vector` | `<vector>` | 연속된 배열 | **뒤에 추가**, 임의 접근(`[i]`) |
| `deque` | `<deque>` | 양쪽에서 넣고 뺄 수 있는 배열 | **앞/뒤 추가**, 임의 접근 |
| `list` | `<list>` | 이중 연결 리스트 | **중간 삽입/삭제** |
| `map` | `<map>` | 키-값 쌍 (정렬됨) | **키로 검색** |
| `set` | `<set>` | 중복 없는 집합 | 존재 여부 확인 |

---

### 6.2 반복자(iterator)

```cpp
vector<int> v = { 10,20,30,40,50 };

// (1) 인덱스 방식 - vector 에서만 가능
for (size_t i = 0; i < v.size(); ++i)
	cout << v[i] << " ";

// (2) 반복자 방식 - 모든 컨테이너에서 가능
for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
	cout << *it << " ";

// (3) auto 로 짧게
for (auto it = v.begin(); it != v.end(); ++it)
	cout << *it << " ";

// (4) 범위 기반 for - 가장 짧다
for (auto x : v)
	cout << x << " ";
```

**반복자는 "포인터처럼 동작하는 객체" 입니다.**

| 연산 | 뜻 |
|------|-----|
| `*it` | 현재 원소 |
| `++it` | 다음으로 |
| `it != end()` | 끝인가? |
| `v.begin()` | 첫 원소를 가리킴 |
| `v.end()` | **마지막 원소의 다음**을 가리킴 |

> **왜 반복자가 필요한가?**
> `list` 는 `[i]` 로 접근할 수 없습니다. 하지만 반복자는 됩니다.
> 덕분에 `sort`, `find`, `for_each` 같은 알고리즘이 **컨테이너 종류와 무관하게** 동작합니다.

---

### 6.3 알고리즘 사용

```cpp
#include <algorithm>

int main()
{
	auto arr = { 10,20,30,40,50,60,70,80,90,100 }; //initialzer_list
	for (auto x : arr)
		cout << x << " ";
	cout << endl;

	vector<int> v(arr.begin(), arr.end());   // 반복자 구간으로 초기화
	for (auto x : v)
		cout << x << " ";
	cout << endl;

	reverse(v.begin(), v.end());
	for (auto x : v)
		cout << x << " ";
	cout << endl;

	sort(v.begin(), v.end(), less<int>());
	for (auto x : v)
		cout << x << " ";
	cout << endl;
}
```

| 알고리즘 | 하는 일 |
|----------|---------|
| `sort(b, e)` / `sort(b, e, cmp)` | 정렬 |
| `reverse(b, e)` | 뒤집기 |
| `find(b, e, val)` | 값 찾기 |
| `find_if(b, e, pred)` | 조건에 맞는 첫 원소 |
| `count_if(b, e, pred)` | 조건에 맞는 개수 |
| `for_each(b, e, f)` | 각 원소에 f 적용 |
| `shuffle(b, e, gen)` | 섞기 |

> ### `random_shuffle` 은 사라졌습니다
> 원본 예제의 `random_shuffle` 은 **C++14 에서 폐기되고 C++17 에서 삭제**되었습니다.
> 이 프로젝트는 C++20 이므로 사용할 수 없습니다. 대신 `shuffle` 을 쓰세요.
>
> ```cpp
> #include <random>
> std::random_device rd;
> std::mt19937 gen(rd());
> shuffle(v.begin(), v.end(), gen);
> ```

---

## 7. 완성 코드 (ex14.cpp 맨 위)

```cpp
template <typename T, typename Cont = deque<T> >
class Stack
{
	Cont buf;
public:
	Stack() { }
	void Push(const T& data) { buf.push_back(data); }
	void Pop()               { buf.pop_back(); }
	const T& Top()const      { return buf.back(); }
	bool Empty()const        { return buf.empty(); }
};

int main()
{
	Stack<int> s;                        // Cont 는 기본값 deque<int>
	cout << typeid(s).name() << endl;

	s.Push(10);
	s.Push(20);
	s.Push(30);
	while (!s.Empty()) { cout << s.Top() << " "; s.Pop(); }   // 30 20 10
	cout << endl;

	Stack<double, vector<double> > ds;    // 그릇을 vector 로 바꿔 끼운다
	ds.Push(1.5);
	ds.Push(2.5);
	ds.Push(6.5);
	while (!ds.Empty()) { cout << ds.Top() << " "; ds.Pop(); }
	cout << endl;

	cout << "--- 반복자와 알고리즘 ---" << endl;
	vector<int> v = { 10,20,30,40,50 };
	reverse(v.begin(), v.end());
	for (auto x : v) cout << x << " ";
	cout << endl;

	sort(v.begin(), v.end(), less<int>());
	for (auto x : v) cout << x << " ";
	cout << endl;
}
```

**예상 출력**
```
class Stack<int,class std::deque<int,class std::allocator<int> > >
30 20 10 
6.5 2.5 1.5 
--- 반복자와 알고리즘 ---
50 40 30 20 10 
10 20 30 40 50 
```

---

## 8. 정리 표

| 주제 | 내용 |
|------|------|
| static 멤버 함수 | `클래스::함수()`, **`this` 없음**, const/virtual 불가 |
| static 멤버 변수 | **클래스 밖에서 반드시 정의** (`int Unit::count = 0;`) |
| 정의를 빼면 | **링크 에러 LNK2001** |
| 유틸리티 클래스 | static 함수만 모아 놓고 생성자를 private 로 |
| 함수 템플릿 | `template <typename T> void f(T)` — **인자 추론 O** |
| 클래스 템플릿 | `template <typename T> class Stack` — **`Stack<int>` 명시 필수** |
| 인스턴스화 | 쓰는 순간 컴파일러가 진짜 코드를 만든다 |
| 배열 매개변수 | `const K*` 로 받아야 안전 |
| 비타입 인자 | `template <typename T, int N>` — **컴파일 시점 상수만** |
| 기본 템플릿 인자 | `template <typename T=int>` → `Stack<>` (`<>` 필수) |
| `T()` | 형식이 무엇이든 기본값 (0, 빈 문자열 등) |
| 컨테이너를 인자로 | `template <typename T, typename Cont=deque<T> >` |
| `> >` 공백 | C++11 이전 문법. 지금은 붙여 써도 된다 |
| 반복자 | `*it`, `++it`, `begin()`, `end()` — **포인터처럼** |
| `end()` | **마지막의 다음**을 가리킨다 |
| `random_shuffle` | C++17 에서 **삭제됨** → `shuffle` 사용 |

---

## 9. 연습 문제 (스스로 풀어보기)

1. `class Counter` 에 `static int total;` 을 두고, 생성자에서 증가·소멸자에서 감소시켜 현재 객체 수를 세어 보세요.
2. 1번에서 `int Counter::total = 0;` 을 지우고 어떤 에러가 나는지 확인하세요.
3. static 멤버 함수 안에서 일반 멤버 변수를 써 보고 어떤 에러가 나는지 확인하세요.
4. `template <typename T> T Max(T a, T b)` 를 만들고 `int`, `double`, `string` 으로 각각 호출하세요.
5. 4번에서 `Max(10, 5.5)` 를 호출해 보세요. 왜 에러인가요? 어떻게 고칠 수 있나요?
6. `template <typename T> class Queue` 를 만들어 `Push`, `Pop`(앞에서), `Front` 를 구현하세요.
7. `Stack<string>` 을 만들어 문자열을 넣고 빼 보세요.
8. `Stack<int, list<int>>` 로 그릇을 `list` 로 바꿔 보세요. 동작하나요?
9. `list<int>` 를 만들고 반복자로 순회하며 출력해 보세요. `[i]` 로 접근하면 어떤 에러가 나나요?
10. `map<string, int>` 로 간단한 단어 개수 세기를 만들어 보세요.

---

이 강의 자료는 **example/src/ex14.cpp** 에 있는 모든 예제를 바탕으로 구성했습니다. 이번 시간의 핵심은 **템플릿은 "코드를 찍어 내는 틀"** 이라는 이해입니다. `Stack<int>` 와 `Stack<double>` 은 **완전히 다른 클래스**이며, 이 사실을 알면 컴파일 에러 메시지가 왜 그렇게 긴지도 이해하게 됩니다.
