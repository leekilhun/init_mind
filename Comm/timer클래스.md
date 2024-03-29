## 타이머/ 틱타이머/ 스케줄 콜백  

```c++
#pragma once
#include "pch.h"

typedef void (*CallbackType)(void* obj, const CString& name, void* data);


const int DEF_TIMER_SUCCESS					= 0;
const int ERR_TIMER_NOT_STARTED			= 1;
const int ERR_TIMER_ALREADY_STARTED = 2;


/// <summary>
/// Timer 클래스 
/// inline 폼
/// 21.10.30 이길훈
/// </summary>
class CtTimer
{
	/*************************************************
	 * data
	 *************************************************/
public:

private:
	BOOL m_isStart;
	ULONGLONG m_vluStart; // vlu = value 

public:

	/*************************************************
	 * constructor (ctor)
	 *************************************************/
	CtTimer()
		:m_isStart(FALSE),m_vluStart(NULL)
	{
	}
	virtual ~CtTimer(){
		;
	}

private:
	
public:
	/// <summary>
	/// Timer 시작
	/// </summary>
	/// <returns> 0 : 성공, others : 에러 </returns>
	int Start() { 
		m_vluStart = GetTickCount64();
		m_isStart = TRUE;
		return DEF_TIMER_SUCCESS;
	}
	
	/// <summary>
	/// Timer 종료
	/// </summary>
	/// <returns> 0 : 성공, others : 에러 </returns>
	int Stop() {
		if (!m_isStart) return ERR_TIMER_NOT_STARTED;
		m_isStart = FALSE;
		return DEF_TIMER_SUCCESS;
	}

	/// <summary>
	/// 경과 시간을 리턴한다.
	/// </summary>
	/// <returns>경과 시간 (초)</returns>
	double GetElaspTime() { 
		ULONGLONG stop_vlu; // Stop시의 값
		ULONGLONG elasp_vlu; // 경과 시간 

		stop_vlu = GetTickCount64();

		if (!m_isStart) return 0.;

		if (m_vluStart <= stop_vlu)  //check for wrap around condition
			elasp_vlu = stop_vlu - m_vluStart; //no wrap
		else	{
			elasp_vlu = (0xFFFFFFFFL - m_vluStart); //calculate time from start up to wrap
			elasp_vlu += 1;  //time to wrap from FFFFFFFF to 0 is 1 tick
			elasp_vlu += stop_vlu; //add in time after  0
		}
		return (double)elasp_vlu / 1000.;
	}

	/// <summary>
	/// 경과 시간을 리턴한다.
	/// </summary>
	/// <returns>경과 시간</returns>
	ULONGLONG GetElaspTime_ms() {
		ULONGLONG stop_vlu; // Stop시의 값
		ULONGLONG elasp_vlu; // 경과 시간 

		stop_vlu = GetTickCount64();

		if (!m_isStart) return 0;

		if (m_vluStart <= stop_vlu)  //check for wrap around condition
			elasp_vlu = stop_vlu - m_vluStart; //no wrap
		else {
			elasp_vlu = (0xFFFFFFFFL - m_vluStart); //calculate time from start up to wrap
			elasp_vlu += 1;  //time to wrap from FFFFFFFF to 0 is 1 tick
			elasp_vlu += stop_vlu; //add in time after  0
		}
		return elasp_vlu;
	}


	/// <summary>
	/// 지정 시간을 초과하지 않았는지 체크
	/// </summary>
	/// <param name="sec">지정시간 초(sec)</param>
	/// <returns>TRUE : 경과함 , FALSE: 경과하지 않음</returns>
	BOOL LessThan(double sec) {
		if (GetElaspTime() < sec) return TRUE;
		else return FALSE;
	}

	/// <summary>
	/// 지정 시간을 초과 했는지 체크
	/// </summary>
	/// <param name="sec">지정시간 초(sec)</param>
	/// <returns>TRUE : 경과하지 않음 , FALSE: 경과함 </returns>
	BOOL MoreThan(double sec) {
		if (GetElaspTime() > sec) return TRUE;
		else return FALSE;
	}

	/// <summary>
	/// Timer 시작 여부를 알려줌
	/// </summary>
	/// <returns></returns>
	BOOL IsStartTimer() const {
		return m_isStart;
	}

};






class CtimeScheduler
{

public:
	enum class eTickType
	{
		none = -1,
		ms_20,
		ms_50,
		ms_100,
		ms_1000,

		ms_max
	};


	/*************************************************
	 * data
	 *************************************************/
private:
	eTickType m_type;
	void (*func_Callback)(LPVOID);
	CallbackType m_callback;

	BOOL m_isStart;
	ULONGLONG m_vluPass; // vlu = value 
	LPVOID m_obj;

	CtTimer m_timer;
	CWinThread* m_thread;
	BOOL m_threadLife;
public:

	/*************************************************
	 * constructor (ctor)
	 *************************************************/
	CtimeScheduler(eTickType type)
		:m_type(eTickType::none), m_isStart(FALSE), m_vluPass(NULL), m_thread(nullptr)
		, m_obj(nullptr), m_callback(nullptr)
	{
		m_type = type;
		m_threadLife = TRUE;
		
		m_timer.Start();
		run();
	}
	virtual ~CtimeScheduler() {

		terminateThread();
	}

	/*************************************************
	 * function (method)
	 *************************************************/
private:
	/// <summary>
	/// create work thread
	/// </summary>
	void run() {
		if (m_thread != nullptr) return;
		m_thread = ::AfxBeginThread(timeThread, this);

	}

	/// <summary>
	/// timer thread function
	/// </summary>
	/// <param name="pParam">object address</param>
	/// <returns></returns>
	static UINT timeThread(LPVOID pParam)	{
		CtimeScheduler* pThis;
		pThis = (CtimeScheduler*)pParam;
		TRACE("\r Start Time Thread \n");
		while (pThis->m_threadLife)
		{
			pThis->timerISR();
			Sleep(20);
		}
		TRACE("\r Exit Time Thread \n");

		return 0;

	}

	/// <summary>
	/// interrupt service Routine 
	/// </summary>
	/// <param name=""></param>
	void timerISR(void){
		if (m_isStart != TRUE) return;

		ULONGLONG value = 0;
		m_vluPass = m_timer.GetElaspTime_ms() ;

		CString str;
		//str.Format(L"%ld", m_vluPass);
		str = getNowFormat();
		if (func_Callback != nullptr)		{
			switch (m_type)
			{
			case eTickType::ms_20:
			{
				if (m_vluPass > 20) {
					//(*func_Callback)(m_obj);
					m_callback(m_obj, str, NULL);
					m_timer.Start();
				}
			}
			break;
			case eTickType::ms_50:
			{
				if (m_vluPass > 50) {
					//(*func_Callback)(m_obj);
					m_callback(m_obj, str, NULL);
					m_timer.Start();
				}
			}
			break;
			case eTickType::ms_100:
			{
				if (m_vluPass > 100) {
					//(*func_Callback)(m_obj);
					m_callback(m_obj, str, NULL);
					m_timer.Start();
				}
			}
			break;
			case eTickType::ms_1000:
			{
				if (m_vluPass > 1000) {
					//(*func_Callback)(m_obj);
					m_callback(m_obj, str, NULL);
					m_timer.Start();
				}
			}
			break;
			} // switch
		}
	}


	/// <summary>
	/// terminate time thread
	/// </summary>
	void terminateThread() {

		m_threadLife = FALSE;		
		/*CtTimer t;
		t.Start();
		while (1) {
			if (t.MoreThan(0.5)) break;
		}*/		
		if (m_thread == nullptr) 
			return;
		
		DWORD result;
		result = ::WaitForSingleObject(m_thread->m_hThread, 5*1000); // 1초 기다림
		if (result == WAIT_OBJECT_0)
		{
			// 이곳은 스레드를 확실히 종료된 상태임
			TRACE(L"\rTerminate Timer Thread OK!\n");
		}
		else if (result == WAIT_TIMEOUT)
		{
			// 1초가 지나도 스레도가 종료되지 않음
			TRACE(L"\rTerminate Timer Thread Timeout!\n");
			::TerminateThread(m_thread->m_hThread, 1);
			::CloseHandle(m_thread->m_hThread);
		}
		m_thread = nullptr;
	}

	/// <summary>
	/// 시간 문자열을 반환
	/// </summary>
	/// <returns>[year-month-day][hh:mm:ss]</returns>
	CString getNowFormat()
	{
		CString date_now;
		SYSTEMTIME	systemDate;

		::GetLocalTime(&systemDate);

		date_now.Format(_T("[%04d-%02d-%02d][%02d:%02d:%02d] "),
			systemDate.wYear,
			systemDate.wMonth,
			systemDate.wDay,
			systemDate.wHour,
			systemDate.wMinute,
			systemDate.wSecond);

		return date_now;
	}



public:
	/// <summary>
	/// isr (인터럽트 서비스 루틴)시작
	/// </summary>
	/// <returns></returns>
	int Start() { 
		m_isStart = TRUE;
		m_vluPass = 1;

		return DEF_TIMER_SUCCESS;
	}

	/// <summary>
	/// isr 정지
	/// </summary>
	/// <returns></returns>
	int Stop() { 
		m_isStart = FALSE;
		m_vluPass = 0;
		return DEF_TIMER_SUCCESS;
	}
	
	/// <summary>
	/// 경과 시간 반환
	/// </summary>
	/// <returns></returns>
	ULONGLONG GetElaspTime() {
		return m_vluPass;
	}

	/// <summary>
	/// thread 종료
	/// </summary>
	void EndThread() {
		terminateThread();
	}

	/// <summary>
	/// isr 콜밸 함수 등록
	/// </summary>
	/// <param name="obj"></param>
	/// <param name="func"></param>
	void AttachCallbackFunc(LPVOID obj, void (*func)(LPVOID)){
		m_obj = obj; 
		func_Callback = func;
	}

	/// <summary>
	/// isr 콜밸 함수 등록
	/// </summary>
	/// <param name="obj"></param>
	/// <param name="cb"></param>
	void AddISRFunc(LPVOID obj, CallbackType cb) {
		m_obj = obj;
		m_callback = cb;
	}


};
```
