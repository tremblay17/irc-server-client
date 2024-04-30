#include "client.h"
#include <crypt.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <utility>



class MessageService : public Client {
private:
    std::string sharedSecret;

public:
    MessageService(const std::string& serverIP, int serverPort)
        : Client(serverIP, serverPort), sharedSecret("") {}
    ~MessageService() { close(clientSocket); }
    std::string sendMessage(std::string& message);
    std::string receiveMessage(std::string& message);

    //Don't need to implement functions below  
    void encryptMessage(std::string& message, RSA* privateKey);
    void decryptMessage(std::string& message);
    void sendPublicKey();
    void generateKeys();
    void keyExchange();
    void generateSharedSecret(RSA* publicKey, RSA* privateKey);
};

