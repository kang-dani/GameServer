#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>

int main()
{
	WORD wVersionRequested;									//���� ���� ��û�� ���� ����
	WSADATA wsaData;										//���� ���� ������ ���� ����ü

	//WSAStartup�� ���� ������ �ʱ�ȭ�ϴ� �Լ�
	wVersionRequested = MAKEWORD(2, 2);

	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		printf("WSAStartup failed with error\n");
		return 1;
	}

	//������ �����ϴ� �Լ�
	SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (connectSocket == INVALID_SOCKET)
	{
		printf("Socket creation failed with error : %d\n", WSAGetLastError());
	}
	else
	{
		printf("Socket created successfully\n");
	}

	//������ �ּ�
	SOCKADDR_IN serverAddr;									//���� �ּҸ� ���� ����ü
	memset(&serverAddr, 0, sizeof(serverAddr));				//����ü�� 0���� �ʱ�ȭ
	serverAddr.sin_family = AF_INET;						//�ּ� ü�� ����
	//serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");	//���� �ּ� ����
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);	//���� �ּ� ����
	serverAddr.sin_port = htons(27015);						//��Ʈ ���� (27015) : ���� ������ Ŭ���̾�Ʈ�� ����ϴ� ��Ʈ


	//������ ���� (������ ���� ����)
	//connect(����, ���� �ּҸ� ���� ����ü, ������ ũ��)
	if (connect(connectSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) //���Ͽ� �ּҸ� ���ε��ϴ� �Լ�
	{
		printf("Connect failed with error : %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return -1;
	}
	printf("Connect successful\n");

	while (true)
	{
		send(connectSocket, "Hello", 5, 0);	//������ �޽����� ������ �Լ�
		Sleep(100000);	//1�� ���
	}

	closesocket(connectSocket);	//������ �ݴ� �Լ�
	WSACleanup();
}