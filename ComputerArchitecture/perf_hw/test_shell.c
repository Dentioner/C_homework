#include <stdio.h>
#include <stdbool.h>

#define ARR_LEN 255
#define elemType int 

void shell_sort(int arr[], int len) {
	int gap, i, j;
	int temp;
	for (gap = len >> 1; gap > 0; gap >>= 1)
		for (i = gap; i < len; i++) {
			temp = arr[i];
			for (j = i - gap; j >= 0 && arr[j] > temp; j -= gap)
				arr[j + gap] = arr[j];
			arr[j + gap] = temp;
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


	//bubbleSort (arr, len);
	shell_sort(arr, len);


	for (i=0; i<len; i++)
		printf ("%d\t", arr[i]);
	putchar ('\n');
	
	return 0;
}
