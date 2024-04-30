#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 5035
#define BUFSIZE 60
#define ERROR_FILE_NOT_FOUND "1"

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in server_addr;
    char *server_ip = "127.0.0.1"; // Assuming server is running on the same machine
    char request[BUFSIZE], response[BUFSIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return 1;
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Error setting server address");
        return 1;
    }

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        return 1;
    }

    // Send request
    printf("Enter the file name to request: ");
    fgets(request, BUFSIZE, stdin);
    request[strcspn(request, "\n")] = 0; // Remove newline character
    send(sockfd, request, strlen(request), 0);

    // Receive response
    recv(sockfd, response, BUFSIZE, 0);

    // Check for error code
    if (strncmp(response, ERROR_FILE_NOT_FOUND, strlen(ERROR_FILE_NOT_FOUND)) == 0) {
        printf("Error: %s\n", response + strlen(ERROR_FILE_NOT_FOUND) + 1); // Skip the error code and space
    } else {
        // Open a file for writing
        char *filename = "received_file.txt";
        FILE *fp = fopen(filename, "w");
        if (fp == NULL) {
            printf("Error opening the file %s", filename);
            return 1;
        }

        // Write the received content to the file
        fprintf(fp, "%s", response);

        // Close the file
        fclose(fp);

        printf("File content saved to %s\n", filename);
    }

    // Close socket
    close(sockfd);

    return 0;
}
