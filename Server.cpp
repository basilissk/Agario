#include "Server.h"
using namespace sf;
using namespace std;

TcpListener* listener = new TcpListener();

void Server::StartServer()
{       
        
        Socket::Status status = listener->listen(8888); // Прослушивание порта 8888

        if (status != Socket::Done) {
            cout << "Ошибка при запуске сервера." << std::endl;
            return;
        }

        cout << "Сервер запущен. Ожидание подключений..." << std::endl;
}

void Server::WaitingClients()
{
    while (clients.size() < 1) {
        // Ожидание и обработка подключений
        TcpSocket* tcp_client = new TcpSocket;
        int Id = 1;
        if (listener->accept(*tcp_client) != sf::Socket::Done) {
            std::cout << "Ошибка при принятии подключения." << std::endl;
            continue;
        }
        std::cout << "Подключение принято. IP адрес: " << tcp_client->getRemoteAddress() << std::endl;

         Client *client = new Client(*tcp_client);
         client->SetId(Id);
         Id++;
        // Добавление клиента в вектор clients
         clients.push_back(client);

        std::cout << "Клиент добавлен в список подключенных." << std::endl;
    }

}

void Server::SendPacketToAllClients(Packet packet)
{
    for (Client* client : clients) {
        sf::TcpSocket* temp = client->GetSocket();
        temp->send(packet);
    }
}

void Server::SendMessageW()
{
    for (Client *client : clients) {
        sf::TcpSocket *temp = client->GetSocket();
        Packet packet;
        packet << "hello";
        temp->send(packet);

    }
   
}
