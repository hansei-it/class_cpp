//===========================================================
// ex14.cpp - static 멤버와 템플릿(template), STL 컨테이너
//   1) static 멤버 함수 / static 멤버 변수 - 객체 없이 쓰는 것들
//   2) 함수 템플릿 : template <typename T>
//   3) 템플릿 인자 추론과 명시적 지정  Print<int>(5.5)
//   4) 클래스 템플릿 : Stack<int> , Stack<double>
//   5) 비타입 템플릿 인자  Stack<int, 100>
//   6) 기본 템플릿 인자    template <typename T=int, int MAXSIZE=100>
//   7) 컨테이너를 템플릿 인자로  Stack<double, vector<double> >
//   8) 반복자(iterator) 와 STL 알고리즘
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <deque>
#include <list>
using namespace std;

template <typename T, typename Cont = deque<T> >   // 담을 그릇까지 바꿔 끼울 수 있다
class Stack
{
	Cont buf;
public:
	Stack() { }
	void Push(const T& data)
	{
		buf.push_back(data);
	}
	void Pop()
	{
		buf.pop_back();
	}
	const T& Top()const
	{
		return buf.back();
	}
	bool Empty()const { return buf.empty(); }
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

//----------------------------------------------------------
// [단계 12] 반복자(iterator) - 컨테이너 종류와 상관없이 순회하는 방법
//   * random_shuffle 은 C++17 에서 삭제되었습니다. shuffle 을 쓰세요.
//----------------------------------------------------------
//#include <iostream>
//#include <algorithm>
//#include <vector>
//using namespace std;
//
//int main()
//{
//	auto arr = { 10,20,30,40,50,60,70,80,90,100 }; //initialzer_list
//	for (auto x : arr)
//		cout << x << " ";
//	cout << endl;
//
//	vector<int> v(arr.begin(), arr.end());   // 반복자 구간으로 초기화
//	for (auto x : v)
//		cout << x << " ";
//	cout << endl;
//
//	reverse< vector<int>::iterator >(v.begin(), v.end());
//	for (auto x : v)
//		cout << x << " ";
//	cout << endl;
//
//	sort< vector<int>::iterator, less<int> >(v.begin(), v.end(), less<int>() );
//	for (auto x : v)
//		cout << x << " ";
//	cout << endl;
//}

//----------------------------------------------------------
// [단계 11] 컨테이너까지 템플릿 인자로 - Stack<double, vector<double> >
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//#include <vector>
//#include <deque>
//#include <list>
//using namespace std;
//
//template <typename T, typename Cont=deque<T> >
//class Stack
//{
//	Cont buf;
//public:
//	Stack() { }
//	void Push(const T& data)
//	{
//		buf.push_back(data);
//	}
//	void Pop()
//	{
//		buf.pop_back();
//	}
//	const T& Top()const
//	{
//		return buf.back();
//	}
//};
//int main()
//{
//	Stack<int> s;
//
//	cout << typeid(s).name() << endl;
//	s.Push(10);
//	s.Push(20);
//	s.Push(30);
//
//	cout << s.Top() << endl;
//	s.Pop();
//	cout << s.Top() << endl;
//	s.Pop();
//	cout << s.Top() << endl;
//	s.Pop();
//
//	Stack<double, vector<double> > ds;
//	ds.Push(1.5);
//	ds.Push(2.5);
//	ds.Push(6.5);
//	cout << ds.Top() << endl;
//	ds.Pop();
//}

//----------------------------------------------------------
// [단계 10] 고정 배열 대신 vector 를 쓰면 크기 걱정이 사라진다
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//#include <vector>
//using namespace std;
//
//template <typename T>
//class Stack
//{
//	vector<T> buf;     // T buf[100]; 대신
//public:
//	Stack(){ }
//	void Push(const T& data)
//	{
//		buf.push_back(data);
//	}
//	void Pop()
//	{
//		buf.pop_back();
//	}
//	const T& Top()const
//	{
//		return buf.back();
//	}
//};
//int main()
//{
//	Stack<int> s;
//
//	cout << typeid(s).name() << endl;
//	s.Push(10);
//	s.Push(20);
//	s.Push(30);
//
//	cout << s.Top() << endl;
//	s.Pop();
//	cout << s.Top() << endl;
//	s.Pop();
//
//	Stack<double> ds;
//	ds.Push(1.5);
//	cout << ds.Top() << endl;
//}

//----------------------------------------------------------
// [단계 9] 기본 템플릿 인자 - Stack<> 처럼 생략할 수 있게
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//template <typename T=int, int MAXSIZE=100>
//class Stack
//{
//	T buf[MAXSIZE];
//	int top;
//public:
//	Stack() :top(0)
//	{
//		for (auto& x : buf)
//			x = T();          // 어떤 형식이든 기본값으로 초기화
//	}
//	void Push(const T& data)
//	{
//		buf[top++] = data;
//	}
//	void Pop()
//	{
//		--top;
//	}
//	const T& Top()const
//	{
//		return buf[top - 1];
//	}
//};
//int main()
//{
//	Stack<> s;              // <> 는 생략할 수 없다 (Stack s; 는 에러)
//
//	cout << typeid(s).name() << endl;
//	s.Push(10);
//	s.Push(20);
//	cout << s.Top() << endl;
//	s.Pop();
//
//	Stack<double> ds;       // MAXSIZE 만 기본값 사용
//	ds.Push(1.5);
//	cout << ds.Top() << endl;
//}

//----------------------------------------------------------
// [단계 8] 비타입 템플릿 인자 - 형식이 아니라 "값" 을 템플릿 인자로 준다
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//template <typename T,int MAXSIZE>
//class Stack
//{
//	T buf[MAXSIZE];       // 배열 크기를 사용하는 쪽이 정한다
//	int top;
//public:
//	Stack() :top(0) {}
//	void Push(const T& data)
//	{
//		buf[top++] = data;
//	}
//	void Pop()
//	{
//		--top;
//	}
//	const T& Top()const
//	{
//		return buf[top - 1];
//	}
//};
//int main()
//{
//	Stack<int,100> s;
//
//	cout << typeid(s).name() << endl;
//	s.Push(10);
//	s.Push(20);
//	cout << s.Top() << endl;
//	s.Pop();
//
//	Stack<double, 50> ds;
//	ds.Push(1.5);
//	cout << ds.Top() << endl;
//}

//----------------------------------------------------------
// [단계 7] 클래스 템플릿 - Stack<int> 와 Stack<double> 을 한 코드로
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//template <typename T>
//class Stack
//{
//	T buf[100];
//	int top;
//public:
//	Stack() :top(0) {}
//	void Push(const T& data)   // 복사 비용을 줄이려면 const T&
//	{
//		buf[top++] = data;
//	}
//	void Pop()
//	{
//		--top;
//	}
//	const T& Top()const
//	{
//		return buf[top - 1];
//	}
//};
//int main()
//{
//	Stack<int> s;
//
//	cout << typeid(s).name() << endl;
//	s.Push(10);
//	s.Push(20);
//	s.Push(30);
//
//	cout << s.Top() << endl;
//	s.Pop();
//	cout << s.Top() << endl;
//	s.Pop();
//
//	Stack<double> ds;
//	ds.Push(1.5);
//	ds.Push(2.5);
//	cout << ds.Top() << endl;
//}

//----------------------------------------------------------
// [단계 6] 먼저 int 전용 Stack 클래스를 만들어 본다
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//class Stack
//{
//	int buf[100];    // int 만 담을 수 있다 -> double 용은 또 만들어야 한다
//	int top;
//public:
//	Stack() :top(0) {}
//	void Push(int data)
//	{
//		buf[top++] = data;
//	}
//	void Pop()
//	{
//		--top;
//	}
//	int Top()const
//	{
//		return buf[top-1];
//	}
//};
//int main()
//{
//	Stack s;
//	s.Push(10);
//	s.Push(20);
//	s.Push(30);
//
//	cout << s.Top() << endl;
//	s.Pop();
//	cout << s.Top() << endl;
//	s.Pop();
//	cout << s.Top() << endl;
//	s.Pop();
//}

//----------------------------------------------------------
// [단계 5] 템플릿 + 함수 객체 + 람다 + 함수 포인터를 모두 받는 Find
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//template <typename T, typename F>
//int Find(T* begin, T* end, F pred)
//{
//	cout << typeid(T).name() << ":" << typeid(F).name() << endl;
//	for (int i = 0; begin != end; ++begin, ++i)
//		if (pred(*begin))
//			return i;
//
//	return -1;
//}
////클라이언트
//struct Pred
//{
//	bool operator()(int data)const
//	{
//		return data == 30;
//	}
//};
//bool cmp(int data)
//{
//	return data == 30;
//}
//int main()
//{
//	int arr[] = { 10,20,30,40,50 };
//
//	int idx = Find(arr, arr + 5, Pred());              // 함수 객체
//	if (idx != -1)
//		cout << idx << " : " << arr[idx] << endl;
//
//	double darr[] = { 1.1, 4.5, 7.7, 8.8, 9.1 };
//	idx = Find(darr, darr + 5, [](double data)->bool { return 8.8 == data; });  // 람다
//	if (idx != -1)
//		cout << idx << " : " << darr[idx] << endl;
//
//	idx = Find(arr, arr + 5, cmp);                     // 함수 포인터
//	if (idx != -1)
//		cout << idx << " : " << arr[idx] << endl;
//}

//----------------------------------------------------------
// [단계 4b] 템플릿 인자를 명시적으로 지정하기 - Find<int,Pred> / Find<double>
//   추론이 되더라도 명시할 수 있고, 일부만 지정하고 나머지는 추론에 맡길 수도 있다
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//template <typename T, typename F>
//int Find(T* begin, T* end, F pred)
//{
//	for (int i = 0; begin != end; ++begin, ++i)
//		if (pred(*begin))
//			return i;
//
//	return -1;
//}
////클라이언트
//struct Pred
//{
//	bool operator()(int data)const
//	{
//		return data == 30;
//	}
//};
//int main()
//{
//	int arr[] = { 10,20,30,40,50 };
//
//	int idx = Find<int,Pred>(arr, arr + 5, Pred());    // 둘 다 명시
//	if (idx != -1)
//		cout << idx << " : " << arr[idx] << endl;
//
//	double darr[] = { 1.1, 4.5, 7.7, 8.8, 9.1 };
//	idx = Find<double>(darr, darr + 5, [](double data)->bool { return 8.8 == data; });
//	//    ^ T 만 명시하고 F(람다의 형식)는 추론에 맡긴다 - 람다는 이름이 없으므로
//	if (idx != -1)
//		cout << idx << " : " << darr[idx] << endl;
//
//	auto f = [](double data)->bool { return 8.8 == data; };
//	cout << typeid(f).name() << endl;   // 컴파일러가 만든 이름 없는 클래스
//}

//----------------------------------------------------------
// [단계 4] Pred 함수 객체를 받는 Find - 아직 int 전용이다
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//struct Pred
//{
//	bool operator()(int data)const
//	{
//		return data == 30;
//	}
//};
//int Find(int begin[], int end[], Pred pred)
//{
//	for(int i = 0; begin != end ; ++begin, ++i)
//		if(pred(*begin))
//			return i;
//
//	return -1;
//}
//int main()
//{
//	int arr[] = { 10,20,30,40,50 };
//
//	int idx = Find(arr, arr + 5, Pred());
//	if (idx != -1)
//		cout << idx << " : " << arr[idx] << endl;
//}

//----------------------------------------------------------
// [단계 3c] 배열을 받는 함수 템플릿 - const K* 로 받아야 안전하다
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//template <typename K>
//void PrintArray(const K* arr, int size)   // K* 가 아니라 const K*
//{
//	cout << typeid(K).name() << endl;
//	for (int i = 0; i < size; ++i)
//		cout << arr[i] << endl;
//	cout << endl;
//}
//int main()
//{
//	int arr[10] = { 1,2,3,4,5,6,7 };
//	PrintArray<int>(arr, 10);        // 명시적으로 지정
//
//	string sarr[] = { "abc","defg","12345" };
//	PrintArray<string>(sarr, 3);
//
//	//int n = 10;
//	//PrintArray(n, 10);    // X - 포인터가 아니라서 에러가 난다 (다행이다!)
//}

//----------------------------------------------------------
// [단계 3b] K* 로 받기 - 배열이 아닌 것도 통과해 버리는 문제
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//template <typename K>
//void PrintArray(K* arr, int size)     // const 가 없다
//{
//	cout << typeid(K).name() << endl;
//	for (int i = 0; i < size; ++i)
//		cout << arr[i] << endl;
//	cout << endl;
//}
//int main()
//{
//	int arr[10] = { 1,2,3,4,5,6,7 };
//	PrintArray(arr, 10);
//
//	string sarr[] = { "abc","defg","12345" };
//	PrintArray(sarr, 3);
//
//	int n = 10;
//	PrintArray(n, 10);    // X - 여기서 에러가 난다 (n 은 포인터가 아니다)
//}

//----------------------------------------------------------
// [단계 3a] K 로만 받으면 - 배열이 아닌 값도 그냥 통과한다 (위험)
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//template <typename K>
//void PrintArray(K arr, int size)     // 포인터인지 아닌지 검사할 방법이 없다
//{
//	cout << typeid(K).name() << endl;
//	for (int i = 0; i < size; ++i)
//		cout << arr[i] << endl;
//	cout << endl;
//}
//int main()
//{
//	int arr[10] = { 1,2,3,4,5,6,7 };
//	PrintArray(arr, 10);
//
//	string sarr[] = { "abc","defg","12345" };
//	PrintArray(sarr, 3);
//}

//----------------------------------------------------------
// [단계 3] 시작점 : 템플릿이 아닌 int 전용 함수
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//void PrintArray(int* arr,int size)    // int 배열만 출력할 수 있다
//{
//	for (int i = 0; i < size; ++i)
//		cout << arr[i] << endl;
//	cout << endl;
//}
//int main()
//{
//	int arr[10] = { 1,2,3,4,5,6,7 };
//
//	PrintArray(arr, 10);
//}

//----------------------------------------------------------
// [단계 2b] 템플릿 인자를 명시적으로 지정하기
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//template <typename T>
//void Print(T data)
//{
//	cout << typeid(T).name() << " : " << data << endl;
//}
//int main()
//{
//	Print(10);              // T 를 int 로 추론
//	Print("Hello!");        // T 를 const char* 로 추론
//	Print(5.5);             // T 를 double 로 추론
//	Print<int>(5.5);        // 강제로 int 로 지정 -> 5 가 출력된다
//}

//----------------------------------------------------------
// [단계 2] 함수 템플릿 - 형식만 다르고 하는 일이 같은 함수를 하나로
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//template <typename T>
//void Print(T data)
//{
//	cout << typeid(T).name() << " : " << data << endl;
//}
//int main()
//{
//	Print<int>(10);
//	Print<const char*>("Hello!");
//	Print<double>(5.5);
//}

//----------------------------------------------------------
// [단계 1c] static 멤버 함수만 모아 놓은 유틸리티 클래스
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//struct Util
//{
//	static void Menu()
//	{
//		cout << "1. 메뉴1" << endl;
//		cout << "2. 메뉴2" << endl;
//		cout << "3. 메뉴3" << endl;
//	}
//	static void SubMenu()
//	{
//		cout << "1. 서브 메뉴1" << endl;
//		cout << "2. 서브 메뉴2" << endl;
//		cout << "3. 서브 메뉴3" << endl;
//	}
//};
//int main()
//{
//	Util::Menu();       // 객체를 만들 필요가 없다
//	Util::SubMenu();
//}

//----------------------------------------------------------
// [단계 1b] static 멤버 변수 - 모든 객체가 함께 쓰는 하나뿐인 변수
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//class Unit
//{
//	static int count;
//public:
//	static int CreateUnit()
//	{
//		//Unit 생성;
//		return ++count;
//	}
//	static int GetCount() { return count; }
//private:
//	Unit();            // 생성자를 private 로 -> 객체를 만들 수 없다
//};
//int Unit::count = 0;   // static 멤버 변수는 클래스 밖에서 반드시 정의해야 한다
//int main()
//{
//	cout << Unit::GetCount() << endl;
//	//Unit u;              // X
//	//Unit* p = new Unit;  // X
//	cout << Unit::CreateUnit() << endl;
//	cout << Unit::CreateUnit() << endl;
//	cout << Unit::CreateUnit() << endl;
//	cout << Unit::CreateUnit() << endl;
//
//	cout << "unit count : " << Unit::GetCount() << endl;
//}

//----------------------------------------------------------
// [단계 1] static 멤버 함수 - 객체 없이 클래스이름::함수() 로 부른다
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//
//class Math
//{
//public:
//	static int Add(int a, int b) { return a + b; }
//	static int Sub(int a, int b) { return a - b; }
//};
//int main()
//{
//	int n1 = 10;
//	int n2 = 20;
//
//	// Math m; m.Add(...) 처럼 객체를 만들 필요가 없다
//	cout << " + : " << Math::Add(n1, n2) << endl;
//	cout << " - : " << Math::Sub(n1, n2) << endl;
//}
