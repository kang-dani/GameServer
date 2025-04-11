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
	SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (connectSocket == INVALID_SOCKET)
	{
		printf("Socket creation failed with error : %d\n", WSAGetLastError());
	}
	else
	{
		printf("Socket created successfully\n");
	}

	//서버의 주소
	SOCKADDR_IN serverAddr;									//서버 주소를 담을 구조체
	memset(&serverAddr, 0, sizeof(serverAddr));				//구조체를 0으로 초기화
	serverAddr.sin_family = AF_INET;						//주소 체계 설정
	//serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");	//서버 주소 설정
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);	//서버 주소 설정
	serverAddr.sin_port = htons(27015);						//포트 설정 (27015) : 게임 서버와 클라이언트가 통신하는 포트


	//서버에 접속 (접속할 서버 정보)
	//connect(소켓, 서버 주소를 담은 구조체, 정보의 크기)
	if (connect(connectSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) //소켓에 주소를 바인딩하는 함수
	{
		printf("Connect failed with error : %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return -1;
	}
	printf("Connect successful\n");

	while (true)
	{
		send(connectSocket, "Hello", 5, 0);	//서버에 메시지를 보내는 함수
		Sleep(100000);	//1초 대기
	}

	closesocket(connectSocket);	//소켓을 닫는 함수
	WSACleanup();
}