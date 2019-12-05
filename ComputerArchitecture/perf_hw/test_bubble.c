#include <stdio.h>
#include <stdbool.h>

#define ARR_LEN 255 /* 數組長度上限 */
#define elemType int /* 元素類型 */

void bubbleSort (int arr[], int len)
{

	int i, j,temp;
	bool exchanged = true;
	
	for (i=0; exchanged && i<len-1; i++) /* 外迴圈為排序趟數，len個數進行len-1趟,只有交換過,exchanged值為true才有執行迴圈的必要,否則exchanged值為false不執行迴圈 */
		for (j=0; j<len-1-i; j++) 
		{ /* 內迴圈為每趟比較的次數，第i趟比較len-i次  */
		
		exchanged = false;
		
			if (arr[j] > arr[j+1])
			{ /* 相鄰元素比較，若逆序則互換（升序為左大於右，逆序反之） */
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
				exchanged = true; /*只有數值互換過, exchanged才會從false變成true,否則數列已經排序完成,exchanged值仍然為false,沒必要排序 */
			}
		}
}

int main (void) {
	int arr[ARR_LEN] = {3,5,1,-7,4,9,-6,8,10,4};
	int len = 10;
	int i;
	
	bubbleSort (arr, len);
	
	for (i=0; i<len; i++)
		printf ("%d\t", arr[i]);
	putchar ('\n');
	
	return 0;
}
