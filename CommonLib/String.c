#include <stdio.h>
#include "String.h"


/***********************************************************************
Function Name:  		strcmp
Input Paramater:		dest_str1, dest_str2
Function Description:	Compare string dest_str1 and dest_str2, if 
						dest_str1 > dest_str2: 	return 1
						dest_str1 < dest_str2: 	return -1
						dest_str1 = dest_str2: 	return 0
						Invalid Paramater:		return -2
************************************************************************/
int strcmp(char* dest_str1, char* dest_str2)
{
	int ret = 0;
	char* dest_str1_ptr = dest_str1;
	char* dest_str2_ptr = dest_str2;

	if(dest_str1 == NULL | dest_str2 == NULL)
	{
		return -2;
	}
		
	while(((*dest_str1_ptr) != '\0') && ((*dest_str2_ptr) != '\0'))
	{	
		if((*dest_str1_ptr) < (*dest_str2_ptr))
		{
	
			ret = -1;
			break;
		}
		if((*dest_str1_ptr) > (*dest_str2_ptr))
		{
			ret = 1;
			break;
		}
		dest_str1_ptr++;
		dest_str2_ptr++;
	}
	
	return ret;
}

/***********************************************************************
Function Name:  		strlen
Input Paramater:		dest_string
Function Description:	Caculate dest_str length
************************************************************************/
int strlen(char* dest_str)
{
	int str_len = 0;

	if(dest_str != NULL)
	{
		while(dest_str[str_len] != '\0')
		{
			str_len++;
		}
	}
	
	return str_len;
}

/***********************************************************************
Function Name:  		strcpy
Input Paramater:		dest_str, src_str
Function Description:	Copy src_str to dest_str, return the number char copy
************************************************************************/
int strcpy(char* dest_str, char* src_str)
{
	int index = 0;
	
	if(dest_str != NULL && src_str != NULL)
	{
		while(src_str[index] != '\0')
		{
			dest_str[index] = src_str[index];
			index++;
		}
	}
	
	return index;
}

/***********************************************************************
Function Name:  		memcpy
Input Paramater:		dest_str, src_str
Function Description:	Copy src_str to dest_str, return the number char copy
************************************************************************/
int memcpy(char* dest_str, char* src_str, int length)
{
	int index = length;
	
	if(dest_str != NULL && src_str != NULL)
	{
		while(index)
		{
			dest_str[index - 1] = src_str[index - 1];
			index--;
		}
	}
	
	return length;
}

/***********************************************************************
Function Name:  		clean_string
Input Paramater:		dest_string
Function Description:	Delete control char on the end of string
************************************************************************/
void clean_string(char* dest_string)
{
	if(dest_string != NULL)
	{
		while(*dest_string)
		{
			if(*dest_string == '\n')
			{
				*dest_string = '\0';
				break;
			}
			
			dest_string++;
		}
	}
}

/***********************************************************************
Function Name:  		memset
Input Paramater:		dest_string, set_char, length
Function Description:	Use set_char to set the memory dest_string
************************************************************************/
void memset(void* dest_string, int set_char, unsigned length)
{
	unsigned index = 0;
	char* dest_string_temp = dest_string;
	
	if(dest_string != NULL)
	{
		for(index = 0; index < length; index++)
		{
			*dest_string_temp = set_char;
			dest_string_temp++;
		}
	}
}

/***********************************************************************
Function Name:  		clean_string_prox
Input Paramater:		dest_string
Function Description:	Delete control char on the end of string
************************************************************************/
void clean_string_prox(char* dest_string)
{
	int str_index = 0;

	if(dest_string != NULL && dest_string[0] == '-' && dest_string[1] == '-')
	{
		while(dest_string[str_index + 2] != '\0')
		{
			dest_string[str_index] = dest_string[str_index + 1];
			dest_string[str_index + 1] = dest_string[str_index + 2];
			str_index++;
		}
	}
}
