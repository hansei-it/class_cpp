//===========================================================
// ex11.cpp - 연산자 오버로딩(operator overloading)
//   1) rvalue 참조(&&) 와 이동 생성자 맛보기
//   2) Add() 멤버 함수를 operator+ 로 바꾸기
//   3) operator== , operator!= - 비교 연산자는 짝으로 만든다
//   4) operator+= 가 참조를 반환해야 하는 이유
//   5) 전위 ++x 와 후위 x++ 의 구분 (int 더미 매개변수)
//   6) operator= (복사 대입) 과 operator[] (첨자)
//   7) 만든 연산자를 Array 클래스에 적용하기
// * 맨 위가 가장 발전된 코드, 아래로 내려갈수록 기초 코드입니다.
//===========================================================

#include <iostream>
#include <vector>
using namespace std;

class Point
{
	int x, y;
public:
	Point(int x = 0, int y = 0) :x(x), y(y) { }
	Point(const Point& arg) :x(arg.x), y(arg.y) { }
	void Print()const { cout << x << ',' << y << endl; }
	int GetX()const { return x; }
	int GetY()const { return y; }

	Point operator+(const Point& arg)const              // p1 + p2
	{
		return Point(x + arg.x, y + arg.y);
	}
	bool operator==(const Point& arg)const              // p1 == p2
	{
		return x == arg.x && y == arg.y;
	}
	bool operator!=(const Point& arg)const              // != 는 == 로 만든다
	{
		return !(*this == arg);
	}
	const Point& operator+=(const Point& arg)           // p1 += p2
	{
		x += arg.x;
		y += arg.y;
		return *this;      // 참조를 반환해야 p3 = p1 += p2; 가 된다
	}
	const Point& operator++()                           // 전위 : ++p1
	{
		++x;
		++y;
		return *this;
	}
	Point operator++(int)                               // 후위 : p1++ (int 는 구분용 더미)
	{
		Point t(*this);    // 증가 전의 값을 복사해 두었다가
		++x;
		++y;
		return t;          // 그것을 반환한다 -> 그래서 후위가 더 비싸다
	}
	const Point& operator=(const Point& arg)            // p1 = p2
	{
		x = arg.x;
		y = arg.y;
		return *this;
	}
	int& operator[](int idx)                            // p1[0] = 15;
	{
		if (idx == 0) return x;
		else if (idx == 1) return y;
		else throw "index 범위 설정 실패!";
	}
	int operator[](int idx)const                        // const 객체용 읽기 전용
	{
		if (idx == 0) return x;
		else if (idx == 1) return y;
		else throw "index 범위 설정 실패!";
	}
};

int main()
{
	Point p1(2, 3);
	Point p2(4, 5);
	Point p3;

	p3 = p1 + p2;        // p1.operator+(p2)
	p3.Print();          // 6,8

	if (p1 != p2)        // p1.operator!=(p2)
		cout << "다르다" << endl;

	p3 = p1 += p2;       // p1.operator+=(p2)
	p1.Print();          // 6,8
	p3.Print();          // 6,8

	p2 = ++p1;           // 전위 : 증가 후의 값
	p1.Print(); p2.Print();
	p2 = p1++;           // 후위 : 증가 전의 값
	p1.Print(); p2.Print();

	p1[0] = 15;          // p1.operator[](0) 이 int& 를 반환
	cout << p1[0] << ',' << p1[1] << endl;

	const Point cpt(0, 0);
	cout << cpt[0] << ',' << cpt[1] << endl;   // const 버전이 호출된다
}

//----------------------------------------------------------
// [단계 13] 종합 과제 - Matrix(행렬) 클래스에 연산자 붙이기
//   2차원 배열을 1차원으로 펼쳐서 관리하는 행렬 클래스입니다.
//   지금까지 배운 것이 모두 들어 있습니다 :
//     동적 할당 / 복사 생성자 / 소멸자 / operator= / == != + - *
//   * 아래 코드에는 일부러 남겨 둔 문제가 있습니다. 연습 문제 10~12 를 보세요.
//----------------------------------------------------------
//#include<iostream>
//#include<string>
//#include<cstring>
//using namespace std;
//class Matrix
//{
//    int* matrix;      // row * col 크기의 1차원 배열
//    int row;
//    int col;
//public:
//    // (i, j) 원소의 위치 = matrix[(col * i) + j]
//    explicit Matrix(int _row = 0, int _col = 0) :matrix(nullptr), row(_row), col(_col)
//    {
//        matrix = new int[_row * _col];
//        for (int i = 0; i < row; ++i)
//            for (int j = 0; j < col; ++j)
//                matrix[(col * i) + j] = 0;
//    }
//    Matrix(int _matrix[], int _row = 0, int _col = 0) :matrix(nullptr), row(_row), col(_col)
//    {
//        matrix = new int[_row * _col];
//        for (int i = 0; i < row; ++i)
//            for (int j = 0; j < col; ++j)
//                matrix[(col * i) + j] = _matrix[(col * i) + j];
//    }
//    Matrix(const Matrix& arg) :matrix(nullptr), row(arg.row), col(arg.col)   // 복사 생성자
//    {
//        matrix = new int[arg.row * arg.col];
//        for (int i = 0; i < arg.row; ++i)
//            for (int j = 0; j < arg.col; ++j)
//                matrix[(arg.col * i) + j] = arg.matrix[(arg.col * i) + j];
//    }
//    ~Matrix() { cout << "~Matrix" << endl;  delete[] matrix; }
//
//    int Getrow()const { return row; }
//    int Getcol()const { return col; }
//    int GetElement(int _row, int _col)const
//    {
//        return matrix[(col * (_row - 1)) + _col - 1];    // 1부터 세는 방식
//    }
//    void SetElement(int data, int _row, int _col)
//    {
//        if (row < _row || col < _col)
//            throw 1;                                     // 범위를 벗어나면 예외 (ex15)
//        matrix[(col * (_row - 1)) + _col - 1] = data;
//    }
//    void Print()const
//    {
//        for (int i = 0; i < row; ++i)
//        {
//            for (int j = 0; j < col; ++j)
//                cout << matrix[(col * i) + j] << " ";
//            cout << endl;
//        }
//        cout << endl;
//    }
//    bool operator==(const Matrix& arg)const
//    {
//        return this == &arg;      // 주의 : 내용이 아니라 "같은 객체인가" 를 본다
//    }
//    bool operator!=(const Matrix& arg)const
//    {
//        return !(*this == arg);
//    }
//    const Matrix& operator=(const Matrix& arg)
//    {
//        if (*this == arg)
//            return*this;          // 자기 대입 방지
//
//        delete[] matrix;
//
//        int size = arg.col * arg.row;
//        matrix = new int[size];
//        row = arg.row;
//        col = arg.col;
//        for (int i = 0; i < arg.row; ++i)
//            for (int j = 0; j < arg.col; ++j)
//                matrix[(arg.col * i) + j] = arg.matrix[(arg.col * i) + j];
//        return *this;
//    }
//    const Matrix operator+(const Matrix& arg)      // <= 문제 1 : 자기 자신을 바꾼다
//    {
//        if (row == arg.row && col == arg.col)
//        {
//            for (int i = 0; i < arg.row; ++i)
//                for (int j = 0; j < arg.col; ++j)
//                    matrix[(arg.col * i) + j] = matrix[(arg.col * i) + j] + arg.matrix[(arg.col * i) + j];
//        }
//        else
//            throw 1;
//
//        return *this;
//    }
//    const Matrix operator-(const Matrix& arg)      // <= 문제 1 : 여기도 마찬가지
//    {
//        if (row == arg.row && col == arg.col)
//        {
//            for (int i = 0; i < arg.row; ++i)
//                for (int j = 0; j < arg.col; ++j)
//                    matrix[(arg.col * i) + j] = matrix[(arg.col * i) + j] - arg.matrix[(arg.col * i) + j];
//        }
//        else
//            throw 1;
//        return *this;
//    }
//    const Matrix operator*(const Matrix& arg)      // <= 문제 2 : 곱셈이 비어 있다
//    {
//        if (col == arg.row && row == arg.col)
//        {
//            Matrix tempMatrix(row, arg.col);
//
//            /* 여기를 채워 보세요
//            for (int p = 0; p < row; ++p)
//                for (int i = 0; i < arg.col; ++i)
//                    for (int j = 0; j < col; ++j)
//                        tempMatrix.matrix[(arg.col * p) + i] +=
//                            matrix[(col * p) + j] * arg.matrix[(arg.col * j) + i];
//            */
//
//            return tempMatrix;
//        }
//        else
//            throw 1;
//    }
//};
//
//int main()
//{
//    Matrix arr1(2, 1);
//    Matrix arr2(1, 2);
//    Matrix arr3(2, 2);
//
//    arr3 = arr1 * arr2;
//    arr3.Print();
//}

//----------------------------------------------------------
// [단계 12] Array 클래스에 operator[] , operator== , operator= 적용하기
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Array
//{
//public:
//	typedef unsigned int size_type;
//private:
//	int* buf;
//	size_type count;
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
//	const int& at(size_type idx)const
//	{
//		if (0 <= idx && idx < count)
//			return buf[idx];
//		else
//			throw out_of_range("idx 범위를 벗어 났어요!");   // 예외는 ex15 에서
//	}
//	const int& operator[](int idx)const { return buf[idx]; }
//	size_type size() const { return count; }
//
//	bool operator==(const Array& arg)const
//	{
//		return this == &arg;      // 같은 객체인지(주소 비교)
//	}
//	const Array& operator=(const Array& arg)
//	{
//		if (*this == arg)
//			throw "자신의 객체를 대입할 수 없음!";
//
//		delete[] buf;
//
//		count = arg.count;        // count 를 빠뜨리면 값이 안 옮겨진다!
//		buf = new int[100];
//		for (size_type i = 0; i < count; ++i)
//			buf[i] = arg.buf[i];
//
//		return *this;
//	}
//};
//int main()
//{
//	Array arr;
//	arr.push_back(10);
//	arr.push_back(20);
//	arr.push_back(30);
//	arr.push_back(40);
//	arr.push_back(50);
//
//	for (Array::size_type i = 0; i < arr.size(); ++i)
//		cout << arr[i] << endl;
//
//	Array arr2;
//	arr2 = arr;
//	for (Array::size_type i = 0; i < arr2.size(); ++i)
//		cout << arr2[i] << endl;
//}

//----------------------------------------------------------
// [단계 11] Array 에 operator[] 추가 - arr[i] 로 읽기
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Array
//{
//public:
//	typedef unsigned int size_type;
//private:
//	int* buf;
//	size_type count;
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
//	int& at(int idx) { return buf[idx]; }
//	const int& at(int idx)const { return buf[idx]; }
//	const int& operator[](int idx)const { return buf[idx]; }
//
//	size_type size() const { return count; }
//};
//int main()
//{
//	Array arr;
//	arr.push_back(10);
//	arr.push_back(20);
//	arr.push_back(30);
//	arr.push_back(40);
//	arr.push_back(50);
//
//	for (Array::size_type i = 0; i < arr.size(); ++i)
//		cout << arr[i] << endl;    // arr.operator[](i)
//}

//----------------------------------------------------------
// [단계 10] operator[] 를 const / 비-const 로 오버로딩
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) { }
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	int& operator[](int idx)          // 쓰기 가능
//	{
//		if (idx == 0)
//			return x;
//		else if (idx == 1)
//			return y;
//		else
//			throw "index 범위 설정 실패!";
//	}
//	int operator[](int idx)const      // 읽기 전용
//	{
//		if (idx == 0)
//			return x;
//		else if (idx == 1)
//			return y;
//		else
//			throw "index 범위 설정 실패!";
//	}
//};
//int main()
//{
//	Point pt(2, 3);
//
//	pt[0] = 15;
//	cout << pt[0] << endl; //pt.operator[](0)
//	cout << pt[1] << endl;
//
//	const Point cpt(0, 0);
//	cout << cpt[0] << endl;   // const 버전이 호출된다
//	cout << cpt[1] << endl;
//}

//----------------------------------------------------------
// [단계 9] operator[] 가 int& 를 반환하면 pt[0] = 15; 가 된다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) { }
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	int& operator[](int idx)      // <= 참조 반환
//	{
//		if (idx == 0)
//			return x;
//		else if (idx == 1)
//			return y;
//		else
//			throw "index 범위 설정 실패!";
//	}
//};
//int main()
//{
//	Point pt(2, 3);
//
//	pt[0] = 15;
//	cout << pt[0] << endl; //pt.operator[](0)
//	cout << pt[1] << endl;
//}

//----------------------------------------------------------
// [단계 8] operator[] - 클래스를 배열처럼 쓰기 (읽기 전용 버전)
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) { }
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	int operator[](int idx)const   // 값 반환이라 pt[0] = 15; 는 안 된다
//	{
//		if (idx == 0)
//			return x;
//		else if (idx == 1)
//			return y;
//		else
//			throw "index 범위 설정 실패!";
//	}
//};
//int main()
//{
//	Point pt(2, 3);
//
//	cout << pt[0] << endl; //pt.operator[](0)
//	cout << pt[1] << endl;
//}

//----------------------------------------------------------
// [단계 7] operator= - 포인터 멤버가 있으면 깊은 복사로 구현한다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, *py;
//public:
//	Point(int x = 0, int y = 0) :x(x), py(new int(y)) { cout << "Point()" << endl; }
//	~Point() { cout << "~Point()" << endl; delete py; }
//	Point(const Point& arg) :x(arg.x), py(new int(*arg.py)) { cout << "copy Point()" << endl; }
//	void Print()const { cout << x << ',' << *py << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return *py; }
//	const Point& operator=(const Point& arg)
//	{
//		x = arg.x;
//		*py = *arg.py;     // 주소가 아니라 "값" 을 옮긴다
//		cout << "operator=" << endl;
//		return *this;
//	}
//};
//int main()
//{
//	Point p1(2, 3);
//	Point p2(4, 5);
//
//	p1 = p2; //p1.operator=(p2)
//
//	p1.Print();
//	p2.Print();
//}

//----------------------------------------------------------
// [단계 6] 후위 ++ : Point operator++(int) - int 는 전위와 구분하기 위한 더미
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) { cout << "Point()" << endl; }
//	~Point() { cout << "~Point()" << endl; }
//	Point(const Point& arg) :x(arg.x), y(arg.y) { cout << "copy Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	const Point& operator++()      // 전위 : 참조 반환 (복사 없음)
//	{
//		++x;
//		++y;
//		return *this;
//	}
//	Point operator++(int)          // 후위 : 값 반환 (복사가 생긴다)
//	{
//		Point t(*this);
//		++x;
//		++y;
//		return t;
//	}
//};
//int main()
//{
//	Point p1(2, 3);
//	Point p2(4, 5);
//
//	//p2 = ++p1; //p1.operator++()
//	p2 = p1++; //p1.operator++(0)
//
//	p1.Print();   // 3,4
//	p2.Print();   // 2,3 (증가 전의 값)
//}

//----------------------------------------------------------
// [단계 5] 전위 ++ : const Point& operator++()
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) { cout << "Point()" << endl; }
//	~Point() { cout << "~Point()" << endl; }
//	Point(const Point& arg) :x(arg.x), y(arg.y) { cout << "copy Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	Point operator+(const Point& arg)const
//	{
//		return Point(x + arg.x, y + arg.y);
//	}
//	bool operator==(const Point& arg)const
//	{
//		return x == arg.x && y == arg.y;
//	}
//	bool operator!=(const Point& arg)const
//	{
//		return !(*this == arg);
//	}
//	const Point& operator+=(const Point& arg)
//	{
//		x += arg.x;
//		y += arg.y;
//
//		return *this;
//	}
//	const Point& operator++()
//	{
//		++x;
//		++y;
//		return *this;
//	}
//};
//int main()
//{
//	Point p1(2, 3);
//	Point p2(4, 5);
//
//	p2 = ++p1; //p1.operator++()
//
//	p1.Print();
//	p2.Print();
//}

//----------------------------------------------------------
// [단계 4] operator+= 가 참조를 반환하면 p3 = p1 += p2; 가 된다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) { cout << "Point()" << endl; }
//	~Point() { cout << "~Point()" << endl; }
//	Point(const Point& arg) :x(arg.x), y(arg.y) { cout << "copy Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	Point operator+(const Point& arg)const
//	{
//		return Point(x + arg.x, y + arg.y);
//	}
//	bool operator==(const Point& arg)const
//	{
//		return x == arg.x && y == arg.y;
//	}
//	bool operator!=(const Point& arg)const
//	{
//		return !(*this == arg);
//	}
//	const Point& operator+=(const Point& arg)   // void 였다면 p3 = p1 += p2; 가 에러
//	{
//		x += arg.x;
//		y += arg.y;
//
//		return *this;
//	}
//};
//int main()
//{
//	Point p1(2, 3);
//	Point p2(4, 5);
//	Point p3;
//
//	p3 = p1 += p2; //p1.operator+=(p2)
//	p1.Print();
//	p2.Print();
//	p3.Print();
//}

//----------------------------------------------------------
// [단계 3] operator== 와 operator!= - != 는 == 를 뒤집어 만든다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) { cout << "Point()" << endl; }
//	~Point() { cout << "~Point()" << endl; }
//	Point(const Point& arg) :x(arg.x), y(arg.y) { cout << "copy Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	Point operator+(const Point& arg)const
//	{
//		return Point(x + arg.x, y + arg.y);
//	}
//	bool operator==(const Point& arg)const
//	{
//		return x == arg.x && y == arg.y;
//	}
//	bool operator!=(const Point& arg)const
//	{
//		return !(*this == arg);   // == 를 재사용 -> 규칙이 어긋날 수 없다
//	}
//};
//int main()
//{
//	Point p1(2, 3);
//	Point p2(4, 5);
//
//	if (p1 != p2) //p1.operator!=(p2)
//		cout << "true " << endl;
//	else
//		cout << "false " << endl;
//}

//----------------------------------------------------------
// [단계 2b] Add() 를 operator+ 로 이름만 바꾸면 p1 + p2 가 된다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) {}
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	Point operator+(const Point& arg)   // Add -> operator+
//	{
//		return Point(x + arg.x, y + arg.y);
//	}
//};
//int main()
//{
//	Point p1(2, 3);
//	Point p2(4, 5);
//	Point p3 = p1 + p2; // p1.operator+(p2);
//
//	p3.Print();// 6, 8
//}

//----------------------------------------------------------
// [단계 2a] 먼저 Add() 라는 보통 멤버 함수로 만들어 본다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) {}
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//	Point Add(const Point& arg)
//	{
//		return Point(x + arg.x, y + arg.y);
//	}
//};
//int main()
//{
//	Point p1(2, 3);
//	Point p2(4, 5);
//	Point p3 = p1.Add(p2);
//
//	p3.Print();// 6, 8
//}

//----------------------------------------------------------
// [단계 2] 클래스끼리는 그냥 + 를 못 쓴다 (에러 확인용)
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) {}
//	void Print()const { cout << x << ',' << y << endl; }
//	int GetX()const { return x; }
//	int GetY()const { return y; }
//};
//int main()
//{
//	Point p1(2, 3);
//	Point p2(4, 5);
//	Point p3 = p1 + p2;   // X - 에러 : + 를 어떻게 하라는지 알려 준 적이 없다
//
//	p3.Print();// 6, 8
//}

//----------------------------------------------------------
// [단계 1b] 이동 생성자 Point(Point&&) - 임시 객체는 옮겨 담는다
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//class Point
//{
//	int x, y;
//public:
//	Point(int x = 0, int y = 0) :x(x), y(y) { cout << "Point()" << endl; }
//	Point(const Point& arg) {//복사 생성자
//		this->x = arg.x;
//		this->y = arg.y;
//	}
//	Point(Point&& arg) { //이동 생성자
//		this->x = arg.x;
//		this->y = arg.y;
//	}
//	~Point() { cout << "~Point()" << endl; }
//	void Print()const { cout << x << ',' << y << endl; }
//};
//void PrintPoint(const Point& arg)
//{
//	arg.Print();
//	cout << "Point&" << endl;
//}
//void PrintPoint(const Point&& arg)
//{
//	arg.Print();
//	cout << "Point&&" << endl;
//}
//int main()
//{
//	Point p1(2, 3);
//	PrintPoint(p1);            // 이름이 있는 객체   -> Point&
//	PrintPoint(Point(0, 0));   // 임시 객체(rvalue)  -> Point&&
//
//	cout << "main() end" << endl;
//}

//----------------------------------------------------------
// [단계 1] rvalue 참조(&&) - "곧 사라질 임시 값"에 붙이는 참조
//----------------------------------------------------------
//#include <iostream>
//#include <vector>
//using namespace std;
//
//int main()
//{
//	int n = 10;
//	int& r = n;      // lvalue 참조 : 이름이 있는 값에만 붙는다
//	int&& r2 = 50;   // rvalue 참조 : 임시 값(리터럴)에 붙는다
//	//int&& r3 = n;  // X - 에러 : n 은 이름이 있으므로 rvalue 가 아니다
//
//	cout << n << ',' << r << ',' << r2 << endl;
//}
