## MFC, CString to char*(문자열 변환, LPSTR, LPCSTR)
LPSTR은  char* 형이다. char*형을 CString형에다 넣으면 들어가지만 반대로는 에러가 납니다.

1. CString to char* 
```c
char a[100] = {"하하"};
CString b = a;
```
2. 이 경우에는 에러가 납니다.
```c
CString b = "하하";
char a[100];
a = b;
```
문제 해결방법은 여러가지가 있다. 하지만 이렇게 strcpy를 써서 char*형인 a변수로 "하하"를 옮긴 다음에   
a를 인자로 넘겨주면 에러가 없을 것 입니다.
```c
CString b = "하하";
char a[100];
strcpy(a,b);
```

```c++
char* ch;
CString *str;

CString   -->  char*  변환
1) ch = (LPSTR)(LPCSTR)str;
2) ch = str.GetBuffer(str,GetLength());
3) wsprintf( ch, "%s", str);

char*  --> CString  변환
1) str = (LPCSTR)(LPSTR)ch;
2) str = ch;

```

#### CString 클래스의 GetBuffer()는 CString을  char*로 바꿔줍니다.
```c
CString strTemp = _T("test");
char *getTemp = NULL;

getTemp = malloc(strTemp.GetLength()+1);
strcpy(getTemp, strTemp.GetBuffer(strTemp.GetLength());
printf("결과:%sn", getTemp);
free(getTemp);

//operator LPCSTR()도 마찬가지
CString strTemp = _T("test");
char* getTemp = (LPSTR)(LPCSTR)strData;

```
