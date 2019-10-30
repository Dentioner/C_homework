#include "time.h"
#include "test3.h"
#include "lock.h"
#include "sched.h"
#include "stdio.h"
#include "syscall.h"

static char blank[] = {"                                                "};

//mutex_lock_t lock_array[0];
//mutex_lock_t lock_array[1];
static int has_init = 0;

// pid = 2
void ready_to_exit_task()
{

    int i = 0, print_location = 0;

    if(has_init == 0)
    {

        mutex_lock_init(&lock_array[0]);
        mutex_lock_init(&lock_array[1]);
        lock_array[0].lock_self_id = 0;
        lock_array[1].lock_self_id = 1;
        has_init = 1;
    }
    mutex_lock_acquire(&lock_array[0]);
    mutex_lock_acquire(&lock_array[1]);

    // sys_spawn(&task1);
    // sys_spawn(&task2);

    for (i = 0; i < 4000; i++)
    {
        sys_move_cursor(0, print_location);
        printf("> [TASK] I am task with pid %d, I have acquired two mutex lock. (%d)", current_running->pid, i++);
    }
    sys_exit(); // test exit

	//while(1);//test for using kill() to manually exit this function
    
}


// pid = 3
void wait_lock_task()
{
    int i, print_location = 1;

    if(has_init == 0)
    {

        mutex_lock_init(&lock_array[0]);
        mutex_lock_init(&lock_array[1]);
        lock_array[0].lock_self_id = 0;
        lock_array[1].lock_self_id = 1;
        has_init = 1;
    }
    sys_move_cursor(0, print_location);
    printf("> [TASK] I want to acquire a mute lock from task(pid=2).");

    mutex_lock_acquire(&lock_array[0]);

    sys_move_cursor(0, print_location);
    printf("> [TASK] I have acquired a mutex lock from task(pid=2).");

    sys_exit(); // test exit

}
// pid = 4
void wait_exit_task()
{
    int i, print_location = 2;

    if(has_init == 0)
    {

        mutex_lock_init(&lock_array[0]);
        mutex_lock_init(&lock_array[1]);
        lock_array[0].lock_self_id = 0;
        lock_array[1].lock_self_id = 1;       
        has_init = 1;
    }

    sys_move_cursor(0, print_location);
    printf("> [TASK] I want to wait task (pid=2) to exit.");

    sys_waitpid(2); //test waitpid

    sys_move_cursor(0, print_location);
    printf("> [TASK] Task (pid=2) has exited.                ");

    sys_exit(); // test exit

}

