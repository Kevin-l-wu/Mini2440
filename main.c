#include <stdio.h>
#include <stdarg.h>

#include "Common.h"
#include "Led.h"
#include "Uart.h"
#include "Interrupt.h"
#include "String.h"
#include "Framework.h"
#include "Getopt.h"

int raise(int a)
{

}

char gArgv[MAX_COMMAND_NUMBER][MAX_COMMAND_LENGTH] = {'\0'};
int gArgc = 0;

int gboot_main()
{	
	char receive_from_terminal;
	char receive_string[MAX_COMMAND_LENGTH] = {'\0'};
	int a = 10;
	int index = 0;

	uart0_init();

	print_string("---------------- Mini2440 Diags ----------------\n");
	print_string(":-) \n");
/*
	printf_string("Hello world %d", 20);
	printf_string("Hello world %c", 'a');
	printf_string("Hello world %s", "Nihao");
*/	
	while(1)
	{
		int index = 0;
		uart0_recv_string(receive_string);					//Receive command string from terminal
		
		print_string(receive_string);
		clean_string(receive_string);
		GenerateArgv(&gArgc, &gArgv, receive_string);
		print_string("gArgc = ");
		print_int_deci_data(gArgc);
		print_string("\n");
		print_string("gArgv = ");
		print_string("\n");
		
		for(index = 0; index < gArgc; index++)
		{
			print_string(gArgv[index]);
			print_string("\n");
		}
		
		printf_string("Hello world %d", 20);
		printf_string("Hello world %c", 'a');
		printf_string("Hello world %s", "Nihao");
		
		handle_event_commond(receive_string, gArgc, gArgv);
		print_string(":-) ");
	}

    return 0;    
}


