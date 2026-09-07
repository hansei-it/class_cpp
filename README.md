# C++ 기초 수업 (class_cpp)

C언어를 배운 학생이 **C++ 와 객체 지향 프로그래밍**으로 넘어가는 16강 분량의 수업 자료입니다.
`C++_ATI` 폴더의 날짜별 프로젝트(ex220622 ~ ex220701, 8일치)를 주제별로 재구성했습니다.

---

## 폴더 구조

```
class_cpp/
├─ class/                    강의 문서
│   ├─ init.md               개발 환경 준비 · 한글 깨짐 방지
│   └─ class1.md ~ class16.md
└─ example/
    ├─ example.slnx          <- 이 파일을 Visual Studio 로 연다
    ├─ example/              프로젝트 설정 (.vcxproj)
    └─ src/
        ├─ init.cpp
        ├─ ex1.cpp ~ ex16.cpp     강의별 예제 코드
        └─ app/                   여러 파일로 나눈 예제
            ├─ point/             헤더/소스 분리 (class5)
            ├─ string/            String 클래스 (class6, class7)
            └─ telphonebook/      종합 실습 (class16)
```

---

## 사용법

1. **`example/example.slnx`** 를 Visual Studio 로 엽니다.
2. `class/classN.md` 를 읽으면서 `example/src/exN.cpp` 를 실행합니다.
3. 각 `.cpp` 파일은 **맨 위가 완성 코드**, 아래로 갈수록 **주석 처리된 이전 단계**입니다.
   단계별로 확인하려면 위쪽을 주석 처리하고 아래 단계의 주석을 풀어 실행하세요.

### 실행할 예제 바꾸기

`ex1.cpp` ~ `ex16.cpp` 는 각각 `main()` 을 가지고 있어 **한 번에 하나만** 빌드에 포함해야 합니다.
처음에는 **`ex1.cpp` 만** 포함되어 있습니다.

| 단계 | 할 일 |
|------|-------|
| 1 | 솔루션 탐색기에서 현재 파일 우클릭 → **속성** → **일반** → **빌드에서 제외 : 예** |
| 2 | 실행할 파일 우클릭 → **속성** → **일반** → **빌드에서 제외 : 아니오** |
| 3 | **Ctrl + F5** (디버깅하지 않고 시작) |

> `LNK2005: main이(가) 이미 정의되어 있습니다` → 두 개 이상 포함됨
> `LNK1561: 진입점을 정의해야 합니다` → 아무것도 포함되지 않음

자세한 내용은 **[class/init.md](class/init.md)** 를 참고하세요.

---

## 강의 목차

| # | 강의 | 예제 | 주요 내용 |
|---|------|------|-----------|
| 0 | [init.md](class/init.md) | `init.cpp` | 개발 환경, 빌드 방법, 한글 깨짐 방지 |
| 1 | [class1.md](class/class1.md) | `ex1.cpp` | `cout`/`cin`, 초기화 3형식, **참조자**, `bool` |
| 2 | [class2.md](class/class2.md) | `ex2.cpp` | 참조 매개변수, 오버로딩, 기본 인자, **`const 형식&`**, `namespace`, `string`, `auto` |
| 3 | [class3.md](class/class3.md) | `ex3.cpp` | `new`/`delete`, 멤버 함수, **생성자·소멸자**, `private`/`public` |
| 4 | [class4.md](class/class4.md) | `ex4.cpp` | `class`, 생성자 오버로딩, **객체의 수명**(지역/전역/static/동적) |
| 5 | [class5.md](class/class5.md) | `ex5.cpp`, `app/point` | Get/Set, **`const` 멤버 함수**, 초기화 리스트, `this`, **파일 분리** |
| 6 | [class6.md](class/class6.md) | `ex6.cpp`, `app/string` | **복사 생성자**, 얕은 복사 vs **깊은 복사**, RAII, **3의 법칙** |
| 7 | [class7.md](class/class7.md) | `ex7.cpp`, `app/string` | `vector`, 값/참조/포인터 **반환**, `at()` 참조 반환, **`operator=`** |
| 8 | [class8.md](class/class8.md) | `ex8.cpp` | **상속**, `protected`, 부모 생성자 호출, 업캐스팅, **객체 슬라이싱** |
| 9 | [class9.md](class/class9.md) | `ex9.cpp` | **`virtual`**, 다형성, 순수 가상 함수, 추상 클래스, **가상 소멸자** |
| 10 | [class10.md](class/class10.md) | `ex10.cpp` | 추상 클래스 설계, **인터페이스**, 다중 상속 |
| 11 | [class11.md](class/class11.md) | `ex11.cpp` | **연산자 오버로딩** (`+`, `==`, `+=`, `++`, `=`, `[]`), rvalue 참조 |
| 12 | [class12.md](class/class12.md) | `ex12.cpp` | **함수 객체(Functor)**, **람다**, `sort`/`for_each` |
| 13 | [class13.md](class/class13.md) | `ex13.cpp` | 전역 연산자, **`operator<<`**, `friend`, 형변환 연산자, `explicit` |
| 14 | [class14.md](class/class14.md) | `ex14.cpp` | `static` 멤버, **템플릿**(함수/클래스), STL 컨테이너와 **반복자** |
| 15 | [class15.md](class/class15.md) | `ex15.cpp` | **예외 처리**, 예외 클래스 계층, **형변환 4종**, `typeid` |
| 16 | [class16.md](class/class16.md) | `ex16.cpp`, `app/telphonebook` | **스마트 포인터**, **파일 입출력**, 종합 실습 |

---

## 다중 파일 예제 (app/)

헤더/소스 분리와 실제 앱 구조를 익히는 예제입니다. 빌드할 때는 **`ex*.cpp` 를 모두 제외**하고 해당 폴더의 `.cpp` 만 포함하세요.

| 폴더 | 파일 | 관련 강의 |
|------|------|-----------|
| `app/point` | `Point.h`, `Point.cpp`, `PointMain.cpp` | [class5](class/class5.md) — 선언/정의 분리, `namespace` |
| `app/string` | `String.h`, `String.cpp`, `StringMain.cpp` | [class6](class/class6.md), [class7](class/class7.md) — 3의 법칙 |
| `app/telphonebook` | `Person`/`Student`/`Professor`/`TelPhoneBook`/`Util` + `TelMain.cpp` | [class16](class/class16.md) — 상속·다형성·스마트 포인터·파일 IO |

---

## 원본과의 관계

| 원본 폴더 | 재구성된 강의 |
|-----------|---------------|
| `ex220622` | class1, class2, class3 |
| `ex220623` | class4, class5 (+ `app/point`) |
| `ex220624` | class6, class7, class8 (+ `app/string`) |
| `ex220627` | class8, class9, class10 |
| `ex220628` | class11, class12 |
| `ex220629` | class12, class13, class14 |
| `ex220630` | class15, class16 |
| `ex220701` | class16 (+ `app/telphonebook`) |

원본 소스는 `C++_ATI` 폴더에 그대로 보존되어 있습니다.

> 원본은 **CP949** 인코딩이었으나, 이 프로젝트의 소스는 모두 **UTF-8(BOM 포함)** 으로 변환되어 있습니다.

---

## 빌드 환경

| 항목 | 값 |
|------|-----|
| IDE | Visual Studio (v145 툴셋) |
| C++ 표준 | **C++20** (`stdcpp20`) |
| 플랫폼 | x64 / Win32 |
| 전처리기 정의 | `_CRT_SECURE_NO_WARNINGS` |
| 소스 인코딩 | UTF-8 (BOM 포함) |

전체 예제는 MSVC 로 **경고 없이 컴파일**되는 것을 확인했습니다.
