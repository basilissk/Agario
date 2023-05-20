#include "Client.h"

Client::Client(sf::TcpSocket& tcpSocket)
{
    socket = &tcpSocket;
}

int Client::GetId()
{
	return Id;
}

void Client::SetId(int value)
{
	Id = value;
}

sf::TcpSocket* Client::GetSocket()
{
    return socket;
}

sf::Packet Client::getClientInfo()
{
	return sf::Packet();
}

sf::Packet Client::getClientPacket()
{
    sf::Packet packet;
    
    sf::Socket::Status status = socket->receive(packet);

    if (status == sf::Socket::Done)
    {
        return packet;
    }
    else if (status == sf::Socket::Error)
    {
        std::cout << "Ошибка при получении пакета." << std::endl;
    }
}
