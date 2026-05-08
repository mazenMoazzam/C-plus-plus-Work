#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./producer_client <message>\n";
        std::cerr << "Example: ./producer_client \"hello world\"\n";
        return 1;
    }

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

    std::string message = argv[1];
    message += "\n";
    
    ssize_t bytes_sent = send(sock, message.c_str(), message.length(), 0);
    
    if (bytes_sent < 0) {
        std::cerr << "Failed to send message\n";
    } else {
        std::cout << "Sent: " << message;
    }

    close(sock);
    std::cout << "Disconnected\n";

    return 0;
}