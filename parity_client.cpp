#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
using namespace std;
#define MAX_MSG 100
string func(string &a)
{
    vector<string> v;
    int size1 = a.size() / 7;
    int j = 0;
    for (int i = 0; i < size1; i++)
    {
        v.push_back(a.substr(j, 7));
        j += 7;
    }
    for (int i = 0; i < v.size(); i++)
    {
        int t = v[i][0] - '0';
        for (int j = 1; j < v[v.size() - 1].size(); j++)
        {
            t ^= (v[i][j] - '0');
        }
        v[i] += (t + '0');
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
 
    }
    v.push_back(temp);

    temp = "";

    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << endl;
        temp += v[i];
    }
    return temp;
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
    //  connect to server 
        connect(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    printf("connected to server successfully\n");
    // send data to server
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
        cout << "Data to be sent " << buffer << endl;
        send(sd, buffer, strlen(buffer) + 1, 0);
        printf("data sent (%s)\n", buffer);
    }
    while (strcmp(buffer, "quit"))
        ;

    printf("closing connection with the server\n");
    close(sd);
    return 0;
}