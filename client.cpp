#include "msg-service.h"

int main() {
    //initialize the client
    //MessageService client("146.85.64.142", 65200);
    Client client("146.85.64.142", 65200);
    //connect to the server
    if (!client.connectToServer()) {
        return 1;
    }
    //handshake with the server
    std::string handshakeMessage = "";
    std::cin >> handshakeMessage;
    if (!client.sendHandshake(handshakeMessage)) {
        return 1;
    }
    std::string serverHandshakeMessage = "";
    if (!client.receiveHandshake(serverHandshakeMessage)) {
        return 1;
    }
    std::cout << "Server: " << serverHandshakeMessage << std::endl;
    //send the mailing info
    std::string sender = "";
    std::cout << "Enter your initials: ";
    std::cin >> sender;
    std::string receiver = "";
    std::cout << "Enter the receiver's initials: ";
    std::cin >> receiver;
    if (!client.sendMailingInfo(sender, receiver)) {
        throw std::runtime_error("Failed to send mailing info");
        return 1;
    }
    //send and receive messages
    std::string messageToSend = "";
    std::string messageReceived = "";
    while(messageToSend != "/exit") {

        std::cout << "Enter a message: ";
        std::cin >> messageToSend;
        if (!client.sendMessage(messageToSend)) {
            return 1;
        }
        if (!client.receiveMessage(messageReceived)) {
            return 1;
        }
        std::cout << "Server: " << messageReceived << std::endl;
    }

    return 0;
}
