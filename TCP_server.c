#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_MSG 100

int main(int argc, char *argv[]) {
    int sd, newSd, n;
    socklen_t cliLen;
    struct sockaddr_in servAddr, cliAddr;
    char line[MAX_MSG];

    if (argc < 3) {
        printf("input error\n");
        exit(0);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    memset(&(servAddr.sin_zero), '\0', 8);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    printf("successfully created stream socket\n");

    bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    printf("successfully bound socket\n");

    listen(sd, 5);

    while (1) {
        printf("waiting for client connection on TCP port %d\n", atoi(argv[2]));

        cliLen = sizeof(cliAddr);
        newSd = accept(sd, (struct sockaddr *)&cliAddr, &cliLen);
        printf("received connection from host [IP %s, TCP port %d]\n",
               inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

        do {
            memset(line, 0, MAX_MSG);
            n = recv(newSd, line, MAX_MSG, 0);
            line[n] = '\0';
            printf("received from host [IP %s, TCP port %d]: %s\n",
                   inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), line);
        } while (strcmp(line, "quit") != 0);

        printf("closing connection with host [IP %s, TCP port %d]\n",
               inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
        close(newSd);
    }

    return 0;
}
