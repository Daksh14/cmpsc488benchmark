#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>

#define SERVER_IP "127.0.0.1"
#define PORT 5001
#define MESSAGE_SIZE (1024 * 1024) // 1MB per message
#define NUM_MESSAGES 100 // Total of 100 messages

void run_client() {
    int client_socket;
    struct sockaddr_in server_addr;
    char *data = new char[MESSAGE_SIZE];
    memset(data, 'a', MESSAGE_SIZE);
    
    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }
    
    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected to server at " << SERVER_IP << ":" << PORT << std::endl;
    
    size_t total_bytes = MESSAGE_SIZE * NUM_MESSAGES;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Send data in a loop
    for (int i = 0; i < NUM_MESSAGES; ++i) {
        if (send(client_socket, data, MESSAGE_SIZE, 0) < 0) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
        std::cout << "Sent message " << (i + 1) << "/" << NUM_MESSAGES << std::endl;
    }
    
    // Shutdown sending side
    shutdown(client_socket, SHUT_WR);
    
    // Receive acknowledgment
    char ack[1024];
    recv(client_socket, ack, sizeof(ack), 0);
    std::cout << "Received acknowledgment: " << ack << std::endl;
    
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    double throughput = total_bytes / elapsed.count();
    
    std::cout << "Sent " << total_bytes << " bytes in " << elapsed.count() << " seconds." << std::endl;
    std::cout << "Throughput: " << (throughput / 1024) << " KB/s" << std::endl;
    
    // Clean up
    delete[] data;
    close(client_socket);
}

int main() {
    run_client();
    return 0;
}
