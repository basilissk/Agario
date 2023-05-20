#pragma once

#include "SFML/Network.hpp"
#include <iostream>

class Client
{
	int Id;
	sf::TcpSocket* socket;

public:
	Client(sf::TcpSocket &tcpSocket);
	void trans();
	int GetId();
	void SetId(int value);
	sf::TcpSocket* GetSocket();
	sf::Packet getClientInfo();
	sf::Packet getClientPacket();
};

