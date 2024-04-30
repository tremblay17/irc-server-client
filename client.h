#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

//packet data structure
struct Packet {
    int packetType;
    char data[1024];
};

class Client {
public:
    int clientSocket;
    sockaddr_in serverAddress{};
    std::string serverIP;
    int serverPort;

    Client(const std::string& serverIP, int serverPort);
    ~Client();
    bool connectToServer();
    bool sendHandshake(const std::string& handshakeMessage);
    bool receiveHandshake(std::string& serverHandshakeMessage);
    bool sendMailingInfo(const std::string& sender, const std::string& receiver);
    bool sendMessage(const std::string& message);
    bool receiveMessage(std::string& message);
    void serializePacket(Packet& packet, const std::string& message);
};

Client::Client(const std::string& serverIP, int serverPort) : serverIP(serverIP), serverPort(serverPort) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());
}

Client::~Client() {
    close(clientSocket);
}

bool Client::connectToServer() {
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to connect to the server" << std::endl;
        return false;
    }
    return true;
}

bool Client::sendHandshake(const std::string& handshakeMessage) {
    if (send(clientSocket, handshakeMessage.c_str(), handshakeMessage.size(), 0) < 0) {
        std::cerr << "Failed to send handshake message" << std::endl;
        return false;
    }
    return true;
}

bool Client::receiveHandshake(std::string& serverHandshakeMessage) {
    char buffer[1024];
    if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0) {
        std::cerr << "Failed to receive handshake message" << std::endl;
        return false;
    }
    serverHandshakeMessage = buffer;
    return true;
}

bool Client::sendMailingInfo(const std::string& sender, const std::string& receiver) {
    std::string mailingInfo = sender + " " + receiver;
    if (send(clientSocket, mailingInfo.c_str(), mailingInfo.size(), 0) < 0) {
        std::cerr << "Failed to send mailing info" << std::endl;
        return false;
    }
    return true;
}

bool Client::sendMessage(const std::string& message) {
    if (send(clientSocket, message.c_str(), message.size(), 0) < 0) {
        std::cerr << "Failed to send message" << std::endl;
        return false;
    }
    return true;
}

bool Client::receiveMessage(std::string& message) {
    char buffer[1024];
    if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0) {
        std::cerr << "Failed to receive message" << std::endl;
        return false;
    }
    message = buffer;
    return true;
}

void Client::serializePacket(Packet& packet, const std::string& message) {
    packet.packetType = 0;
    strcpy(packet.data, message.c_str());
}
