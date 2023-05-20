#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

struct sock_sockaddr_in {
	SOCKET sock;
	sockaddr_in serv_addr;
};

sock_sockaddr_in* CreateSocket();
bool TryingToConnect(SOCKET sock, sockaddr_in serv_addr);
int CommunicateWithServer(SOCKET sock);
bool WaitingForPlayers(SOCKET sock);