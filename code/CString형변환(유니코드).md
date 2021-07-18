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
