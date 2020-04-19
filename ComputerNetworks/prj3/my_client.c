#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

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
    FILE * f_get = NULL;
    int index1;
    time_t time_tmp;
    char file_name_get[20];

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


    //while(1)
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
            
            //printf("server reply : ");
            //printf("%s\n", server_reply);

            for(index1 = 3; index1 < len; index1++)
            {
                if (server_reply[index1 - 3] == '\r' && server_reply[index1 - 2] == '\n' && server_reply[index1 - 1] == '\r' && server_reply[index1] == '\n')
                {
                    break;
                }
            }

            if(index1 >= len)
            {
                perror("recv failed\n");
                return -1;
            }

            index1++;

            //write data
            time_tmp = time(NULL);
            sprintf(file_name_get, "%ld", ((long)time_tmp % sizeof(file_name_get)));
            f_get = fopen(file_name_get, "w");
            fwrite((server_reply + index1), (strlen(server_reply) - index1), 1, f_get);
            fclose(f_get);

            printf("recv succeeded\n");

        }

    }
    close(sock);
    return 0;
}