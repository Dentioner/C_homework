#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>



typedef enum {
    IDLE,
    WORKING,
    //BLOCKED,
} status_t;

const char server_head_simple[] = "HTTP/1.1 200 OK\r\n";
const char server_head_error[] = "HTTP/1.1 404 Not Found\r\n\r\n";
const char server_head_busy[] = "HTTP/1.1 500.13 Web Server Busy\r\n\r\n";


struct sockaddr_in server, client;

//int server_socket_fd;


void* little_process(void * client_socket_fd_p)
{
    int find = 0;
    int msg_len = 0;
    int index1, index2;
    char msg[2000];
    char filename[100];
    FILE *directory = NULL;
    FILE *big_file_p = NULL;
    char dir_array[1000];
    char big_file[524288];
    char packet[1048576];
    char * p1 = NULL;
    char filesz_str[1000];
    //int thread_index;
    int socket_fd;
    socket_fd = *((int *)client_socket_fd_p);

    while(1)
    {
        

        

        // receive a message from client
        while ((msg_len = recv(socket_fd, msg, sizeof(msg), 0)) > 4) 
        {// ����Ϊ����4����ΪΪ��ʶ��\r\n\r\n

            find = 0;

            for(index1 = 3; index1 < msg_len; index1++)
            {
                if (msg[index1 - 3] == '\r' && msg[index1 - 2] == '\n' && msg[index1 - 1] == '\r' && msg[index1] == '\n')
                {
                    break;
                }
            }

            if(index1 >= msg_len)
            {
                perror("recv failed\n");
                //thread_status[thread_index] = IDLE;
                return NULL;
            }


            //ʶ�� "HTTP"�ĸ���
            for(index1 = 3; index1 < msg_len; index1++)
            {
                if (msg[index1 - 3] == 'H' && msg[index1 - 2] == 'T' && msg[index1 - 1] == 'T' && msg[index1] == 'P')
                {
                    break;
                }
            }


            index2 = index1 - 4; // index2 ������"HTTP"ǰ����Ǹ��ո��λ��
            
            for (index1 = index2; index1 > 0 ; index1 --)
            {// index1��ǰ�ң�ֱ���ҵ�/Ϊֹ����ʾλ��http://10.0.0.2/filename �����һ��/��
                if (msg[index1] == '/')
                    break;
            }

            index1++; // index1 �������ļ����ĵ�һ����ĸ�ˣ�Ҳ����˵msg[index1:index2]�����ļ���

            memset(filename, 0, sizeof(filename));
            strncpy(filename, &msg[index1], index2 - index1); // ��index1~index2��Χ�ڵ����ָ��Ƶ�filename��
            

            // get directory
            system("ls > server_dir");
            directory = fopen("server_dir", "r");

            memset(dir_array, 0, sizeof(dir_array));
            fread(dir_array, sizeof(dir_array), 1, directory);

            // find filename
            p1 = strtok(dir_array, "\n");
            if (!strcmp(p1, filename))
            {
                find = 1;
            }
            else
            {
                while( p1 != NULL)
                {
                    p1 = strtok(NULL, "\n");
                    if ((p1 != NULL) && (!strcmp(p1, filename)))
                    {
                        find = 1;
                        break;
                    }
                }
            }
            
            if(find)
            {  
                //read file
                big_file_p = fopen(filename, "r");
                memset(big_file, 0, sizeof(big_file));
                fread(big_file, sizeof(big_file), 1, big_file_p);

                // combining a http packet
                memset(packet, 0, sizeof(packet));
                strcpy(packet, server_head_simple);//����װ���ʼ��״̬��
                strncat(packet, "Content-Length: ", 17); //Ȼ��װ���ײ��еĴ�С�ֶ�Content-Length
                
                memset(filesz_str, 0, sizeof(filesz_str));
                //_itoa(strlen(big_file), filesz_str, 10);
                //sprintf(filesz_str, "%u", (unsigned)strlen(big_file));
                struct stat statbuf;
                stat(filename, &statbuf);
                sprintf(filesz_str, "%u", (unsigned)(statbuf.st_size));


                strncat(packet, filesz_str, strlen(filesz_str));//���ļ����ȷ���Content_Length����

                strncat(packet, "\r\n\r\n", sizeof("\r\n\r\n"));//����

                //strncat(packet, big_file, strlen(big_file));
                strncat(packet, big_file, (size_t)(statbuf.st_size));


                //send data
                size_t total_length = sizeof(server_head_simple) + sizeof("Content-Length: ") + strlen(filesz_str) + sizeof("\r\n\r\n") + (size_t)(statbuf.st_size);
                write(socket_fd, packet, total_length);

            }
            else // NOT FOUND
            {
                write(socket_fd, server_head_error, sizeof(server_head_error));
            }
            
            
            fcloseall();

        }
        


        
        if (msg_len == 0) 
        {
            printf("client disconnected\n");
        }
        else 
        { // msg_len < 0 or 0 < msg_len < 4
            perror("recv failed\n");
            //thread_status[thread_index] = IDLE;
            return NULL;
        }

        //thread_status[thread_index] = IDLE;

    }
    return NULL;
}





int main()
{
    int server_socket_fd;
    int client_socket_fd;
    
    
    //int index3, index4;

    //char path[2000];
    //DIR * directory;
    
    //int thread_index;

    // init threads status



    // create socket
    if ((server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("create socket failed\n");
		return -1;
    }
    printf("socket created\n");

    // prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(80);

    // bind
    if (bind(server_socket_fd,(struct sockaddr *)&server, sizeof(server)) < 0) 
    {
        perror("bind failed\n");
        return -1;
    }
    printf("bind done\n");
    
    // listen
    listen(server_socket_fd, THREAD_NUM);
    printf("waiting for incoming connections...\n");



    while(1)
    {
        int c = sizeof(struct sockaddr_in);
        if ((client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&client, (socklen_t *)&c)) < 0) 
        {
            perror("accept failed\n");
            return -1;
        }
        printf("connection accepted\n");

        

        pthread_t new_process;
        pthread_create(&(new_process), NULL, little_process, (void *)&(client_socket_fd));




    }





    
    return 0;
}