## 문자열 코드

### 1. 콘솔 환경 *sprintf*
```c
#include <stdio.h>
int main() {
  int integer = 123;
  char character = 'c';
  char string[] = "hello, world";
  int* pointer = &integer;
  double pi = 3.141592;
  char buf[100];

  sprintf(buf, "integer : (decimal) %d (octal) %o \n", integer, integer);
  printf("%s \n", buf);

  sprintf(buf, "character : %c \n", character);
  printf("%s \n", buf);

  sprintf(buf, "string : %s \n", string);
  printf("%s \n", buf);

  sprintf(buf, "pointer addr : %p \n", pointer);
  printf("%s \n", buf);

  sprintf(buf, "floating point : %e // %f \n", pi, pi);
  printf("%s \n", buf);

  sprintf(buf, "percent symbol : %% \n");
  printf("%s \n", buf);

  return 0;
}
```

### 2. 윈도우 환경 *wsprintf*
```c++
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR lpOut[1024];

	switch(iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		wsprintf(lpOut,TEXT("정수=%d, 부호없는 정수=%u"),1234567890,3234567890);
		TextOut(hdc,10,10,lpOut,lstrlen(lpOut));
		wsprintf(lpOut,TEXT("16진수=%x, 16진수=%X"),0xabcd,0xabcd);
		TextOut(hdc,10,30,lpOut,lstrlen(lpOut));

		wsprintf(lpOut,TEXT("문자=%c, 문자=%C"),'S',L'한');
		TextOut(hdc,10,50,lpOut,lstrlen(lpOut));

		wsprintf(lpOut,TEXT("문자열=%s, 문자열=%S"),"대한민국",L"우리나라");
		TextOut(hdc,10,70,lpOut,lstrlen(lpOut));

		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

```
