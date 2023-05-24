#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <cstring>


int main(int argc, char *argv[]) {
    int sd, newSd;
    socklen_t cliLen;
    struct sockaddr_in servAddr, cliAddr;
    time_t currentTime;
    char *timeString;

    if (argc < 3) {
        printf("Input error\n");
        exit(0);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    memset(&(servAddr.sin_zero), '\0', 8);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Successfully created stream socket\n");

    bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    printf("Successfully bound socket\n");

    listen(sd, 5);

    while (1) {
        printf("Waiting for client connection on TCP port %d\n", atoi(argv[2]));

        cliLen = sizeof(cliAddr);
        newSd = accept(sd, (struct sockaddr *)&cliAddr, &cliLen);
        printf("Received connection from host [IP %s, TCP port %d]\n",
               inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

        currentTime = time(NULL);
        timeString = ctime(&currentTime);

        // Send the time to the client
        send(newSd, timeString, strlen(timeString), 0);
        printf("Sent time to host [IP %s, TCP port %d]: %s",
               inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), timeString);

        printf("Closing connection with host [IP %s, TCP port %d]\n",
               inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
        close(newSd);
    }

    return 0;
}
