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
void func(string s)
{
    string a = s.substr(0, d), b = s.substr(d, d), checkSum = s.substr(d * 2, d);
    string addition = sum(a, b);
    string f = comp(sum(addition, checkSum));
    cout << "Checking" << endl;
    for (int i = 0; i < f.size(); i++)
    {
        if (f[i] != '0')
        {
            cout << "Error" << endl;
            return;
        }
    }
    cout << "No error" << endl;
    cout << f << endl;
}
int main(int argc, char *argv[]) {
    int sd, cliLen, newSd, n;
    struct sockaddr_in servAddr, cliAddr;
    char buffer[MAX_MSG];
    
    if (argc < 3) {
        printf("Input error\n");
        exit(0);
    }
    
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    memset(&servAddr.sin_zero, '\0', sizeof(servAddr.sin_zero));
    
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd > 0)
        printf("Successfully created stream socket.\n");
    
    bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    printf("Bound local port successfully\n");
    
    listen(sd, 5);
    
    while (1) {
        printf("Waiting for client connection on TCP Port %u\n", atoi(argv[2]));
        
        cliLen = sizeof(cliAddr);
        newSd = accept(sd, (struct sockaddr *)&cliAddr, (socklen_t *)&cliLen);
        printf("Received connection from host (IP %s, TCP Port %d): %s\n", inet_ntoa(cliAddr.sin_addr),
               ntohs(cliAddr.sin_port), buffer);
        
        do {
            memset(buffer, 0, MAX_MSG);
            n = recv(newSd, buffer, MAX_MSG, 0);
            buffer[n] = '\n';
            
            // Convert buffer to a string
            char m[MAX_MSG];
            strncpy(m, buffer, n);
            
            func(m);
            printf("Received from host [IP %s, TCP port %d]: %s\n", inet_ntoa(cliAddr.sin_addr),
                   ntohs(cliAddr.sin_port), buffer);
        } while (abs(strcmp(buffer, "quit")));
        
        printf("Closing connection with host [IP %s, TCP port %d]\n", inet_ntoa(cliAddr.sin_addr),
               ntohs(cliAddr.sin_port));
        close(newSd);
    }
    
    return 0;
}