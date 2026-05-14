#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./consumerClient <topic>\n";
        std::cerr << "Example: ./consumerClient payments\n";
        std::cerr << "Example: ./consumerClient logs\n";
        return 1;
    }

    std::string topic = argv[1];

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9092);
    
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        close(sock);
        return 1;
    }

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed. Is the broker running?\n";
        close(sock);
        return 1;
    }

    std::cout << "Connected to broker!\n";
    
    // Send subscription request
    std::string subscribe_msg = "SUBSCRIBE:" + topic + "\n";
    send(sock, subscribe_msg.c_str(), subscribe_msg.length(), 0);
    
    std::cout << "📥 Subscribed to topic: '" << topic << "'\n";
    std::cout << "Waiting for messages... (Press Ctrl+C to exit)\n\n";

    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = read(sock, buffer, sizeof(buffer) - 1);
        
        if (bytes_read <= 0) {
            std::cout << "\nBroker disconnected\n";
            break;
        }
        
        std::cout << "📩 " << buffer;
    }

    close(sock);
    return 0;
}