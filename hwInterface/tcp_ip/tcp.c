#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#include "tcp.h"

int32_t client_fd;
struct sockaddr_in client_addr;

int32_t TCP_fnInit()
{
    client_fd = socket(AF_INET , SOCK_STREAM , 0);
    if(client_fd < 0)
    {
        printf("failed to create tcp socket %d\n" , client_fd);
        return -1;
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT);
    //client_addr.sin_addr.s_addr = "184.106.153.149";

    if(inet_pton(AF_INET, "184.106.153.149" , &client_addr.sin_addr) < 0)
    {
        printf("failed to convert ip into binary\n");
    }

    if(connect(client_fd , (struct sockaddr *)&client_addr , sizeof(client_addr)) != 0)
    {
        printf("client failed connect to server\n");
        return -1;
    }

    return 0;
}

int32_t TCP_fnSend(int32_t temp_val)
{
    char send_buffer[256];
    snprintf(send_buffer , sizeof(send_buffer) ,
   "GET /update.json?api_key=%s&field1=%d HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n" , 
   API_KEY, temp_val, SERVER);

    send(client_fd , send_buffer , sizeof(send_buffer) , 0);
}   

int32_t TCP_fnRecv()
{
    char read_buffer[100];
    read(client_fd , read_buffer , sizeof(read_buffer));
    printf("%s\n" , read_buffer);
}

int32_t TCP_fnClose()
{
    if(client_fd != -1)
    {
        close(client_fd);
        client_fd = -1;
        return 0;
    }
    return -1;
}


#if 0

int main()
{
    TCP_fnInit();
    TCP_fnSend(100);
    TCP_fnRecv();
  //  TCP_fnClose();

    return 0;
}

#endif


/* server code */

// #include <stdio.h>
// #include <stdint.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <stdlib.h>
// #include <string.h>

// #define PORT_NUM 5566
// //#define IP 


// int main()
// {
//     int32_t server_fd , client_fd , len;
//     struct sockaddr_in server_addr , client_addr;

//     server_fd = socket(AF_INET , SOCK_STREAM , 0);

//     if(server_fd < 0)
//     {
//         printf("error in socket\n");
//     }

//     server_addr.sin_family = AF_INET; // IPV4
//     server_addr.sin_port = htons(PORT_NUM);
//     server_addr.sin_addr.s_addr = INADDR_ANY;

//     if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
//     {
//         printf("Bind failed");
//     }

//     if(listen(server_fd , 5) < 0)
//     {
//         printf("failed to listen\n");
//     }

//     char readBuff[30];
//     while(1)
//     {
//         client_fd = accept(server_fd , (struct sockaddr *)&client_addr , &len);
//         if(client_fd < 0)
//         {
//             printf("failed to accept\n");   
//         }
//         printf("connection is done\n");

//         read(client_fd , readBuff , sizeof(readBuff));
//         printf("%s\n" , readBuff);

//         sleep(1);

//     }

//     return 0;
// }