#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define SERV_TCP_PORT 5035
#define MAX 60
#define ERROR_FILE_NOT_FOUND "1"

void* handle_client(void* arg) {
    int newsockfd = *(int*)arg;
    char str[MAX], buff[4096], msg[MAX];;
    FILE *f1;

    read(newsockfd, &str, MAX);
    printf("\nClient message\n File Name : %s\n", str);

    f1 = fopen(str, "r");
    if (f1 == NULL) {
        sprintf(msg, "Error: File not found. PID: %d", getpid());
        write(newsockfd, ERROR_FILE_NOT_FOUND, strlen(ERROR_FILE_NOT_FOUND));
        write(newsockfd, msg, strlen(msg));
        close(newsockfd);
        return NULL;
    }

    while(fgets(buff, 4096, f1) != NULL) {
        write(newsockfd, buff, strlen(buff));
        sprintf(msg, "File found. PID: %d", getpid());
        write(newsockfd, msg, strlen(msg));
        printf("\n");
    }
    fclose(f1);
    printf("\nFile Transferred\n");
    close(newsockfd);
    return NULL;
}

int main(int afg, char *argv) {
    int sockfd, newsockfd, clength;
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t thread_id;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    printf("\nBinded");
    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    printf("\nListening...");
    listen(sockfd, 5);

    while(1) {
        clength = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clength);
        if (newsockfd < 0) {
            perror("Accept failed");
            continue;
        }

        pthread_create(&thread_id, NULL, handle_client, (void*)&newsockfd);
        pthread_detach(thread_id); // Detach the thread so it can exit independently
    }

    close(sockfd);
    return 0;
} 