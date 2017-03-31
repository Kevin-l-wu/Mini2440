#include <stdio.h>
#include <stdarg.h>
#include "GlobalDefine.h"
#include "Common.h"
#include "Uart.h"


static char deci_convert_table[10] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

static char hex_convert_table[16] = 
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};


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
	int start_bit = 10;
	
	bit_values[9] = int_data % 10 / 1;
	bit_values[8] = int_data % 100 / 10;
	bit_values[7] = int_data % 1000 / 100;
	bit_values[6] = int_data % 10000 / 1000;
	bit_values[5] = int_data % 100000 / 10000;
	bit_values[4] = int_data % 1000000 / 100000;
	bit_values[3] = int_data % 10000000 / 1000000;
	bit_values[2] = int_data % 100000000 / 10000000;
	bit_values[1] = int_data % 1000000000 / 100000000;
	bit_values[0] = int_data / 1000000000;
	
	for(index = 0; index < 10; index++)
	{
		if(bit_values[index] != 0)
		{
			start_bit = index;
			break;
		}
	}
	
	for(index = start_bit; index < 10; index++)
	{
		uart0_send_data(bit_values[index] + '0');
	}
	
	if(start_bit == 10)
	{
		uart0_send_data('0');
	}
}

void print_unsinged_data(unsigned unsigned_data, PRINT_OUT_TYPE print_out_type)
{
	char bit_values[10] = {0};
	int index = 0;
	int start_store_bit = 0;
	
	PRINT_OUT print_out = {0}; 
	
	if(print_out_type == PrintOutTypeDec)
	{
		print_out.type = PrintOutTypeDec;
		print_out.bit_divesor = 10;
		print_out.convert_table = deci_convert_table;
		start_store_bit = 9;									//Deci need 10 char buffer;
	}
	else
	{
		print_out.type = PrintOutTypeHex;
		print_out.bit_divesor = 16;
		print_out.convert_table = hex_convert_table;
		start_store_bit = 7;									//Hex need 8 char buffer, so start store bit 7;
	}
	
	bit_values[start_store_bit] = unsigned_data / caculate_power(print_out.bit_divesor, start_store_bit);
	
	for(index = start_store_bit - 1; index >= 0; index--)
	{

		bit_values[index] = (char)((unsigned_data % caculate_power(print_out.bit_divesor, index + 1))
			/ caculate_power(print_out.bit_divesor, index));
	}

#ifdef DEBUG_ON	
	print_string("Dump bit_values\n");
	for(index = start_store_bit; index >= 0; index--)
	{		
		printf_string("bit_values[%d] = %d\n", index, bit_values[index]);
	}
#endif
	
	for(index = start_store_bit; index >= 0; index--)
	{	
		uart0_send_data(print_out.convert_table[bit_values[index]]);
	}
}

void print_int_data(int int_data, PRINT_OUT_TYPE print_out_type)
{
	char bit_values[10] = {0};
	int index = 0;
	int start_store_bit = 0;
	int int_data_temp = 0;
	int is_negative_value = 0;
	int start_output_bit = 0;		//For flagging first None 0 bit
	
	PRINT_OUT print_out = {0}; 
	
	//if int_data < 0, set is_negative_value to 1, others, set to 0
	if(int_data < 0)
	{
		int_data_temp = (~int_data) + 1;
		is_negative_value = 1;
	}
	else
	{
		int_data_temp = int_data;
		is_negative_value = 0;
	}
	
	if(print_out_type == PrintOutTypeDec)
	{
		print_out.type = PrintOutTypeDec;
		print_out.bit_divesor = 10;
		print_out.convert_table = deci_convert_table;
		start_store_bit = 9;									//Deci need 10 char buffer;
	}
	else
	{
		print_out.type = PrintOutTypeHex;
		print_out.bit_divesor = 16;
		print_out.convert_table = hex_convert_table;
		start_store_bit = 7;									//Hex need 8 char buffer, so start store bit 7;
	}
	
	bit_values[start_store_bit] = int_data_temp / caculate_power(print_out.bit_divesor, start_store_bit);
	
	for(index = start_store_bit - 1; index >= 0; index--)
	{
		bit_values[index] = int_data_temp % caculate_power(print_out.bit_divesor, index + 1)
			/ caculate_power(print_out.bit_divesor, index);
	}

#ifdef DEBUG_ON
	print_string("Dump bit_values\n");
	for(index = start_store_bit; index >= 0; index--)
	{		
		printf_string("bit_values[%d] = %d\n", index, bit_values[index]);
	}
#endif

	for(index = start_store_bit; index >= 0; index--)
	{	
		if(bit_values[index] != 0)
		{
			start_output_bit = index;
			break;
		}
	}
	
	if(is_negative_value == 1)
	{
		uart0_send_data('-');
	}
	
	for(index = start_output_bit; index >= 0; index--)
	{	
		uart0_send_data(print_out.convert_table[bit_values[index]]);
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
					char char_data;
					char* string_data;
					unsigned unsigned_data;
					
					case 'd':
						int_data = va_arg(arg_ptr, int);
						print_int_data(int_data, PrintOutTypeDec);
//						print_int_deci_data(int_data);
						break;
					case 'c':
						char_data = (char)va_arg(arg_ptr, int);
						uart0_send_data(char_data);
						break;
					case 's':
						string_data = (char*)va_arg(arg_ptr, char*);
						print_string(string_data);
						break;
					case 'x':
						unsigned_data = (unsigned)va_arg(arg_ptr, int);
						print_unsinged_data(unsigned_data, PrintOutTypeHex);
						break;
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
		for(index = 0; index < MAX_COMMAND_LENGTH * MAX_COMMAND_NUMBER; index++)
		{
			uart0_recv_data(&recv_string[index]);
//			uart0_send_data(recv_string[index]);
			if(recv_string[index] == '\n')
			{
				recv_string[index] = '\0';
				break;
			}
		}
	}
}
