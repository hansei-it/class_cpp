//===========================================================
// ex7.cpp - 반환값의 3가지 형태와 Array 클래스 · 대입 연산자
//   1) std::vector 를 먼저 써 보고 같은 것을 직접 만든다
//   2) 함수가 값 / 참조 / 포인터를 반환할 때의 차이
//   3) at() 이 참조를 반환하면 arr.at(0) = 100; 이 가능해진다
//   4) const 멤버 함수 오버로딩 (읽기 전용 버전 / 수정 가능 버전)
//   5) 복사 대입 연산자 operator= 구현과 자기 대입 방지
//   -> 파일로 분리한 예제는 src/app/string 폴더를 참고하세요.
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
#include <vector>
using namespace std;

class Array
{
public:
	typedef unsigned int size_type;
private:
	int* buf;
	size_type count;
public:
	Array() :buf(nullptr), count(0)
	{
		buf = new int[100];
	}
	Array(const Array& arg) :buf(nullptr), count(arg.count)   // 복사 생성자
	{
		buf = new int[100];
		for (size_type i = 0; i < count; ++i)
			buf[i] = arg.buf[i];
	}
	~Array() { delete[] buf; }

	const Array& operator=(const Array& arg)   // 복사 대입 연산자
	{
		if (this == &arg)        // 자기 대입 방지 : arr = arr;
			return *this;

		delete[] buf;            // 1. 기존 메모리 해제
		buf = new int[100];      // 2. 새 메모리 확보
		count = arg.count;
		for (size_type i = 0; i < count; ++i)   // 3. 값 복사
			buf[i] = arg.buf[i];

		return *this;            // 4. 자기 자신을 반환 -> a = b = c; 가 된다
	}

	void push_back(int data)
	{
		buf[count++] = data;
	}
	int& at(int idx) { return buf[idx]; }                  // 수정 가능 버전
	const int& at(int idx)const { return buf[idx]; }       // 읽기 전용 버전
	size_type size() const { return count; }
};

void PrintArray(const Array& arr)   // const 객체 -> const at() 이 호출된다
{
	for (Array::size_type i = 0; i < arr.size(); ++i)
		cout << arr.at(i) << " ";
	cout << endl;
}

int main()
{
	Array arr;
	arr.push_back(10);
	arr.push_back(20);
	arr.push_back(30);
	arr.push_back(40);
	arr.push_back(50);

	arr.at(0) = 100;    // at() 이 int& 를 반환하므로 대입이 된다
	PrintArray(arr);    // 100 20 30 40 50

	Array arr2 = arr;   // 복사 생성자
	Array arr3;
	arr3 = arr;         // 복사 대입 연산자
	PrintArray(arr2);
	PrintArray(arr3);
}

//----------------------------------------------------------
// [단계 9] typedef 로 STL 컨테이너에 짧은 이름 붙이기
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//typedef vector<string> StrArray;   // 긴 형식 이름을 짧게
//
//void PrintArray(const StrArray& arr)
//{
//	for (StrArray::size_type i = 0; i < arr.size(); ++i)
//		cout << arr.at(i).c_str() << " ";
//	cout << endl;
//}
//int main()
//{
//	StrArray arr;
//	arr.push_back(string("10"));
//	arr.push_back(string("20"));
//	arr.push_back(string("30"));
//	arr.push_back(string("40"));
//	arr.push_back(string("50"));
//
//	arr.at(0) = string("100");
//	PrintArray(arr);
//}

//----------------------------------------------------------
// [단계 8] at() 을 const / 비-const 로 오버로딩하기
//   - 비-const 객체 -> int& at(int)        (읽기 + 쓰기)
//   - const  객체   -> const int& at(int)  (읽기 전용)
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//class Array
//{
//public:
//	typedef unsigned int size_type;
//private:
//	int* buf;
//	size_type count;
//	const Array& operator=(const Array&);   // 아직 대입은 막아 둔다
//public:
//	Array() :buf(nullptr), count(0)
//	{
//		buf = new int[100];
//	}
//	Array(const Array& arg) :buf(nullptr), count(arg.count)
//	{
//		buf = new int[100];
//		for (size_type i = 0; i < count; ++i)
//			buf[i] = arg.buf[i];
//	}
//	~Array() { delete[] buf; }
//	void push_back(int data)
//	{
//		buf[count++] = data;
//	}
//	int& at(int idx) { return buf[idx]; }
//	const int& at(int idx)const { return buf[idx]; }
//	size_type size() const { return count; }
//};
//void PrintArray(const Array& arr)
//{
//	for (Array::size_type i = 0; i < arr.size(); ++i)
//		cout << arr.at(i) << " ";
//	cout << endl;
//}
//int main()
//{
//	Array arr;
//	arr.push_back(10);
//	arr.push_back(20);
//	arr.push_back(30);
//	arr.push_back(40);
//	arr.push_back(50);
//
//	arr.at(0) = 100;
//	PrintArray(arr);
//
//	Array arr2 = arr;   // O : 복사 생성자
//	arr2 = arr;         // X : operator= 가 private 이라 에러
//}

//----------------------------------------------------------
// [단계 7] at() 이 int& 를 반환하면 왼쪽 값(lvalue)이 된다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//class Array
//{
//public:
//	typedef unsigned int size_type;
//private:
//	int* buf;
//	int count;
//	const Array& operator=(const Array&);
//public:
//	Array() :buf(nullptr), count(0)
//	{
//		buf = new int[100];
//	}
//	Array(const Array& arg) :buf(nullptr), count(arg.count)
//	{
//		buf = new int[100];
//		for (size_type i = 0; i < count; ++i)
//			buf[i] = arg.buf[i];
//	}
//	~Array() { delete[] buf; }
//	void push_back(int data)
//	{
//		buf[count++] = data;
//	}
//	int& at(int idx) { return buf[idx]; }   // <= 참조 반환
//	int size() const { return count; }
//};
//int main()
//{
//	Array arr;
//
//	arr.push_back(10);
//	arr.push_back(20);
//	arr.push_back(30);
//	arr.push_back(40);
//	arr.push_back(50);
//
//	Array arr2 = arr;
//	arr2.at(0) = 100;   // 참조 반환이라 대입이 가능하다
//
//	for (Array::size_type i = 0; i < arr.size(); ++i)
//		cout << arr.at(i) << " ";
//	cout << endl;
//	for (Array::size_type i = 0; i < arr2.size(); ++i)
//		cout << arr2.at(i) << " ";
//	cout << endl;   // arr 는 그대로, arr2 만 바뀐다 -> 깊은 복사가 되었다는 증거
//}

//----------------------------------------------------------
// [단계 6] 구조체를 값 / 참조 / 포인터로 반환하기
//   GetData() 는 복사본을 돌려주므로 GetData() = d2; 는 원본을 바꾸지 못한다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//struct Data { int item; };
//Data data = { 0 };
//Data* GetAddress()      // 주소 반환
//{
//	return &::data;
//}
//Data& GetReference()    // 참조 반환
//{
//	return ::data;
//}
//Data GetData()          // 값(복사본) 반환
//{
//	return ::data;
//}
//int main()
//{
//	Data d2 = { 1 };
//	//*GetAddress() = d2;   // 원본이 바뀐다
//	//GetReference() = d2;  // 원본이 바뀐다
//	GetData() = d2;         // 임시 복사본만 바뀌고 사라진다 (의미 없음)
//	cout << GetAddress()->item << endl;
//	cout << GetReference().item << endl;
//	cout << GetData().item << endl;
//}

//----------------------------------------------------------
// [단계 5] const 를 붙여 수정을 막아 보기 (에러 확인용)
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//int data = 100;
//const int* GetAddress()   // const int* -> 가리키는 값을 바꿀 수 없다
//{
//	return &::data;
//}
//int& GetReference()
//{
//	return ::data;
//}
//int GetData()
//{
//	return ::data;
//}
//int main()
//{
//	*GetAddress() = 200;   // X - 에러 : const 라서 대입 불가
//	cout << *GetAddress() << endl;
//	cout << GetReference() << endl;
//	cout << GetData() << endl;
//}

//----------------------------------------------------------
// [단계 4] 값 / 참조 / 포인터 반환 - 무엇을 돌려주느냐에 따라 원본이 바뀐다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//int data = 100;
//int* GetAddress()
//{
//	return &::data;    // :: 는 전역 범위 지정 연산자
//}
//int& GetReference()
//{
//	return ::data;
//}
//int GetData()
//{
//	return ::data;
//}
//int main()
//{
//	*GetAddress() = 200;    // O : 원본 data 가 200 이 된다
//	//GetReference() = 200; // O : 원본 data 가 200 이 된다
//	//GetData() = 200;      // X : 복사본이라 대입 자체가 에러
//	cout << *GetAddress() << endl;
//	cout << GetReference() << endl;
//	cout << GetData() << endl;
//}

//----------------------------------------------------------
// [단계 3] 복사 생성자 추가 - Array arr2 = arr; 가 깊은 복사가 된다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//class Array
//{
//public:
//	typedef unsigned int size_type;
//private:
//	int* buf;
//	int count;
//	const Array& operator=(const Array&);   // 대입은 막아 둔다
//public:
//	Array() :buf(nullptr), count(0)
//	{
//		buf = new int[100];
//	}
//	Array(const Array& arg) :buf(nullptr), count(arg.count)
//	{
//		buf = new int[100];
//		for (size_type i = 0; i < count; ++i)
//			buf[i] = arg.buf[i];
//	}
//	~Array() { delete[] buf; }
//	void push_back(int data)
//	{
//		buf[count++] = data;
//	}
//	int at(int idx)const { return buf[idx]; }   // 아직 값 반환이라 대입 불가
//	int size() const { return count; }
//};
//int main()
//{
//	Array arr;
//
//	arr.push_back(10);
//	arr.push_back(20);
//	arr.push_back(30);
//	arr.push_back(40);
//	arr.push_back(50);
//
//	Array arr2 = arr;
//	//arr2.at(0) = 100;   // X : at() 이 값을 반환하므로 대입 불가
//
//	for (Array::size_type i = 0; i < arr.size(); ++i)
//		cout << arr.at(i) << " ";
//	cout << endl;
//	for (Array::size_type i = 0; i < arr2.size(); ++i)
//		cout << arr2.at(i) << " ";
//	cout << endl;
//}

//----------------------------------------------------------
// [단계 2] vector 를 흉내 낸 Array 클래스 만들기 (아직 복사 생성자 없음)
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//class Array
//{
//public:
//	typedef unsigned int size_type;
//private:
//	int* buf;
//	int count;
//public:
//	Array() :buf(nullptr), count(0)
//	{
//		buf = new int[100];
//	}
//	~Array() { delete[] buf; }
//	void push_back(int data)
//	{
//		buf[count++] = data;
//	}
//	int at(int idx)const  { return buf[idx]; }
//	int size() const { return count; }
//};
//int main()
//{
//	Array arr;
//
//	arr.push_back(10);
//	arr.push_back(20);
//	arr.push_back(30);
//	arr.push_back(40);
//	arr.push_back(50);
//
//	Array arr2 = arr;   // 얕은 복사! 소멸자가 두 번 delete[] -> 크래시
//
//	for (Array::size_type i = 0; i < arr.size(); ++i)
//		cout << arr.at(i) << " ";
//	cout << endl;
//	for (Array::size_type i = 0; i < arr2.size(); ++i)
//		cout << arr2.at(i) << " ";
//	cout << endl;
//}

//----------------------------------------------------------
// [단계 1b] vector 는 복사해도 안전하다 (깊은 복사가 이미 구현되어 있다)
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//typedef vector<int>	Array;
//
//int main()
//{
//	Array arr;
//
//	arr.push_back(10);
//	arr.push_back(20);
//	arr.push_back(30);
//	arr.push_back(40);
//	arr.push_back(50);
//
//	Array arr2 = arr;
//	arr2.at(0) = 100;   // arr2 만 바뀐다
//
//	for (Array::size_type i = 0; i < arr.size(); ++i)
//		cout << arr.at(i) << " ";
//	cout << endl;
//	for (Array::size_type i = 0; i < arr2.size(); ++i)
//		cout << arr2.at(i) << " ";
//	cout << endl;
//}

//----------------------------------------------------------
// [단계 1] std::vector 사용해 보기 - 크기가 자동으로 늘어나는 배열
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//typedef vector<int>	Array;
//
//int main()
//{
//	Array arr;
//
//	arr.push_back(10);   // 뒤에 하나씩 추가
//	arr.push_back(20);
//	arr.push_back(30);
//	arr.push_back(40);
//	arr.push_back(50);
//
//	for (Array::size_type i = 0; i < arr.size(); ++i)
//		cout << arr.at(i) << " ";
//	cout << endl;
//}
