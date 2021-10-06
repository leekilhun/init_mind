## 내가 만들 MS Access를 이용 데이터 관리

```c++
#pragma once
#include "pch.h"
#include "afxdb.h"


#define CAST_INT(x)  static_cast<int>(x)

struct iniData_tbl
{
    CString section;
    CString item;
    CString value;
    CString object;
};


/*
 * 작성자 : 이길훈
 * 21.10.06 MS Access를 DB로 활용하는 클래스
 */ 

//{Microsoft Access Driver (*.mdb)};DBQ=kListDB.mdb;

class CDataDB
{
public:
    
public:
    CString m_Path;
    CString m_Id;
    CString m_Pw;
    CString m_Connection;
    BOOL m_IsConnected;
   
    CDatabase *m_pDb;
    CRecordset *m_pRs;

public:
    CDataDB(CString path, CString id, CString pw = _T("")) {
        init();

        m_Path = path;
        m_Id = id;
        m_Pw = pw;
        m_Connection.Empty();
    }

    CDataDB() {
        init();
    }
    virtual ~CDataDB() { 
        destory();
    }
protected:
    void init() {
        m_Path.Empty();
        m_Id.Empty();
        m_Pw.Empty();
        m_Connection.Empty();
       
        m_IsConnected = FALSE;
        m_pDb = nullptr;
        m_pRs = nullptr;

        m_pDb = new CDatabase; 
        m_pRs = new CRecordset(m_pDb);
    }

    void destory() {       
        if (m_pRs != nullptr)
        {
            if (m_pRs->IsOpen()) { m_pRs->Close(); }
            delete m_pRs;
            m_pRs = nullptr;
        }
        if (m_pDb != nullptr) {
            if (m_pDb->IsOpen()) { m_pDb->Close(); }
            delete m_pDb;
            m_pDb = nullptr;
        }        
    }
    CString now() {
        CString date, time, strResult;

        CTime t = CTime::GetCurrentTime();
        date.Format(_T("%04d/%02d/%02d"),
            t.GetYear(),
            t.GetMonth(),
            t.GetDay()
        );
        time.Format(_T("%02d:%02d:%02d"),
            t.GetHour(),
            t.GetMinute(),
            t.GetSecond()
        );

        strResult = date +_T(" ") + time;
        return strResult;
    }
public:
    /// <summary>
    /// MSaccess 연결, 오픈
    /// </summary>
    /// <returns></returns>
    inline BOOL AccessOpen() {
        if (m_pDb->IsOpen()) { m_pDb->Close(); };
        m_Connection.Format(_T("DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s;UID=%s;PWD=%s")
            , m_Path.GetString()
            , m_Id.GetString()
            , m_Pw.GetString()
        );
        try
        {
            m_IsConnected = m_pDb->OpenEx(m_Connection);
        }
        catch (CException* e)
        {
            e->ReportError();
        }
        return m_IsConnected;
    }

    /// <summary>
    /// MSaccess 연결, 오픈
    /// </summary>
    /// <param name="path">원본db파일 위치</param>
    /// <param name="id">접근 아이디</param>
    /// <param name="pw">접근 비밀번호</param>
    /// <returns>m_IsConnected</returns>
    inline BOOL AccessOpen(CString path, CString id, CString pw = _T(""))   {
        if (m_pDb->IsOpen()) { m_pDb->Close(); };        
        m_Connection.Format(_T("DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s;UID=%s;PWD=%s")
            , path.GetString() /*_T("C://DB/dataTableDB.mdb");*/
            , id.GetString()
            , pw.GetString()
        );        
            
        try
        {
            m_IsConnected = m_pDb->OpenEx(m_Connection);
        }
        catch (CException* e)
        {
            e->ReportError();
        }
        return m_IsConnected;
    }

    /// <summary>
    /// MSaccess 연결을 차단
    /// </summary>
    inline void AccessClose() {
        /*if (m_pRs->IsOpen())
        {
            m_pRs->Close();
        }*/
        if (m_pDb->IsOpen()) { 
            m_pDb->Close(); 
        }       
        m_IsConnected = FALSE;
    }

    /// <summary>
    /// 레코드셋 생성
    /// </summary>
    /// <param name="table_name">data table name</param>
    /// <returns>생성 TRUE, 실패 FALSE</returns>
    inline BOOL SetRecordset(CString table_name ) {
        if (m_pRs->IsOpen()) {  m_pRs->Close();   }   

        CString strQuery;
        strQuery.Format(_T("select * from %s")
            , table_name.GetString());
        return (m_pRs->Open(CRecordset::dynaset, strQuery));
    }
   
    /// <summary>
    /// 쿼리문 실행 후 레코드셋을 생성
    /// </summary>
    /// <param name="sql">쿼리문</param>
    /// <param name="table_name">data table name</param>
    /// <returns></returns>
    inline BOOL ExeSQL(CString sql, CString table_name) {
        if (m_pDb->IsOpen() == FALSE) {
            if (AccessOpen() == FALSE) { return FALSE; }
        }
        m_pDb->BeginTrans();
        try
        {
            m_pDb->ExecuteSQL(sql);
        }
        catch (CException* e)
        {
            e->ReportError();
        }
        m_pDb->CommitTrans();
        return SetRecordset(table_name);
    }

};



class iniDataDB : public CDataDB
{
public:
    CString m_Tablename;


public:
    iniDataDB(CString path, CString id, CString pw, CString table)   {
        m_Tablename.Empty();
        m_Tablename = table;
        CDataDB::m_Path = path;
        CDataDB::m_Id = id;
        CDataDB::m_Pw = pw;
        CDataDB::m_Connection.Empty();
    }

    iniDataDB(CString table) {
        m_Tablename.Empty();
        m_Tablename = table;
    }
    virtual ~iniDataDB() {
        CDataDB::destory();
    }
 
public:
    inline BOOL Open() {
        return CDataDB::AccessOpen();
    }


    inline void Close() {
        CDataDB::AccessClose();
    }

    inline BOOL GetDataset() {
        return SetRecordset(m_Tablename);
    }

    inline BOOL InsertData(iniData_tbl* data) {
        CString sql;
        sql.Format(_T("INSERT INTO iniData (DSection, DItem, DValue, DUpdate) values('%s', '%s', '%s', '%s')")
            , data->section.GetString()
            , data->item.GetString()
            , data->value.GetString()
            , now().GetString()
        );
        return CDataDB::ExeSQL(sql, m_Tablename);
    }    

    inline BOOL UpdateValue(iniData_tbl& data) {
        CString sql;
        sql.Format(_T("UPDATE iniData SET DValue ='%s', DUpdate ='%s' WHERE DSection='%s' AND DItem ='%s'")
            , data.value//L"Update_1"
            , now().GetString()
            , data.section//L"Section_0"
            , data.item//L"Item_0"
        );

        return CDataDB::ExeSQL(sql, m_Tablename);
    }

    inline BOOL DeleteRecord(iniData_tbl& data) {
        CString sql;
        sql = _T("DELETE FROM iniData WHERE ");
        sql.AppendFormat(_T("DSection = '%s' AND DItem = '%s'")
            , data.section  //L"Section_0"
            , data.item     //L"Item_0"
        );

        return CDataDB::ExeSQL(sql, m_Tablename);
    }
};


```
