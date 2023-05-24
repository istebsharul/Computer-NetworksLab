#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>  

using namespace std;
#define MAX_MSG 100

string g = "1101";

string div(string a, string b)
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

string func(string m)
{
    string zero = "";
    string temp = m;
    for (int i = 0; i < g.size() - 1; i++)
    {
        m += '0';
        zero += '0';
    }
    zero += '0';
    string a = m.substr(0, g.size());
    for (int i = g.size(); i <= m.size(); i++)
    {
        if (a[0] == '0')
            a = div(a, zero);
        else
            a = div(a, g);
        if (i == m.size())
            break;
        a += m[i];
    }
    cout << "CRC generated " << a << endl;
    return temp + a;
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

    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = INADDR_ANY;
    cliAddr.sin_port = htons(0);

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd > 0)
        printf("Successfully created stream socket.\n");

    connect(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    printf("connected to server successfully\n");

    do
    {
        string input;
        printf("Enter string to send to server: ");
        cin >> input;

        input = func(input);

        for (int i = 0; i < input.size(); i++)
        {
            buffer[i] = input[i];
        }
        buffer[input.size()] = '\0';

        cout << buffer << endl;

        send(sd, buffer, strlen(buffer) + 1, 0);
        printf("data sent (%s)\n", buffer);
    } while (strcmp(buffer, "quit") != 0);

    printf("closing connection with the server\n");
    close(sd);

    return 0;
}
