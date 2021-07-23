// TCPClient.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "framework.h"
#include "TCPClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 유일한 애플리케이션 개체입니다.

CWinApp theApp;

using namespace std;

void ErrQuit(int err) { //에러를 처리하는 함수.
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL
    );
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, _T("오류발생"), MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}


int main()
{
    int nRetCode = 0;
    HMODULE hModule = ::GetModuleHandle(nullptr);
    if (hModule != nullptr)
    {
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            wprintf(L"심각한 오류: MFC를 초기화하지 못했습니다.\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: 응용 프로그램의 동작은 여기에서 코딩합니다.
            _tsetlocale(LC_ALL, _T(""));
            AfxSocketInit();
            CSocket sock;
            if (!sock.Create())
            { // 소켓하나 만든다.
                ErrQuit(sock.GetLastError());
            }
            if (!sock.Connect(_T("127.0.0.1"), 8000))
            { // 해당 IP주소에 8000포트로 연결을 시도한다.
                ErrQuit(sock.GetLastError());
            }
            TCHAR buf[256];
            int nbytes;
            for (int i = 0; i < 5; ++i)
            { // 총5개 메세지 보낼거임.
                wsprintf(buf, _T("%d번째 테스트 메세지\r\n"), i); // 보낼메세지를 출력도하고 buf에도 저장한다.
                nbytes = sock.Send(buf, 256); //sock을 통해서 256의 buf메세지를 보내고, 길이 반환.
                if (nbytes == SOCKET_ERROR)
                { // 에러메세지나오면 에러.
                    ErrQuit(sock.GetLastError());
                }
                else
                { //아니면 지금 단계와, 몇바이트보냈는지 출력
                    _tprintf(_T("<%d> %d바이트 전송\n"), i, nbytes);
                    Sleep(1000); //1초대기
                }
            }
            sock.Close(); //5번다했으면 scok닫기
        }
    }
    else
    {
        wprintf(L"심각한 오류: GetModuleHandle 실패\n");
        nRetCode = 1;
    }
    return nRetCode;

}
