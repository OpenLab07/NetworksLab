#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#define PORT 8445
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
int clients[MAX_CLIENTS];
int client_count = 0;
void broadcastMessage(char *msg, int sender) {
for (int i = 0; i < client_count; i++) {
if (clients[i] != sender) {
send(clients[i], msg, strlen(msg), 0);} }}
void *clientHandler(void *socket_desc) {
int sock = *(int *)socket_desc;
char buffer[BUFFER_SIZE];
while (1) {
int read_size = recv(sock, buffer, BUFFER_SIZE
- 1, 0);
if (read_size <= 0) break;
buffer[read_size] = '\0';
broadcastMessage(buffer, sock);}
close(sock);
return NULL;}
int main() {
int socket_desc, new_socket;
struct sockaddr_in server, client;
socket_desc = socket(AF_INET,
SOCK_STREAM, 0);
if (socket_desc == -1) {
perror("Could not create socket");
return 1;}
server.sin_family = AF_INET;
server.sin_addr.s_addr = INADDR_ANY;
server.sin_port = htons(PORT);
if (bind(socket_desc, (struct sockaddr *)&server,
sizeof(server)) < 0) {
perror("bind failed");
return 1;}
listen(socket_desc, MAX_CLIENTS);
puts("Waiting for incoming connections...");
while ((new_socket = accept(socket_desc, NULL,
NULL))) {
if (new_socket < 0) {
perror("accept failed");
continue;}
puts("Connection accepted");
if (client_count < MAX_CLIENTS) {
clients[client_count++] = new_socket;
pthread_t thread_id;
if (pthread_create(&thread_id, NULL,
clientHandler, (void *)&new_socket) != 0) {
perror("could not create thread");}
} else {char *message = "Max clients reached. Try again later\n";
write(new_socket, message, strlen(message));
close(new_socket);}}
return 0;}
