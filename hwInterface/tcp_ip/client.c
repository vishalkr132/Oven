#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define PORT_NUM 5566

int main()
{

    int fd;
    struct sockaddr_in sock_add;

    fd = socket(AF_INET , SOCK_STREAM , 0);
    if(fd < 0)
    {
        printf("failed\n");
    }

    sock_add.sin_family = AF_INET;
    sock_add.sin_port = htons(PORT_NUM);
    sock_add.sin_addr.s_addr = INADDR_ANY;

    if(connect(fd , (struct sockaddr *)&sock_add, sizeof(sock_add)) < 0)
    {
        printf("failed to connect\n");
    }
    
    char buff[30];
    strcpy(buff , "hello from thingspeak!");

    send(fd , buff , sizeof(buff) , 0);



    return 0;
}