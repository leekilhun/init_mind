## 유니코드 환경에선 CString to char*를 위해 두 단계를 거쳐야 한다 
1. CString to wchar_t* 간의 형변환  
2. wchar_t* to char* 간의 형변환  
```c
CString  str;           //형변환할 문자열이 저장된 CString 변수
wchar_t* wchar_str;     //첫번째 단계(CString to wchar_t*)를 위한 변수
char*    char_str;      //char* 형의 변수
int      char_str_len;  //char* 형 변수의 길이를 위한 변수

//1. CString to wchar_t* conversion
wchar_str = str.GetBuffer(str.GetLength());

//2. wchar_t* to char* conversion
//char* 형에 대한길이를 구함
char_str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
char_str = new char[char_str_len];  //메모리 할당
//wchar_t* to char* conversion
WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, char_str, char_str_len, 0,0);  

//Done.
```

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
2) ch = str.GetBuffer(str.GetLength());
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
