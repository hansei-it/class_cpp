# C++ 기초 강의 16 - ex16.cpp 코드를 통한 스마트 포인터·파일 입출력·종합 실습

이 강의는 **example/src/ex16.cpp** 와 **example/src/app/telphonebook/** 의 예제들을 바탕으로, **`delete` 를 직접 쓰지 않게 해 주는 스마트 포인터**와 **파일 입출력**, 그리고 지금까지 배운 것을 모두 모은 **종합 실습(전화번호부)** 을 정리한 내용입니다.

---

## 0. 마지막 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- 원시 포인터의 문제(메모리 누수, 예외 안전성)를 설명할 수 있다.
- **`unique_ptr`** 와 **`shared_ptr`** 를 구분해 쓸 수 있다.
- `make_shared`, `reset`, `swap`, `use_count`, `weak_ptr` 를 사용할 수 있다.
- 클래스 멤버를 `shared_ptr` 로 바꾸면 소멸자가 필요 없어지는 것을 확인할 수 있다.
- **참조 카운트 방식의 스마트 포인터를 직접 구현**할 수 있다.
- `ofstream` 으로 텍스트 파일에 저장할 수 있다.
- 지금까지 배운 것으로 **메뉴 기반 프로그램**을 완성할 수 있다.

---

## 1. 원시 포인터의 문제

```cpp
#include <iostream>
using namespace std;

class Point
{
	int x, y;
public:
	explicit Point(int x = 0, int y = 0) :x(x), y(y) { cout << "Point()" << endl; }
	~Point() { cout << "~Point()" << endl; }
	void Print()const { cout << x << ',' << y << endl; }
};
int main()
{
	int* p = new int(10);
	Point* p1 = new Point(2, 3);

	cout << *p << endl;
	p1->Print();

	delete p;      // 이 두 줄을 빠뜨리면 메모리 누수
	delete p1;     // 중간에 예외가 나면 아예 실행되지도 않는다!
}
```

**세 가지 문제**

| 문제 | 설명 |
|------|------|
| **메모리 누수** | `delete` 를 잊으면 메모리가 계속 쌓인다 |
| **이중 해제** | 실수로 두 번 `delete` 하면 크래시 |
| **예외 안전성** | 중간에 `throw` 가 나면 `delete` 줄에 도달하지 못한다 ([class15.md](class15.md)) |

```cpp
Point* p = new Point(2, 3);
DoSomething();     // 여기서 예외가 나면
delete p;          // 이 줄은 실행되지 않는다 -> 누수
```

> **해결 원리는 이미 배웠습니다 : RAII** ([class6.md](class6.md))
> **"포인터를 클래스로 감싸서, 소멸자가 알아서 `delete` 하게 하자."**
> 그것이 **스마트 포인터**입니다.

---

## 2. unique_ptr - 소유자가 하나뿐

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main()
{
	//스마트포인터(unique_ptr/shared_ptr - weak_ptr)
	unique_ptr<int> p( new int(10) );
	unique_ptr<Point> p1( new Point(2, 3) );

	cout << *p << endl;
	p1->Print();       // -> 로 원래 포인터처럼 쓴다
	(*p1).Print();     // * 로도 쓸 수 있다

	//unique_ptr<Point> p2 = p1;   // X - 복사 불가 (소유자는 한 명뿐)
}                                // delete 를 쓰지 않아도 여기서 해제된다
```

**예상 출력**
```
Point()
10
2,3
2,3
~Point()
```

| 특징 | 설명 |
|------|------|
| 헤더 | `<memory>` |
| 소유권 | **딱 한 개의 unique_ptr 만** 소유 |
| 복사 | **불가** (`p2 = p1` 은 컴파일 에러) |
| 이동 | 가능 (`p2 = std::move(p1)`) |
| 비용 | **원시 포인터와 거의 같다** (추가 메모리 없음) |
| 언제 | **기본으로 이것을 쓴다** |

```cpp
unique_ptr<Point> p2 = std::move(p1);   // 소유권을 넘긴다
// 이제 p1 은 nullptr, p2 가 소유자
```

> **`make_unique` 를 쓰는 것이 더 좋습니다** (C++14)
> ```cpp
> auto p1 = make_unique<Point>(2, 3);   // new 를 쓰지 않는다
> ```

---

## 3. shared_ptr - 여럿이 함께 소유

### 3.1 참조 카운트

```cpp
int main()
{
	shared_ptr<Point> p4;
	{
		shared_ptr<Point> p1(new Point(2, 3));
		shared_ptr<Point> p2 = p1;    // 복사가 된다 (unique_ptr 과 다른 점)
		shared_ptr<Point> p3 = p1;

		cout << p1.use_count() << endl;   // 3
		p1->Print();
		p2->Print();
		p3->Print();
		p4 = p3;                          // 블록 밖의 p4 가 하나 더 든다
	}
	cout << "블록을 벗어남 - 아직 소멸자가 안 불렸다" << endl;
	p4->Print();
}   // 여기서 마지막 p4 가 사라지며 ~Point() 호출
```

**예상 출력**
```
Point()
3
2,3
2,3
2,3
블록을 벗어남 - 아직 소멸자가 안 불렸다
2,3
~Point()
```

**동작 원리**

```
shared_ptr<Point> p1(new Point(2,3));

    p1 ──┐
         ├──▶ [Point(2,3)]     참조 카운트 = 1
         │    [control block: count=1]

shared_ptr<Point> p2 = p1;

    p1 ──┐
    p2 ──┼──▶ [Point(2,3)]     참조 카운트 = 2
         │

... p2 소멸 → 카운트 1 → 아직 해제 안 함
... p1 소멸 → 카운트 0 → 이때 delete!
```

| 특징 | 설명 |
|------|------|
| 소유권 | **여럿이 공유** |
| 복사 | **가능** (카운트 증가) |
| 해제 시점 | **마지막 한 명이 사라질 때** |
| 비용 | 제어 블록(카운트)만큼 메모리 추가 + 카운트 조작 비용 |
| `use_count()` | 현재 몇 명이 소유 중인지 |

---

### 3.2 주요 멤버 함수

```cpp
shared_ptr<Point> p1 = make_shared<Point>(2, 3);   // new 를 쓰지 않는 권장 방식
p1->Print();

shared_ptr<Point> p2(new Point(4, 5));
p2->Print();

p2.reset(new Point(1, 1));    // (4,5) 해제 후 새것으로 교체
p2->Print();

p1.swap(p2);                  // 서로 바꿔 든다
p1->Print();
p2->Print();
```

| 멤버 | 하는 일 |
|------|---------|
| `make_shared<T>(args)` | **권장 생성 방법.** 객체와 제어 블록을 한 번에 할당 (빠르고 안전) |
| `reset()` | 소유권을 놓는다 (카운트 감소) |
| `reset(p)` | 놓고 새것을 든다 |
| `swap(other)` | 서로 교환 |
| `use_count()` | 참조 카운트 |
| `get()` | 원시 포인터를 얻는다 (**소유권은 안 넘어감**) |

> ### `make_shared` 를 쓰는 이유
> 1. **메모리 할당이 1번**입니다. (`shared_ptr<T>(new T)` 는 2번 — 객체 + 제어 블록)
> 2. **예외 안전**합니다.
>    ```cpp
>    f(shared_ptr<Point>(new Point), g());   // g() 에서 예외가 나면 new Point 가 샐 수 있다
>    f(make_shared<Point>(), g());           // 안전
>    ```

---

### 3.3 weak_ptr - 카운트를 올리지 않고 들여다보기

```cpp
shared_ptr<Point> p1(new Point(2, 3));
shared_ptr<Point> p2 = p1;
shared_ptr<Point> p3 = p2;

weak_ptr<Point> w1 = p1;    // 카운트를 올리지 않는다
weak_ptr<Point> w2 = w1;

cout << p1.use_count() << endl;   // 3
cout << w1.use_count() << endl;   // 3 (보기만 한다)

p1->Print();
w1.lock()->Print();               // lock() 으로 잠시 shared_ptr 을 얻어 쓴다
```

**왜 필요한가? — 순환 참조 문제**

```cpp
class Node
{
public:
	shared_ptr<Node> next;
	shared_ptr<Node> prev;   // 서로를 가리키면?
};

auto a = make_shared<Node>();
auto b = make_shared<Node>();
a->next = b;    // b 의 카운트 = 2
b->prev = a;    // a 의 카운트 = 2

// a, b 가 사라져도 카운트가 1 씩 남아서 영원히 해제되지 않는다!
```

- 한쪽을 **`weak_ptr`** 로 바꾸면 카운트가 올라가지 않아 순환이 끊어집니다.

```cpp
class Node
{
public:
	shared_ptr<Node> next;
	weak_ptr<Node>   prev;   // <- 이렇게
};
```

| 사용법 | 설명 |
|--------|------|
| `w.lock()` | 살아 있으면 `shared_ptr` 을, 죽었으면 빈 `shared_ptr` 을 반환 |
| `w.expired()` | 대상이 이미 해제되었는가 |

---

### 3.4 vector 에 담기

```cpp
// (1) 원시 포인터 : erase 하기 전에 delete 를 직접 해야 한다
//vector<Point*> v;
//v.push_back(new Point(1, 1));
//delete* v.begin();
//v.erase(v.begin());
//for (auto p : v) delete p;

// (2) shared_ptr : erase 만 하면 알아서 해제된다
vector< shared_ptr<Point> > v;
v.push_back(make_shared<Point>(1, 1));
v.push_back(make_shared<Point>(2, 2));
v.push_back(make_shared<Point>(3, 3));

for (size_t i = 0; i < v.size(); ++i)
	v[i]->Print();

v.erase(v.begin());     // 여기서 (1,1) 의 소멸자가 호출된다
cout << endl;

for (size_t i = 0; i < v.size(); ++i)
	v[i]->Print();
```

> `vector<Point*>` 는 **`erase` 를 해도 `delete` 가 되지 않습니다.**
> 컨테이너는 담긴 포인터가 무엇을 가리키는지 모르기 때문입니다.
> `vector<shared_ptr<Point>>` 로 바꾸면 이 문제가 사라집니다.

---

## 4. 멤버를 스마트 포인터로

### 4.1 원시 포인터 멤버 - 3의 법칙이 필요하다

```cpp
class Rect
{
	Point* ploc;
	int width;
	int height;
public:
	Rect(int x, int y, int w, int h) :ploc(nullptr), width(w), height(h)
	{
		ploc = new Point(x, y);
	}
	Rect(const Rect& arg) :ploc(nullptr), width(arg.width), height(arg.height)
	{
		ploc = new Point(*arg.ploc);
	}
	~Rect() { delete ploc; }              // 직접 해제해야 한다
	const Rect& operator=(const Rect& arg)
	{
		if (this == &arg) return *this;
		delete ploc;
		ploc = new Point(*arg.ploc);
		width = arg.width;
		height = arg.height;
		return *this;
	}
	void Print()const { ... }
};
```

- **소멸자 + 복사 생성자 + 대입 연산자** 세 개를 모두 만들어야 합니다. ([class6.md](class6.md) 3의 법칙)

---

### 4.2 shared_ptr 멤버 - 소멸자가 필요 없어진다

```cpp
class Rect
{
	shared_ptr<Point> ploc;     // Point* 대신 shared_ptr<Point>
	int width;
	int height;
public:
	Rect(int x, int y, int w, int h) :ploc(nullptr), width(w), height(h)
	{
		ploc = shared_ptr<Point>(new Point(x, y));
	}
	Rect(const Rect& arg) :ploc(nullptr), width(arg.width), height(arg.height)
	{
		ploc = shared_ptr<Point>(new Point(*arg.ploc));   // 깊은 복사는 그대로 필요
	}
	// ~Rect() 가 없다! shared_ptr 이 알아서 해제한다
	const Rect& operator=(const Rect& arg)
	{
		if (this == &arg) return *this;
		ploc.reset(new Point(*arg.ploc));   // delete 를 직접 쓰지 않는다
		width = arg.width;
		height = arg.height;
		return *this;
	}
	void Print()const { ... }
};
```

> **주의** : `shared_ptr` 은 **해제를 자동화**해 줄 뿐, **깊은 복사를 대신해 주지는 않습니다.**
> 복사 생성자에서 `ploc = arg.ploc;` 이라고 쓰면 **두 `Rect` 가 같은 `Point` 를 공유**하게 됩니다.
> 그것이 의도라면 괜찮지만, 아니라면 위처럼 **새로 만들어야** 합니다.

---

## 5. shared_ptr 을 직접 만들어 보기

원리를 이해하기 위해 참조 카운트 방식을 직접 구현해 봅니다.

```cpp
template <typename T>
class Shared_ptr
{
	T* ptr;
	int* pCount;      // 몇 명이 이 객체를 가리키는지 세는 카운터 (공유한다!)
public:
	Shared_ptr(T* p = nullptr) :ptr(p), pCount(nullptr) {
		if (ptr != nullptr)
			pCount = new int(1);
	}
	Shared_ptr(const Shared_ptr& arg) :ptr(arg.ptr), pCount(arg.pCount) {
		++* pCount;                    // 복사되면 한 명 늘어난다
	}
	~Shared_ptr() {
		reset();
	}
	int use_count()const
	{
		if (ptr) return *pCount;
		else     return 0;
	}
	void reset()
	{
		if (ptr == nullptr) return;

		--* pCount;                    // 한 명 줄이고
		if (*pCount == 0)              // 마지막 한 명이었으면
		{
			delete pCount;             // 그때 진짜로 해제한다
			delete ptr;
			pCount = nullptr;
			ptr = nullptr;
		}
	}
	void reset(T* p)
	{
		reset();
		ptr = p;
		pCount = new int(1);
	}
	T* operator->()      // p->Print() 를 가능하게
	{
		return ptr;
	}
	T& operator*()       // (*p).Print() 를 가능하게
	{
		return *ptr;
	}
	const Shared_ptr& operator=(const Shared_ptr& arg)
	{
		if (this == &arg) return *this;

		reset();                       // 내가 들고 있던 것을 먼저 놓고
		ptr = arg.ptr;
		pCount = arg.pCount;
		++* pCount;                    // 새 것을 같이 든다

		return *this;
	}
};
int main()
{
	Shared_ptr<Point> p1(new Point(1, 2));
	Shared_ptr<Point> p2(new Point(3, 4));

	p1 = p2;              // (1,2) 는 여기서 해제된다
	p1->Print();
	(*p1).Print();
	p2->Print();
	cout << p1.use_count() << endl;   // 2
	p1.reset(new Point(5, 5));
	p1->Print();
	cout << p1.use_count() << endl;   // 1
}
```

**핵심 아이디어**

| 요소 | 왜 필요한가 |
|------|-------------|
| `int* pCount` | **포인터로 두어야** 여러 `Shared_ptr` 이 **같은 카운터**를 공유한다 |
| 복사 생성자 | 카운트 **증가** |
| 소멸자 → `reset()` | 카운트 **감소**, 0 이면 진짜 `delete` |
| `operator->` | `p->Print()` 를 원래 포인터처럼 ([class11.md](class11.md)) |
| `operator*` | `(*p).Print()` |
| `operator=` | 옛것 놓고(감소) 새것 들기(증가) |

> **`operator->` 의 특별한 규칙**
> `p->Print()` 는 컴파일러가 이렇게 처리합니다.
> ```cpp
> p.operator->()->Print();
> //  ↑ T* 를 반환하면  ↑ 그 포인터로 다시 -> 를 적용
> ```
> 즉 `operator->` 는 **포인터를 반환하기만 하면** 되고, 나머지는 컴파일러가 이어서 처리합니다.

---

## 6. 파일 입출력 - ofstream

```cpp
#include <iostream>
#include <fstream>
using namespace std;
int main()
{
	ofstream fout("test.txt");        // 파일 열기 (없으면 만든다)
	int arr[10] = { 10,20,30,40,50 };

	for (int i = 0; i < 10; ++i)
		fout << "[" << i << "]: " << arr[i] << endl;   // cout 쓰듯이 쓴다

	fout.close();                     // 생략해도 소멸자가 닫아 준다
}
```

**test.txt 내용**
```
[0]: 10
[1]: 20
[2]: 30
[3]: 40
[4]: 50
[5]: 0
...
```

| 클래스 | 헤더 | 용도 |
|--------|------|------|
| `ofstream` | `<fstream>` | 파일에 **쓰기** (output file stream) |
| `ifstream` | `<fstream>` | 파일에서 **읽기** (input file stream) |
| `fstream` | `<fstream>` | 읽기 + 쓰기 |

**C 와 비교**

| | C | C++ |
|---|---|-----|
| 열기 | `FILE* f = fopen("a.txt", "w");` | `ofstream fout("a.txt");` |
| 쓰기 | `fprintf(f, "%d\n", n);` | `fout << n << endl;` |
| 닫기 | `fclose(f);` **(잊으면 안 됨)** | **자동** (소멸자) |
| 형식 지정자 | 필요 | 불필요 |

> **`close()` 를 안 써도 되는 이유** : `ofstream` 도 **RAII** 이기 때문입니다. ([class6.md](class6.md))
> 객체가 사라질 때 소멸자가 파일을 닫아 줍니다. 예외가 나도 마찬가지입니다.

**파일 읽기 예**

```cpp
ifstream fin("test.txt");
if (!fin)                       // 열기 실패 검사
{
	cout << "파일을 열 수 없습니다." << endl;
	return 1;
}
string line;
while (getline(fin, line))      // 한 줄씩 읽기
	cout << line << endl;
```

---

### 6.1 `ostream&` 매개변수의 활용

```cpp
void Print(ostream& out = cout)const     // 화면에도, 파일에도 출력할 수 있다
{
	out << "name:" << name << ", " << "phone:" << phone << endl;
}
```

```cpp
per.Print();          // 화면으로 (기본값 cout)
per.Print(fout);      // 파일로
```

- `cout` 과 `ofstream` 은 **둘 다 `ostream` 을 상속**합니다.
- 그래서 `ostream&` 하나로 **둘 다 받을 수 있습니다.** ← 다형성! ([class9.md](class9.md))

```
        ostream
       ╱       ╲
   ofstream   ostringstream    (그리고 cout 은 ostream 객체)
```

> **반드시 참조(`&`)로 받아야 합니다.** 스트림은 복사할 수 없습니다. ([class13.md](class13.md))

---

## 7. 종합 실습 - 전화번호부

### 7.1 설계부터 시작하기

```cpp
class TelPhoneBook
{
public:
	void AddTel();          //전화 번호를 등록
	void PrintTel()const;   //전화 번호 출력
	void SearchTel()const;  //전화 검색(이름 or 전화 번호)
	void RemoveTel();       //전화 삭제(이름 or 전화 번호)
	void WriteTextTel()const; //텍스트 파일의 등록 전화번호 출력
};
```

- **무엇이 필요한지 먼저 적고** 나중에 채웁니다.
- 이 단계에서 이름과 `const` 여부를 정해 두면 구현이 훨씬 수월합니다.

---

### 7.2 Person - char* 대신 string

원본 예제는 `char*` 로 직접 관리하는 버전도 보여 줍니다.

```cpp
class Person
{
	char* name;
	char* phone;
private:
	void _copy(const char* n, const char* p) { ... new char[] ... }
public:
	Person(const char* n = "", const char* p = "") { _copy(n, p); }
	Person(const Person& arg) { _copy(arg.name, arg.phone); }   // 복사 생성자
	~Person() { delete[] name; delete[] phone; }                // 소멸자
	const Person& operator=(const Person& arg) { ... }          // 복사 대입
	...
};
```

- **3의 법칙**을 전부 손으로 지켜야 합니다. ([class6.md](class6.md))
- 문자열 비교도 `strcmp(a, b) == 0` 으로 해야 합니다.

**string 을 쓰면**

```cpp
class Person
{
	string name;
	string phone;
public:
	Person(const string& n = "", const string& p = "") :name(n), phone(p) { }
	// 복사 생성자 / 소멸자 / operator= 를 만들 필요가 없다!
	...
};
```

```cpp
if (per.GetName() == name)   // == 로 바로 비교
```

> **string 을 쓰면 3의 법칙을 지킬 필요가 없습니다.**
> `string` 이 이미 자기 몫을 알아서 하기 때문에, **컴파일러가 만든 기본 동작으로 충분**합니다.
> 이것을 **0의 법칙(Rule of Zero)** 이라고 합니다.

---

### 7.3 완성 코드 (ex16.cpp 맨 위)

```cpp
class Person
{
	string name;
	string phone;
public:
	Person(const string& n = "", const string& p = "") :name(n), phone(p) { }
	void Print(ostream& out = cout)const     // 화면에도, 파일에도 출력할 수 있다
	{
		out << "name:" << name << ", " << "phone:" << phone << endl;
	}
	const string& GetName()const { return name; }
	const string& GetPhone()const { return phone; }
};

class TelPhoneBook
{
	vector<Person> phoneList;
public:
	void AddTel()
	{
		cout << "이름 입력: ";
		string name;   cin >> name;
		cout << "전화 입력: ";
		string phone;  cin >> phone;

		phoneList.push_back(Person(name, phone));
	}
	void PrintTel()const
	{
		for (auto& per : phoneList)
			per.Print();
	}
	void SearchTel()const { ... }
	void RemoveTel()
	{
		...
		phoneList.erase(phoneList.begin() + i);   // 반복자 + 정수
		...
	}
	void WriteTextTel()const
	{
		ofstream fout("phonebook.txt");      // 파일을 연다 (소멸자가 알아서 닫는다)
		for (vector<Person>::size_type i = 0; i < phoneList.size(); ++i)
		{
			fout << "[" << i << "]: ";
			phoneList[i].Print(fout);        // cout 대신 fout 을 넘긴다
		}
	}
};

class Util
{
public:
	static void Menu()      // static : 객체 없이 호출 (class14.md)
	{
		cout << "1. 전화 등록" << endl;
		...
	}
};

int main()
{
	TelPhoneBook tpb;
	bool run = true;

	while (run)
	{
		Util::Menu();
		switch (_getch())
		{
		case '1': tpb.AddTel();        break;
		case '2': tpb.PrintTel();      break;
		case '3': tpb.SearchTel();     break;
		case '4': tpb.RemoveTel();     break;
		case '5': tpb.WriteTextTel();  break;
		case '0': run = false;         break;
		default:                       break;
		}
	}
}
```

**실행 화면**
```
1. 전화 등록
2. 전화 번호 출력
3. 전화 번호 검색
4. 전화 번호 삭제
5. 전화 번호 텍스트 파일 출력
0. 프로그램 종료

(1 을 누르면)
이름 입력: 홍길동
전화 입력: 010-1234-5678
```

| 요소 | 설명 |
|------|------|
| `<conio.h>` | Windows 전용 헤더 |
| `_getch()` | **엔터를 누르지 않아도** 키 하나를 바로 읽는다 |
| `switch (_getch())` | 문자 상수(`'1'`)와 비교 |

> `_getch()` 는 표준이 아니며 **Windows 에서만** 동작합니다.
> 이식성이 필요하면 `cin >> menu;` 를 쓰세요.

---

## 8. 파일 분리 버전 - app/telphonebook

`example/src/app/telphonebook/` 에는 같은 프로그램을 **9개 파일로 나누고 상속까지 적용**한 완성판이 있습니다.

```
app/telphonebook/
├─ Person.h / Person.cpp            <- 부모 클래스 (virtual Print, virtual ~)
├─ Student.h / Student.cpp          <- Person 상속 (+ grade)
├─ Professor.h / Professor.cpp      <- Person 상속 (+ position)
├─ TelPhoneBook.h / TelPhoneBook.cpp <- vector<shared_ptr<Person>>
├─ Util.h / Util.cpp                <- static Menu()
├─ Main.h                           <- 공통 헤더 모음
└─ TelMain.cpp                      <- main()
```

### 8.1 다형성 적용

```cpp
class Person
{
protected:
	string name;
	string phone;
public:
	Person(const string& n = "", const string& p = "");
	virtual ~Person();                                // 가상 소멸자 (필수)
	virtual void Print(ostream& out = cout)const;     // 화면에도 파일에도 출력
	const string& GetName()const;
	const string& GetPhone()const;
};
```

```cpp
class TelPhoneBook
{
	std::vector< std::shared_ptr<Person> > phoneList;
	...
};
```

```cpp
void TelPhoneBook::PrintTel()const
{
	for (auto& per : phoneList)
		per->Print();      // Person / Student / Professor 각자의 Print()
}
```

- `phoneList` 는 `Person` 을 가리키지만 **실제로는 `Student` / `Professor`** 가 들어 있습니다.
- `per->Print()` 한 줄이 각자 다르게 동작합니다. ← **다형성** ([class9.md](class9.md))

---

### 8.2 원시 포인터 vs shared_ptr

```cpp
// (A) 원시 포인터 버전 - 직접 정리해야 한다
class TelPhoneBook
{
	std::vector<Person*> phoneList;
public:
	~TelPhoneBook()                       // 소멸자를 반드시 만들어야 하고
	{
		for (auto p : phoneList)
			delete p;
	}
	void RemoveTel();                     // erase 전에 delete 도 직접 해야 한다
};

// (B) shared_ptr 버전 - 소멸자가 필요 없다
class TelPhoneBook
{
	std::vector< std::shared_ptr<Person> > phoneList;
	// 소멸자 없음!
};
```

```cpp
phoneList.erase(phoneList.begin() + i);   // shared_ptr 이 알아서 해제
```

---

### 8.3 자주 하는 실수 : `ostream` 을 값으로 받기

```cpp
void Print(ostream out)const   // X - 컴파일 에러 : ostream 은 복사할 수 없다
void Print(ostream& out)const  // O
```

원본 예제에는 이 실수가 있었습니다. 이 프로젝트에서는 **참조로 고쳐** 두었습니다.

---

### 8.4 빌드 방법

1. `ex1.cpp` ~ `ex16.cpp` 를 **모두 "빌드에서 제외 : 예"** 로 둡니다.
2. `app/telphonebook` 폴더의 **`.cpp` 6개**(Person, Student, Professor, TelPhoneBook, Util, TelMain)만 제외를 풉니다.
3. **Ctrl + F5** 로 실행합니다.

---

## 9. 이 수업에서 배운 것 총정리

| 강의 | 주제 | 핵심 문장 |
|------|------|-----------|
| [1](class1.md) | C++ 입문 | `cout` 은 자료형을 알아서 처리한다. 참조자는 변수의 별명이다 |
| [2](class2.md) | 함수·namespace·string | 클래스를 넘길 땐 `const 형식&` |
| [3](class3.md) | new/delete, struct→class | 데이터와 함수를 하나로 묶는다 |
| [4](class4.md) | 클래스·생성자·소멸자 | 객체는 언제 태어나고 언제 죽는가 |
| [5](class5.md) | 접근자·const·this | 안 바꾸는 함수엔 `const` 를 붙인다 |
| [6](class6.md) | 복사 생성자·깊은 복사 | 포인터 멤버가 있으면 **3의 법칙** |
| [7](class7.md) | 반환값·operator= | 참조를 반환하면 대입이 가능해진다 |
| [8](class8.md) | 상속 | 값으로 업캐스팅하면 **슬라이싱** |
| [9](class9.md) | 가상 함수·다형성 | `virtual` 과 **가상 소멸자** |
| [10](class10.md) | 추상 클래스·인터페이스 | 같은 객체를 여러 관점으로 |
| [11](class11.md) | 연산자 오버로딩 | 새 값은 값 반환, 자기 변경은 참조 반환 |
| [12](class12.md) | 함수 객체·람다 | 동작을 값처럼 넘긴다 |
| [13](class13.md) | 전역 연산자·friend | `cout << 내객체;` 만들기 |
| [14](class14.md) | static·템플릿·STL | 템플릿은 코드를 찍어 내는 틀 |
| [15](class15.md) | 예외·형변환 | catch 는 구체적인 것부터 |
| [16](class16.md) | 스마트 포인터·파일 | **`delete` 를 직접 쓰지 않는다** |

---

## 10. 정리 표

| 주제 | 내용 |
|------|------|
| 원시 포인터의 문제 | 누수 / 이중 해제 / **예외가 나면 delete 못 함** |
| 스마트 포인터의 원리 | **RAII** — 소멸자가 알아서 `delete` |
| `unique_ptr` | 소유자 **한 명**. 복사 불가, 이동만. **기본으로 이것** |
| `shared_ptr` | 소유자 **여럿**. 참조 카운트가 0 이 되면 해제 |
| `weak_ptr` | 카운트를 **안 올리고** 들여다본다. **순환 참조 해결** |
| `make_shared` | 할당 1번, 예외 안전. **권장** |
| `reset()` / `reset(p)` | 놓기 / 놓고 새것 들기 |
| `use_count()` | 참조 카운트 |
| 멤버를 shared_ptr 로 | **소멸자를 안 써도 된다.** 단, 깊은 복사는 여전히 직접 |
| `operator->` | 포인터만 반환하면 컴파일러가 이어서 처리 |
| `<fstream>` | `ofstream`(쓰기) / `ifstream`(읽기) / `fstream` |
| 파일 닫기 | **자동** (RAII). `close()` 생략 가능 |
| `ostream&` 매개변수 | `cout` 과 `ofstream` 을 **하나로** 받는다 (다형성) |
| **0의 법칙** | `string`, `vector`, 스마트 포인터를 쓰면 3의 법칙이 필요 없다 |
| `_getch()` | Windows 전용. 엔터 없이 키 하나 |

---

## 11. 연습 문제 (스스로 풀어보기)

1. `unique_ptr<int> p2 = p1;` 을 시도해 보고 어떤 에러가 나는지 확인한 뒤, `std::move` 로 고쳐 보세요.
2. `shared_ptr` 3개가 같은 객체를 가리키게 하고, 하나씩 `reset()` 하며 `use_count()` 를 출력해 보세요.
3. `Node` 클래스로 순환 참조를 만들어 소멸자가 안 불리는 것을 확인한 뒤, `weak_ptr` 로 고쳐 보세요.
4. `Shared_ptr` 을 직접 구현하고, `use_count()` 가 표준 `shared_ptr` 과 같게 나오는지 비교하세요.
5. `Shared_ptr::operator=` 에서 자기 대입 검사를 지우고 `p1 = p1;` 을 실행해 보세요.
6. `ifstream` 으로 `phonebook.txt` 를 읽어 화면에 출력하는 기능을 전화번호부에 추가하세요.
7. 전화번호부에 **파일에서 불러오기(LoadTextTel)** 를 추가해, 프로그램을 다시 실행해도 데이터가 남게 만드세요.
8. 전화번호를 **이름순으로 정렬**하는 메뉴를 추가하세요. (힌트 : `sort` + 람다, [class12.md](class12.md))
9. 중복된 이름을 등록하려 하면 **예외를 던지고** 메뉴에서 잡아 메시지를 출력하세요. ([class15.md](class15.md))
10. `app/telphonebook` 에 `Staff` 클래스를 추가하고 메뉴 항목도 늘려 보세요. **`PrintTel()` 을 고쳐야 하나요?**

---

## 12. 여기서 더 나아가려면

이 수업에서 다루지 못한 것들입니다. 관심 있는 순서대로 공부해 보세요.

| 주제 | 왜 필요한가 |
|------|-------------|
| **이동 의미론** (`std::move`, 이동 생성자) | 큰 객체를 복사 없이 옮긴다 |
| **STL 컨테이너 심화** (`map`, `set`, `unordered_map`) | 검색이 필요한 자료 구조 |
| **`std::function`** | 함수 객체·람다·함수 포인터를 하나의 형식으로 |
| **가변 인자 템플릿** | `make_shared` 처럼 인자를 그대로 전달하기 |
| **멀티스레딩** (`<thread>`, `<mutex>`) | 동시에 여러 일 하기 |
| **디자인 패턴** | 팩토리, 옵저버, 전략 패턴 등 |

---

이 강의 자료는 **example/src/ex16.cpp** 와 **example/src/app/telphonebook/** 예제를 바탕으로 구성했습니다.

마지막 시간의 핵심은 **"`new` 와 `delete` 를 직접 쓰지 않는다"** 는 현대 C++ 의 원칙입니다.
`unique_ptr`, `shared_ptr`, `vector`, `string` 을 쓰면 메모리 관리로 고생할 일이 거의 사라집니다.
그런데 그 도구들이 **어떻게 만들어졌는지**를 [class6.md](class6.md) 부터 직접 만들어 봤기 때문에, 여러분은 이제 **그것들을 믿고 쓸 수 있게** 되었습니다.

수고하셨습니다.
