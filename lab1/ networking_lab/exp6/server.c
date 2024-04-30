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
