#include <iostream>
#include <cstring>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Global list of connected clients
std::vector<int> clients;
std::mutex clients_mutex;

// Broadcast a message to all connected clients
void broadcast_message(const std::string& message, int sender_fd) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    
    for (int client_fd : clients) {
        if (client_fd != sender_fd) { // Don't send back to sender
            send(client_fd, message.c_str(), message.length(), 0);
        }
    }
}

// Handle a single client connection
void handle_client(int client_fd) {
    std::cout << "Client connected (fd: " << client_fd << ")\n";
    
    // Add client to the list
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.push_back(client_fd);
    }
    
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
        
        if (bytes_read <= 0) {
            std::cout << "Client disconnected (fd: " << client_fd << ")\n";
            break;
        }
        
        std::string message(buffer);
        std::cout << "Received from client " << client_fd << ": " << message;
        
        // Broadcast to all other clients
        broadcast_message(message, client_fd);
    }
    
    // Remove client from the list
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(
            std::remove(clients.begin(), clients.end(), client_fd),
            clients.end()
        );
    }
    
    close(client_fd);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9092);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        std::cerr << "Listen failed\n";
        close(server_fd);
        return 1;
    }

    std::cout << "Broker listening on port 9092...\n";

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        
        if (client_fd < 0) {
            std::cerr << "Accept failed\n";
            continue;
        }

        // Spawn a new thread to handle this client
        std::thread(handle_client, client_fd).detach();
    }

    close(server_fd);
    return 0;
}