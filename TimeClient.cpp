#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define MAX_MSG 100

int main(int argc, char *argv[]) {
    int sd, n;
    struct sockaddr_in servAddr;
    char line[MAX_MSG];

    if (argc < 3) {
        printf("Input error. Usage: %s <server_ip> <server_port>\n", argv[0]);
        exit(0);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    memset(&(servAddr.sin_zero), '\0', 8);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Successfully created stream socket\n");

    if (connect(sd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Failed to connect to the server");
        exit(1);
    }

    // Receive and print the time from the server
    memset(line, 0, MAX_MSG);
    if ((n = recv(sd, line, MAX_MSG, 0)) < 0) {
        perror("Failed to receive time from the server");
        exit(1);
    }

    printf("Time received from the server: %s", line);

    // Close the socket
    close(sd);

    return 0;
}
