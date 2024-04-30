#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;
    char receiveBuffer[1024] = {0};
    char *clientMessage = "Hello from client";
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY; 
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
    send(clientSocket, clientMessage, strlen(clientMessage), 0);
    printf("Message sent\n");
    read(clientSocket, receiveBuffer, 1024);
    printf("Server response: %s\n", receiveBuffer);
    close(clientSocket);
    return 0;
}

