#include <stdio.h>
#include <stdbool.h>

#define ARR_LEN 255
#define elemType int 

//快速排序
void quick_sort(int s[], int l, int r)
{
    if (l < r)
    {
		//Swap(s[l], s[(l + r) / 2]); //将中间的这个数和第一个数交换 参见注1
        int i = l, j = r, x = s[l];
        while (i < j)
        {
            while(i < j && s[j] >= x) // 从右向左找第一个小于x的数
				j--;  
            if(i < j) 
				s[i++] = s[j];
			
            while(i < j && s[i] < x) // 从左向右找第一个大于等于x的数
				i++;  
            if(i < j) 
				s[j--] = s[i];
        }
        s[i] = x;
        quick_sort(s, l, i - 1); // 递归调用 
        quick_sort(s, i + 1, r);
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

    quick_sort(arr, 0 , len);
	
	for (i=0; i<len; i++)
		printf ("%d\t", arr[i]);
	putchar ('\n');
	
	return 0;
}
