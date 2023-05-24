#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bits/stdc++.h>

using namespace std;
#define MAX_MSG 100

void func(string& a)
{
    vector<string> v;
    int size1 = a.size() / 8;
    int j = 0;
    for (int i = 0; i < size1; i++)
    {
        v.push_back(a.substr(j, 8));
        j += 8;
    }
    for (int i = 0; i < v.size(); i++)
    {
        int t = v[i][0] - '0';
        for (int j = 1; j < v[v.size() - 1].size(); j++)
        {
            t ^= (v[i][j] - '0');
        }
        v[i] += (t + '0');
        if (t == 1)
        {
            cout << "Error" << endl;
            return;
        }
    }
    string temp = "";
    for (int j = 0; j < v[0].size(); j++)
    {
        int t = v[0][j] - '0';
        for (int i = 1; i < v.size(); i++)
        {
            t ^= (v[i][j] - '0');
        }
        temp += (t + '0');
        if (t == 1)
        {
            cout << "Error" << endl;
            return;
        }
    }
    v.push_back(temp);
    temp = "";

    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << endl;
    }
    cout << "No error found" << endl;
}

int main(int argc, char *argv[])
{
    int sd, cliLen, newSd, n;
    struct sockaddr_in servAddr, cliAddr;
    char buffer[MAX_MSG];
    if (argc < 3) // name of program, ip address, port number
    {
        printf("Input error\n");
        exit(0);
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]); // inet_addr -> converts ip address into 32-bit binary
    servAddr.sin_port = htons(atoi(argv[2])); // atoi -> string to integer, htons -> works only on int (host to network short) converts into 16-bit binary number
    memset(&(servAddr.sin_zero), '\0', 8); // zero the rest of the struct.sin_zero

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd > 0)
        printf("Successfully created stream socket.\n");

    bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    printf("Bound local port successfully\n");

    listen(sd, 5); // server is listening connection request and 5 is the number of clients allowed

    while (1)
    {
        printf("Waiting for client connection on TCP Port %u\n", atoi(argv[2]));

        /* wait for the client connection */
        cliLen = sizeof(cliAddr);
        newSd = accept(sd, (struct sockaddr *)&cliAddr, (socklen_t *)&cliLen);
        printf("Received connection from host (IP %s, TCP Port %d): %s\n",
               inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), buffer);

        do
        {
            memset(buffer, 0, MAX_MSG);
            n = recv(newSd, buffer, MAX_MSG, 0);
            buffer[n] = '\0';

            string m = "";
            for (int i = 0; i < n; i++)
            {
                m += buffer[i];
            }

            func(m);

            printf("Received from host [IP %s, TCP port %d]: %s\n", inet_ntoa(cliAddr.sin_addr),
                   ntohs(cliAddr.sin_port), buffer);

        } while (strcmp(buffer, "quit"));

        /* close client connection */
        printf("Closing connection with host [IP %s, TCP port %d]\n", inet_ntoa(cliAddr.sin_addr),
               ntohs(cliAddr.sin_port));

        close(newSd);
    }

    return 0;
}
