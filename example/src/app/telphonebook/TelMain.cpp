//===========================================================
// app/telphonebook/TelMain.cpp - 종합 실습 : 전화번호부 프로그램
//   이 수업에서 배운 것이 거의 모두 들어 있습니다.
//     클래스 / 상속 / 가상 함수와 다형성 / vector / string /
//     스마트 포인터 / 파일 입출력 / static 멤버 함수 / 헤더 분리
//
// [빌드 방법]
//   1. ex1.cpp ~ ex16.cpp 를 모두 "빌드에서 제외 : 예" 로 둔다.
//   2. app/telphonebook 폴더의 .cpp 5개(Person, Student, Professor,
//      TelPhoneBook, Util)와 TelMain.cpp 만 제외를 푼다.
//   3. 실행 (Ctrl + F5)
//===========================================================
#include "Main.h" //공통으로 정의되어야 하는 헤더

int main()
{
	TelPhoneBook tpb;
	bool run = true;

	while (run)
	{
		Util::Menu();
		switch (_getch())      // 엔터를 누르지 않아도 키 하나를 바로 읽는다
		{
		case '1': tpb.AddTel();        break;
		case '2': tpb.PrintTel();      break;
		case '3': tpb.SearchTel();     break;
		case '4': tpb.RemoveTel();     break;
		case '5': tpb.WriteTextTel();  break;
		case '6': tpb.AddStudent();    break;
		case '7': tpb.AddProfessor();  break;
		case '0': run = false;         break;
		default:                       break;
		}
	}
}
