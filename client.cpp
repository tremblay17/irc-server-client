#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Set up server address
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);  // Replace with the actual server port
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with the actual server IP

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to connect to the server" << std::endl;
        return 1;
    }

    // Handshake with the server
    std::string handshakeMessage = "";
    std::cin >> handshakeMessage;

    //send the handshake message
    if (send(clientSocket, handshakeMessage.c_str(), handshakeMessage.size(), 0) < 0) {
        std::cerr << "Failed to send handshake message" << std::endl;
        return 1;
    }
    //receive the server's handshake message
    char serverHandshakeMessage[4];
    if (recv(clientSocket, serverHandshakeMessage, sizeof(serverHandshakeMessage), 0) < 0) {
        std::cerr << "Failed to receive handshake message" << std::endl;
        return 1;
    }
    //remove the null terminator
    serverHandshakeMessage[4] = '\0';
    std::cout << "Server: " << serverHandshakeMessage << std::endl;
    //send the mailing info
    std::string sender = "";
    std::cout << "Enter your initials: ";
    std::cin >> sender;
    //std::cin.ignore();
    std::string receiver = "";
    std::cout << "Enter the receiver's initials: ";
    std::cin >> receiver;
    //std::cin.ignore();

    std::string mailingInfo = sender + " " + receiver;

    if (send(clientSocket, mailingInfo.c_str(), mailingInfo.size(), 0) < 0) {
        std::cerr << "Failed to send mailing info" << std::endl;
        return 1;
    }

    // Close the socket
    close(clientSocket);

    return 0;
}

void keyExchange() {
    // Generate a public key
    // Send the public key to the server
    // Generate a private key
    // Generate a shared secret using the server's public key and the private key
    // Encrypt the shared secret using the private key
    // Send the encrypted shared secret to the server
}

void encryptMessage() {
    // Encrypt the message using the shared secret
}

void sendMessage() {
    // Encrypt the message using the shared secret
    // Send the encrypted message to the server
}

void receiveMessage() {
    // Receive the encrypted message from the server
    // Decrypt the message using the shared secret
    // Print the decrypted message
}

void decryptMessage() {
    // Decrypt the message using the shared secret
}

void displayHelp() {
    std::cout << "Usage: messaging-service-client [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help\t\tDisplay this help message" << std::endl;
    std::cout << "  -m, --message\t\tSend a message to the server" << std::endl;
    std::cout << "  -r, --receive\t\tReceive a message from the server" << std::endl;
}

void getMessages() {
    // Receive messages from the server
    // Print the messages
}
