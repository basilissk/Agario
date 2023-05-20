#include "Server.h"
using namespace sf;
using namespace std;

TcpListener* listener = new TcpListener();

void Server::StartServer()
{       
        
        Socket::Status status = listener->listen(8888); // ������������� ����� 8888

        if (status != Socket::Done) {
            cout << "������ ��� ������� �������." << std::endl;
            return;
        }

        cout << "������ �������. �������� �����������..." << std::endl;
}

void Server::WaitingClients()
{
    while (clients.size() < 1) {
        // �������� � ��������� �����������
        TcpSocket* tcp_client = new TcpSocket;
        int Id = 1;
        if (listener->accept(*tcp_client) != sf::Socket::Done) {
            std::cout << "������ ��� �������� �����������." << std::endl;
            continue;
        }
        std::cout << "����������� �������. IP �����: " << tcp_client->getRemoteAddress() << std::endl;

         Client *client = new Client(*tcp_client);
         client->SetId(Id);
         Id++;
        // ���������� ������� � ������ clients
         clients.push_back(client);

        std::cout << "������ �������� � ������ ������������." << std::endl;
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
