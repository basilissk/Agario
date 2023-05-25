#include "Server.h"
using namespace sf;
using namespace std;

void Server::StartServer()
{
	//Socket::Status status = listener->listen(8888,"192.168.111.70"); 
	sf::TcpListener* listenerTemp = new TcpListener;
	listener = listenerTemp;

	Socket::Status status = listener->listen(8888);

	if (status != Socket::Done) {
		cout << "Ошибка при запуске сервера." << std::endl;
		return;
	}

	cout << "Сервер запущен. Ожидание подключений..." << std::endl;
}

void Server::WaitingClients()
{
	int Id = 0;
		while (clients.size() < 2) {
			TcpSocket* tcp_client = new TcpSocket;
			if (listener->accept(*tcp_client) != sf::Socket::Done) {
				std::cout << "Ошибка при принятии подключения." << std::endl;
				continue;
			}
			std::cout << "Подключение принято. IP адрес: " << tcp_client->getRemoteAddress() << std::endl;

			Client* client = new Client(*tcp_client);
			client->SetId(Id);
			Id++;
			clients.push_back(client);

			std::cout << "Клиент добавлен в список подключенных." << std::endl;
		}

}

int Server::GetAmountOfClients() {
	int amount;
	amount = clients.size();
	return amount;
}

void Server::SendPacketToAllClients(Packet packet)
{
	for (Client* client : clients) {
		try {
			sf::TcpSocket* temp = client->GetSocket();
			temp->send(packet);
		}
		catch (exception ex) {
			RemoveClient(client->GetId());
		}
	}
}

void Server::SendPacketToClient(sf::Packet packet, Client* client)
{
	try {
		sf::TcpSocket* temp = client->GetSocket();
		temp->send(packet);
	}
	catch (exception ex) {
		RemoveClient(client->GetId());
	}
}

void Server::RemoveClient(int id)
{
	auto it = std::find_if(clients.begin(), clients.end(), [&](Client* client) {
		return client->GetId() == id;
		});

	if (it != clients.end()) {
		(*it)->DisconnectClient();
		delete* it;
		*it = nullptr;
		clients.erase(it);
	}
}

void Server::ShutsownServer()
{
	for (Client* client : clients) {
		client->DisconnectClient();
	}
	clients.clear();
	listener->close();
	delete listener;

}

