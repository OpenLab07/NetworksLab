#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 100

void *handle_client(void *sock) {
    int client_sock = *(int*)sock;
    char buffer[1024];
    int read_size;

    while ((read_size = recv(client_sock, buffer, 1024, 0)) > 0) {
        buffer[read_size] = '\0';
        printf("Client: %s\n", buffer);
        send(client_sock, buffer, read_size, 0);
    }

    if (read_size == 0) {
        puts("Client disconnected");
    } else if (read_size == -1) {
        perror("recv failed");
    }

    close(client_sock);
    return 0;
}

int main() {
    int server_sock, client_sock, *new_sock;
    struct sockaddr_in server, client;
    socklen_t sock_in_size;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    listen(server_sock, MAX_CLIENTS);

    puts("Waiting for connections...");

    while ((client_sock = accept(server_sock, (struct sockaddr *)&client, &sock_in_size))) {
        puts("Connection accepted");
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if (pthread_create(&sniffer_thread, NULL, handle_client, (void*)new_sock) < 0) {
            perror("Could not create thread");
            exit(EXIT_FAILURE);
        }
    }

    if (client_sock < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}
// gcc server.c -o server -pthread
// ./server.c
