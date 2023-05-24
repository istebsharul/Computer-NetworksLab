#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_MSG 100

int main(int argc, char *argv[]) {

    int sd, newSd, clilen, n;
    struct sockaddr_in servAddr, cliAddr;
    char line[MAX_MSG];

    if (argc < 3) {
        std::cout << "input error" << std::endl;
        exit(0);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    memset(&(servAddr.sin_zero), '\0', 8);

    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = INADDR_ANY;
    cliAddr.sin_port = htons(0);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "successfully created stream socket" << std::endl;

    connect(sd, (struct sockaddr*)&servAddr, sizeof(servAddr));
    std::cout << "connected to server successfully" << std::endl;

    do {
        int operand1, operand2;
        char oper;

        std::cout << "Enter first operand: ";
        std::cin >> operand1;

        std::cout << "Enter operator (+,-,*,/): ";
        std::cin >> oper;

        std::cout << "Enter second operand: ";
        std::cin >> operand2;

        sprintf(line, "%d %c %d", operand1, oper, operand2);

        send(sd, line, strlen(line) + 1, 0);
        std::cout << "Data sent (" << line << ")" << std::endl;

        memset(line, 0, MAX_MSG);
        n = recv(sd, line, MAX_MSG, 0);
        line[n] = '\0';

        std::cout << "Result received from server: " << line << std::endl;

    } while (strcmp(line, "quit") != 0);

    std::cout << "Closing connection with the server" << std::endl;
    close(sd);
    return 0;
}
