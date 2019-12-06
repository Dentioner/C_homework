#include <stdio.h>
#include <stdbool.h>

#define ARR_LEN 255
#define elemType int 

void bubbleSort (int arr[], int len)
{

	int i, j,temp;
	bool exchanged = true;
	
	for (i=0; exchanged && i<len-1; i++) 
		for (j=0; j<len-1-i; j++) 
		{
		
		exchanged = false;
		
			if (arr[j] > arr[j+1])
			{ 
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
				exchanged = true; 
			}
		}
}

int main (void) {
	int arr[ARR_LEN] ;
	int len = ARR_LEN;
	int i;
	int index1;
	for(index1 = 0;index1 < ARR_LEN ; index1++)
	{
		arr[index1] = ARR_LEN - index1;
	}


	bubbleSort (arr, len);
	
	for (i=0; i<len; i++)
		printf ("%d\t", arr[i]);
	putchar ('\n');
	
	return 0;
}
