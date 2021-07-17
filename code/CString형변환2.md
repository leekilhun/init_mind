## MFC, CString to char*(문자열 변환, LPSTR, LPCSTR)
LPSTR은  char* 형이다. char*형을 CString형에다 넣으면 들어가지만 반대로는 에러가 납니다.

1. CString to char* 
```c
char a[100] = {"하하"};
CString b = a;
```
