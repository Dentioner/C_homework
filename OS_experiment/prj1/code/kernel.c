char ch[14] = "Version:0000\n";
int value = 2019;

void __attribute__((section(".entry_function"))) _start(void)
{
	// Call PMON BIOS printstr to print message "Hello OS!"
	void (*printstr)(char *) = (void*)0x80011100;
	int res = 0;
	printstr("Hello OS! ");
	if(value >= 1000)
	{
			res = value / 1000;
			ch[8] += res;
			value -= res*1000;
	}

	if(value >= 100)
	{
			res = value / 100;
			ch[9] += res;
			value -= res*100;   
	}

	if (value >= 10)
	{
			res = value/10;
			ch[10] += res;
			value -= res*10;
	}

	if (value >= 1)
	{
			ch[11] += value;
	}

	printstr(ch);
	

	return;

/*	char* info= "Hello OS!\n";
	asm(
		"la	$a0, %0\n\t"
		"lw	$t2, 0x80011100\n\t"
		"jalr	$t2\n\t"
		"nop	\n\t"
		:
		:"r"(info)
		:"a0", "t2");
*/
	//printf("Hello OS!\n");
	//return;
}
