#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#define PORT 8445
#define BUFFER_SIZE 1024
#define IP_STR "127.0.0.1"
void *receiveMessage(void *socket) {
int sock = *(int *)socket;
char message[BUFFER_SIZE];
while(1) {
int len = recv(sock, message,
BUFFER_SIZE - 1, 0);
if(len > 0) {
message[len] = '\0';
printf("%s", message);
} else {
printf("Server disconnected or recv error\n");
exit(0); }}
return NULL; }
int main() {
int sock = socket(AF_INET,
SOCK_STREAM, 0);
struct sockaddr_in server = {
.sin_addr.s_addr = inet_addr(IP_STR),
.sin_family = AF_INET,
.sin_port = htons(PORT),};
if(connect(sock, (struct sockaddr *)&server,
sizeof(server)) < 0) {
perror("connect failed");
return 1;}
printf("Connected\n");
pthread_t recv_thread;
pthread_create(&recv_thread, NULL,
receiveMessage, &sock);
char message[BUFFER_SIZE];
while(fgets(message, BUFFER_SIZE, stdin) != NULL) {
if(send(sock, message,strlen(message), 0) < 0) {
perror("send failed");
break; }}
close(sock);
return 0;}
