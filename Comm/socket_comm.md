## 소캣통신 
![캡처](/img/socket_comm1.JPG)

| api | 기능 | 비고 |
|---|:---:|---:|
| Socket() | 소켓 생성 | 1.stream, 2.DataGram, 3.Raw, Sequenced Packet Service |
| Bind() | Server, 소켓을 listen port에 대응 |  |
| Listen() | 접속 준비 상태 | stream socket에서 필요 |
| Accept() | 접속 수락 | 연결 시 신규 소켓이 생성됨, accept시 ip주소 반환 |
| Connet() | client의 소켓서버에 연결 |  |
| Close() | 소켓 닫기 |  |
| Send() | 데이터 송신 |  |
| Receive() | 데이터 수신 |  |
| SetSocket() | 소켓 옵션 결정  |  |

