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
COleDateTime timeNow;
timeNow = COleDateTime::GetCurrentTime();
```
