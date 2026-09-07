//===========================================================
// app/telphonebook/TelPhoneBook.h - 전화번호부 본체
//   vector<shared_ptr<Person>> 로 담기 때문에
//   erase 만 해도 메모리가 새지 않고, 소멸자를 따로 쓸 필요도 없습니다.
//   Person* 로 담으면 delete 를 직접 챙겨야 합니다(아래 주석 참고).
//===========================================================
#pragma once
#include <vector>
#include <memory>
#include "Person.h"

class TelPhoneBook
{
	std::vector< std::shared_ptr<Person> > phoneList;
public:
	void AddTel();
	void AddStudent();
	void AddProfessor();
	void PrintTel()const;
	void SearchTel()const;
	void RemoveTel();
	void WriteTextTel()const;
};

//----------------------------------------------------------
// [원시 포인터 버전] 이렇게 만들면 직접 정리해야 한다
//----------------------------------------------------------
//class TelPhoneBook
//{
//	std::vector<Person*> phoneList;
//public:
//	~TelPhoneBook()                       // 소멸자를 반드시 만들어야 하고
//	{
//		for (auto p : phoneList)
//			delete p;
//	}
//	void RemoveTel();                     // erase 전에 delete 도 직접 해야 한다
//	...
//};
