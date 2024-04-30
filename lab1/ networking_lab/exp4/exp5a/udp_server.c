#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 10000

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char messageBuffer[BUFFER_SIZE];

    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Server is running \n");
    if (recvfrom(serverSocket, messageBuffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen) < 0) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
    }
    printf("Received message: %s \n", messageBuffer);
    close(serverSocket);
    return 0;
}
