## 현재 시간을 가져오려면  
1 COleDateTime 개체를 만듭니다.  
2 GetCurrentTime을 호출합니다.  
```c++
COleDateTime timeNow;
timeNow = COleDateTime::GetCurrentTime();
```

## 경과 된 시간 계산  
1 두 COleDateTime 개체를 만듭니다.  
2 개체 중 하나 COleDateTime 를 현재 시간으로 설정 합니다.  
3 시간이 많이 걸리는 작업을 수행 합니다.  
4 다른 개체를 COleDateTime 현재 시간으로 설정 합니다.  
5 두 시간 사이의 차이를 가져옵니다.  
```c++
COleDateTime timeStart, timeEnd;
timeStart = COleDateTime::GetCurrentTime();
// ... perform time-consuming task
timeEnd = COleDateTime::GetCurrentTime();
COleDateTimeSpan spanElapsed = timeEnd - timeStart;
```

## 시간 형식을 지정
Format COleDateTime 또는 COleDateTimeSpan 의 멤버 함수를 사용하여 시간 또는 경과 시간을 나타내는 문자열을 만듭니다.
```c++
COleDateTime time(1970, 12, 18, 17, 30, 0);
// 18 December 1970, 5:30 PM
CString s = time.Format(VAR_DATEVALUEONLY);
// s contains the date formatted based on 
// the current national language specifications
// (locale ID). The time portion is ignored for 
// formatting purposes in this case.
```
