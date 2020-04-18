#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

//const char client_head_part1[] = "GET http://10.0.0.1/";
const char client_head_part1[] = "GET /";
const char client_head_part2[] = " HTTP/1.1\r\n";
const char client_head_part3[] = "Connection: Keep-Alive\r\n\r\n";

int main()
{
	int sock;
    struct sockaddr_in server;
    char filename[100], server_reply[2000];
    char packet[1000];

    // create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("create socket failed\n");
		return -1;
    }
    printf("socket created\n");
    
    server.sin_addr.s_addr = inet_addr("10.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    // connect to server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) 
	{
        perror("connect failed\n");
        return 1;
    }
    
    printf("connected\n");


    while(1)
    {
        memset(packet, 0, sizeof(packet));
        memset(filename, 0, sizeof(filename));
        memset(server_reply, 0, sizeof(server_reply));

        //get filename
        printf("get : ");
        scanf("%s", filename);

        // combining a http packet
        
        strcpy(packet, client_head_part1);
        strncat(packet, filename, strlen(filename));
        strncat(packet, client_head_part2, strlen(client_head_part2));
        strncat(packet, client_head_part3, strlen(client_head_part3));

        

        // send some data
        if (send(sock, packet, strlen(packet), 0) < 0) 
        {
            printf("send failed\n");
            return 1;
        }

        // receive a reply from the server
        int len = recv(sock, server_reply, 2000, 0);
        if (len < 0) {
            printf("recv failed\n");
            //break;
        }
        else
        {
        
            server_reply[len] = 0;
            
            printf("server reply : ");
            printf("%s\n", server_reply);
        }

    }
    close(sock);
    return 0;
}