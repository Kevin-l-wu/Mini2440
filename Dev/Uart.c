#include <stdio.h>
#include <stdarg.h>
#include "Common.h"
#include "Uart.h"

void uart0_init()
{
    GPHCON &= ~(0xf<<4);
    GPHCON |= (0xa<<4);
	
	*(ULCON0) = 0x03;
	*(UCON0) = 0x05;
	UBRDIV0 = 0x1a;
}

void uart0_send_data(char data)
{
	while (!(UTRSTAT0 & (1<<1)));
	UTXH0 = data;
}

void uart0_recv_data(char* data)
{
	while(!(UTRSTAT0 & 0x01));
	*data = (URXH0);
}

void print_string(const char* string)
{
	const char* string_temp = string;
	if(string_temp != NULL)
	{
		while(*string_temp)
		{
			uart0_send_data(*string_temp);
			string_temp++;
		}
	}
}

void print_int_deci_data(int int_data)
{
	char bit_values[10] = {0};
	char bit_value = 0;
	int bit_divesor = 10;
	int index = 9;
	
	bit_values[9] = int_data % 10 / 1;
	bit_values[8] = int_data % 100 / 10;
	bit_values[7] = int_data % 1000 / 100;
	bit_values[6] = int_data % 10000 / 1000;
	bit_values[5] = int_data % 100000 / 10000;
	bit_values[4] = int_data % 1000000 / 100000;
	bit_values[3] = int_data % 10000000 / 1000000;
	bit_values[2] = int_data % 100000000 / 10000000;
	bit_values[1] = int_data % 1000000000 / 100000000;
	bit_values[1] = int_data / 1000000000;
	
	for(index = 0; index < 10; index++)
	{
		uart0_send_data(bit_values[index] + '0');
	}
}

void printf_string(const char* string, ...)
{
	const char* string_temp = string;
	va_list arg_ptr;
	va_start(arg_ptr, string);
	
	if(string_temp != NULL)
	{
		while(*string_temp)
		{
			if(*string_temp == '%')
			{
				switch(*(string_temp + 1))
				{
					int int_data;
//					char char_data;
//					char* string_data;
//					unsigned unsigned_data;
					
					case 'd':
						int_data = va_arg(arg_ptr, int);
						print_int_deci_data(int_data);
						break;
/*					case 'c':
						char_data = (char)va_arg(arg_ptr, int);
						uart0_send_data(char_data);
						break;
					case 's':
						string_data = (char*)va_arg(arg_ptr, char*);
						print_string(string_data);
					case 'x':
						unsigned_data = va_arg(arg_ptr, unsigned);
*/						
					default:
						break;
				}
				string_temp++;
			}else
			{
				uart0_send_data(*string_temp);
			}
					
			string_temp++;
		}
	}
	
	va_end(arg_ptr);
}

void uart0_recv_string(char* recv_string)
{
	int index = 0;
	
	if(recv_string != NULL)
	{
		for(index = 0; index < MAX_COMMOND; index++)
		{
			uart0_recv_data(&recv_string[index]);
//			uart0_send_data(recv_string[index]);
			if(recv_string[index] == '\n')
			{
				recv_string[index + 1] = '\0';
				break;
			}
		}
	}
}
