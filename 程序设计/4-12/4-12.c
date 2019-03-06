#include<stdio.h>
#include<ctype.h>
#include<math.h>
#include<stdlib.h>
#define MAXLEN 1000000
char c_number[MAXLEN];
int full_digit;
int ten_pow(int index)
{
    int answer = 1;
    for (;index > 0; index --)
    {
        answer*=10;
    }
    return answer;
}
void initialize()
{
    int i;
    for (i = 0; i < MAXLEN; i++)
        c_number[i] = '\0';
}
void itoa(int number, int digit)
{
    //Ĭ��Ϊʮ������
    int i;
    if (digit != 0)
    {
        i = number / ten_pow(digit - 1);
        c_number[full_digit - digit] = (char)(i+'0');
        number = number % ten_pow(digit - 1);
        itoa(number, digit - 1);
    }
    else
    {
        return;
    }
}
int find_digit(int number)
{
    int digit;
    for (digit = 0; digit<MAXLEN && number > 0; digit ++)
    {
        number = number / 10;
    }
    full_digit = digit;
    return digit;
}
int main()
{
    int number, digit;
	initialize();
    printf("������һ��С��%d������\n", MAXLEN);
	scanf("%d", &number);
    digit = find_digit(number);
    printf("�������������%d", number);
    itoa(number, digit);
    printf("ת��֮����ַ�������ˣ�\n");
    printf(c_number);
    printf("\n");
    system("pause");
	return 0;
}
