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


