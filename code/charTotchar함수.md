멀티바이트 코드로 작업중 문제가 발생하여 유니코드로 바꾸면서 정리 합니다.  
앞으로 TCHAR 타입을 사용하는 습관을 들여야겠습니다.   

strcpy -> wcscpy -> _tcscpy (문자열을 복사)   
strncpy -> wcsncpy -> _tcsncpy ( 사이즈 만큼 복사)  
strlen -> wcslen -> _tcslen (문자열 길이 확인)    
strcat -> wcscat -> _tcscat (두 문자열 이어 붙이기)  
strncat -> wcsncat -> _tcsncat (사이즈 만큼 이어 붙이기)     
strcmp -> wcscmp -> _tcscmp (문자열 비교) (반환 값(-1, 0, 1))  
strncmp -> wcsncmp -> _tcsncmp ( 사이즈 만큼 문자열 비교)   
stricmp -> wcsicmp -> _tcsicmp (대소문자를 구별하지 않고 문자열을 비교)  
strnicmp -> wcsnicmp -> _tcsnicmp (사이즈 만큼 대소문자를 구별하지 않고 문자열을 비교)  
strchr -> wcschr -> _tcschr (문자 찾기)   
strrchr -> wcsrchr -> _tcsrchr (문자 찾기 (문자열 끝에서 부터 검색))  
strstr -> wcsstr -> _tcsstr (문자열 찾기)  
strpbrk -> wcspbrk -> _tcspbrk (문자 찾기 (두번째 인수를 찾고자 하는 문자들의 집합(문자열)으로 구성)   
strtok -> wcstok -> _tcstok (문자열 자르기 (두번째 인수를 집합(문자열)으로 구성 가능)) => 해당 문자가 NULL로 치환 됨  
strset -> wcsset -> _tcsset (문자 치환, 첫째 인수의 모든 문자를 두번째 인수 문자로 변경함) => "abc" -> "bbb"   
strnset -> wcsnset -> _tscnset (사이즈 만큼 문자 치환)   
strupr -> wcsupr -> _tcsupr (대문자로 치환)  
strlwr -> wcslwr -> _tcslwr (소문자로 치환)   
strrev -> wcsrev -> _tcsrev (문자열 역정렬) => "가나다라마" -> "마라다나가"    

---
### 기본적인 문자열 관련 함수
| 기능 | 멀티바이트  | 유니코드 | 통합형  (권장)| 
|---|:---|:---|:---| 
| 문자열 복사 | strcpy_s | wcscpy_s | _tcscpy_s | 
| 문자열 복사 <br> (길이 지정) | strncpy_s | _mbsnbcpy_s | _tcsncpy_s | 
| 문자열 서식 | sprintf_s <br>sprintf | swprintf_s<br>swprintf | _stprintf_s<br> _stprintf| 
| 문자열 비교 | strcmp | wcscmp | _tcscmp | 
| 문자열 길이 |strlen|wcslen| _tcslen|
| 문자열 추가| strcat_s<br>strcat|wcscat_s<br>wcscat|_tcscat_s<br>_tcscat|
|문자 검색|strchr|wcschr|_tcschr|
|문자열 검색|strstr|wcsstr|_tcsstr|
| 문자열 스트림 입력|scanf_s|wscanf_s|_tscanf_s|
---
### 문자열을 정수 또는 실수형으로 변환
| 기능 | 멀티바이트  | 유니코드 | 통합형  (권장)| 
|---|:---|:---|:---| 
| 문자열 정수(int) 변환| atoi|_wtoi|_ttoi|
| 문자열 정수(long) 변환| atol| _wtol| _ttol|
| 문자열 정수(long long) 변환| atoll| _wtoll| _ttoll|
| 문자열 실수(double) 변환|atof| _wtof|_ttof|

---
### 파일 입출력 관련
| 기능 | 멀티바이트  | 유니코드 | 통합형  (권장)| 
|---|:---|:---|:---| 
|파일 열기|fopen_s|_wfopen_s|_tfopen_s|
|파일 쓰기(스트림 출력)|fprintf_s|fwprintf_s|_ftprintf_s|
|스트림에서 문자열 읽기|fgets|fgetws|_fgetts|

 





