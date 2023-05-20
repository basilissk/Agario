#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include "Client.h"

class Server
{
public:
	void StartServer();
	void WaitingClients();
	void SendPacketToAllClients(sf::Packet packet);
	void SendMessageW();
	std::vector<Client*> clients;
};

