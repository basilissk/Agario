#include "Client.h"

Client::Client(sf::TcpSocket& tcpSocket)
{
    socket = &tcpSocket;
    Id = 0;
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

void Client::DisconnectClient() {
    socket->disconnect();
}

sf::Packet Client::GetClientInfo()
{
	return sf::Packet();
}

sf::Packet Client::GetClientPacket()
{
    sf::Packet packet;

    sf::Socket::Status status = socket->receive(packet);

    if (status == sf::Socket::Done)
    {
        return packet;
    }
    else if (status == sf::Socket::NotReady)
    {
        std::cout << "Сокет не готов для получения данных." << std::endl;
    }
    else if (status == sf::Socket::Partial)
    {
        std::cout << "Получение данных было выполнено частично." << std::endl;
    }
    else if (status == sf::Socket::Disconnected)
    {
        std::cout << "Соединение разорвано." << std::endl;
        throw std::exception("Соединение разорвано."); // Выбросить исключение, чтобы указать обрыв соединения
    }
    else if (status == sf::Socket::Error)
    {
        std::cout << "Ошибка при получении данных." << std::endl;
        throw std::runtime_error("Ошибка при получении данных."); // Выбросить исключение, чтобы указать ошибку получения данных
    }

    return packet; // Вернуть пакет, если ни одно из условий не выполнилось
}


