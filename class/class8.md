# C++ 기초 강의 8 - ex8.cpp 코드를 통한 상속(inheritance)

이 강의는 **example/src/ex8.cpp** 에 담긴 예제들을 바탕으로, **중복 코드를 부모 클래스로 뽑아내는 상속**과 **업캐스팅**, 그리고 초보자가 반드시 만나는 **객체 슬라이싱** 문제를 정리한 내용입니다.

---

## 0. 여덟 번째 수업 준비

### 0.1 이 수업에서 배우는 것 (학습 목표)

- **임시 객체(무명 객체)** 가 언제 생기고 언제 사라지는지 안다.
- 중복되는 멤버를 **부모 클래스로 뽑아낼** 수 있다.
- `class Student : public Person` 문법을 쓸 수 있다.
- `protected` 가 `private` 와 무엇이 다른지 설명할 수 있다.
- 자식 생성자에서 **부모 생성자를 호출**할 수 있다.
- **업캐스팅**(자식 → 부모) 3가지 방법과, 값 복사가 일으키는 **객체 슬라이싱**을 안다.
- 왜 `virtual` 이 필요한지 **문제 상황으로** 이해한다. (해결은 [class9.md](class9.md))

### 0.2 수업 진행 순서 제안

1. 단계 1~2 : 임시 객체
2. 단계 3 : 상속 없이 만들면 얼마나 중복되는가
3. 단계 4~5 : 상속 도입, `protected`
4. 단계 6~7 : 형제 클래스, 업캐스팅과 슬라이싱
5. 단계 8 : **virtual 이 필요한 이유** (다음 시간 예고)

---

## 1. 임시 객체(무명 객체)

### 1.1 이름이 없으면 그 줄이 끝나자마자 사라진다

```cpp
#include <iostream>
using namespace std;

class Data
{
public:
	Data(int = 0) { cout << "Data()" << endl; }
	~Data() { cout << "~Data()" << endl; }
	void Print() const { cout << "Data" << endl; }
};
int main()
{
	//임시 객체(무명 객체)
	cout << "start main()" << endl;
	int();
	double();
	Data();        // 생성자와 소멸자가 곧바로 이어서 호출된다
	cout << "end main()" << endl;
}
```

**예상 출력**
```
start main()
Data()
~Data()
end main()
```

| 종류 | 예 | 생명 |
|------|-----|------|
| 이름 있는 객체 | `Data d1;` | 블록이 끝날 때까지 |
| **임시 객체** | `Data();` | **그 문장(;)이 끝나면 바로** |

---

### 1.2 함수가 객체를 반환할 때

```cpp
Data CreateData()
{
	Data d1;
	return d1;
	//return Data();
}
int main()
{
	cout << "start main()" << endl;
	vector<Data> v;
	v.push_back(Data(100));   // 임시 객체를 만들어 벡터에 복사하고 바로 소멸
	cout << "end main()" << endl;
}
```

- `v.push_back(Data(100));` 은 임시 객체를 만들어 **벡터 안으로 복사**한 뒤, 임시 객체는 사라집니다.
- 요즘 컴파일러는 이런 불필요한 복사를 **자동으로 없애 줍니다.**
  (RVO / 복사 생략 최적화, C++17 부터는 의무화)
  그래서 출력 결과가 교재와 다를 수 있는데, **정상입니다.**

---

### 1.3 객체 배열에는 기본 생성자가 필요하다

```cpp
class Data
{
public:
	Data(int =0) { }       // 기본값이 없으면 Data arr[3]; 는 에러
	void Print() const { cout << "Data" << endl; }
};
int main()
{
	Data arr[3];

	arr[0].Print();
}
```

- `Data arr[3];` 는 원소 3개를 **매개변수 없이** 만들어야 하므로 기본 생성자가 필요합니다.
- `Data(int = 0)` 처럼 **모든 매개변수에 기본값**이 있으면 기본 생성자 역할을 겸합니다.

---

## 2. 상속이 없으면 - 중복의 지옥

```cpp
class Person
{
	string name;
	int age;
public:
	Person(const string& n = "", int a = 0) :name(n), age(a) {}
	void Print() const { cout << "name: " << name << ", " << "age: " << age << endl; }
	const string& GetName()const { return name; }
	int GetAge()const { return age; }
};
class Student            // Person 과 아무 관계가 없다
{
	string name;         // 중복!
	int age;             // 중복!
	int grade;
public:
	Student(const string& n = "", int a = 0, int g=0) :name(n), age(a), grade(g) {}
	void Print() const
	{
		cout << "name: " << name << ", " << "age: " << age <<
			", " << "grade: " << grade << endl;
	}
	const string& GetName()const { return name; }   // 중복!
	int GetAge()const { return age; }               // 중복!
	int GetGrade()const { return grade; }
};
```

**예상 출력**
```
name: 홍길동, age: 25
name: 이몽룡, age: 29, grade: 3
```

**문제점**

| 문제 | 설명 |
|------|------|
| 코드 중복 | `name`, `age`, `GetName`, `GetAge` 가 그대로 복사되어 있다 |
| 수정 지옥 | `name` 을 `string` 에서 다른 것으로 바꾸려면 **모든 클래스를 고쳐야** 한다 |
| **관계 표현 불가** | "학생은 사람이다" 라는 사실을 코드가 모른다 |
| 함께 다룰 수 없다 | `Person` 과 `Student` 를 한 배열에 담을 방법이 없다 |

---

## 3. 상속으로 중복 제거

### 3.1 기본 문법

```cpp
class Student : public Person   // Person 을 물려받는다
{
	int grade;      // 자기만의 멤버는 이것 하나뿐
public:
	// 부모 생성자를 먼저 호출한 뒤 자기 멤버를 초기화한다
	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g) {}
	void Print() const
	{
		Person::Print();                       // 부모 것을 그대로 재사용
		cout << "grade: " << grade << endl;
	}
	int GetGrade()const { return grade; }
};
```

**용어 정리**

| 이름 | 다른 표현 |
|------|-----------|
| **부모 클래스** | 기반 클래스(base), 상위 클래스(super), 슈퍼 클래스 |
| **자식 클래스** | 파생 클래스(derived), 하위 클래스(sub), 서브 클래스 |

**`class Student : public Person`**

| 상속 방식 | 부모의 public 멤버가 자식에서 | 언제 쓰나 |
|-----------|-------------------------------|-----------|
| `public` | public 그대로 | **거의 항상 이것** ("~는 ~이다" 관계) |
| `protected` | protected 로 | 거의 안 쓴다 |
| `private` | private 로 | 구현만 빌려 쓸 때 (드물다) |

> `class` 는 생략하면 `private` 상속, `struct` 는 `public` 상속이 기본입니다.
> **헷갈리니 항상 `public` 을 명시적으로 쓰세요.**

---

### 3.2 부모 생성자 호출

```cpp
Student(const string& n = "", int a = 0, int g = 0)
	: Person(n, a),    // 1. 반드시 초기화 리스트에서 부모 생성자를 부른다
	  grade(g)         // 2. 그 다음 자기 멤버
{ }
```

**생성 순서**

```
Student s("이몽룡", 29, 3);
    ↓
1. Person(n, a) 실행     <- 부모부터
2. grade(g) 실행         <- 자식 멤버
3. Student 생성자 몸통 { } 실행
```

- 부모 생성자를 **명시하지 않으면 부모의 기본 생성자**가 불립니다.
- 부모에 기본 생성자가 없으면 **반드시 명시해야 하며, 안 하면 컴파일 에러**입니다.

---

### 3.3 `Person::Print()` - 부모의 함수를 직접 부르기

```cpp
void Print() const
{
	Person::Print();                          // 부모 것을 먼저 호출하고
	cout << "grade: " << grade << endl;       // 자기 것을 덧붙인다
}
```

- 자식이 같은 이름의 함수를 만들면 **부모 것이 가려집니다(hiding).**
- 부모 것을 부르려면 `부모클래스이름::` 을 붙입니다.

---

## 4. protected - 자식에게만 열어 주기

### 4.1 private 이면 자식도 못 쓴다

```cpp
class Person
{
	string name;     // private 이라 자식이 직접 못 쓴다 -> GetName() 을 써야 한다
	int age;
public:
	...
};
class Student : public Person
{
	void Print() const
	{
		cout << name << endl;   // X - 에러 : private 멤버
		cout << GetName();      // O - public 함수로 우회
	}
};
```

---

### 4.2 protected 로 바꾸면

```cpp
class Person
{
protected:      // private -> protected
	string name;
	int age;
public:
	...
};
class Student : public Person
{
	void Print() const
	{
		cout << "name: " << name << ", " << "age: " << age <<", ";   // 직접 접근 가능
		cout << "grade: " << grade << endl;
	}
};
```

**접근 지정자 최종 정리**

| 지정자 | 클래스 자신 | **자식 클래스** | 클래스 밖 |
|--------|-------------|-----------------|-----------|
| `public` | O | O | **O** |
| `protected` | O | **O** | X |
| `private` | O | **X** | X |

> **`protected` 를 남용하지 마세요.**
> 자식이 부모의 내부 데이터를 직접 만지면, 부모의 구현을 바꿀 때 **모든 자식이 깨집니다.**
> 되도록 `private` + `protected` **함수**(getter) 조합이 안전합니다.

---

## 5. 업캐스팅과 객체 슬라이싱

### 5.1 자식은 부모 자리에 들어갈 수 있다

```cpp
Person p1("홍길동", 25);
Student s1("이몽룡", 29, 3);

Person p2 = s1;    //X : 슬라이싱 - grade 가 사라진다
Person& rp = s1;   //O
Person* p = &s1;   //O
```

| 방식 | 결과 |
|------|------|
| `Person p2 = s1;` (**값**) | **객체 슬라이싱** — `Person` 부분만 복사되고 `grade` 는 잘려 나간다 |
| `Person& rp = s1;` (**참조**) | 안전. `s1` 을 `Person` 이라는 창으로 들여다볼 뿐 |
| `Person* p = &s1;` (**포인터**) | 안전. 같은 이유 |

**객체 슬라이싱 그림**

```
s1 (Student)                 p2 (Person)
┌────────────┐              ┌────────────┐
│ name       │  ──복사──▶   │ name       │
│ age        │  ──복사──▶   │ age        │
│ grade  ✂   │              └────────────┘
└────────────┘                 grade 가 잘려 나갔다!
```

> **이름의 유래** : 자식 객체의 윗부분(부모 부분)만 **잘라서(slice)** 가져가기 때문입니다.
> 이것이 **클래스는 값이 아니라 참조/포인터로 다뤄야 하는** 가장 큰 이유입니다.

---

### 5.2 왜 자식→부모는 되고 부모→자식은 안 되나

```
    Person  (name, age)
       ▲
       │ 상속
    Student (name, age, grade)
```

- `Student` 는 **`Person` 이 가진 것을 모두 가지고 있습니다.** 그래서 `Person` 자리에 들어가도 문제가 없습니다. (**업캐스팅** — 자동으로 됨)
- 반대로 `Person` 은 `grade` 가 없으므로 `Student` 자리에 들어갈 수 없습니다. (**다운캐스팅** — 명시적 캐스팅 필요, 위험. [class15.md](class15.md))

---

## 6. 문제 발견 - 부모의 함수가 호출된다

```cpp
Person p1("홍길동", 25);
Student s1("이몽룡", 29, 3);

Person& rp = s1;   //O
Person* p = &s1;   //O

rp.Print();   // Person::Print() 가 호출된다 (기대와 다르다!)
p->Print();   // Person::Print() 가 호출된다
```

**예상 출력**
```
name: 이몽룡, age: 29
name: 이몽룡, age: 29
```

**기대했던 것**
```
name: 이몽룡, age: 29, grade: 3
```

**왜 이런가?**

- `rp` 와 `p` 의 **형식은 `Person`** 입니다.
- 컴파일러는 **형식만 보고** "아, `Person::Print()` 를 부르면 되겠군" 하고 **컴파일 시점에 결정**해 버립니다.
- 실제로 그 안에 `Student` 가 들어 있다는 사실은 **실행 시점에나 알 수 있는데**, 이미 결정이 끝난 뒤입니다.

| 용어 | 뜻 |
|------|-----|
| **정적 바인딩 (static binding)** | 컴파일 시점에 어떤 함수를 부를지 결정 (기본 동작) |
| **동적 바인딩 (dynamic binding)** | **실행 시점에** 실제 객체를 보고 결정 (`virtual` 필요) |

> ### 해결책
> 함수 앞에 **`virtual`** 한 단어만 붙이면 됩니다.
> 이것이 **[class9.md](class9.md) 다형성**의 주제입니다.

---

## 7. 완성 코드 (ex8.cpp 맨 위)

```cpp
class Person
{
protected:                  // 자식 클래스는 접근 가능, 외부는 불가
	string name;
	int age;
public:
	Person(const string& n = "", int a = 0) :name(n), age(a) {}
	void Print() const { cout << "name: " << name << ", " << "age: " << age << endl; }
	const string& GetName()const { return name; }
	int GetAge()const { return age; }
};

class Student : public Person   // Person 을 물려받는다
{
	int grade;
public:
	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g) {}
	void Print() const
	{
		Person::Print();                          // 부모 것을 먼저 호출하고
		cout << "grade: " << grade << endl;       // 자기 것을 덧붙인다
	}
	int GetGrade()const { return grade; }
};

class Professor : public Person
{
	string position;
public:
	Professor(const string& n = "", int a = 0, const string& pos = "")
		:Person(n, a), position(pos) {}
	void Print() const
	{
		cout << "name: " << GetName() << ", " << "age: " << GetAge() << ", "
			<< "position: " << position << endl;
	}
	const string& GetPosition()const { return position; }
};

int main()
{
	Person p1("홍길동", 25);
	Student s1("이몽룡", 29, 3);
	Professor pf1("임꺽정", 40, "조교수");

	p1.Print();
	s1.Print();
	pf1.Print();

	cout << "--- 업캐스팅 ---" << endl;
	//Person p2 = s1;    // X - 객체 슬라이싱 : grade 가 잘려 나간다
	Person& rp = s1;     // O - 참조로 받기
	Person* p = &s1;     // O - 주소로 받기

	rp.Print();          // Person::Print() 가 호출된다! (아직 virtual 이 없다)
	p->Print();          // -> 이 문제는 ex9 에서 virtual 로 해결한다
}
```

**예상 출력**
```
name: 홍길동, age: 25
name: 이몽룡, age: 29
grade: 3
name: 임꺽정, age: 40, position: 조교수
--- 업캐스팅 ---
name: 이몽룡, age: 29
name: 이몽룡, age: 29
```

> 마지막 두 줄에 **`grade: 3` 이 없다는 것**을 반드시 확인하고 넘어가세요. 다음 강의의 출발점입니다.

---

## 8. 정리 표

| 주제 | 내용 |
|------|------|
| 임시 객체 | `Data();` — 문장이 끝나면 바로 소멸 |
| 객체 배열 | `Data arr[3];` 는 **기본 생성자**가 필요 |
| 상속 문법 | `class Student : public Person` |
| 상속의 목적 | 중복 제거 + **"~는 ~이다" 관계 표현** |
| 부모 생성자 호출 | `Student(...) : Person(n,a), grade(g) { }` |
| 생성 순서 | **부모 → 자식 멤버 → 자식 생성자 몸통** |
| `Person::Print()` | 가려진 부모 함수를 명시적으로 호출 |
| `protected` | 자식은 접근 가능, 외부는 불가 |
| 업캐스팅 | 자식 → 부모. **자동으로** 된다 |
| **객체 슬라이싱** | `Person p = s1;` — 자식 부분이 **잘려 나간다** |
| 안전한 업캐스팅 | **참조(`&`) 또는 포인터(`*`)** 로 받는다 |
| 정적 바인딩 | 컴파일러가 **형식만 보고** 함수를 결정 → 부모 함수가 불린다 |
| 해결책 | **`virtual`** ([class9.md](class9.md)) |

---

## 9. 연습 문제 (스스로 풀어보기)

1. `class Animal { protected: string name; public: Animal(const string&); void Speak() const; };` 를 만들고 `Dog`, `Cat` 을 상속시켜 각자 다른 `Speak()` 를 만드세요.
2. 1번에서 `Animal` 의 `name` 을 `private` 로 바꿔 보고 어떤 에러가 나는지 확인하세요.
3. 부모 생성자 호출을 빠뜨리면(`: Animal(n)` 을 지우면) 어떤 에러가 나는지 확인하세요.
4. `Person`, `Student` 생성자와 소멸자에 각각 메시지를 출력하고, `Student s;` 를 만들 때 **호출 순서**를 확인하세요.
5. `Person p2 = s1;` 를 실행하고 `p2.Print()` 결과가 왜 `grade` 없이 나오는지 설명해 보세요.
6. `Person* p = &s1; p->GetGrade();` 를 시도해 보고 어떤 에러가 나는지 확인하세요. (왜 `Person*` 으로는 자식 함수를 못 부를까요?)
7. `Professor` 를 `private` 상속(`class Professor : Person`)으로 바꿔 보고, `Person* p = &pf1;` 이 왜 에러가 나는지 생각해 보세요.

---

이 강의 자료는 **example/src/ex8.cpp** 에 있는 모든 예제를 바탕으로 구성했습니다. 이번 시간의 핵심은 **객체 슬라이싱**과 **"업캐스팅해도 부모의 함수가 불린다"** 는 문제 발견입니다. 이 문제를 확실히 체감해야 다음 시간의 `virtual` 이 왜 필요한지 와닿습니다.
