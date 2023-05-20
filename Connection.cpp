#include "Connection.h"
#include <thread>


#pragma comment(lib, "ws2_32.lib")

using namespace std;

sock_sockaddr_in* CreateSocket() {
	sock_sockaddr_in* result = new sock_sockaddr_in;
	WSADATA wsaData;
	WORD DLLVersion = MAKEWORD(2, 2);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cerr << "Error" << endl;
		exit(NULL);
	};

	int sock = 0;
	struct sockaddr_in serv_addr;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cerr << "Failed to create socket\n";
		return NULL;
	}

	// установка параметров сервера
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8888);

	// преобразование IP адреса из текстового в бинарный формат
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		cerr << "Invalid address\n";
		return NULL;
	}
	result->sock = sock;
	result->serv_addr = serv_addr;

	return result;
}

bool TryingToConnect(SOCKET sock, sockaddr_in serv_addr) {

	Sleep(500);

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		cerr << "Failed to connect\n";
		return false;
	}
	else {
		cout << "Connected to server\n";
		return true;
	}
}
int CommunicateWithServer(SOCKET sock) {

	while (true) {
		// отправка случайного числа на сервер
		char buffer[512] = { 0 };
		int numberToSend = rand() % 1000;
		char bufferSend[sizeof(int)];
		memcpy(bufferSend, &numberToSend, sizeof(numberToSend));
		send(sock, bufferSend, sizeof(bufferSend), 0);
		cout << "Sent: " << bufferSend << endl;
		printf("Sent: %d ", *(int*)bufferSend);

		// прием сообщения от сервера
		int valread = recv(sock, buffer, 512, 0);

		int receivedInt = *(int*)buffer;
		cout << "Received: " << receivedInt << endl;

		Sleep(500);
	}

	return 0;
}

bool WaitingForPlayers(SOCKET sock) {

	/*char buffer[512] = { 0 };

	thread waitingPlayers([&]()
		{
			int valread = recv(sock, buffer, 512, 0);
		});

	Sleep(10);
	waitingPlayers.detach();

	int receivedInt = *(int*)buffer;

	cout << "Amount of connected players: " << receivedInt << endl;

	if (receivedInt == 2) {
		cout << "Complete!" << endl;
		return true;
	}
	return false;*/

	char buffer[512] = { 0 };
	fd_set readfds;
	struct timeval timeout;
	timeout.tv_sec = 0.1;  
	timeout.tv_usec = 0;

	FD_ZERO(&readfds);
	FD_SET(sock, &readfds);

	int activity = select(sock + 1, &readfds, NULL, NULL, &timeout);
	if (activity == -1) {
		cout << "Select error" << endl;
		return false;
	}
	else if (activity == 0) {
		cout << "Timeout occurred" << endl;
		return false;
	}
	else {
		int valread = recv(sock, buffer, 512, 0);
		int receivedInt = *(int*)buffer;

		cout << "Amount of connected players: " << receivedInt << endl;

		if (receivedInt == 2) {
			cout << "Complete!" << endl;
			return true;
		}
		return false;
	}

}