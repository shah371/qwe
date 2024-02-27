#include <iostream>
#include <map>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
 
const int PORT = 8080;
const int BUFFER_SIZE = 1024;
 
// Simulated devices
std::map<std::string, bool> devices = {
    {"lamp", false},
    {"fan", false},
    {"refrigerator", false},
    {"air_cooler", false},
    {"washing_machine", false}
};
 
void handleCommand(const std::string& command) {
    std::string device, action;
    std::istringstream iss(command);
    iss >> device >> action;
 
    if (devices.find(device) != devices.end()) {
        if (action == "on") {
            devices[device] = true;
            std::cout << "Turned " << device << " on\n";
        } else if (action == "off") {
            devices[device] = false;
            std::cout << "Turned " << device << " off\n";
        } else {
            std::cerr << "Invalid action\n";
        }
    } else {
        std::cerr << "Device not found\n";
    }
}
 
int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];
 
    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }
 
    // Prepare the server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
 
    // Bind
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding\n";
        return 1;
    }
 
    // Listen
    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error listening\n";
        return 1;
    }
 
    std::cout << "Home automation system server listening on port " << PORT << std::endl;
 
    socklen_t clientAddrLen = sizeof(clientAddr);
 
    // Accept incoming connections
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen)) == -1) {
        std::cerr << "Error accepting connection\n";
        return 1;
    }
 
    std::cout << "Client connected\n";
 
    // Receive commands from client
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived == -1) {
            std::cerr << "Error receiving data\n";
            break;
        } else if (bytesReceived == 0) {
            std::cout << "Client disconnected\n";
            break;
        }
 
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        std::string command(buffer);
        handleCommand(command);
    }
 
    close(clientSocket);
    close(serverSocket);
 
    return 0;
}