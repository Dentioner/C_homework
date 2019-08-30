#include <stdio.h>


int var_global_init_0 = 0;
int var_global_init = 2;
int var_global_un_init;
static int var_static_global_un_init;
static int var_static_global_init = 5;
static int var_static_global_init_0 = 0;
int func()
{
    static int var_static_local_un_init;
    static int var_static_local_init = 5;
    static int var_static_local_init_0 = 0;
    return 0;
}

int main()
{
    func();
    return 0;
} 
