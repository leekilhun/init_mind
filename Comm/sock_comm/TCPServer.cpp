// TCPServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "framework.h"
#include "TCPServer.h"

//#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 유일한 애플리케이션 개체입니다.

CWinApp theApp;

using namespace std;



void ErrQuit(int err) { //에러를 처리하는 함수
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
			_tsetlocale(LC_ALL, _T("")); //C++에서 io를 할때, 경로명이나 파일명에 한글이 포함되면 오작동의 위험이있다.
			//그래서 locale를 변경 해줘서 지역을 업데이트 해준다. 우리나라에서 하면 _tsetlocale(LC_ALL, _T("Korean"));효과가 있을것이다.
			AfxSocketInit(); //소켓 사용을 위해 관련된 것들을 초기화 시켜줌.
			CSocket sock; // 소켓변수선언
			if (!sock.Create(8000))
			{ //포트 8000의 socket을 생성한다. 
//실패하면 0을 반환해서 에러뜨게함.
				ErrQuit(sock.GetLastError()); // GetLastError()는 호출된 스레드의 마지막 오류 코드값 반환
			}
			if (!sock.Listen())
			{ // 클라이언트의 접속요청을 확인하는 상태로 변경
				ErrQuit(sock.GetLastError());
			}
			TCHAR buf[256 + 1];
			int nbytes;
			while (1)
			{ // 무한정 대기
				CSocket newsock;
				if (!sock.Accept(newsock))
				{ //클라이언트의 접속을 기다리다가(대기), 요청이오면 허락(승인)하고 
//그 클라이언트와의 통신을 위해 new socket 만듬
//실패하면 에러함수 호출
					ErrQuit(sock.GetLastError());
				}
				CString PeerAddress;
				UINT PeerPort;
				newsock.GetPeerName(PeerAddress, PeerPort); // 지금 연결된 클라이언트와 연결을 담당하는 newsock으로 부터,
				//클라이언트의 주소와 포트얻기.
				_tprintf(_T("### IP주소: %s, 포트 번호: %d ###\n"), (LPCTSTR)PeerAddress, PeerPort); // 정보출력
				while (1)
				{
					nbytes = newsock.Receive(buf, 256); // 현재 연결된 클라이언트와의 통신을 담당하는 newsock이 계속 정보를 받게한다.
					//받은정보는 buf에 256만큼 수신한다.
					//Receive()는 받은 데이터의 길이를 반환한다.
					//클라이언트쪽에서 아무것도 안보내도 0을 반환한다.
					if (nbytes == 0 || nbytes == SOCKET_ERROR)
					{ // 받았는데 길이가 0이거나 에러메세지면 그만 수신한다.
						break;
					}
					else
					{ // 1이상의 데이터를 받아왔으면
						buf[nbytes] = _T('\0'); // buf에서 nbyte번째에 해당하는곳에 \0을 붙여준다. char*문자열형에서는 끝에 \0를 붙여주는 것이 안전하다.
						_tprintf(_T("%s"), buf); //출력한다. Client쪽에서 ?번째 테스트 메세지 라고 메세지를 전송할 것이다
					}
				}
				newsock.Close(); //newsock이 클라이언트와 작업이 다 끝났으면 소켓종료
				_tprintf(_T("### 접속종료 ###\n\n"));

			}

		}
	}
	else
	{
		wprintf(L"심각한 오류: GetModuleHandle 실패\n");
		nRetCode = 1;
	}
	return nRetCode;
}

