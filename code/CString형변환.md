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
```c++
LPWSTR szAA;
CString strBB;
szAA = (LPWSTR)(LPCWSTR)strBB;
```
방법 2  
```c++
CString aa = _T("123");
LPWSTR dd = aa.GetBuffer();
```
**LPWSTR -> CString**   
```c++
LPWSTR szAA;
CString strBB;

strBB.Format(_T("%s"),szAA);
```

## CString <-> short*(unicode, multibyte), PLC ASCII
### short* -> CString
multibyte
```c++
short* sshort = new short[2];
sshort[0] = 20826;
sshort[1] = 67;

CString strTest;
strTest.Format("%s", sshort);

--> strTest에는 "ZQC"
```
unicode
```c++
short sshort[2];
sshort[0] = 20826;
sshort[1] = 67;

CString strTest;
strTest.Format(_T("%s"), sshort);

--> strTest에는 "ZQC"
```
### CString -> short*
multibyte
```c++
CString strMessage = "ABC";
short pData[10];
sprintf_s((char*)pData, 10, "%s", strMessage);

pData[0]에는 16961
pData[1]에는 67
```
unicode
```c++
CString strMessage = _T("ABC");
short *pData = new short[10];
sprintf_s((char*)pData, 10, "%s", strMessage);

pData[0]에는 16961
pData[1]에는 67
```


### CString -> BSSTR
multibyte
```c++
```
unicode
```c++
```
