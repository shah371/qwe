#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
 
const char *SERVER_IP = "127.0.0.1";
const int PORT = 8080;
const int BUFFER_SIZE = 1024;
 
int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
 
    // Create socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }
 
    // Prepare the server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
 
    // Connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server\n";
        return 1;
    }
 
    std::cout << "Connected to home automation system server\n";
 
    // Send commands to control devices
    while (true) {
        std::cout << "Enter command (device action): ";
        std::cin.getline(buffer, BUFFER_SIZE);
 
        // Send command
        if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
            std::cerr << "Error sending data\n";
            break;
        }
    }
 
    close(clientSocket);
 
    return 0;
}