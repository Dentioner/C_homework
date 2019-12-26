#include "string.h"

int strlen(char *src)
{
	int i;
	for (i = 0; src[i] != '\0'; i++)
	{
	}
	return i;
}

void memcpy(uint8_t *dest, uint8_t *src, uint32_t len)
{
	for (; len != 0; len--)
	{
		*dest++ = *src++;
	}
}

void os_memcpy(char *dest, char *src, uint32_t len)
{
	for (; len != 0; len--)
	{
		*dest++ = *src++;
	}
}

int my_strncmp(char a[], char b[], int length)
{
    int i;
    for(i = 0; i < length; i++)
    {
        if(a[i] == '\0' && b[i] != '\0')
        {
            /*printf("out of range. Error.\n");
            while(1);*/
            return -1;
        }
        else if(a[i] != '\0' && b[i] == '\0')
        {
            return 1;
        }
        else if(a[i] == '\0' && b[i] == '\0')
        {
            return 0;
        }
        
        
        
        if(a[i] > b[i])
            return 1;
        else if(a[i] < b[i])
            return -1;
        
        
    }

    return 0;
}


void memset(void *dest, uint8_t val, uint32_t len)
{
	uint8_t *dst = (uint8_t *)dest;

	for (; len != 0; len--)
	{
		*dst++ = val;
	}
}

void bzero(void *dest, uint32_t len)
{
	memset(dest, 0, len);
}

int strcmp(char *str1, char *str2)
{
	while (*str1 && *str2 && (*str1++ == *str2++))
	{
	};

	if (*str1 == '\0' && *str2 == '\0')
	{
		return 0;
	}

	if (*str1 == '\0')
	{
		return -1;
	}

	return 1;
}

char *strcpy(char *dest, char *src)
{
	char *tmp = dest;

	while (*src)
	{
		*dest++ = *src++;
	}

	*dest = '\0';

	return tmp;
}
