#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <chrono>

#define PORT 5001
#define BUFFER_SIZE 4096

void start_server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    
    // Create socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set up the server address struct
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind the socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Server listening on 0.0.0.0:" << PORT << std::endl;
    
    // Accept a client connection
    new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected to client." << std::endl;
    
    size_t total_received = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Receive data
    ssize_t bytes_received;
    while ((bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        total_received += bytes_received;
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    
    double throughput = total_received / elapsed.count();
    std::cout << "Received " << total_received << " bytes in " << elapsed.count() << " seconds." << std::endl;
    std::cout << "Throughput: " << (throughput / 1024) << " KB/s" << std::endl;
    
    // Send acknowledgment
    send(new_socket, "ACK", 3, 0);
    
    // Close sockets
    close(new_socket);
    close(server_fd);
}

int main() {
    start_server();
    return 0;
}
