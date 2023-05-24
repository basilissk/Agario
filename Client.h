#pragma once

#include "SFML/Network.hpp"
#include <iostream>

class Client
{
	int Id;
	sf::TcpSocket* socket;

public:
	Client(sf::TcpSocket &tcpSocket);
	int GetId();
	void SetId(int value);
	void DisconnectClient();
	sf::TcpSocket* GetSocket();
	sf::Packet GetClientInfo();
	sf::Packet GetClientPacket();
};

