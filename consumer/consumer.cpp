#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
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
    std::cout << "Waiting for messages... (Press Ctrl+C to exit)\n\n";

    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = read(sock, buffer, sizeof(buffer) - 1);
        
        if (bytes_read <= 0) {
            std::cout << "\nBroker disconnected\n";
            break;
        }
        
        std::cout << "📩 Received: " << buffer;
    }

    close(sock);
    return 0;
}