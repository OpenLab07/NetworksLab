#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main()
{
    int c_sock;
    c_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(9009);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(c_sock, (struct sockaddr *)&client, sizeof(client)) == -1)
    {
        printf("Connection failed");
        return 0;
    }
    printf("\n\tClient -with individual acknowledgement scheme\n\n");
    char msg1[50] = "akwnowledgementof-";
    char msg2[50];
    //	char msg3[50]="Final cumulative aknowledgement";
    char buff[100];
    int flag = 1, flg = 1;
    for (int i = 0; i <= 9; i++)
    {
        flg = 1;
        bzero(buff, sizeof(buff));
        bzero(msg2, sizeof(msg2));
        if (i == 8 && flag == 1)
        {
            printf("here\n"); // simulating loss
            // i--;
            flag = 0;
            read(c_sock, buff, sizeof(buff));
        }
        int n = read(c_sock, buff, sizeof(buff));
        if (buff[strlen(buff) - 1] != i + '0')
        { // outoforder
            printf("Discarded as out of order \n");
            // printf("%c, %c, -- %s\n",buff[strlen(buff)-1],'0'+i,buff);
            // flg=0;
            i--;
        }
        else
        {
            printf("Message received from server : %s \n", buff);
            printf("Aknowledgement sent for message \n");
            strcpy(msg2, msg1);
            msg2[strlen(msg2)] = i + '0';
            // if(i==8)
            // continue; 	//we will send cumulative for last 2
            // if(i==9){
            // write(c_sock,msg3,sizeof(msg3));
            // printf("- %s",msg3);}
            // else
            write(c_sock, msg2, sizeof(msg2));
            // printf("- %s",msg2);
        }
    }

    close(c_sock);
    return 0;
}
/*
2021_SUJITHTS@ltsp1217:~/Desktop/ networking_lab/exp5/go_back_N$ gcc client.c -o client 
2021_SUJITHTS@ltsp1217:~/Desktop/ networking_lab/exp5/go_back_N$ ./client

        Client -with individual acknowledgement scheme

Message received from server : server message :0 
Aknowledgement sent for message 
Message received from server : server message :1 
Aknowledgement sent for message 
Message received from server : server message :2 
Aknowledgement sent for message 
Message received from server : server message :3 
Aknowledgement sent for message 
Message received from server : server message :4 
Aknowledgement sent for message 
Discarded as out of order 
Discarded as out of order 
Discarded as out of order 
Message received from server : server message :5 
Aknowledgement sent for message 
Message received from server : server message :6 
Aknowledgement sent for message 
Message received from server : server message :7 
Aknowledgement sent for message 
here
Discarded as out of order 
Message received from server : server message :8 
Aknowledgement sent for message 
Message received from server : server message :9 
Aknowledgement sent for message 
*/