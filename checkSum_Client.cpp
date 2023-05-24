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
int d = 4;
string sum(string &a, string &b)
{
    string res = "";
    char carry = '0';
    for (int i = a.size(); i >= 0; i--)
    {
        if (a[i] == '0' && b[i] == '0' && carry == '0')
        {
            res += '0';
        }
        else if (a[i] == '1' && b[i] == '0' && carry == '0')
        {
            res += '1';
        }
        else if (a[i] == '0' && b[i] == '1' && carry == '0')
        {
            res += '1';
        }
        else if (a[i] == '1' && b[i] == '1' && carry == '0')
        {
            res += '0';
            carry = '1';
        }
        else if (a[i] == '0' && b[i] == '0' && carry == '1')
        {
            res += '1';
            carry = '0';
        }
        else if (a[i] == '1' && b[i] == '0' && carry == '1')
        {
            res += '0';
            carry = '1';
        }
        else if (a[i] == '0' && b[i] == '1' && carry == '1')
        {
            res += '0';
            carry = '1';
        }
        else if (a[i] == '1' && b[i] == '1' && carry == '1')
        {
            res += '1';
            carry = '1';
        }
    }
    reverse(res.begin(), res.end());
    if (carry == '1')
    {
        string c = "";
        for (int i = 0; i < a.size() - 1; i++)
        {
            c += '0';
        }
        c += '1';
        res = sum(res, c);
    }
    return res;
}
string comp(string str)
{
    string res = "";
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == '0')
            res += '1';
        else
            res += '0';
    }
    return res;
}
string func(string s)
{
    string a = s.substr(0, d), b = s.substr(d, d);
    string checkSum = comp(sum(a, b));
    cout << "CheckSum generated: " << checkSum << endl;
    a += b;
    a += checkSum;
    return a;
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
    /*connect to server*/
    connect(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    printf("connected to server successfully\n");
    /*send data to server*/
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
    } while (strcmp(buffer, "quit"));
    printf("closing connection with the server\n");
    close(sd);
    return 0;
}