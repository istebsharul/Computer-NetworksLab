#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>


#define MAX_MSG 100

int main(int argc, char *argv[])
{

    int sd, newSd, cliLen, n;
    struct sockaddr_in servAddr, cliAddr;
    char line[MAX_MSG];

    if (argc < 3)
    {
        printf("input error\n");
            exit(0);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr=inet_addr(argv[1]);
    servAddr.sin_port=htons(atoi(argv[2]));
    memset(&(servAddr.sin_zero),'\0',8);


    sd=socket(AF_INET,SOCK_DGRAM,0);
    printf("successfullycreated stream socket\n");

    bind (sd,(struct sockaddr*)&servAddr, sizeof(servAddr));
    printf ("success fully created stream socket\n");
 
    listen(sd,5);

    while(1){
        memset(line,0,MAX_MSG);
        cliLen=sizeof(cliAddr);
        n=recvfrom(sd,line, MAX_MSG,0,(struct sockaddr*)&cliAddr,(socklen_t*) &cliLen);
        printf("from%s : UDP port %u : %s \n",inet_ntoa(cliAddr.sin_addr),ntohs(cliAddr.sin_port), line);
    }

    return 0;
}


