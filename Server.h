#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include "Client.h"

class Server
{
	sf::TcpListener* listener;
public:
	void StartServer();
	void WaitingClients();
	int GetAmountOfClients();
	void SendPacketToAllClients(sf::Packet packet);
	void SendPacketToClient(sf::Packet packet, Client* client);
	void RemoveClient(int id);
	void ShutsownServer();
	std::vector<Client*> clients;
};

