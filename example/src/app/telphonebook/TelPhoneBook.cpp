//===========================================================
// app/telphonebook/TelPhoneBook.cpp
//   phoneList 는 Person 을 가리키지만 실제로는 Student / Professor 가 들어 있고,
//   per->Print() 는 가상 함수라 각자의 Print() 가 호출됩니다. <- 다형성
//===========================================================
#include <fstream>
#include <string>
#include "TelPhoneBook.h"
#include "Student.h"
#include "Professor.h"
using namespace std;

void TelPhoneBook::AddTel()
{
	cout << "이름 입력: ";
	string name;
	cin >> name;
	cout << "전화 입력: ";
	string phone;
	cin >> phone;

	phoneList.push_back(make_shared<Person>(name, phone));
}

void TelPhoneBook::AddStudent()
{
	cout << "이름 입력: ";
	string name;
	cin >> name;
	cout << "전화 입력: ";
	string phone;
	cin >> phone;
	cout << "학년 입력: ";
	int grade = 0;
	cin >> grade;

	phoneList.push_back(make_shared<Student>(name, phone, grade));
}

void TelPhoneBook::AddProfessor()
{
	cout << "이름 입력: ";
	string name;
	cin >> name;
	cout << "전화 입력: ";
	string phone;
	cin >> phone;
	cout << "직위 입력: ";
	string position;
	cin >> position;

	phoneList.push_back(make_shared<Professor>(name, phone, position));
}

void TelPhoneBook::PrintTel()const
{
	if (phoneList.empty())
	{
		cout << "등록된 전화번호가 없습니다." << endl;
		return;
	}
	for (auto& per : phoneList)
		per->Print();      // Person / Student / Professor 각자의 Print()
}

void TelPhoneBook::SearchTel()const
{
	cout << "찾을 이름 입력:";
	string name;
	cin >> name;

	for (auto& per : phoneList)
	{
		if (per->GetName() == name)
		{
			cout << "찾은 원소 : ";
			per->Print();
			return;
		}
	}
	cout << "찾지 못했습니다." << endl;
}

void TelPhoneBook::RemoveTel()
{
	cout << "삭제할 이름 입력:";
	string name;
	cin >> name;

	for (vector< shared_ptr<Person> >::size_type i = 0; i < phoneList.size(); ++i)
	{
		if (phoneList[i]->GetName() == name)
		{
			cout << "삭제 원소: ";
			phoneList[i]->Print();
			phoneList.erase(phoneList.begin() + i);   // shared_ptr 이 알아서 해제
			return;
		}
	}
	cout << "찾지 못했습니다." << endl;
}

void TelPhoneBook::WriteTextTel()const
{
	ofstream fout("phonebook.txt");
	for (vector< shared_ptr<Person> >::size_type i = 0; i < phoneList.size(); ++i)
	{
		fout << "[" << i << "]: ";
		phoneList[i]->Print(fout);    // cout 대신 fout 을 넘긴다
	}
	cout << "phonebook.txt 에 저장했습니다." << endl;
}
