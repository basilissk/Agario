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
	void SendPacketToClient(sf::Packet packet, Client* client);
	void RemoveClient(int id);
	void ShutsownServer();
	std::vector<Client*> clients;
	sf::TcpListener* listener;
};

