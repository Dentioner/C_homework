#include<stdio.h>

unsigned int shld5(unsigned int a, unsigned int b)
{

	unsigned int result;

	//result = (a << 5) | (b >> (32 - 5)); //´ËÓï¾äÓÃÇ¶ÈëÊ½»ã±à±àĞ´    
	asm(
	"SHL 5, %1\n\t"
		"SHR 27, %2\n\t"
		"OR %1, %2\n\t"
		"MOV %2, %0"
		: "=r"(result)
		: "r"(a), "r"(b)
		);


	return result;
}

int main()
{
	unsigned int a, b, result;
	printf("a=? b=?\n");
	scanf("%u %u", &a, &b);
	result = shld5(a, b);
	printf("%u\n", result);
	printf("real result = %u\n", (a << 5) | (b >> (32 - 5)));
	system("pause");
	return 0;
}