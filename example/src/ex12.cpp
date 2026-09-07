//===========================================================
// ex12.cpp - 함수 객체(Functor) 와 람다, 그리고 STL 알고리즘
//   1) sort / for_each 에 "비교 방법" 을 넘겨 주기
//   2) operator() 를 가진 클래스 = 함수처럼 호출되는 객체(Functor)
//   3) 함수 포인터 vs 함수 객체 - 함수 객체는 상태를 가질 수 있다
//   4) template <typename F> 로 어떤 함수 객체든 받는 For_each
//   5) 람다(lambda) : 이름 없는 함수 객체를 그 자리에서 만든다
//   6) 캡처 [ ] , [=] , [&] 의 차이
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

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

//----------------------------------------------------------
// [단계 12] 람다 캡처 [&] - 바깥 변수를 참조로 잡아 수정까지 한다
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//using namespace std;
//int main()
//{
//	string s = "Hello";
//
//	cout << [&](int a, int b) -> int {   // [&] : 참조 캡처
//		cout << s << " : " << a << ',' << b << endl;
//		s = "Done";                      // 바깥 s 가 실제로 바뀐다
//
//		return a + b;
//	}(10, 20) << endl;
//
//	cout << s << endl;   // Done
//}

//----------------------------------------------------------
// [단계 11] 람다 캡처 [s] / [=] - 바깥 변수를 값으로 복사해 온다
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//using namespace std;
//int main()
//{
//	string s = "Hello";
//
//	cout << [s](int a, int b) -> int {   // [s] : s 만 값으로 캡처
//		cout << s << " : " << a << ',' << b << endl;
//		return a + b;
//	}(10, 20) << endl;
//
//	cout << [=](int a, int b) -> int {   // [=] : 쓰이는 바깥 변수를 전부 값으로 캡처
//		cout << s << " : " << a << ',' << b << endl;
//		return a + b;
//	}(10, 20) << endl;
//}

//----------------------------------------------------------
// [단계 10] STL 알고리즘에 람다 넘기기 - sort / for_each
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//#include <algorithm>
//using namespace std;
//int main()
//{
//	int arr[] = { 15,16,29,30,56,20,40 };
//	string s = "string";
//
//	sort(arr, arr + 7, [s](int a, int b)->bool {
//		cout << s << endl;
//		return a > b; });
//	for_each(arr, arr + 7, [](int n)->void { cout << n << endl; });
//	for_each(arr, arr + 7, [](int n)->void { cout << n << ", "; });
//}

//----------------------------------------------------------
// [단계 9] 람다 - 이름 없는 함수를 그 자리에서 정의하고 바로 호출하기
//   [캡처](매개변수) -> 반환형 { 몸통 }
//----------------------------------------------------------
//#include <iostream>
//#include <string>
//using namespace std;
//int main()
//{
//	//lambda : 변수에 담아 두었다가 나중에 호출
//	auto f = [](int a, int b) -> int { 	return a + b; };
//	cout << f(10, 20) << endl;
//
//	//정의하자마자 바로 호출
//	cout << [](int a, int b) -> int {
//				cout << a << ',' << b << endl;
//				return a + b;
//			}(10,20) << endl;
//}

//----------------------------------------------------------
// [단계 8] 함수 포인터 방식 - 상태를 가질 수 없다는 한계가 있다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <algorithm>
//using namespace std;
//
//void For_each(int* begin, int* end, void (*f)(int) )   // 함수 포인터 매개변수
//{
//	for (; begin != end; ++begin)
//		f(*begin);
//}
//void PrintInt(int data)
//{
//	cout << "data : " << data << endl;   // 접두어를 바꾸려면 함수를 새로 만들어야 한다
//}
//int main()
//{
//	int arr[] = { 57,12,35,96,82 };
//
//	For_each(arr, arr + 5, PrintInt);
//}

//----------------------------------------------------------
// [단계 7] 상태를 갖는 함수 객체 - 합계를 누적한다 (참조로 넘겨야 결과가 남는다)
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <algorithm>
//using namespace std;
//
//struct Functor3 // Function Object(Functor)
//{
//	int sum;
//	Functor3(int init):sum(init) { }
//	void operator()(int data)
//	{
//		sum += data;      // 호출될 때마다 자기 안에 값을 쌓는다
//	}
//};
//
//void For_each(int* begin, int* end, Functor3& f)   // 값이 아니라 참조로 받는다
//{
//	for (; begin != end; ++begin)
//		f(*begin);
//}
//int main()
//{
//	int arr[] = { 57,12,35,96,82 };
//
//	Functor3 f(0);
//	For_each(arr, arr + 5, f);
//	cout << "sum : " << f.sum << endl;   // 282
//}

//----------------------------------------------------------
// [단계 6] 소멸자를 이용한 마무리 - Functor2 는 끝날 때 줄바꿈을 한다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <algorithm>
//using namespace std;
//
//struct Functor // Function Object(Functor)
//{
//	void operator()(int data)const
//	{
//		cout << "data: " << data << endl;
//	}
//};
//struct Functor2 // Function Object(Functor)
//{
//	~Functor2() { cout << endl; }      // 다 쓰고 사라질 때 개행
//	void operator()(int data)const
//	{
//		cout << data << " ";
//	}
//};
//void For_each(int* begin, int* end, Functor2 f)
//{
//	for (; begin != end; ++begin)
//		f(*begin);
//}
//int main()
//{
//	int arr[] = { 57,12,35,96,82 };
//
//	For_each(arr, arr + 5, Functor2());
//}

//----------------------------------------------------------
// [단계 5] template <typename F> - 어떤 함수 객체든 받을 수 있게 만든다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <algorithm>
//using namespace std;
//
//struct Functor
//{
//	void operator()(int data)const
//	{
//		cout << "data: " << data << endl;
//	}
//};
//struct Functor2
//{
//	~Functor2() { cout << endl; }
//	void operator()(int data)const
//	{
//		cout << data << " ";
//	}
//};
//template <typename F>
//void For_each(int* begin, int* end, F f)   // F 가 Functor 든 Functor2 든 상관없다
//{
//	for (; begin != end; ++begin)
//		f(*begin);
//}
//int main()
//{
//	int arr[] = { 57,12,35,96,82 };
//
//	For_each(arr, arr + 5, Functor());
//	For_each(arr, arr + 5, Functor2());
//}

//----------------------------------------------------------
// [단계 4] For_each 를 직접 만들어 보기 - 알고리즘의 정체를 확인한다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <algorithm>
//using namespace std;
//
//struct Functor // Function Object(Functor)
//{
//	void operator()(int data)const
//	{
//		cout << "data: " << data << endl;
//	}
//};
//void For_each(int* begin, int* end, Functor f)
//{
//	for (; begin != end; ++begin)
//		f(*begin);
//}
//int main()
//{
//	int arr[] = { 57,12,35,96,82 };
//
//	For_each(arr, arr + 5, Functor());
//}

//----------------------------------------------------------
// [단계 3] STL 의 for_each 에 함수 객체 넘기기
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <algorithm>
//using namespace std;
//
//struct Functor // Function Object(Functor)
//{
//	void operator()(int data)const
//	{
//		cout << "data: " << data << endl;
//	}
//};
//int main()
//{
//	int arr[] = { 57,12,35,96,82 };
//
//	for_each(arr, arr + 5, Functor());   // <algorithm> 의 for_each
//}

//----------------------------------------------------------
// [단계 2] 함수 객체(Functor) - operator() 를 가지면 함수처럼 호출된다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <algorithm>
//using namespace std;
//
//struct Functor // Function Object(Functor)
//{
//	void operator()(int data)const
//	{
//		cout << "data: " << data << endl;
//	}
//};
//int main()
//{
//	Functor f1;
//	f1(10);            //f1.operator()(10)
//	f1(15);
//	Functor()(100);    // 임시 객체를 만들어 바로 호출
//}

//----------------------------------------------------------
// [단계 1b] greater<int> 도 사실은 함수 객체다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <algorithm>
//using namespace std;
//
//int main()
//{
//	greater<int> g;      // 객체를 하나 만들고
//
//	cout << g(1, 2) << endl;      // 0 (1 > 2 가 거짓)
//	cout << g(15, 2) << endl;     // 1
//	cout << g(100, 20) << endl;   // 1
//}

//----------------------------------------------------------
// [단계 1] sort 에 "정렬 기준" 을 넘겨 주기
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <algorithm>
//using namespace std;
//
//int main()
//{
//	int arr[] = { 16,90,30,67,25 };
//
//	for (auto x : arr)
//		cout << x << " ";
//	cout << endl;
//
//	sort(arr, arr + 5);                     // 기본 : 오름차순
//	for (auto x : arr)
//		cout << x << " ";
//	cout << endl;
//
//	sort(arr, arr + 5, greater<int>() );    // 기준을 바꾸면 내림차순
//	for (auto x : arr)
//		cout << x << " ";
//	cout << endl;
//}
