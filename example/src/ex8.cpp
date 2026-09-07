//===========================================================
// ex8.cpp - 상속(inheritance) 의 기초
//   1) 임시 객체(무명 객체) 와 함수가 객체를 반환할 때의 생성/소멸
//   2) 중복되는 코드를 부모 클래스로 뽑아내기
//   3) protected - 자식은 쓰고 남은 못 쓰게
//   4) 부모 생성자 호출 : Student(...) : Person(n, a), grade(g)
//   5) 업캐스팅 : Person* p = &s1;  /  Person& rp = s1;
//   6) 객체 슬라이싱(object slicing) : Person p2 = s1;  <- 자식 정보가 잘려 나간다
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
#include <vector>
#include <string>
using namespace std;

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
	// 부모 생성자를 먼저 호출한 뒤 자기 멤버를 초기화한다
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

//----------------------------------------------------------
// [단계 8] 업캐스팅하면 부모의 함수가 호출된다 - virtual 이 필요한 이유
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
//	void Print() const { cout << "name: " << name << ", " << "age: " << age << endl; }
//	const string& GetName()const { return name; }
//	int GetAge()const { return age; }
//};
//class Student : public Person
//{
//	int grade;
//public:
//	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g) {}
//	void Print() const
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
//	Person& rp = s1;   //O
//	Person* p = &s1;   //O
//
//	rp.Print();   // Person::Print() 가 호출된다 (기대와 다르다!)
//	p->Print();   // Person::Print() 가 호출된다
//}

//----------------------------------------------------------
// [단계 7] 업캐스팅 3가지 - 값 복사는 슬라이싱, 참조/포인터는 안전
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
//	void Print() const { cout << "name: " << name << ", " << "age: " << age << endl; }
//	const string& GetName()const { return name; }
//	int GetAge()const { return age; }
//};
//class Student : public Person
//{
//	int grade;
//public:
//	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g) {}
//	void Print() const
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
//	p1.Print();
//	s1.Print();
//	pf1.Print();
//
//	Person p2 = s1;    //X : 슬라이싱 - grade 가 사라진다
//	Person& rp = s1;   //O
//	Person* p = &s1;   //O
//}

//----------------------------------------------------------
// [단계 6] Professor 도 같은 부모를 상속 - 형제 클래스가 생긴다
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
//	void Print() const { cout << "name: " << name << ", " << "age: " << age << endl; }
//	const string& GetName()const { return name; }
//	int GetAge()const { return age; }
//};
//class Student : public Person
//{
//	int grade;
//public:
//	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g) {}
//	void Print() const
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
//	p1.Print();
//	s1.Print();
//	pf1.Print();
//}

//----------------------------------------------------------
// [단계 5] protected 로 바꾸면 자식이 name, age 를 직접 쓸 수 있다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//class Person
//{
//protected:      // private -> protected
//	string name;
//	int age;
//public:
//	Person(const string& n = "", int a = 0) :name(n), age(a) {}
//	void Print() const { cout << "name: " << name << ", " << "age: " << age << endl; }
//	const string& GetName()const { return name; }
//	int GetAge()const { return age; }
//};
//class Student : public Person
//{
//	int grade;
//public:
//	Student(const string& n = "", int a = 0, int g = 0) :Person(n, a), grade(g) {}
//	void Print() const
//	{
//		cout << "name: " << name << ", " << "age: " << age <<", ";   // 직접 접근 가능
//		cout << "grade: " << grade << endl;
//	}
//	int GetGrade()const { return grade; }
//};
//int main()
//{
//	Person p1("홍길동", 25);
//	Student s1("이몽룡", 29, 3);
//
//	p1.Print();
//	s1.Print();
//}

//----------------------------------------------------------
// [단계 4] 상속으로 중복 제거 - Student : public Person
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//class Person
//{
//	string name;     // private 이라 자식이 직접 못 쓴다 -> GetName() 을 써야 한다
//	int age;
//public:
//	Person(const string& n = "", int a = 0) :name(n), age(a) {}
//	void Print() const { cout << "name: " << name << ", " << "age: " << age << endl; }
//	const string& GetName()const { return name; }
//	int GetAge()const { return age; }
//};
//class Student : public Person
//{
//	int grade;      // 자기만의 멤버는 이것 하나뿐
//public:
//	Student(const string& n = "", int a = 0, int g = 0) :Person(n,a), grade(g) {}
//	void Print() const
//	{
//		Person::Print();                       // 부모 것을 그대로 재사용
//		cout << "grade: " << grade << endl;
//	}
//	int GetGrade()const { return grade; }
//};
//int main()
//{
//	Person p1("홍길동", 25);
//	Student s1("이몽룡", 29, 3);
//
//	p1.Print();
//	s1.Print();
//}

//----------------------------------------------------------
// [단계 3] 상속 없이 만들면? - name, age 코드가 그대로 중복된다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//class Person
//{
//	string name;
//	int age;
//public:
//	Person(const string& n = "", int a = 0) :name(n), age(a) {}
//	void Print() const { cout << "name: " << name << ", " << "age: " << age << endl; }
//	const string& GetName()const { return name; }
//	int GetAge()const { return age; }
//};
//class Student            // Person 과 아무 관계가 없다
//{
//	string name;         // 중복!
//	int age;             // 중복!
//	int grade;
//public:
//	Student(const string& n = "", int a = 0, int g=0) :name(n), age(a), grade(g) {}
//	void Print() const
//	{
//		cout << "name: " << name << ", " << "age: " << age <<
//			", " << "grade: " << grade << endl;
//	}
//	const string& GetName()const { return name; }
//	int GetAge()const { return age; }
//	int GetGrade()const { return grade; }
//};
//int main()
//{
//	Person p1("홍길동", 25);
//	Student s1("이몽룡", 29, 3);
//
//	p1.Print();
//	s1.Print();
//}

//----------------------------------------------------------
// [단계 2] 함수가 객체를 반환할 때 - 임시 객체가 생겼다가 사라진다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Data
//{
//	int data;
//public:
//	Data(int d = 0) :data(d) {
//		cout << "Data()" << endl;
//	}
//	~Data() { cout << "~Data()" << endl; }
//	void Print() const {
//		cout << "Data : " << data << endl;
//	}
//};
//Data CreateData()
//{
//	Data d1;
//	return d1;
//	//return Data();
//}
//int main()
//{
//	//임시 객체(무명 객체)
//	cout << "start main()" << endl;
//	vector<Data> v;
//	v.push_back(Data(100));   // 임시 객체를 만들어 벡터에 복사하고 바로 소멸
//	cout << "end main()" << endl;
//}

//----------------------------------------------------------
// [단계 1b] 임시 객체(무명 객체) - 이름이 없어서 그 줄이 끝나면 바로 사라진다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Data
//{
//public:
//	Data(int = 0) {
//		cout << "Data()" << endl;
//	}
//	~Data() { cout << "~Data()" << endl; }
//	void Print() const {
//		cout << "Data" << endl;
//	}
//};
//int main()
//{
//	//임시 객체(무명 객체)
//	cout << "start main()" << endl;
//	int();
//	double();
//	Data();        // 생성자와 소멸자가 곧바로 이어서 호출된다
//	cout << "end main()" << endl;
//}

//----------------------------------------------------------
// [단계 1a] 초기화 목록으로 객체 배열 만들기 - 기본값이 없어도 된다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Data
//{
//public:
//	Data(int) { }          // 기본값이 없다 -> Data arr[3]; 는 에러
//	void Print() const {
//		cout << "Data" << endl;
//	}
//};
//int main()
//{
//	// 원소를 하나씩 직접 만들어 주면 기본 생성자가 없어도 된다
//	Data arr[3] = { Data(1), Data(2), Data(17) };
//
//	arr[0].Print();
//}

//----------------------------------------------------------
// [단계 1] 객체 배열 - 매개변수 기본값이 있어야 Data arr[3]; 이 된다
//----------------------------------------------------------
//#include <iostream>
//using namespace std;
//
//class Data
//{
//public:
//	Data(int =0) { }       // 기본값이 없으면 Data arr[3]; 는 에러
//	void Print() const {
//		cout << "Data" << endl;
//	}
//};
//int main()
//{
//	Data arr[3];
//
//	arr[0].Print();
//}
