#include "msg-service.h"
#include <iostream>

std::string MessageService::sendMessage(std::string& message) {
    // Encrypt the message using the shared secret
    //encryptMessage(message, privateKey);
    // Send the message no encryption
    if (send(clientSocket, message.c_str(), message.size(), 0) < 0) {
        std::cerr << "Failed to send message" << std::endl;
        return "Message failed to send";
    }
    std::cout << "Message sent" << std::endl;
    return message;
}

std::string MessageService::receiveMessage(std::string& message) {
    // Receive the message no encryption
    char buffer[1024];
    if (recv(clientSocket, buffer, 1024, 0) < 0) {
        std::cerr << "Failed to receive message" << std::endl;
        return "Message failed to receive";
    }
    message = buffer;
    std::cout << "Message received" << std::endl;
    return message;
}