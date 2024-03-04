# 코드 프로파일러

## 사용법
`#define PROFILE`을 선언한다. 
`PROFILE` 선언 삭제 시, 프로파일러는 작동하지 않도록 매크로가 설정 되어있다.
측정할 코드 영역은 아래 2가지 방법을 지원한다.

1. 시작과 끝 점을 지정하는 방식
```c++
#define PROFILE
#include "Profiler.h"

int main()
{
	int i = 0;
	while (i < 10)
	{
		{
			PRO_BEGIN("AAA");
      // 측정 영역
			Sleep(100);
			PRO_END("AAA");
		}

		i++;
	}
}
```
측정 영역의 시작을 `PRO_BEGIN` 매크로를 사용해 태그를 지정.
측정 영역의 끝에 `PRO_END` 매크로를 사용해 끝낼 태그를 지정.

2. 클래스의 생성자와 소멸자로 자동으로 측정하는 방식
```c++
#define PROFILE
#include "Profiler.h"

int main()
{
	int i = 0;
	while (i < 10)
	{
		{
			PRO(AAA, "AAA");
			Sleep(100);
		}

		i++;
	}
}
```
측정을 시작하고 싶은 곳에 `PRO` 매크로 사용. 인수로는 생성될 객체명, 태그를 지정한다.

## 지원 함수

- `ProfileOutText` 함수
해당 함수는 인수로 받은 파일 이름으로 `txt` 파일을 생성해 프로파일 결과를 저장한다.
이 함수는 프로파일 중에도 호출하여 프로파일 결과를 중간에 확인할 수 있다.

- `CreateProfileFileName` 함수
파일 이름을 간편하게 생성하기 위한 함수. 기본 양식은 `Profile_YYYYMMDD_HHMMSS` 이다.

- `ProfileReset` 함수
프로파일한 내용을 모두 초기화한다.

## 예외
`PRO_BEGIN` 매크로 호출 후, 대응되는 `PRO_END`가 호출되지 않으면 `PRO_BEGIN`에서 예외를 발생시킨다.
