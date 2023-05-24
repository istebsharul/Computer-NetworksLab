#include<bits/stdc++.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string> 

using namespace std;
#define MAX_MSG 100

string g = "1101";

string div(const string& a, const string& b)
{
    string res = "";
    for (int i = 1; i < b.size(); i++)
    {
        if (a[i] == b[i])
            res += '0';
        else
            res += '1';
    }
    return res;
}

void func(string m)
{
    string zero = "";
    for (int i = 0; i < g.size() - 1; i++)
    {
        zero += '0';
    }
    zero += '0';
    string a = m.substr(0, g.size());
    for (int i = g.size(); i < m.size(); i++)
    {
        if (a[0] == '0')
            a = div(a, zero);
        else
            a = div(a, g);
        if (i == m.size())
            break;
        a += m[i];
    }
        cout<<"checking" << endl << flush;
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] != '0')
        {
            cout << "Error" << endl;
        }
    }
    cout << "No error" << endl;
}

int main(int argc, char *argv[])
{
    int sd, cliLen, newSd, n;
    struct sockaddr_in servAddr, cliAddr;
    char buffer[MAX_MSG];

    if (argc < 3)
    {
        printf("Input error\n");
        exit(0);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    memset(&(servAddr.sin_zero), '\0', 8);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd > 0)
        printf("Successfully created stream socket.\n");

    bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    printf("Bound local port successfully\n");

    listen(sd, 5); // server is listening connection request and 5 is the number of clients allowed

    while (1)
    {
        printf("Waiting for client connection on TCP Port %u\n", atoi(argv[2]));

        /*wait for the client connection*/
        cliLen = sizeof(cliAddr);
        newSd = accept(sd, (struct sockaddr *)&cliAddr, (socklen_t *)&cliLen);

        printf("recieved connection from host (IP %s, TCP Port %d): %s\n", inet_ntoa(cliAddr.sin_addr),
               ntohs(cliAddr.sin_port), buffer);
        strcpy(buffer, ""); // Initialize buffer before receiving data

        do
        {
            memset(buffer, 0, MAX_MSG);
            n = recv(newSd, buffer, MAX_MSG, 0);
            buffer[n] = '\n';
            string m = "";
            for (int i = 0; i < n; i++)
            {
                m += buffer[i];
            }
            func(m);
            printf("recieved from host [IP %s, TCP port %d] : %s\n", inet_ntoa(cliAddr.sin_addr),
                   ntohs(cliAddr.sin_port), buffer);
        } while (strcmp(buffer, "quit") != 0);

        printf("closing connection with host [IP %s, TCP port %d]\n", inet_ntoa(cliAddr.sin_addr),
               ntohs(cliAddr.sin_port));
        close(newSd);
    }

    return 0;
}
