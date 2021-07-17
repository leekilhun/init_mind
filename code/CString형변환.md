### CString -> const char*
```c++
CString aa = _T("123");
CStringA bb = (CStringA)aa;
const char* dd = bb.GetBuffer();
```

### const char* -> CString 
```c++
const char* dd = "123";
CString aa;
aa = (CString)dd;
```

### CString <-> LPWSTR
unicode 환경에서의 형변환  
**CString -> LPWSTR**  
방법 1
```
LPWSTR szAA;
CString strBB;
szAA = (LPWSTR)(LPCWSTR)strBB;
```
방법 2
```
CString aa = _T("123");
LPWSTR dd = aa.GetBuffer();
```
**LPWSTR -> CString**   
```c
LPWSTR szAA;
CString strBB;

strBB.Format(_T("%s"),szAA);
```
