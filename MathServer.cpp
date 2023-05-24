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
    int sd, newSd, n;
    socklen_t cliLen;
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

    sd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "successfully created stream socket" << std::endl;

    bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    std::cout << "successfully bound socket" << std::endl;

    listen(sd, 5);

    while (1) {
        std::cout << "waiting for client connection on TCP port " << atoi(argv[2]) << std::endl;

        cliLen = sizeof(cliAddr);
        newSd = accept(sd, (struct sockaddr *)&cliAddr, &cliLen);
        std::cout << "received connection from host [IP " << inet_ntoa(cliAddr.sin_addr) << ", TCP port " << ntohs(cliAddr.sin_port) << "]" << std::endl;

        do {
            memset(line, 0, MAX_MSG);
            n = recv(newSd, line, MAX_MSG, 0);
            line[n] = '\0';
            std::cout << "received from host [IP " << inet_ntoa(cliAddr.sin_addr) << ", TCP port " << ntohs(cliAddr.sin_port) << "]: " << line << std::endl;

            // Perform math operation
            int operand1, operand2, result;
            char oper;
            sscanf(line, "%d %c %d", &operand1, &oper, &operand2);

            switch (oper) {
                case '+':
                    result = operand1 + operand2;
                    break;
                case '-':
                    result = operand1 - operand2;
                    break;
                case '*':
                    result = operand1 * operand2;
                    break;
                case '/':
                    if (operand2 != 0) {
                        result = operand1 / operand2;
                    } else {
                        strcpy(line, "Error: Division by zero");
                    }
                    break;
                default:
                    strcpy(line, "Error: Invalid operator");
                    break;
            }

            if (strcmp(line, "Error") != 0) {
                sprintf(line, "Result: %d", result);
            }

            // Send the result back to the client
            send(newSd, line, strlen(line), 0);
            std::cout << "sent to host [IP " << inet_ntoa(cliAddr.sin_addr) << ", TCP port " << ntohs(cliAddr.sin_port) << "]: " << line << std::endl;

        } while (strcmp(line, "quit") != 0);

        std::cout << "closing connection with host [IP " << inet_ntoa(cliAddr.sin_addr) << ", TCP port " << ntohs(cliAddr.sin_port) << "]" << std::endl;
        close(newSd);
    }

    return 0;
}
