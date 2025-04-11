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
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Socket creation failed with error : %d\n", WSAGetLastError());
	}
	else
	{
		printf("Socket created successfully\n");
	}

	SOCKADDR_IN serverAddr;									//���� �ּҸ� ���� ����ü
	memset(&serverAddr, 0, sizeof(serverAddr));				//����ü�� 0���� �ʱ�ȭ
	serverAddr.sin_family = AF_INET;						//�ּ� ü�� ����
	//serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");	//���� �ּ� ����
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);	//���� �ּ� ����
	serverAddr.sin_port = htons(27015);						//��Ʈ ���� (27015) : ���� ������ Ŭ���̾�Ʈ�� ����ϴ� ��Ʈ

	// bind(����, ����ü �ּ�, ������ ũ��)
	if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) //���Ͽ� �ּҸ� ���ε��ϴ� �Լ�
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

	//listen�� ���ε�� ���� & �� �� ������� ����
	if (listen(listenSocket, 5) == SOCKET_ERROR) //������ �����ϴ� �Լ�
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
		printf("Waiting for client...\n");	//Ŭ���̾�Ʈ ��� ��
		SOCKET acceptSocket = accept(listenSocket, NULL, NULL);			//Ŭ���̾�Ʈ ���� ����
		printf("Client connected\n");		//Ŭ���̾�Ʈ ���� ����

		if (acceptSocket == INVALID_SOCKET)
		{
			printf("Accept failed with error : %d\n", WSAGetLastError());

			continue;	//Ŭ���̾�Ʈ ���� ���� �� ��� ���
		}

		while (true)
		{
			char buffer[1024] = {};	//�޽����� ���� ����
			int received = recv(acceptSocket, buffer, 5, 0);	//Ŭ���̾�Ʈ�κ��� �޽����� �޴� �Լ�
			printf("Message received\n");	//�޽��� ���� ����
			printf("Message : %s\n", buffer);	//�޽��� ���
			Sleep(10000000);	//1�� ���
		}
		
	}

	closesocket(listenSocket);										//������� ������ �ݴ� �Լ�
	WSACleanup();											//WSAStartup���� �ʱ�ȭ�� ������ �����ϴ� �Լ�
}