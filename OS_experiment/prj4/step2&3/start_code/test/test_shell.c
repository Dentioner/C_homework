/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *                  The shell acts as a task running in user mode. 
 *       The main function is to make system calls through the user's output.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this 
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
 * persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#include "test.h"
#include "stdio.h"
#include "screen.h"
#include "syscall.h"
#include "sched.h"

#define command_prompt "> root@UCAS_OS: "
#define blank "                                                                        "



static void disable_interrupt()
{
    uint32_t cp0_status = get_cp0_status();
    cp0_status &= 0xfffffffe;
    set_cp0_status(cp0_status);
}

static void enable_interrupt()
{
    uint32_t cp0_status = get_cp0_status();
    cp0_status |= 0x01;
    set_cp0_status(cp0_status);
}

static char read_uart_ch(void)
{
    char ch = 0;
    unsigned char *read_port = (unsigned char *)(0xbfe48000 + 0x00);
    unsigned char *stat_port = (unsigned char *)(0xbfe48000 + 0x05);

    while ((*stat_port & 0x01))
    {
        ch = *read_port;
    }
    return ch;
}
/*
struct task_info task1 = {"task1", (uint32_t)&ready_to_exit_task, USER_PROCESS};
struct task_info task2 = {"task2", (uint32_t)&wait_lock_task, USER_PROCESS};
struct task_info task3 = {"task3", (uint32_t)&wait_exit_task, USER_PROCESS};

struct task_info task4 = {"task4", (uint32_t)&semaphore_add_task1, USER_PROCESS};
struct task_info task5 = {"task5", (uint32_t)&semaphore_add_task2, USER_PROCESS};
struct task_info task6 = {"task6", (uint32_t)&semaphore_add_task3, USER_PROCESS};

struct task_info task7 = {"task7", (uint32_t)&producer_task, USER_PROCESS};
struct task_info task8 = {"task8", (uint32_t)&consumer_task1, USER_PROCESS};
struct task_info task9 = {"task9", (uint32_t)&consumer_task2, USER_PROCESS};

struct task_info task10 = {"task10", (uint32_t)&barrier_task1, USER_PROCESS};
struct task_info task11 = {"task11", (uint32_t)&barrier_task2, USER_PROCESS};
struct task_info task12 = {"task12", (uint32_t)&barrier_task3, USER_PROCESS};

struct task_info task13 = {"SunQuan",(uint32_t)&SunQuan, USER_PROCESS};
struct task_info task14 = {"LiuBei", (uint32_t)&LiuBei, USER_PROCESS};
struct task_info task15 = {"CaoCao", (uint32_t)&CaoCao, USER_PROCESS};

static struct task_info *test_tasks[16] = { &task1, &task2, &task3,
                                            &task4, &task5, &task6,
                                            &task7, &task8, &task9,
                                            &task10, &task11, &task12,
                                            &task13, &task14, &task15};
static int num_test_tasks = 15;
*/

//Running project_4 from shell is recommended. You can also run it from loadboot.
struct task_info task1 = {"task1", (uint32_t)&drawing_task1, USER_PROCESS};
struct task_info task2 = {"task2", (uint32_t)&rw_task1, USER_PROCESS};

//static struct task_info *test_tasks[2] = {&task1, &task2};
struct task_info *test_tasks[2] = {&task1, &task2};


static int num_test_tasks = 2;

char cmd_buffer[MAX_LENGTH_OF_CMD];

unsigned long argv[6];

uint32_t current_line; // this is used to help the printk move to the correct location
uint32_t current_start_point; // the same reason as above. this is used to locate the x-axis

int my_strncmp(char a[], char b[], int length)
{
    int i;
    for(i = 0; i < length; i++)
    {
        if(a[i] == '\0' && b[i] != '\0')
        {
            /*printf("out of range. Error.\n");
            while(1);*/
            return -1;
        }
        else if(a[i] != '\0' && b[i] == '\0')
        {
            return 1;
        }
        else if(a[i] == '\0' && b[i] == '\0')
        {
            return 0;
        }
        
        
        
        if(a[i] > b[i])
            return 1;
        else if(a[i] < b[i])
            return -1;
        
        
    }

    return 0;
}

int my_ctoi(char *start)
{
    int num1, num2;
    int answer;
    num1 = (int)(*start - '0');
    num2 = (int)(*(start + 1) - '0');
    if(num1 > 9 || num1 < 0)
    {
        printk("param error.\n");
        while(1);
    }

    if(num2 <= 9 && num2 >= 0)
    {
        answer = 10*num1 + num2;
    }
    else
    {
        answer = num1;
    }

    return answer;
    
}

unsigned long my_ctoi_hex(const char* src, int *index)
{
	unsigned long s = 0;
	//int isMinus = 0;

	while (*src == ' ')  //跳过空白符
	{
		src++;
        (*index)++;
	}
    if(*src == '0' && *(src+1) == 'x')
    {
        src += 2;
        (*index) += 2;
    }
    else if (*src == '0' && *(src+1) == 'X')
    {
        src += 2;
        (*index) += 2;
    }

	if ((*src < '0' || *src > '9') && (*src < 'a' || *src > 'f') && (*src < 'A' || *src > 'F'))  //如果第一位既不是符号也不是数字，直接返回异常值
	{
		s = -999999;
		return s;
	}

	while (*src != '\0' && *src != ' ' && *src != '\r' && *src != '\n') 
	{
		if (*src >= '0' && *src <= '9')
		{
			s = s * 16 + *src - '0';
			src++;
            (*index)++;
		}
		else if (*src >= 'a' && *src <= 'f')
		{
			s = s * 16 + (*src - 'a' + 10);
			src++;
            (*index)++;
		}
		else if (*src >= 'A' && *src <= 'F')
		{
			s = s * 16 + (*src - 'A' + 10);
			src++;
            (*index)++;
		}
	}
	return s;
}

void do_cmd()
{
    if(my_strncmp(cmd_buffer, "ps", 3) == 0)
    {
        //disable_interrupt();
        //printk("\n");
        //enable_interrupt();
        printf("\n");
        sys_process_show();
    }
    else if(my_strncmp(cmd_buffer, "clear", 6) == 0)
    {
        sys_clear(SHELL_START, SCREEN_HEIGHT - 1); // shell is from SHELL_START to the bottom of screen
        current_line = SHELL_START - 1;
    }
    //emm...... shell may not need to execute 'exit' command
    /*else if(my_strncmp(cmd_buffer, "exit", 4) == 0)
    {
        sys_exit();
    }*/
    else if(my_strncmp(cmd_buffer, "exec", 4) == 0)
    {// cmd_buffer = "exec X"
        int exec_id;
        exec_id = (int)(cmd_buffer[5] - '0');
        if(exec_id > 9  || exec_id < 0)
        {
            current_line++;
            sys_move_cursor(1, current_line);
            printf("Undefined parameter. Contact the administrator to add it.");
        }
        else
        {
            int index3 = 0;
            int index4 = 0;
            exec_id = my_ctoi(&cmd_buffer[5]);
            if (exec_id < 10)
            {
                index3 = 7;
            }
            else
            {
                index3 = 8;
            }

            for(index4 = 0; index4 < 6; index4++)
            {
                //reset argv[]
                argv[index4] = 0;
            }
            index4 = 0;
            while(cmd_buffer[index3]!= '\r' && cmd_buffer[index3] != '\0' && cmd_buffer[index3] != '\n')
            {
                argv[index4] = my_ctoi_hex(&cmd_buffer[index3], &index3);
                index4++;
            }

            //test
            /*
            index4 = 0;
            sys_move_cursor(1, 1);
            printf("pid = %d\t", exec_id);
            for(; index4 <6; index4++)
                printf("argv[%d] = %u\t", index4, (unsigned)argv[index4]);
            printf("\n");
            while(1);
            */

            sys_spawn(test_tasks[exec_id], argv);
            current_line++;
            sys_move_cursor(1, current_line);
            printf("exec process[%d]", exec_id);
        }
    }
    else if(my_strncmp(cmd_buffer, "kill", 4) == 0)
    {// cmd_buffer = "kill X"
        int exec_id;
        exec_id = (int)(cmd_buffer[5] - '0');
        if(exec_id > 9  || exec_id < 0)
        {
            current_line++;
            sys_move_cursor(1, current_line);
            printf("Undefined parameter. Contact the administrator to add it.");
        }
        else
        {
            exec_id = my_ctoi(&cmd_buffer[5]);
            if(exec_id == 0 || exec_id == 1)
            {
                current_line++;
                sys_move_cursor(1, current_line);
                printf("You cannot kill 'idle' or 'shell'.");
            }
            else
            {
                sys_kill(exec_id);
                current_line++;
                sys_move_cursor(1, current_line);
                printf("kill process pid=%d", exec_id);
            }   
        }
    }
    

    else
    {
        /*disable_interrupt();
        current_line++;
        vt100_move_cursor(1, current_line);
        printk("Undefined command. Contact the administrator to add it.");   // show the new char on screen 
        
        //vt100_move_cursor(1, current_line);
        enable_interrupt();*/
        current_line++;
        sys_move_cursor(1, current_line);
        printf("Undefined command. Contact the administrator to add it.");   // show the new char on screen 

    }
}


void test_shell()
{
    int index1, index2;
    current_line = SHELL_START;
    current_start_point = sizeof(command_prompt); 


    /*disable_interrupt();
    vt100_move_cursor(1, SHELL_START - 1);
    printk("--------------------COMMAND--------------------\n");*/
    sys_move_cursor(1, SHELL_START - 1);
    printf("--------------------COMMAND--------------------\n");


    /*vt100_move_cursor(1, SHELL_START);
    printk(command_prompt);
    vt100_move_cursor(current_start_point, current_line); 
    printk(blank);
    enable_interrupt();*/
    
    
    sys_move_cursor(1, SHELL_START);
    printf(command_prompt);
    sys_move_cursor(current_start_point, current_line); 
    printf(blank);
    


    for(index1 = 0; index1 < MAX_LENGTH_OF_CMD; index1++)
        cmd_buffer[index1] = 0;
    
    index2 = 0;

// sizeof() take '\0' into consideration, so the answer is 1 larger than strlen(command_prompt)
// but the next character should start at the x = strlen(command_prompt) + 1 location
// so the "+1" can be used. 
    while (1)
    {
        // read command from UART port
        disable_interrupt();
        char ch = read_uart_ch();
        enable_interrupt();

        // TODO solve command
        if(ch == 8) // backspace
        {
            if(index2 != 0) // it means there are some characters in buffer, we should delete the last one
            {
                index2--;
                cmd_buffer[index2] = 0;
            }
            /*disable_interrupt();
            //printk("%c", ch);   // show the new char on screen
            vt100_move_cursor(current_start_point, current_line); // print blank firstly to cover the old string
            printk(blank);
            vt100_move_cursor(current_start_point, current_line); 
            printk(cmd_buffer);
            enable_interrupt();*/

            sys_move_cursor(current_start_point, current_line); // print blank firstly to cover the old string
            printf(blank);
            sys_move_cursor(current_start_point, current_line); 
            printf(cmd_buffer);

            continue;
        }
        else if(ch == 127) // delete
        {
            if(index2 != 0) // it means there are some characters in buffer, we should delete the last one
            {
                index2--;
                cmd_buffer[index2] = 0;
            }
            /*disable_interrupt();
            //printk("%c", ch);   // show the new char on screen
            vt100_move_cursor(current_start_point, current_line);
            printk(blank);
            vt100_move_cursor(current_start_point, current_line); // print blank firstly to cover the old string  
            printk(cmd_buffer);
            enable_interrupt();*/

            sys_move_cursor(current_start_point, current_line);
            printf(blank);
            sys_move_cursor(current_start_point, current_line); // print blank firstly to cover the old string  
            printf(cmd_buffer);

            continue;
        }
        else if(ch == 0) // nothing to be inputed, just wait......
        {
            continue;
        }
        else if(ch == '\r') // CR or LF? Unix kernel is LF, I tried using this char '\n'
        {
            //finish this input
            /*disable_interrupt();
            //printk("%c", ch);   // show the new char on screen
            vt100_move_cursor(current_start_point, current_line); 
            printk(blank); 
            vt100_move_cursor(current_start_point, current_line); // print blank firstly to cover the old string
            printk(cmd_buffer);
            enable_interrupt();*/

            sys_move_cursor(current_start_point, current_line); 
            printf(blank); 
            sys_move_cursor(current_start_point, current_line); // print blank firstly to cover the old string
            printf(cmd_buffer);

            if (index2 != 0)
            {
                do_cmd();

                for(index1 = 0; index1 < MAX_LENGTH_OF_CMD; index1++)
                    cmd_buffer[index1] = 0;
                index2 = 0;
                //printf("> root@UCAS_OS: ");
            }
            current_line++;
            /*disable_interrupt();
            vt100_move_cursor(1, current_line);
            
            printk(command_prompt);
            
            vt100_move_cursor(current_start_point, current_line); 
            printk(blank);
            
            enable_interrupt();*/
            sys_move_cursor(1, current_line);             
            printf(command_prompt);            
            sys_move_cursor(current_start_point, current_line); 
            printf(blank);
        }
        else
        {
            /* code */
            cmd_buffer[index2] = ch;
            index2++;
            /*disable_interrupt();
            vt100_move_cursor(current_start_point, current_line); 
            printk(blank); 
            vt100_move_cursor(current_start_point, current_line); // print blank firstly to cover the old string
            printk(cmd_buffer);   // show the new char on screen 
            enable_interrupt();*/

            sys_move_cursor(current_start_point, current_line); 
            printf(blank); 
            sys_move_cursor(current_start_point, current_line); // print blank firstly to cover the old string
            printf(cmd_buffer);   // show the new char on screen 

        }
        
        

    }
}
