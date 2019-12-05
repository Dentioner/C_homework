#include <stdio.h>
#include <stdbool.h>

#define ARR_LEN 255
#define elemType int 

//��������
void quick_sort(int s[], int l, int r)
{
    if (l < r)
    {
		//Swap(s[l], s[(l + r) / 2]); //���м��������͵�һ�������� �μ�ע1
        int i = l, j = r, x = s[l];
        while (i < j)
        {
            while(i < j && s[j] >= x) // ���������ҵ�һ��С��x����
				j--;  
            if(i < j) 
				s[i++] = s[j];
			
            while(i < j && s[i] < x) // ���������ҵ�һ�����ڵ���x����
				i++;  
            if(i < j) 
				s[j--] = s[i];
        }
        s[i] = x;
        quick_sort(s, l, i - 1); // �ݹ���� 
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
