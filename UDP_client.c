#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MSG 100

int main(int argc, char *argv[])
{
    int sd, cliLen, n;
    struct sockaddr_in servAddr, cliAddr;
    char line[MAX_MSG];

    if (argc < 3)
    {
        printf("input error\n");
        exit(0);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    memset(&(servAddr.sin_zero), '\0', 8);

    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = INADDR_ANY;
    cliAddr.sin_port = htons(0);

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("successfully created datagram socket\n");

    do
    {
        printf("ENTER data to send to server: ");
        scanf("%s", line);

        sendto(sd, line, strlen(line) + 1, 0, (struct sockaddr*) &servAddr, sizeof(servAddr));
    } while (strcmp(line, "quit") != 0);

    shutdown(sd, SHUT_RDWR);

    return 0;
}
