#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>

#define THREAD_NUM 5

typedef enum {
    IDLE,
    WORKING,
    //BLOCKED,
} status_t;

const char server_head_simple[] = "HTTP/1.1 200 OK\r\n";
const char server_head_error[] = "HTTP/1.1 404 Not Found\r\n\r\n";
const char server_head_busy[] = "HTTP/1.1 500.13 Web Server Busy\r\n\r\n";

pthread_t thread_array[THREAD_NUM];
status_t thread_status[THREAD_NUM];
//int server_socket_fd_arr[THREAD_NUM];
int client_socket_fd_arr[THREAD_NUM];

struct sockaddr_in server, client;

int server_socket_fd;

void* handle_request(void * tmp_socket_p)
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
    int thread_index;
    int socket_fd;

    //search socket
    for(thread_index = 0; thread_index < THREAD_NUM; thread_index++)
    {
        if(tmp_socket_p == &(client_socket_fd_arr[thread_index]))
            break;
    }

    if(thread_index >= THREAD_NUM)
    {
        printf("thread pointer error.\n");
        return NULL;
    }
    else
    {
        socket_fd = client_socket_fd_arr[thread_index];
    }
    

    // receive a message from client
    while ((msg_len = recv(socket_fd, msg, sizeof(msg), 0)) > 4) 
    {// 条件为大于4是因为为了识别\r\n\r\n

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
            thread_status[thread_index] = IDLE;
            return NULL;
        }


        //识别 "HTTP"四个字
        for(index1 = 3; index1 < msg_len; index1++)
        {
            if (msg[index1 - 3] == 'H' && msg[index1 - 2] == 'T' && msg[index1 - 1] == 'T' && msg[index1] == 'P')
            {
                break;
            }
        }


        index2 = index1 - 4; // index2 现在在"HTTP"前面的那个空格的位置
        
        for (index1 = index2; index1 > 0 ; index1 --)
        {// index1往前找，直到找到/为止，表示位于http://10.0.0.2/filename 的最后一个/处
            if (msg[index1] == '/')
                break;
        }

        index1++; // index1 现在是文件名的第一个字母了，也就是说msg[index1:index2]就是文件名

        memset(filename, 0, sizeof(filename));
        strncpy(filename, &msg[index1], index2 - index1); // 将index1~index2范围内的文字复制到filename中
        

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
            strcpy(packet, server_head_simple);//首先装上最开始的状态行
            strncat(packet, "Content-Length: ", 17); //然后装上首部行的大小字段Content-Length
            
            memset(filesz_str, 0, sizeof(filesz_str));
            //_itoa(strlen(big_file), filesz_str, 10);
            //sprintf(filesz_str, "%u", (unsigned)strlen(big_file));
            struct stat statbuf;
            stat(filename, &statbuf);
            sprintf(filesz_str, "%u", (unsigned)(statbuf.st_size));


            strncat(packet, filesz_str, strlen(filesz_str));//将文件长度放在Content_Length后面

            strncat(packet, "\r\n\r\n", sizeof("\r\n\r\n"));//空行

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
        thread_status[thread_index] = IDLE;
		return NULL;
    }

    thread_status[thread_index] = IDLE;
    return NULL;
}

void* little_process(void * nothing)
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

    while(1)
    {
        int c = sizeof(struct sockaddr_in);
        if ((socket_fd = accept(server_socket_fd, (struct sockaddr *)&client, (socklen_t *)&c)) < 0) 
        {
            perror("accept failed\n");
            return NULL;
        }
        printf("connection accepted\n");

        

        // receive a message from client
        while ((msg_len = recv(socket_fd, msg, sizeof(msg), 0)) > 4) 
        {// 条件为大于4是因为为了识别\r\n\r\n

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


            //识别 "HTTP"四个字
            for(index1 = 3; index1 < msg_len; index1++)
            {
                if (msg[index1 - 3] == 'H' && msg[index1 - 2] == 'T' && msg[index1 - 1] == 'T' && msg[index1] == 'P')
                {
                    break;
                }
            }


            index2 = index1 - 4; // index2 现在在"HTTP"前面的那个空格的位置
            
            for (index1 = index2; index1 > 0 ; index1 --)
            {// index1往前找，直到找到/为止，表示位于http://10.0.0.2/filename 的最后一个/处
                if (msg[index1] == '/')
                    break;
            }

            index1++; // index1 现在是文件名的第一个字母了，也就是说msg[index1:index2]就是文件名

            memset(filename, 0, sizeof(filename));
            strncpy(filename, &msg[index1], index2 - index1); // 将index1~index2范围内的文字复制到filename中
            

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
                strcpy(packet, server_head_simple);//首先装上最开始的状态行
                strncat(packet, "Content-Length: ", 17); //然后装上首部行的大小字段Content-Length
                
                memset(filesz_str, 0, sizeof(filesz_str));
                //_itoa(strlen(big_file), filesz_str, 10);
                //sprintf(filesz_str, "%u", (unsigned)strlen(big_file));
                struct stat statbuf;
                stat(filename, &statbuf);
                sprintf(filesz_str, "%u", (unsigned)(statbuf.st_size));


                strncat(packet, filesz_str, strlen(filesz_str));//将文件长度放在Content_Length后面

                strncat(packet, "\r\n\r\n", sizeof("\r\n\r\n"));//空行

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


int search_thread()
{
    int i;
    for (i = 0; i < THREAD_NUM; i++)
    {
        if(thread_status[i] == IDLE)
            break;
    }
    if(i < THREAD_NUM)
        return i;
    else
        return -1;

}

int main()
{
    //int server_socket_fd, client_socket_fd;
    
    
    int index3, index4;

    //char path[2000];
    //DIR * directory;
    
    //int thread_index;

    // init threads status
    for(index3 = 0; index3 < THREAD_NUM; index3++)
    {
        thread_status[index3] = IDLE;
        client_socket_fd_arr[index3] = 0;
    }


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


/*
    while(1)
    {
        int c = sizeof(struct sockaddr_in);
        if ((client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&client, (socklen_t *)&c)) < 0) 
        {
            perror("accept failed\n");
            return -1;
        }
        printf("connection accepted\n");

        thread_index = search_thread();
        if(thread_index == -1) //所有线程都忙
        {
            write(client_socket_fd, server_head_busy, sizeof(server_head_busy));//500.13
        }
        else
        {
            client_socket_fd_arr[thread_index] = client_socket_fd;
            thread_status[thread_index] = WORKING;

            pthread_create(&(thread_array[thread_index]), NULL, handle_request, (void *)&(client_socket_fd_arr[thread_index]));
        }

    }
*/


    for(index4 = 0; index4 < THREAD_NUM; index4++)
    {
        pthread_create(&(thread_array[index4]), NULL, little_process, (void *)NULL);
    }    

    while(1);

    
    return 0;
}