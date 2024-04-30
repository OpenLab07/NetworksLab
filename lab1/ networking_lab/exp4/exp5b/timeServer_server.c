

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define PORT 10000

void *handle_client(void *arg) {
    int client_sock = *(int *)arg;
    char buffer[BUFFER_SIZE];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    send(client_sock, buffer, strlen(buffer), 0);

    close(client_sock);
    return NULL;
}

int main() {
    int sockfd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pthread_t thread_id;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        if ((new_sock = recvfrom(sockfd, NULL, 0, 0, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }

        if (pthread_create(&thread_id, NULL, handle_client, (void *)&new_sock) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }

        pthread_detach(thread_id);
    }

    close(sockfd);
    return 0;
}
