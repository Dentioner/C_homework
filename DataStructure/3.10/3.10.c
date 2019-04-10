void test(int &sum)
{
    int x;
    scanf(x);
    if (x == 0)
        sum = 0;
    else
    {
        {
            int x;
            scanf(x)
            if(x == 0)
                sum = 0;
            else
            {
            ...//another test(sum)
            sum += x;
            }
        }//test(sum);
        sum += x;
    }
    printf(sum);
}