//===========================================================
// ex9.cpp - 가상 함수(virtual) 와 다형성(polymorphism)
//   1) virtual 을 붙이면 "실제 객체의 함수" 가 호출된다
//   2) 함수 재정의(override) 와 함수 중복(overload) 의 차이
//   3) 순수 가상 함수( = 0 ) 와 추상 클래스
//   4) 부모 포인터 배열로 여러 종류의 객체를 한꺼번에 다루기
//   5) 생성자 · 소멸자의 호출 순서
//   6) 가상 소멸자(virtual ~) 가 없으면 메모리가 샌다
//   7) 범위 기반 for : for (auto item : arr)
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Person   // 추상 클래스 : 순수 가상 함수가 하나라도 있으면 객체를 만들 수 없다
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
	{
		cout << "Student()" << endl;
	}
	~Student() { cout << "~Student()" << endl; }
	virtual void Print() const   //메소드 재정의(function override)
	{
		cout << "name: " << GetName() << ", " << "age: " << GetAge() << ", ";
		cout << "grade: " << GetGrade() << endl;
	}
	int GetGrade()const { return grade; }
};

class Professor : public Person
{
	string position;
public:
	Professor(const string& n = "", int a = 0, const string& pos = "")
		:Person(n, a), position(pos)
	{
		cout << "Professor()" << endl;
	}
	~Professor() { cout << "~Professor() " << endl; }
	void Print() const           // virtual 은 물려받으므로 생략해도 가상 함수다
	{
		cout << "name: " << GetName() << ", " << "age: " << GetAge() << ", "
			<< "position: " << position << endl;
	}
	const string& GetPosition()const { return position; }
};

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

//----------------------------------------------------------
// [단계 8] 범위 기반 for - 배열과 vector 를 같은 문법으로 순회한다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//
//int main()
//{
//	double arr[5] = { 10,20,30,40,50 };
//	vector<double> v;
//	v.push_back(100);
//	v.push_back(200);
//	v.push_back(300);
//
//	for (auto item : arr)
//		cout << item << " ";
//	cout << endl;
//
//	for (auto item : v)
//		cout << item << " ";
//	cout << endl;
//}

//----------------------------------------------------------
// [단계 7] 가상 함수와 비 가상 함수의 차이
//   GetName() : 비 가상 -> 포인터의 형식(Person)을 보고 결정
//   Print()   : 가상    -> 실제 객체(Student)를 보고 결정
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//class Person
//{
//protected:
//	string name;
//	int age;
//public:
//	Person(const string& n = "", int a = 0) :name(n), age(a)
//	{
//		cout << "Person() " << endl;
//	}
//	virtual ~Person() { cout << "~Person()" << endl; }
//	virtual void Print() const = 0; //순수 가상 함수
//	const string& GetName()const { return name; }
//	int GetAge()const { return age; }
//};
//class Student : public Person
//{
//	int grade;
//public:
//	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g)
//	{
//		cout << "Student()" << endl;
//	}
//	~Student() { cout << "~Student()" << endl; }
//	virtual void Print() const //메소드 재정의(function override)
//	{
//		cout << "name: " << GetName() << ", " << "age: " << GetAge() << ", ";
//		cout << "grade: " << GetGrade() << endl;
//	}
//	int GetGrade()const { return grade; }
//};
//int main()
//{
//	Person* p = new Student("홍길동", 24, 3);
//
//	p->GetName();//비 가상 함수 -> Person::GetName()
//	p->Print();  // 가상 함수   -> Student::Print()
//
//	delete p;
//}

//----------------------------------------------------------
// [단계 6] virtual ~Person() 을 붙여서 해결
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//class Person
//{
//protected:
//	string name;
//	int age;
//public:
//	Person(const string& n = "", int a = 0) :name(n), age(a)
//	{
//		cout << "Person() " << endl;
//	}
//	virtual ~Person() { cout << "~Person()" << endl; }   // <= virtual 추가
//	virtual void Print() const = 0;
//	const string& GetName()const { return name; }
//	int GetAge()const { return age; }
//};
//class Student : public Person
//{
//	int grade;
//public:
//	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g)
//	{
//		cout << "Student()" << endl;
//	}
//	~Student() { cout << "~Student()" << endl; }
//	virtual void Print() const
//	{
//		cout << "name: " << GetName() << ", " << "age: " << GetAge() << ", ";
//		cout << "grade: " << GetGrade() << endl;
//	}
//	int GetGrade()const { return grade; }
//};
//int main()
//{
//	Person* p = new Student("홍길동", 24, 3);
//
//	p->Print();
//
//	delete p;   // ~Student() -> ~Person() 순서로 호출된다 (정상)
//}

//----------------------------------------------------------
// [단계 5] 가상 소멸자가 없으면? - 자식 소멸자가 호출되지 않는다 (문제 확인용)
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//class Person
//{
//protected:
//	string name;
//	int age;
//public:
//	Person(const string& n = "", int a = 0) :name(n), age(a)
//	{
//		cout << "Person() " << endl;
//	}
//	~Person() { cout << "~Person()" << endl; }   // virtual 이 없다!
//	virtual void Print() const = 0;
//	const string& GetName()const { return name; }
//	int GetAge()const { return age; }
//};
//class Student : public Person
//{
//	int grade;
//public:
//	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g)
//	{
//		cout << "Student()" << endl;
//	}
//	~Student() { cout << "~Student()" << endl; }
//	virtual void Print() const
//	{
//		cout << "name: " << GetName() << ", " << "age: " << GetAge() << ", ";
//		cout << "grade: " << GetGrade() << endl;
//	}
//	int GetGrade()const { return grade; }
//};
//int main()
//{
//	Person* p = new Student("홍길동", 24, 3);
//
//	p->Print();
//
//	delete p;   // ~Person() 만 호출된다! Student 가 잡은 자원이 샌다
//}

//----------------------------------------------------------
// [단계 4] 생성자 · 소멸자의 호출 순서
//   생성 : Person() -> Student()   (부모부터)
//   소멸 : ~Student() -> ~Person() (자식부터, 생성의 역순)
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//class Person
//{
//protected:
//	string name;
//	int age;
//public:
//	Person(const string& n = "", int a = 0) :name(n), age(a)
//	{
//		cout << "Person() " << endl;
//	}
//	~Person() { cout << "~Person()" << endl; }
//	virtual void Print() const = 0; //순수 가상 함수
//	const string& GetName()const { return name; }
//	int GetAge()const { return age; }
//};
//class Student : public Person
//{
//	int grade;
//public:
//	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g)
//	{
//		cout << "Student()" << endl;
//	}
//	~Student() { cout << "~Student()" << endl; }
//	virtual void Print() const
//	{
//		cout << "name: " << GetName() << ", " << "age: " << GetAge() << ", ";
//		cout << "grade: " << GetGrade() << endl;
//	}
//	int GetGrade()const { return grade; }
//};
//int main()
//{
//	Student s1("홍길동", 24, 3);
//
//	s1.Print();
//}

//----------------------------------------------------------
// [단계 3] 순수 가상 함수와 부모 포인터 배열 - 다형성의 진짜 쓸모
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//class Person
//{
//protected:
//	string name;
//	int age;
//public:
//	Person(const string& n = "", int a = 0) :name(n), age(a) {}
//	virtual void Print() const = 0; //순수 가상 함수 -> Person 은 추상 클래스
//	const string& GetName()const { return name; }
//	int GetAge()const { return age; }
//};
//class Student : public Person
//{
//	int grade;
//public:
//	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g) {}
//	virtual void Print() const
//	{
//		cout << "name: " << GetName() << ", " << "age: " << GetAge() << ", ";
//		cout << "grade: " << GetGrade() << endl;
//	}
//	int GetGrade()const { return grade; }
//};
//class Professor : public Person
//{
//	string position;
//public:
//	Professor(const string& n = "", int a = 0, const string& pos = "") :Person(n, a), position(pos) {}
//	void Print() const
//	{
//		cout << "name: " << GetName() << ", " << "age: " << GetAge() << ", "
//			<< "position: " << position << endl;
//	}
//	const string& GetPosition()const { return position; }
//};
//int main()
//{
//	Person* parr[5] = { new Student("홍길동",20,2), new Professor("임꺽정",30,"조교수"),
//	new Student("이몽룡",21,3), new Professor("성춘향",30,"부교수"),
//	new Student("장길산",26,4) };
//
//	for (Person* p : parr)
//		p->Print();     // Student / Professor 각자의 Print() 가 호출된다
//
//	for (Person* p : parr)
//		delete p;       // 아직 가상 소멸자가 없어서 문제가 있다 (단계 5 참고)
//}

//----------------------------------------------------------
// [단계 2] Person* p = &pf1; - 부모 포인터 하나로 어떤 자식이든 가리킨다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//class Person
//{
//protected:
//	string name;
//	int age;
//public:
//	Person(const string& n = "", int a = 0) :name(n), age(a) {}
//	virtual void Print() const
//	{
//		cout << "name: " << name << ", " << "age: " << age << endl;
//	}
//	const string& GetName()const { return name; }
//	int GetAge()const { return age; }
//};
//class Student : public Person
//{
//	int grade;
//public:
//	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g) {}
//	virtual void Print() const //메소드 재정의(function override)
//	{
//		cout << "name: " << GetName() << ", " << "age: " << GetAge() << ", ";
//		cout << "grade: " << GetGrade() << endl;
//	}
//	int GetGrade()const { return grade; }
//};
//class Professor : public Person
//{
//	string position;
//public:
//	Professor(const string& n = "", int a = 0, const string& pos = "") :Person(n, a), position(pos) {}
//	void Print() const
//	{
//		cout << "name: " << GetName() << ", " << "age: " << GetAge() << ", "
//			<< "position: " << position << endl;
//	}
//	const string& GetPosition()const { return position; }
//};
//int main()
//{
//	Person p1("홍길동", 25);
//	Student s1("이몽룡", 29, 3);
//	Professor pf1("임꺽정", 40, "조교수");
//
//	Person* p = &pf1;
//
//	p->GetAge();
//	p->GetName();
//	p->Print();     // Professor::Print() 가 호출된다
//}

//----------------------------------------------------------
// [단계 1] virtual 한 단어로 문제가 해결된다 - 다형적인 메소드
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//class Person
//{
//protected:
//	string name;
//	int age;
//public:
//	Person(const string& n = "", int a = 0) :name(n), age(a) {}
//	virtual void Print() const      // <= virtual 을 붙였다
//	{
//		cout << "name: " << name << ", " << "age: " << age << endl;
//	}
//	const string& GetName()const { return name; }
//	int GetAge()const { return age; }
//};
//class Student : public Person
//{
//	int grade;
//public:
//	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g) {}
//	virtual void Print() const //메소드 재정의(function override)
//	{
//		cout << "name: " << GetName() << ", " << "age: " << GetAge() << ", ";
//		cout << "grade: " << GetGrade() << endl;
//	}
//	int GetGrade()const { return grade; }
//};
//int main()
//{
//	Person p1("홍길동", 25);
//	Student s1("이몽룡", 29, 3);
//
//	Person& rp = s1;   //O
//	Person* p = &s1;   //O
//
//	rp.Print(); //다형적인 메소드 -> Student::Print() 가 호출된다
//	p->Print();
//}
