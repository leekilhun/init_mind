### C++ 문자열(LPSTR, LPCSTR, LPCTSTR, LPCWSTR),메모리 누수(memory leak) 체크, UTF8 변환    
***출처: https://nowonbun.tistory.com/739?category=507115 [명월 일지]***  

### 메모리 누수 발생 코드
```c++
#include <stdio.h>  
#include <iostream>
#include <Windows.h>
#include <string>
// 메모리 릭 체크하는 함수를 사용하기 위한 라이브러리 _CrtDumpMemoryLeaks();
#include <crtdbg.h>
// 메모리 릭을 콘솔에 표시하기 위한 함수
#if _DEBUG 
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__) 
#endif
using namespace std;
// 테스트를 위한 클래스
class Node {
  // c++의 string 객체
  string test;
};
// 실행 함수
int main()
{
  // Node를 선언
  Node a;
  // 메모리 초기화
  memset(&a, 0x00, sizeof(Node));
  
  // 종료 직전에 릭 확인을 한다.
  _CrtDumpMemoryLeaks();
  return 0;
}
```
![leak확인 화면](https://t1.daumcdn.net/cfile/tistory/993207445E999C2F2C "leak확인 화면")  

*char* 배열은 릭이 발생되지 않는다  
![leak](https://t1.daumcdn.net/cfile/tistory/99BA43435E999C2F2F "leak발생되지 않음")  


### 문자열을 다루는 두가지 방법  
```c++
#include <stdio.h>  
#include <iostream>
#include <Windows.h>
// 메모리 릭 체크하는 함수를 사용하기 위한 라이브러리 _CrtDumpMemoryLeaks();
#include <crtdbg.h>
// 메모리 릭을 콘솔에 표시하기 위한 함수
#if _DEBUG 
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__) 
#endif
using namespace std;
// 실행 함수
int main()
{
  // 배열로 데이터를 저장 (ascii)
  char type1[1024] = "Hello world1";
  // 배열로 데이터를 저장 (unicode)
  wchar_t type2[1024] = L"Hello world2";
  // 포인터로 데이터를 저장 (ascii)
  const char* type3 = "Hello world3\0";
  // 포인터로 데이터를 저장 (unicode)
  const wchar_t* type4 = L"Hello world4\0";
  // 콘솔 출력
  cout << "type1 - " << type1 << endl;
  wcout << "type2 - " << type2 << endl;
  cout << "type3 - " << type3 << endl;
  wcout << "type4 - " << type4 << endl;
  // 종료 직전에 릭 확인을 한다.
  _CrtDumpMemoryLeaks();
  return 0;
}
```
![결과](https://t1.daumcdn.net/cfile/tistory/99877C365E999C2F2B)   

### Windows.h의 문자열 타입
![윈도우문자열](https://t1.daumcdn.net/cfile/tistory/99C921345E999C2F30 "윈도우 문자열 타입")  

 정의 | 내용 | 비고 
---|:---|:---
LPSTR|long point string type | 32비트 메모리 포인트  
LPCSTR|long point const string type|
LPTSTR|long point multibyte string type|멀티바이트
LPCTSTR|long point const multibyte string type|멀티바이트
LPWSTR|long point unicode string type| 유니코드
LPCWSTR|long point const unicode string type| 유니코드
```c++
#include <stdio.h>  
#include <iostream>
#include <Windows.h>
// 메모리 릭 체크하는 함수를 사용하기 위한 라이브러리 _CrtDumpMemoryLeaks();
#include <crtdbg.h>
// 메모리 릭을 콘솔에 표시하기 위한 함수
#if _DEBUG 
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__) 
#endif
using namespace std;
// 실행 함수
int main()
{
  // const char*와 같다
  LPCSTR type1 = "Hello world1\0";
  // const wchar_t*와 같다
  LPCWSTR type2 = L"Hello world2\0";
  // 콘솔 출력
  cout << "type1 - " << type1 << endl;
  wcout << "type2 - " << type2 << endl;
  // 종료 직전에 릭 확인을 한다.
  _CrtDumpMemoryLeaks();
  return 0;
}

```
![result](https://t1.daumcdn.net/cfile/tistory/993B13355E999C2F1D)  

### ascii코드에서 unicode로 변환하는 방법  
char -> wchar_t로 변환 ,  wchar_t -> char로 변환  
``` c++
#include <stdio.h>	
#include <iostream>
#include <Windows.h>
// 메모리 릭 체크하는 함수를 사용하기 위한 라이브러리 _CrtDumpMemoryLeaks();
#include <crtdbg.h>
// 메모리 릭을 콘솔에 표시하기 위한 함수
#if _DEBUG 
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__) 
#endif
using namespace std;
// 실행 함수
int main()
{
  // 변수 선언
  LPCSTR type1 = "Hello world\0";
  LPCWSTR type2 = L"Hello world2\0";
  //type2의 wchar_t타입을 char로 변환하기 위한 변수.
  // const char는 소스 레벨의 값을 가져오기 때문에 const가 필요하지만, memcpy하는 값이기 때문에 const를 사용하지 않느다.
  char type3[BUFFERSIZE];
  //type1의 char타입을 wchar_t로 변환하기 위한 변수.
  wchar_t type4[BUFFERSIZE];
  // char를 wchar_t로 변환하는 함수
  // 파라미터 순서는 타입, 0, 소스 값, 소스 값 길이, 대상, 대상 길이
  MultiByteToWideChar(CP_ACP, 0, type1, strlen(type1), type4, BUFFERSIZE);
  // wchar_t를 char로 변환하는 함수
  // 파라미터 순서는 타입, 0 소스 값, 소스 값 길이(메모리 길이임 글자 길이가 아님), 대상, 대상 길이, NULL, NULL
  WideCharToMultiByte(CP_ACP, 0, type2, wcslen(type2) * 2, type3, BUFFERSIZE, NULL, NULL);
  // 콘솔 출력
  cout << "type1 - " << type1 << endl;
  wcout << "type2 - " << type2 << endl;
  cout << "type3 - " << type3 << endl;
  wcout << "type4 - " << type4 << endl;
  // 종료 직전에 릭 확인을 한다.
  _CrtDumpMemoryLeaks();
  return 0;
}

```
![result](https://t1.daumcdn.net/cfile/tistory/99499F365E999C2F27)  

unicode에서 utf8로 변환, utf8에서 unicode로 변환
```c++
#include <stdio.h>  
#include <iostream>
#include <Windows.h>
// 메모리 릭 체크하는 함수를 사용하기 위한 라이브러리 _CrtDumpMemoryLeaks();
#include <crtdbg.h>
// 메모리 릭을 콘솔에 표시하기 위한 함수
#if _DEBUG 
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__) 
#endif
using namespace std;
// 실행 함수
int main()
{
  // unicode 값
  LPCWSTR type1 = L"Hello world\0";
  // UTF8형식으로 변환
  char type2[BUFFERSIZE];
  // 다시 unicode로 변환
  wchar_t type3[BUFFERSIZE];
  // unicode에서 utf8로 변환, 타입을 CP_ACP가 아닌 CP_UTF8를 사용한다.
  WideCharToMultiByte(CP_UTF8, 0, type1, wcslen(type1) * 2, type2, BUFFERSIZE, NULL, NULL);
  // utf8에서 unicode로 변환, 타입을 CP_ACP가 아닌 CP_UTF8를 사용한다.
  MultiByteToWideChar(CP_UTF8, 0, type2, strlen(type2), type3, BUFFERSIZE);
  // 콘솔 출력
  wcout << "type1 - " << type1 << endl;
  cout << "type2 - " << type2 << endl;
  wcout << "type3 - " << type3 << endl;
  // 종료 직전에 릭 확인을 한다.
  _CrtDumpMemoryLeaks();
  return 0;
}

```
![result](https://t1.daumcdn.net/cfile/tistory/997BF74D5E999C2F26)  
타입의 관한 전처리 값은 다음과 같습니다.  
![전처리](https://t1.daumcdn.net/cfile/tistory/995E5F425E999C2F2E)
  



