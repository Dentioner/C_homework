void test(int &sum)
{
    int x, y;
    struct stack s;
    stack_initialize(s);
    scanf(x);
    while(x)
    {
        push(s, x);
        scanf(x);
    }
    
    sum = 0;
    while(s.top!=s.bottom)
    {
        pop(s, y)
        sum += y;
    }
    printf(sum);
}