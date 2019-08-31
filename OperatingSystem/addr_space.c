#include <stdio.h>
#include <stdlib.h>

char *myname="Chen Mingyu";
char gdata[128];
char bdata[16] = {1,2,3,4};
main() {
	char * ldata[16];	
	char * ddata;
	
	ddata = malloc(16);
	system("cat /proc/self/maps");
	printf("\n\n");
	printf("gdata: %llX\nbdata:%llX\nldata:%llx\nddata:%llx\n",
		gdata,bdata,ldata,ddata);
	system("cat /proc/self/maps");
	free(ddata);
	printf("\n\n");
	system("cat /proc/self/maps");
}
