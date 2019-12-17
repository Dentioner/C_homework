#include "../../test/test_fs/test_fs.h"
#include "stdio.h"
#include "../../drivers/screen.h"
#include "type.h"


extern uint32_t current_line;

void do_mkfs()
{
    current_line+=2;
    vt100_move_cursor(1, current_line);



    printk("[FS] Start initialize file system!\n");current_line++;
    printk("[FS] Setting superblock...\n");current_line++;
    printk("asdsad\nsadsadsa\nsdasdsad\nasdsad");current_line++;current_line++;current_line++;
}

void do_statfs()
{
    current_line+=2;
    vt100_move_cursor(1, current_line);



    printk("magic: 1231232132\n");current_line++;
    printk("23333333\n123");current_line++;
}

void do_mkdir(uint32_t arg_filename)
{
    int index1;
    char *my_filename = (char *)arg_filename;
    current_line+=2;
    vt100_move_cursor(1, current_line);




    printk("in mkdir. filename:\n");current_line++;
    for(index1 = 0; (index1 < FILENAME_LENGTH) && (my_filename[index1] != '\0'); index1++)
    {
        printk("%c", my_filename[index1]);
    }

}

void do_rmdir(uint32_t arg_filename)
{
    int index1;
    char *my_filename = (char *)arg_filename;
    current_line+=2;
    vt100_move_cursor(1, current_line);





    printk("in rmdir. filename:\n");current_line++;
    for(index1 = 0; (index1 < FILENAME_LENGTH) && (my_filename[index1] != '\0'); index1++)
    {
        printk("%c", my_filename[index1]);
    }
}

void do_cd(uint32_t arg_filename)
{
    int index1;
    char *my_filename = (char *)arg_filename;
    current_line+=2;
    vt100_move_cursor(1, current_line);





    printk("in cd. filename:\n");current_line++;
    for(index1 = 0; (index1 < FILENAME_LENGTH) && (my_filename[index1] != '\0'); index1++)
    {
        printk("%c", my_filename[index1]);
    }
}

void do_ls()
{   
    current_line+=2;
    vt100_move_cursor(1, current_line);





    
    printk("in do_ls().\n");

}