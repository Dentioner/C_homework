#include "test_fs.h"
#include "stdio.h"
#include "string.h"
#include "fs.h"
#include "syscall.h"

static char buff[64];

extern uint32_t current_line;

void test_fs(void)
{
    
    int i, j;
    int fd = sys_fopen("1.txt", O_RDWR);
    uint32_t print_y = 2;
    uint32_t outer_y = current_line;

    if (fd == 123456) // debug
    {
        sys_exit();
    }

    for (i = 0; i < 10; i++)
    {
        sys_fwrite(fd, "hello world!\n", 13);
    }

    for (i = 0; i < 10; i++)
    {
        sys_fread(fd, buff, 13);
        sys_move_cursor(1, print_y + i);
        current_line = outer_y;
        for (j = 0; j < 13; j++)
        {
            
            printf("%c", buff[j]);
            //current_line = outer_y;
        }
    }

    sys_fclose(fd);
    current_line = outer_y;
    sys_exit();
}
