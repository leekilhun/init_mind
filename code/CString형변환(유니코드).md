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
---
##유니코드 ↔ 멀티바이트 문자열 변환함수
```c++
/************************************************************************/
/* 유니코드 ↔ 멀티바이트 문자열 변환함수 정의부                             */
/************************************************************************/
static char* StringToChar(CString str);              // CString → Char
static TCHAR* StringToTCHAR(CString str);            // CString → TCHAR
static std::string StringToStdString(CString str);   // CString → std::string
static CString CharToString(char *str);              // Char → CString
static TCHAR* CharToTCHAR(char *str);                // Char → TCHAR
static CString TCHARToString(TCHAR *str);            // TCHAR → CString
static char* TCHARToChar(TCHAR *str);                // TCHAR → Char
 
 
/************************************************************************/
/* 유니코드 ↔ 멀티바이트 문자열 변환함수 구현부                             */
/************************************************************************/
//
// CString → Char
//
char* StringToChar(CString str)
{
    char *szStr = NULL;
#if defined(UNICODE) || defined(_UNICODE)
    int nLen = str.GetLength() + 1;
    TCHAR *tszTemp = NULL;
    tszTemp = new TCHAR[nLen];
    memset(tszTemp, 0x00, nLen*sizeof(TCHAR));
    _tcscpy_s(tszTemp, nLen, str);
    // Get size (실제사용되는바이트사이즈)
    int nSize = WideCharToMultiByte(CP_ACP, 0, tszTemp, -1, NULL, NULL, NULL, NULL);
    szStr = new char[nSize];
    memset(szStr, 0x00, nSize);
    WideCharToMultiByte(CP_ACP, 0, tszTemp, -1, szStr, nSize, NULL, NULL);
    if(tszTemp)
    {
        delete [] tszTemp;
        tszTemp = NULL;
    }
#else
    int nLen = str.GetLength() + 1;
    szStr = new char[nLen];
    memset(szStr, 0x00, nLen);
    strcpy(szStr, str);
#endif
    return szStr;
}
 
//
// CString → TCHAR
//
TCHAR* StringToTCHAR(CString str)
{
    TCHAR *tszStr = NULL;
    int nLen = str.GetLength() + 1;
    tszStr = new TCHAR[nLen];
    memset(tszStr, 0x00, nLen*sizeof(TCHAR));
    _tcscpy_s(tszStr, nLen, str);
 
    return tszStr;
}
 
//
// CString → std::string
//
std::string StringToStdString(CString str)
{
    std::string stdStr;
    char *szStr = StringToChar(str);
    if(szStr)
    {
        stdStr = szStr;
        delete [] szStr;
    }
 
    return stdStr;
}
 
//
// Char → CString
//
CString CharToString(char *str)
{
    CString cStr;
#if defined(UNICODE) || defined(_UNICODE)
    int nLen = strlen(str) + 1;
    TCHAR *tszTemp = NULL;
    tszTemp = new TCHAR[nLen];
    memset(tszTemp, 0x00, nLen*sizeof(TCHAR));
    MultiByteToWideChar(CP_ACP, 0, str, -1, tszTemp, nLen*sizeof(TCHAR));
    cStr.Format(_T("%s"), tszTemp);
    if(tszTemp)
    {
        delete [] tszTemp;
        tszTemp = NULL;
    }
#else
    cStr.Format("%s", str);
#endif
    return cStr;
}
 
//
// Char → TCHAR
//
TCHAR* CharToTCHAR(char *str)
{
    TCHAR *tszStr = NULL;
#if defined(UNICODE) || defined(_UNICODE)
    int nLen = strlen(str) + 1;
    tszStr = new TCHAR[nLen];
    memset(tszStr, 0x00, nLen*sizeof(TCHAR));
    MultiByteToWideChar(CP_ACP, 0, str, -1, tszStr, nLen*sizeof(TCHAR));
#else
    int nLen = strlen(str) + 1;
    tszStr = new TCHAR[nLen];
    memset(tszStr, 0x00, nLen*sizeof(TCHAR));
    _tcscpy(tszStr, str);
#endif
    return tszStr;
}
 
//
// TCHAR → CString
//
CString TCHARToString(TCHAR *str)
{
    CString cStr;
    cStr.Format(_T("%s"), str);
    return cStr;
}
 
//
// TCHAR → Char
//
char* TCHARToChar(TCHAR *str)
{
    char *szStr = NULL;
#if defined(UNICODE) || defined(_UNICODE)
    int nSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, NULL, NULL, NULL);
    szStr = new char[nSize];
    memset(szStr, 0x00, nSize);
    WideCharToMultiByte(CP_ACP, 0, str, -1, szStr, nSize, NULL, NULL);
#else
    int nLen = strlen(str) + 1;
    szStr = new char[nLen];
    memset(szStr, 0x00, nLen);
    strcpy(szStr, str);
#endif
    return szStr;
}
```

변환 함수 사용 예

```c++
//
// StringToStdString 사용 예
//
CString strUnicode = _T("유니코드 문자열");
std::string szMultibyte = StringToStdString(strUnicode);
 
 
//
// StringToChar 사용 예
//
CString strUnicode = _T("유니코드 문자열");
char * szMultibyte = StringToChar(strUnicode);
printf("%s", szMultibyte);
 
// 포인터 형식으로 반환하는 변환 함수를 사용한 경우  
// 문자열을 사용한 뒤 delete로 메모리를 해제해야 합니다.
if (szMultibyte)
{
    delete [] szMultibyte;
}
 
 
//
// CharToString 사용 예
//
std::string szMultibyte = "멀티바이트 문자열";
CString strUnicode = CharToString((char *)szMultibyte.c_str());

```
