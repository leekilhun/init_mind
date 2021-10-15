## json 파일 리드  

```c++
#pragma once
#define TOKEN_COUNT 20    // 토큰의 최대 개수

#include <map>


// 토큰 종류 열거형
typedef enum _JSON_TOKEN_TYPE {
    JSON_TOKEN_NONE,
    JSON_TOKEN_STRING,    // 문자열 토큰
    JSON_TOKEN_NUMBER,    // 숫자 토큰
    JSON_TOKEN_BOOL,
} JSON_TOKEN_TYPE;


// 토큰 구조체
typedef struct _JSON_TOKEN {
    JSON_TOKEN_TYPE type;        // 토큰 종류
    union {                 // 두 종류 중 한 종류만 저장할 것이므로 공용체로 만듦
        char* string;       // 문자열 포인터
        double number;      // 실수형 숫자
    };
    bool isArray;           // 현재 토큰이 배열인지 표시
} JSON_TOKEN;


// JSON 구조체
typedef struct _JSON {
    JSON_TOKEN tokens[TOKEN_COUNT];    // 토큰 배열
} JSON;


class CJsonFile
{
public:

public:

public:
    CJsonFile() {
        init();
    }
    virtual ~CJsonFile() {
        init();
    }
protected:
    inline void init() {
    }

public:
    inline char* readFile(char* filename, int* readSize)    // 파일을 읽어서 내용을 반환하는 함수
    {

        //errno_t err = fopen_s(&fp, "TestFile.dat", "r");
       
        FILE* fp ;
        errno_t err = fopen_s(&fp, filename, "rb");

        if (fp == NULL)
            return NULL;

        int size;
        char* buffer;

        // 파일 크기 구하기
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        // 파일 크기 + NULL 공간만큼 메모리를 할당하고 0으로 초기화
        buffer = (char*)malloc(size + 1);
        memset(buffer, 0, size + 1);

        // 파일 내용 읽기
        if (fread(buffer, size, 1, fp) < 1)
        {
            *readSize = 0;
            free(buffer);
            fclose(fp);
            return NULL;
        }

        // 파일 크기를 넘겨줌
        *readSize = size;

        fclose(fp);    // 파일 포인터 닫기

        return buffer;
    }


    inline void parseJSON(char* doc, int size, JSON* json)    // JSON 파싱 함수
    {
        int tokenIndex = 0;    // 토큰 인덱스
        int pos = 0;           // 문자 검색 위치를 저장하는 변수

        if (doc[pos] != '{')   // 문서의 시작이 {인지 검사
            return;

        pos++;    // 다음 문자로

        while (pos < size)       // 문서 크기만큼 반복
        {
            switch (doc[pos])    // 문자의 종류에 따라 분기
            {
            case '"':            // 문자가 "이면 문자열
            {
                // 문자열의 시작 위치를 구함. 맨 앞의 "를 제외하기 위해 + 1
                char* begin = doc + pos + 1;

                // 문자열의 끝 위치를 구함. 다음 "의 위치
                char* end = strchr(begin, '"');
                if (end == NULL)    // "가 없으면 잘못된 문법이므로 
                    break;          // 반복을 종료

                int stringLength = end - begin;    // 문자열의 실제 길이는 끝 위치 - 시작 위치

                // 토큰 배열에 문자열 저장
                // 토큰 종류는 문자열
                json->tokens[tokenIndex].type = JSON_TOKEN_STRING;
                // 문자열 길이 + NULL 공간만큼 메모리 할당
                json->tokens[tokenIndex].string = (char*)malloc(stringLength + 1);
                // 할당한 메모리를 0으로 초기화
                memset(json->tokens[tokenIndex].string, 0, stringLength + 1);

                // 문서에서 문자열을 토큰에 저장
                // 문자열 시작 위치에서 문자열 길이만큼만 복사
                memcpy(json->tokens[tokenIndex].string, begin, stringLength);

                tokenIndex++; // 토큰 인덱스 증가

                pos = pos + stringLength + 1;    // 현재 위치 + 문자열 길이 + "(+ 1)
            }
            break;
            case '[':        // 문자가 [이면 배열
            {
                pos++;       // 다음 문자로

                while (doc[pos] != ']')    // 닫는 ]가 나오면 반복 종료
                {
                    // 여기서는 문자열 배열만 처리
                    if (doc[pos] == '"')   // 문자가 "이면 문자열
                    {
                        // 문자열의 시작 위치를 구함. 맨 앞의 "를 제외하기 위해 + 1
                        char* begin = doc + pos + 1;

                        // 문자열의 끝 위치를 구함. 다음 "의 위치
                        char* end = strchr(begin, '"');
                        if (end == NULL)   // "가 없으면 잘못된 문법이므로
                            break;         // 반복을 종료

                        int stringLength = end - begin;    // 문자열의 실제 길이는 끝 위치 - 시작 위치

                        // 토큰 배열에 문자열 저장
                        // 토큰 종류는 문자열
                        json->tokens[tokenIndex].type = JSON_TOKEN_STRING;
                        // 문자열 길이 + NULL 공간만큼 메모리 할당
                        json->tokens[tokenIndex].string = (char*)malloc(stringLength + 1);
                        // 현재 문자열은 배열의 요소
                        json->tokens[tokenIndex].isArray = true;
                        // 할당한 메모리를 0으로 초기화
                        memset(json->tokens[tokenIndex].string, 0, stringLength + 1);

                        // 문서에서 문자열을 토큰에 저장
                        // 문자열 시작 위치에서 문자열 길이만큼만 복사
                        memcpy(json->tokens[tokenIndex].string, begin, stringLength);

                        tokenIndex++; // 토큰 인덱스 증가

                        pos = pos + stringLength + 1; // 현재 위치 + 문자열 길이 + "(+ 1)
                    }

                    pos++;    // 다음 문자로
                }
            }
            break;
            case '0': case '1': case '2': case '3': case '4': case '5':    // 문자가 숫자이면
            case '6': case '7': case '8': case '9': case '-':              // -는 음수일 때
            {
                // 문자열의 시작 위치를 구함
                char* begin = doc + pos;
                char* end;
                char* buffer;

                // 문자열의 끝 위치를 구함. ,가 나오거나
                end = strchr(doc + pos, ',');
                if (end == NULL)
                {
                    // } 가 나오면 문자열이 끝남
                    end = strchr(doc + pos, '}');
                    if (end == NULL)    // }가 없으면 잘못된 문법이므로
                        break;          // 반복을 종료
                }

                int stringLength = end - begin;    // 문자열의 실제 길이는 끝 위치 - 시작 위치

                // 문자열 길이 + NULL 공간만큼 메모리 할당
                buffer = (char*)malloc(stringLength + 1);
                // 할당한 메모리를 0으로 초기화
                memset(buffer, 0, stringLength + 1);

                // 문서에서 문자열을 버퍼에 저장
                // 문자열 시작 위치에서 문자열 길이만큼만 복사
                memcpy(buffer, begin, stringLength);

                // 토큰 종류는 숫자
                json->tokens[tokenIndex].type = JSON_TOKEN_NUMBER;
                // 문자열을 숫자로 변환하여 토큰에 저장
                json->tokens[tokenIndex].number = atof(buffer);

                free(buffer);    // 버퍼 해제

                tokenIndex++;    // 토큰 인덱스 증가

                pos = pos + stringLength + 1;    // 현재 위치 + 문자열 길이 + , 또는 }(+ 1)
            }
            break;
            }

            pos++; // 다음 문자로
        }
    }


    inline void freeJSON(JSON* json)    // JSON 해제 함수
    {
        for (int i = 0; i < TOKEN_COUNT; i++)            // 토큰 개수만큼 반복
        {
            if (json->tokens[i].type == JSON_TOKEN_STRING)    // 토큰 종류가 문자열이면
                free(json->tokens[i].string);            // 동적 메모리 해제
        }
    }


    inline char* getString(JSON* json, char* key)    // 키에 해당하는 문자열을 가져오는 함수
    {
        for (int i = 0; i < TOKEN_COUNT; i++)    // 토큰 개수만큼 반복
        {
            // 토큰 종류가 문자열이면서 토큰의 문자열이 키와 일치하면
            if (json->tokens[i].type == JSON_TOKEN_STRING &&
                strcmp(json->tokens[i].string, key) == 0)
            {
                // 바로 뒤의 토큰(i + 1)이 문자열이면
                if (json->tokens[i + 1].type == JSON_TOKEN_STRING)
                    return json->tokens[i + 1].string;    // 바로 뒤에 있는 토큰의 문자열 반환
            }
        }

        return NULL;    // 키를 찾지 못했으면 NULL을 반환
    }

    // 키에 해당하는 배열 중 인덱스를 지정하여 문자열을 가져오는 함수
    inline char* getArrayString(JSON* json, char* key, int index)
    {
        for (int i = 0; i < TOKEN_COUNT; i++)    // 토큰 개수만큼 반복
        {
            // 토큰 종류가 문자열이면서 토큰의 문자열이 키와 일치한다면
            if (json->tokens[i].type == JSON_TOKEN_STRING &&
                strcmp(json->tokens[i].string, key) == 0)
            {
                // 바로 뒤의 토큰(i + 1)부터 배열의 요소
                // 인덱스를 지정한 토큰이 문자열이면서 배열이면
                if (json->tokens[i + 1 + index].type == JSON_TOKEN_STRING &&
                    json->tokens[i + 1 + index].isArray == true)
                    return json->tokens[i + 1 + index].string;    // 해당 토큰의 문자열 반환
            }
        }

        return NULL;    // 키를 찾지 못했으면 NULL을 반환
    }

    inline int getArrayCount(JSON* json, char* key) // 키에 해당하는 배열의 요소 개수를 구하는 함수
    {
        for (int i = 0; i < TOKEN_COUNT; i++)    // 토큰 개수만큼 반복
        {
            // 토큰 종류가 문자열이면서 토큰의 문자열이 키와 일치한다면
            if (json->tokens[i].type == JSON_TOKEN_STRING &&
                strcmp(json->tokens[i].string, key) == 0)
            {
                // 바로 뒤의 토큰(i + 1)부터 isArray가 true인 토큰의 개수를 세어서 반환
                int j = 0;
                while (json->tokens[i + 1 + j].isArray == true)
                    j++;

                return j;
            }
        }

        return 0;    // 키를 찾지 못했으면 0을 반환
    }

    inline double getNumber(JSON* json, char* key)    // 키에 해당하는 숫자를 가져오는 함수
    {
        for (int i = 0; i < TOKEN_COUNT; i++)    // 토큰 개수만큼 반복
        {
            // 토큰 종류가 숫자이면서 토큰의 문자열이 키와 일치한다면
            if (json->tokens[i].type == JSON_TOKEN_STRING &&
                strcmp(json->tokens[i].string, key) == 0)
            {
                // 바로 뒤의 토큰(i + 1)이 숫자이면
                if (json->tokens[i + 1].type == JSON_TOKEN_NUMBER)
                    return json->tokens[i + 1].number;    // 바로 뒤에 있는 토큰의 숫자 반환
            }
        }

        return 0.0;    // 키를 찾지 못했으면 0.0을 반환
    }


};

/*
JSON 문서를 만드는 방법은 간단합니다.
JSON 문법에 맞춰서 fprintf 함수로 파일에 출력만 해주면 됩니다.
여기서 주의할 점은 " " (큰따옴표)안에 "를 사용하려면 \"와 같이 앞에 \를 붙여주어야 한다는 점입니다.

작성한 JSON 파서는 다음과 같은 한계를 가지고 있습니다.
고정된 토큰 개수
불 값(true, false) 파싱이 구현되지 않음
숫자, 불 배열 파싱이 구현되지 않음
처음부터 배열로 시작하는 문서를 지원하지 않음
객체 안에 객체 또는 배열 들어가는 문법을 지원하지 않음
배열 안에 객체 또는 배열이 들어가는 문법을 지원하지 않음
키의 값을 가져올 때 매번 모든 토큰을 검사하는 비효율적인 구조
*/





/*
 * 작성자 : 이길훈
 * 작성일 : 21.10.06 
 * 내  용 : UTF-8 JSOn 파일 입출력 
 * 
 * 사  용 :
   1. 클래스 생성 (토큰 카운터)
   2. 키값 또는 index를 통해 값을 얻어오는 함수 이용 
    ┌───────────────────┐
    │    Name Block     │
    └───────────────────┘

    아래 형태의 pair 값을 가지고 있음 n은 생성index값이다
    JSON key[index](n),  value[index + 1](n+1)  

 */

#define FILEJSON_BUFF_MAX  256
class CFileJSON
{
public:
    struct JSON_TOKEN_t   {
        JSON_TOKEN_TYPE type = JSON_TOKEN_NONE;       // 토큰 종류
        union {                     // 두 종류 중 한 종류만 저장할 것이므로 공용체로 만듦
            BOOL   yesorno ;        // boolian 타입
            LPSTR  string ;         // 문자열 포인터
            double number = 0;      // 실수형 숫자
        };
        bool isArray = false;       // 현재 토큰이 배열인지 표시
    } ;

public:
    CHAR       m_Filename[FILEJSON_BUFF_MAX];
    CHAR       m_Path[FILEJSON_BUFF_MAX];
    CHAR       m_DirFile[FILEJSON_BUFF_MAX];
    std::map<int, CFileJSON::JSON_TOKEN_t*> m_Json;
    int        m_TonkenCnt;
public:

public:
    CFileJSON(int token_cnt) {
        init();
        for (int i = 0; i < token_cnt; i++)
        {
            m_Json[i] = /*(void*)*/new JSON_TOKEN_t;
        }
        
     }
    CFileJSON(int token_cnt, LPCSTR filename, LPCSTR dir = "") {
        init();      
        for (int i = 0; i < token_cnt; i++)
        {
            m_Json[i] = /*(void*)*/new JSON_TOKEN_t;
        }
        
        strcpy_s(m_Filename, 256, filename);
        strcpy_s(m_Path, 256, dir);
        if (strlen(dir) > 1)
            sprintf_s(m_DirFile, 256, "%s/%s", m_Path, m_Filename);
        else
            strcpy_s(m_DirFile, 256, filename);

        readJSON(&m_TonkenCnt);



    }
    virtual ~CFileJSON() {
        JSON_TOKEN_t* pJson;
        //deleteJson();
        for (int i = 0; i < (int)m_Json.size(); i++)     {       
            pJson = m_Json.find(i)->second;
            if (pJson != nullptr)    {
                if (pJson->type == JSON_TOKEN_STRING)
                    delete[]pJson->string;// 동적 메모리 해제
                delete[]pJson;// 동적 메모리 해제
                pJson = nullptr;
            }
        }
        
        init();
    }
protected:
    inline void init() {
        m_TonkenCnt = 0;
        memset(m_Filename,  0x00, FILEJSON_BUFF_MAX);
        memset(m_Path,      0x00, FILEJSON_BUFF_MAX);
        memset(m_DirFile,   0x00, FILEJSON_BUFF_MAX);
    }

    inline LPSTR readJSON(LPCSTR filename, int* readSize)    // 파일을 읽어서 내용을 반환하는 함수
    {
        FILE* fp;
        errno_t err = fopen_s(&fp, filename, "rb");

        if (fp == NULL)
            return NULL;

        int size;
        LPSTR buffer;

        // 파일 크기 구하기
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        // 파일 크기 + NULL 공간만큼 메모리를 할당하고 0으로 초기화
        buffer = new CHAR[size * sizeof(CHAR) + 1];
        memset(buffer, 0x00, size * sizeof(CHAR) +1);

        // 파일 내용 읽기
        if (fread(buffer, size , 1, fp) < 1)
        {
            *readSize = 0;
            delete[] buffer;
            buffer = NULL;
            fclose(fp);
            return NULL;
        }

        // 파일 크기를 넘겨줌
        *readSize = size;

        fclose(fp);    // 파일 포인터 닫기

        return buffer;
    }

    inline void parseJSON(LPSTR doc, int size)
    {
        int tokenIndex = 0;    // 토큰 인덱스
        int pos = 0;           // 문자 검색 위치를 저장하는 변수
        JSON_TOKEN_t* pJson;

        if (doc[pos] != '{')   // 문서의 시작이 {인지 검사
            return;

        pos++;    // 다음 문자로

        while (pos < size)       // 문서 크기만큼 반복
        {
            switch (doc[pos])    // 문자의 종류에 따라 분기
            {
            case '"':            // 문자가 "이면 문자열
            {
                // 문자열의 시작 위치를 구함. 맨 앞의 "를 제외하기 위해 + 1
                LPSTR begin = doc + pos + 1;

                // 문자열의 끝 위치를 구함. 다음 "의 위치
                LPSTR end = strchr(begin, '"');
                if (end == NULL)    // "가 없으면 잘못된 문법이므로 
                    break;          // 반복을 종료

                int stringLength = end - begin;    // 문자열의 실제 길이는 끝 위치 -시작 위치

                // 토큰 배열에 문자열 저장
                // 토큰 종류는 문자열

                auto it = m_Json.find(tokenIndex);
                pJson = it->second;

                pJson->type = JSON_TOKEN_STRING;
                // 문자열 길이 + NULL 공간만큼 메모리 할당
                
                pJson->string = new CHAR[stringLength * sizeof(CHAR) + 1];
                // 할당한 메모리를 0으로 초기화
                memset(pJson->string, 0, stringLength * sizeof(CHAR) + 1);

                // 문서에서 문자열을 토큰에 저장
                // 문자열 시작 위치에서 문자열 길이만큼만 복사
                memcpy(pJson->string, begin, stringLength);

                tokenIndex++; // 토큰 인덱스 증가

                pos = pos + stringLength + 1;    // 현재 위치 + 문자열 길이 + "(+ 1)
            }
            break;
            case '[':        // 문자가 [이면 배열
            {
                pos++;       // 다음 문자로

                while (doc[pos] != ']')    // 닫는 ]가 나오면 반복 종료
                {
                    // 여기서는 문자열 배열만 처리
                    if (doc[pos] == '"')   // 문자가 "이면 문자열
                    {
                        // 문자열의 시작 위치를 구함. 맨 앞의 "를 제외하기 위해 + 1
                        char* begin = doc + pos + 1;

                        // 문자열의 끝 위치를 구함. 다음 "의 위치
                        char* end = strchr(begin, '"');
                        if (end == NULL)   // "가 없으면 잘못된 문법이므로
                            break;         // 반복을 종료

                        int stringLength = end - begin;    // 문자열의 실제 길이는 끝위치 - 시작 위치

                        // 토큰 배열에 문자열 저장
                        // 토큰 종류는 문자열
                        auto it = m_Json.find(tokenIndex);
                        pJson = it->second;

                        pJson->type = JSON_TOKEN_STRING;
                        // 문자열 길이 + NULL 공간만큼 메모리 할당
                        pJson->string = new CHAR[stringLength * sizeof(CHAR) + 1];
                        // 현재 문자열은 배열의 요소
                        pJson->isArray = true;
                        // 할당한 메모리를 0으로 초기화
                        memset(pJson->string, 0, stringLength * sizeof(CHAR) + 1);

                        // 문서에서 문자열을 토큰에 저장
                        // 문자열 시작 위치에서 문자열 길이만큼만 복사
                        memcpy(pJson->string, begin, stringLength);

                        tokenIndex++; // 토큰 인덱스 증가

                        pos = pos + stringLength + 1; // 현재 위치 + 문자열 길이 + "( 1)
                    }

                    pos++;    // 다음 문자로
                }
            }
            break;
            case '0': case '1': case '2': case '3': case '4': case '5':    // 문자가숫자이면
            case '6': case '7': case '8': case '9': case '-':              // -는 수일 때
            {
                // 문자열의 시작 위치를 구함
                LPSTR begin = doc + pos;
                LPSTR end;
                LPSTR buffer;

                // 문자열의 끝 위치를 구함. ,가 나오거나
                end = strchr(doc + pos, ',');
                if (end == NULL)
                {
                    // } 가 나오면 문자열이 끝남
                    end = strchr(doc + pos, '}');
                    if (end == NULL)    // }가 없으면 잘못된 문법이므로
                        break;          // 반복을 종료
                }

                int stringLength = end - begin;    // 문자열의 실제 길이는 끝 위치 -시작 위치

                // 문자열 길이 + NULL 공간만큼 메모리 할당
                buffer = new CHAR[stringLength * sizeof(CHAR) + 1];
                // 할당한 메모리를 0으로 초기화
                memset(buffer, 0, stringLength * sizeof(CHAR) + 1);

                // 문서에서 문자열을 버퍼에 저장
                // 문자열 시작 위치에서 문자열 길이만큼만 복사
                memcpy(buffer, begin, stringLength);


                auto it = m_Json.find(tokenIndex);
                pJson = it->second;

                // 토큰 종류는 숫자
                pJson->type = JSON_TOKEN_NUMBER;
                // 문자열을 숫자로 변환하여 토큰에 저장
                pJson->number = atof(buffer);

                delete[]buffer;    // 버퍼 해제

                tokenIndex++;    // 토큰 인덱스 증가

                pos = pos + stringLength + 1;    // 현재 위치 + 문자열 길이 + , 또는 (+ 1)
            }
            break;
            }

            pos++; // 다음 문자로
        }
    }
    

    inline int readJSON(int* readSize) {
        LPSTR pf = readJSON((LPCSTR)m_DirFile, readSize);
        parseJSON(pf, *readSize);
        deleteObj(pf);

        return 0;
    }

    inline void deleteJson() {
        for (int i = 0; i < m_TonkenCnt; i++)            // 토큰 개수만큼 반복
        {            
            if (m_Json.find(i)->second->type == JSON_TOKEN_STRING)    // 토큰 종류가 문자열이면
                delete[]m_Json.find(i)->second->string;  // 동적 메모리 해제
        }
    }

    inline void deleteObj(LPSTR object)    // JSON 해제 함수
    {
        if (object != nullptr)  {
            delete[] object;
            object = nullptr;
        }
    }

public:
    /// <summary>
    /// map 데이터 생성 index번호를 이용하여 문자열 주소를 받는다
    /// </summary>
    inline LPSTR GetString(int make_index)   {
        JSON_TOKEN_t* pJson;
        auto it = m_Json.find(make_index);
        pJson = it->second;
        // 토큰 종류가 문자열이면서 토큰의 문자열이 키와 일치하면
        if (pJson->type == JSON_TOKEN_STRING )   // 바로 뒤에 있는 토큰의 문자열 반환             
                return pJson->string;

        return NULL;    // 키를 찾지 못했으면 NULL을 반환
    }

    /// <summary>
    /// map 데이터의 Key문자열을 검색 후 value값 문자열이면 주소 반환
    /// </summary>
    /// <param name="key">키 문자열</param>
    /// <returns>문자열이면 주소</returns>
    inline LPSTR GetString(LPCSTR key)     {
        JSON_TOKEN_t* pJson; 
        for (int i = 0; i < m_TonkenCnt; i++) {   // 토큰 개수만큼 반복
            auto it = m_Json.find(i);
            pJson = it->second;

            // 토큰 종류가 문자열이면서 토큰의 문자열이 키와 일치하면
            if (pJson->type == JSON_TOKEN_STRING &&
                strcmp(pJson->string, key) == 0)
            {
                // 바로 뒤의 토큰(i + 1)이 문자열이면
                it = m_Json.find(i+1);
                pJson = it->second;
                if (pJson->type == JSON_TOKEN_STRING) 
                    return (LPSTR)pJson->string; // 바로 뒤에 있는 토큰의 문자열 반환                
            }
        }

        return NULL;    // 키를 찾지 못했으면 NULL을 반환
    }


    // 키에 해당하는 배열 중 인덱스를 지정하여 문자열을 가져오는 함수
    inline LPSTR GetArrayString(LPCSTR key, int index)   {
        JSON_TOKEN_t* pJson;
        for (int i = 0; i < m_TonkenCnt; i++)    // 토큰 개수만큼 반복
        {
            auto it = m_Json.find(i);
            pJson = it->second;
            // 토큰 종류가 문자열이면서 토큰의 문자열이 키와 일치한다면
            if (pJson->type == JSON_TOKEN_STRING &&
                strcmp(pJson->string, key) == 0)
            {
                it = m_Json.find(i + 1 + index);
                pJson = it->second;
                // 바로 뒤의 토큰(i + 1)부터 배열의 요소
                // 인덱스를 지정한 토큰이 문자열이면서 배열이면
                if (pJson->type == JSON_TOKEN_STRING &&
                    pJson->isArray == true)
                    return pJson->string;    // 해당 토큰의 문자열 반환
            }
        }

        return NULL;    // 키를 찾지 못했으면 NULL을 반환
    }

    inline int GetArrayCount(LPCSTR key) // 키에 해당하는 배열의 요소 개수를 구하는 함수
    {
        JSON_TOKEN_t* pJson;
        for (int i = 0; i < m_TonkenCnt; i++)    // 토큰 개수만큼 반복
        {
            auto it = m_Json.find(i);
            pJson = it->second;
            // 토큰 종류가 문자열이면서 토큰의 문자열이 키와 일치한다면
            if (pJson->type == JSON_TOKEN_STRING &&
                strcmp(pJson->string, key) == 0)
            {
                // 바로 뒤의 토큰(i + 1)부터 isArray가 true인 토큰의 개수를 세어서 반환
                int j = 0;                
                while (m_Json.find(i + 1 + j)->second->isArray == true)
                    j++;

                return j;
            }
        }

        return 0;    // 키를 찾지 못했으면 0을 반환
    }

    /// <summary>
    /// map 데이터 생성 index번호를 이용하여 숫자값 받는다
    /// </summary>
    /// <param name="make_index">생성 index</param>
    /// <returns>number</returns>
    inline double GetNumber(int make_index)   {
        if (m_Json.find(make_index)->second->type == JSON_TOKEN_NUMBER)
            return m_Json.find(make_index)->second->number;

        return 0.0;
    }

    /// <summary>
    /// 키에 해당하는 숫자를 가져오는 함수
    /// </summary>
    /// <param name="key">키 문자열</param>
    /// <returns>숫자</returns>
    inline double GetNumber(LPCSTR key)    
    {
        JSON_TOKEN_t* pJson;
        for (int i = 0; i < m_TonkenCnt; i++)    // 토큰 개수만큼 반복
        {
            pJson = m_Json.find(i)->second;
            // 토큰 종류가 숫자이면서 토큰의 문자열이 키와 일치한다면
            if (pJson->type == JSON_TOKEN_STRING &&
                strcmp(pJson->string, key) == 0)
            {
                // 바로 뒤의 토큰(i + 1)이 숫자이면
                if (m_Json.find(i+1)->second->type == JSON_TOKEN_NUMBER)
                    return m_Json.find(i + 1)->second->number;    // 바로 뒤에 있는 토큰의 숫자 반환
            }
        }

        return 0.0;    // 키를 찾지 못했으면 0.0을 반환
    }

};
```
