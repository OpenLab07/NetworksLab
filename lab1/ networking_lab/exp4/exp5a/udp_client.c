#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 10000

int main() {
    int socketFd;
    struct sockaddr_in serverAddr;
    char messageBuffer[] = "Hello from client";

    if ((socketFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (sendto(socketFd, messageBuffer, strlen(messageBuffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    printf("Sent message: %s\n", messageBuffer);

    close(socketFd);
    return 0;
}
