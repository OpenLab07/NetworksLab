#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress;
    int addressLength = sizeof(serverAddress);
    char receiveBuffer[1024] = {0};
    char *serverResponse = "Message received";
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket failed:");
        exit(EXIT_FAILURE);
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed:");
        exit(EXIT_FAILURE);
    }
    listen(serverSocket, 3);
    clientSocket = accept(serverSocket, (struct sockaddr *)&serverAddress, (socklen_t*)&addressLength);
    if (clientSocket < 0) {
        perror("Accept failed:");
        exit(EXIT_FAILURE);
    }
    read(clientSocket, receiveBuffer, 1024);
    printf("Client message: %s\n", receiveBuffer);
    send(clientSocket, serverResponse, strlen(serverResponse), 0);
    printf("Response sent\n");
    close(clientSocket);
    close(serverSocket);
    return 0;
}

