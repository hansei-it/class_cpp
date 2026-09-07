# C++ 기초 강의 9 - ex9.cpp 코드를 통한 가상 함수와 다형성

이 강의는 **example/src/ex9.cpp** 에 담긴 예제들을 바탕으로, `virtual` 한 단어가 만드는 **다형성(polymorphism)** 과 **순수 가상 함수 · 추상 클래스**, 그리고 반드시 지켜야 할 **가상 소멸자** 규칙을 정리한 내용입니다.

---

## 0. 아홉 번째 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- `virtual` 을 붙이면 **실제 객체의 함수**가 호출되는 것을 확인할 수 있다.
- **함수 재정의(override)** 와 **함수 중복(overload)** 의 차이를 설명할 수 있다.
- **순수 가상 함수(`= 0`)** 와 **추상 클래스**를 만들 수 있다.
- 부모 포인터 배열로 여러 종류의 객체를 **한꺼번에** 다룰 수 있다.
- 생성자·소멸자의 호출 순서를 안다.
- **가상 소멸자가 없으면 메모리가 샌다**는 것을 실행으로 확인할 수 있다.
- **범위 기반 for** 를 쓸 수 있다.

### 0.2 이번 강의의 한 문장

> **"부모 포인터 하나로 어떤 자식이든 다룰 수 있게 하는 것"** 이 다형성이고, 그것을 가능하게 하는 키워드가 **`virtual`** 입니다.

---

## 1. virtual - 한 단어로 해결된다

### 1.1 [class8.md](class8.md) 의 문제를 다시 보기

```cpp
Person& rp = s1;   // s1 은 Student
rp.Print();        // Person::Print() 가 불렸다  <- 문제
```

### 1.2 `virtual` 을 붙이면

```cpp
class Person
{
protected:
	string name;
	int age;
public:
	Person(const string& n = "", int a = 0) :name(n), age(a) {}
	virtual void Print() const      // <= virtual 을 붙였다
	{
		cout << "name: " << name << ", " << "age: " << age << endl;
	}
	const string& GetName()const { return name; }
	int GetAge()const { return age; }
};
class Student : public Person
{
	int grade;
public:
	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g) {}
	virtual void Print() const //메소드 재정의(function override)
	{
		cout << "name: " << GetName() << ", " << "age: " << GetAge() << ", ";
		cout << "grade: " << GetGrade() << endl;
	}
	int GetGrade()const { return grade; }
};
int main()
{
	Person p1("홍길동", 25);
	Student s1("이몽룡", 29, 3);

	Person& rp = s1;   //O
	Person* p = &s1;   //O

	rp.Print(); //다형적인 메소드 -> Student::Print() 가 호출된다
	p->Print();
}
```

**예상 출력**
```
name: 이몽룡, age: 29, grade: 3
name: 이몽룡, age: 29, grade: 3
```

**무엇이 달라졌나**

| | `virtual` 없음 | `virtual` 있음 |
|---|----------------|----------------|
| 결정 시점 | **컴파일 시점** (정적 바인딩) | **실행 시점** (동적 바인딩) |
| 무엇을 보고 결정 | **포인터/참조의 형식** | **실제 객체의 형식** |
| `Person* p = &s1; p->Print();` | `Person::Print()` | **`Student::Print()`** |

---

### 1.3 어떻게 동작하나 - 가상 함수 테이블(vtable)

```
클래스마다 "가상 함수 주소표(vtable)" 가 하나씩 만들어진다

Person 의 vtable            Student 의 vtable
┌──────────────────┐        ┌──────────────────┐
│ Print → Person::Print │   │ Print → Student::Print │
└──────────────────┘        └──────────────────┘

객체마다 자기 클래스의 vtable 을 가리키는 숨은 포인터(vptr)가 생긴다

s1 (Student 객체)
┌─────────┐
│ vptr ───┼──▶ Student 의 vtable
│ name    │
│ age     │
│ grade   │
└─────────┘

p->Print();  실행 시 → p 가 가리키는 객체의 vptr 를 따라가서 → Student::Print 호출
```

- 그래서 가상 함수가 **하나라도** 있으면 객체 크기가 **포인터 하나만큼(64비트에서 8바이트)** 커집니다.
- 호출할 때 표를 한 번 거치므로 **아주 약간 느립니다.** (거의 무시할 수준)

---

### 1.4 override / overload 구분

| | 중복 (overload) | **재정의 (override)** |
|---|-----------------|----------------------|
| 우리말 | 함수 중복 | 메소드 재정의 |
| 어디서 | **같은 클래스 안** | **부모 ↔ 자식** |
| 조건 | 매개변수가 **달라야** 한다 | 이름·매개변수·const 가 **완전히 같아야** 한다 |
| `virtual` | 무관 | **필요** |
| 배운 곳 | [class2.md](class2.md) | 여기 |

**`override` 키워드 (C++11) 를 쓰면 안전합니다**

```cpp
virtual void Print() const override   // "나는 부모 것을 재정의한다"
{ ... }
```

- 오타나 매개변수 실수로 **재정의가 아니라 새 함수를 만들어 버렸을 때** 컴파일 에러로 잡아 줍니다.

```cpp
class Student : public Person
{
	void Print()          // const 를 빠뜨렸다! -> 재정의가 아니라 새 함수
	{ ... }
	void Print() override // <- override 를 붙였다면 여기서 컴파일 에러로 잡힌다
	{ ... }
};
```

---

### 1.5 자식에서는 `virtual` 을 생략해도 된다

```cpp
class Professor : public Person
{
	void Print() const           // virtual 은 물려받으므로 생략해도 가상 함수다
	{ ... }
};
```

- 부모가 `virtual` 이면 자식의 같은 함수는 **자동으로 가상 함수**입니다.
- 하지만 **읽는 사람을 위해 `virtual` 또는 `override` 를 써 주는 것**이 좋습니다.

---

## 2. 순수 가상 함수와 추상 클래스

### 2.1 "부모는 어떻게 출력할지 모른다"

```cpp
class Person
{
protected:
	string name;
	int age;
public:
	Person(const string& n = "", int a = 0) :name(n), age(a) {}
	virtual void Print() const = 0; //순수 가상 함수 -> Person 은 추상 클래스
	const string& GetName()const { return name; }
	int GetAge()const { return age; }
};
```

**`= 0` 의 의미**

| 항목 | 설명 |
|------|------|
| 문법 | `virtual 반환형 함수이름(...) = 0;` |
| 뜻 | **"몸통을 여기서는 정의하지 않는다. 자식이 반드시 만들어라."** |
| 결과 | 이 클래스는 **추상 클래스(abstract class)** 가 된다 |

**추상 클래스의 성질**

```cpp
Person p("아무개", 20);   // X - 에러 : 추상 클래스는 객체를 만들 수 없다
Person* p = new Student("홍길동", 24, 3);   // O - 포인터는 가능
Person& r = s1;                              // O - 참조도 가능
```

**컴파일 에러**
```
error C2259: 'Person': 추상 클래스를 인스턴스화할 수 없습니다.
```

> **왜 좋은가?**
> 1. `Person` 을 실수로 직접 만드는 것을 막는다 ("사람" 자체는 실체가 없다)
> 2. 자식이 `Print()` 를 **반드시 구현하게 강제**한다 (안 하면 자식도 추상 클래스가 되어 객체를 못 만든다)
> 3. **인터페이스(약속)** 를 표현할 수 있다 ([class10.md](class10.md))

---

### 2.2 순수 가상 함수를 자식이 구현하지 않으면?

```cpp
class Student : public Person
{
	int grade;
public:
	Student(...) : Person(n,a), grade(g) {}
	// Print() 를 구현하지 않았다!
};
Student s;   // X - 에러 : Student 도 추상 클래스다
```

---

## 3. 다형성의 진짜 쓸모 - 부모 포인터 배열

```cpp
int main()
{
	Person* parr[5] = { new Student("홍길동",20,2), new Professor("임꺽정",30,"조교수"),
	new Student("이몽룡",21,3), new Professor("성춘향",30,"부교수"),
	new Student("장길산",26,4) };

	for (Person* p : parr)
		p->Print();     // Student / Professor 각자의 Print() 가 호출된다

	for (Person* p : parr)
		delete p;
}
```

**예상 출력**
```
name: 홍길동, age: 20, grade: 2
name: 임꺽정, age: 30, position: 조교수
name: 이몽룡, age: 21, grade: 3
name: 성춘향, age: 30, position: 부교수
name: 장길산, age: 26, grade: 4
```

> ### 이것이 다형성입니다
>
> ```cpp
> p->Print();
> ```
>
> **이 한 줄이** 상황에 따라 `Student::Print()` 가 되기도 하고 `Professor::Print()` 가 되기도 합니다.
>
> `if (학생이면) ... else if (교수면) ...` 같은 분기가 **전혀 없습니다.**
>
> 나중에 `Staff` 클래스를 추가해도 **이 for 문은 한 글자도 안 고쳐도 됩니다.**

---

### 3.1 범위 기반 for (range-based for)

```cpp
double arr[5] = { 10,20,30,40,50 };
vector<double> v;
v.push_back(100);

for (auto item : arr)     // 배열
	cout << item << " ";
cout << endl;

for (auto item : v)       // vector
	cout << item << " ";
cout << endl;
```

**세 가지 형태**

| 형태 | 뜻 | 언제 |
|------|-----|------|
| `for (auto x : v)` | **복사**해서 받는다 | 원소가 작을 때 (`int`, `double`) |
| `for (const auto& x : v)` | **참조**로 받되 수정 금지 | 원소가 클 때 (**가장 많이 쓴다**) |
| `for (auto& x : v)` | 참조로 받아 **수정** | 원소를 바꿀 때 |

---

## 4. 생성자·소멸자의 호출 순서

```cpp
class Person
{
public:
	Person(...) : name(n), age(a) { cout << "Person() " << endl; }
	~Person() { cout << "~Person()" << endl; }
	...
};
class Student : public Person
{
public:
	Student(...) : Person(n, a), grade(g) { cout << "Student()" << endl; }
	~Student() { cout << "~Student()" << endl; }
	...
};
int main()
{
	Student s1("홍길동", 24, 3);
	s1.Print();
}
```

**예상 출력**
```
Person() 
Student()
name: 홍길동, age: 24, grade: 3
~Student()
~Person()
```

| | 순서 |
|---|------|
| **생성** | **부모 → 자식** (기초 공사부터) |
| **소멸** | **자식 → 부모** (생성의 역순) |

- 자식 생성자 몸통이 실행될 때는 **부모 부분이 이미 완성**되어 있습니다.
- 자식 소멸자가 실행될 때는 **부모 부분이 아직 살아 있습니다.**

---

## 5. 가상 소멸자 - 반드시 지켜야 할 규칙

### 5.1 가상 소멸자가 없으면

```cpp
class Person
{
public:
	Person(...) { cout << "Person() " << endl; }
	~Person() { cout << "~Person()" << endl; }   // virtual 이 없다!
	virtual void Print() const = 0;
	...
};
class Student : public Person
{
public:
	~Student() { cout << "~Student()" << endl; }
	...
};
int main()
{
	Person* p = new Student("홍길동", 24, 3);
	p->Print();
	delete p;   // ~Person() 만 호출된다! Student 가 잡은 자원이 샌다
}
```

**예상 출력 (문제 있음)**
```
Person() 
Student()
name: 홍길동, age: 24, grade: 3
~Person()      <- ~Student() 가 없다!
```

**왜?**

- `delete p;` 에서 `p` 의 **형식은 `Person*`** 입니다.
- 소멸자가 `virtual` 이 아니므로 **컴파일 시점에 `~Person()` 으로 결정**됩니다.
- `Student` 가 `new` 로 잡아 둔 자원이 있었다면 **그대로 샙니다.**

---

### 5.2 `virtual` 을 붙이면 해결

```cpp
virtual ~Person() { cout << "~Person()" << endl; }   // <= virtual 추가
```

**예상 출력 (정상)**
```
Person() 
Student()
name: 홍길동, age: 24, grade: 3
~Student()     <- 자식 먼저
~Person()      <- 그 다음 부모
```

> ### 절대 규칙
> **"상속의 부모가 될 클래스는 소멸자를 반드시 `virtual` 로 만든다."**
>
> 예외 : 그 클래스의 포인터로 `delete` 를 절대 하지 않는다고 **확신**할 때만 생략 가능.
> 판단이 애매하면 **그냥 붙이세요.** 비용은 포인터 하나뿐입니다.

---

### 5.3 가상 함수 vs 비-가상 함수

```cpp
Person* p = new Student("홍길동", 24, 3);

p->GetName();//비 가상 함수 -> Person::GetName()
p->Print();  // 가상 함수   -> Student::Print()

delete p;
```

| 함수 | `virtual`? | 무엇을 보고 결정 |
|------|------------|------------------|
| `GetName()` | X | **포인터의 형식** (`Person`) |
| `Print()` | O | **실제 객체** (`Student`) |

- `GetName()` 은 자식이 재정의하지 않으므로 어차피 결과가 같습니다.
- 자식이 재정의할 가능성이 있는 함수에만 `virtual` 을 붙이면 됩니다.

---

## 6. 완성 코드 (ex9.cpp 맨 위)

```cpp
class Person   // 추상 클래스
{
protected:
	string name;
	int age;
public:
	Person(const string& n = "", int a = 0) :name(n), age(a)
	{
		cout << "Person() " << endl;
	}
	virtual ~Person() { cout << "~Person()" << endl; }   // 가상 소멸자 (필수!)
	virtual void Print() const = 0;                      //순수 가상 함수
	const string& GetName()const { return name; }        //비 가상 함수
	int GetAge()const { return age; }
};

class Student : public Person
{
	int grade;
public:
	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g)
	{ cout << "Student()" << endl; }
	~Student() { cout << "~Student()" << endl; }
	virtual void Print() const   //메소드 재정의(function override)
	{
		cout << "name: " << GetName() << ", " << "age: " << GetAge() << ", ";
		cout << "grade: " << GetGrade() << endl;
	}
	int GetGrade()const { return grade; }
};

class Professor : public Person { ... };   // 같은 방식

int main()
{
	//Person p("아무개", 20);   // X - 추상 클래스는 객체를 만들 수 없다

	Person* parr[5] = { new Student("홍길동",20,2), new Professor("임꺽정",30,"조교수"),
		new Student("이몽룡",21,3), new Professor("성춘향",30,"부교수"),
		new Student("장길산",26,4) };

	for (Person* p : parr)   // 범위 기반 for
		p->Print();          // 각자 자기 Print() 가 호출된다 <- 다형성

	for (Person* p : parr)
		delete p;            // 가상 소멸자 덕분에 ~Student() 까지 호출된다
}
```

**예상 출력**
```
Person() 
Student()
Person() 
Professor()
Person() 
Student()
Person() 
Professor()
Person() 
Student()
name: 홍길동, age: 20, grade: 2
name: 임꺽정, age: 30, position: 조교수
name: 이몽룡, age: 21, grade: 3
name: 성춘향, age: 30, position: 부교수
name: 장길산, age: 26, grade: 4
~Student()
~Person()
~Professor() 
~Person()
~Student()
~Person()
~Professor() 
~Person()
~Student()
~Person()
```

> 마지막 부분에서 **`~Student()` 뒤에 `~Person()`** 이 짝을 이루어 나오는 것을 확인하세요. 가상 소멸자가 제대로 동작한다는 증거입니다.

---

## 7. 정리 표

| 주제 | 내용 |
|------|------|
| `virtual` | 실행 시점에 **실제 객체**를 보고 함수를 결정 (동적 바인딩) |
| vtable / vptr | 가상 함수 주소표와 그것을 가리키는 숨은 포인터 |
| 크기 증가 | 가상 함수가 하나라도 있으면 객체가 포인터 하나만큼 커진다 |
| override | 부모 ↔ 자식, 이름·매개변수·const 가 **완전히 같아야** |
| overload | 같은 클래스, 매개변수가 **달라야** |
| `override` 키워드 | 재정의 실수를 컴파일 에러로 잡아 준다 (C++11) |
| 자식의 `virtual` | 생략 가능하지만 써 주는 것이 좋다 |
| 순수 가상 함수 | `virtual void f() = 0;` — 자식이 반드시 구현 |
| 추상 클래스 | 순수 가상 함수가 하나라도 있으면. **객체 생성 불가** |
| 부모 포인터 배열 | `Person* arr[5]` — 여러 종류를 한꺼번에, `if` 분기 없이 |
| 범위 기반 for | `for (const auto& x : v)` 를 기본으로 |
| 생성 순서 | **부모 → 자식** |
| 소멸 순서 | **자식 → 부모** |
| **가상 소멸자** | 부모가 될 클래스는 **반드시** `virtual ~클래스() {}` |
| 비-가상 함수 | 포인터의 **형식**을 보고 결정된다 |

---

## 8. 연습 문제 (스스로 풀어보기)

1. `class Shape { public: virtual double Area() const = 0; virtual ~Shape() {} };` 를 만들고 `Circle`, `Rect` 를 상속시켜 각자 `Area()` 를 구현하세요.
2. `Shape* arr[3]` 에 담고 범위 기반 for 로 모든 넓이를 출력하세요.
3. 1번에서 `= 0` 을 지우고 `Shape s;` 를 만들어 보세요. 컴파일이 되나요?
4. `Circle` 에서 `Area()` 구현을 지우고 `Circle c;` 를 만들어 보세요. 어떤 에러가 나나요?
5. `virtual ~Shape()` 에서 `virtual` 을 지우고, `Circle` 소멸자에 메시지를 넣은 뒤 `delete` 해 보세요. `~Circle()` 이 불리나요?
6. `Circle::Area()` 의 `const` 를 빼 보고, `override` 키워드를 붙였을 때와 안 붙였을 때 에러가 어떻게 다른지 비교하세요.
7. `Shape` 에 `virtual void Draw() const { cout << "Shape" << endl; }` (몸통 있는 가상 함수)를 추가하고, `Circle` 에서만 재정의한 뒤 두 객체로 각각 호출해 보세요.
8. `sizeof(Shape)` 와, `virtual` 을 모두 지운 버전의 `sizeof` 를 비교해 보세요.

---

이 강의 자료는 **example/src/ex9.cpp** 에 있는 모든 예제를 바탕으로 구성했습니다. 이번 시간의 핵심은 **`p->Print();` 한 줄이 상황에 따라 다르게 동작한다**는 다형성과, **가상 소멸자를 빠뜨리면 안 된다**는 규칙입니다. 특히 5장의 실습은 반드시 직접 실행해서 `~Student()` 가 사라지는 것을 눈으로 확인하세요.
