#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>

int main()
{

	WORD wVersionRequested;									//소켓 버전 요청을 위한 변수
	WSADATA wsaData;										//소켓 버전 정보를 담을 구조체

	//WSAStartup은 소켓 버전을 초기화하는 함수
	wVersionRequested = MAKEWORD(2, 2);

	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		printf("WSAStartup failed with error\n");
		return 1;
	}

	//소켓을 생성하는 함수
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Socket creation failed with error : %d\n", WSAGetLastError());
	}
	else
	{
		printf("Socket created successfully\n");
	}

	SOCKADDR_IN serverAddr;									//서버 주소를 담을 구조체
	memset(&serverAddr, 0, sizeof(serverAddr));				//구조체를 0으로 초기화
	serverAddr.sin_family = AF_INET;						//주소 체계 설정
	//serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");	//서버 주소 설정
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);	//서버 주소 설정
	serverAddr.sin_port = htons(27015);						//포트 설정 (27015) : 게임 서버와 클라이언트가 통신하는 포트

	// bind(소켓, 구조체 주소, 정보의 크기)
	if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) //소켓에 주소를 바인딩하는 함수
	{
		printf("Bind failed with error : %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return -1;
	}
	else
	{
		printf("Bind successful\n");
	}

	//listen에 바인드된 소켓 & 몇 명 대기할지 설정
	if (listen(listenSocket, 5) == SOCKET_ERROR) //소켓을 리슨하는 함수
	{
		printf("Listen failed with error : %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return -1;
	}
	else
	{
		printf("Listening on port 27015\n");
	}

	while (true)
	{
		printf("Waiting for client...\n");	//클라이언트 대기 중
		SOCKET acceptSocket = accept(listenSocket, NULL, NULL);			//클라이언트 연결 수락
		printf("Client connected\n");		//클라이언트 연결 성공

		if (acceptSocket == INVALID_SOCKET)
		{
			printf("Accept failed with error : %d\n", WSAGetLastError());

			continue;	//클라이언트 연결 실패 시 계속 대기
		}

		while (true)
		{
			char buffer[1024] = {};	//메시지를 받을 버퍼
			int received = recv(acceptSocket, buffer, 5, 0);	//클라이언트로부터 메시지를 받는 함수
			printf("Message received\n");	//메시지 수신 성공
			printf("Message : %s\n", buffer);	//메시지 출력
			Sleep(10000000);	//1초 대기
		}
		
	}

	closesocket(listenSocket);										//만들어진 소켓을 닫는 함수
	WSACleanup();											//WSAStartup으로 초기화한 소켓을 정리하는 함수
}