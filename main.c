#include <stdio.h>
#include <stdarg.h>
#include "GlobalDefine.h"
#include "Common.h"
#include "Error.h"
#include "Led.h"
#include "Uart.h"
#include "Interrupt.h"
#include "String.h"
#include "Framework.h"
#include "GetoptTemp.h"

extern char bss_start[];
extern char bss_end[];
//extern char* bss_start;

int raise(int a)
{

}

char gArgv[MAX_COMMAND_NUMBER][MAX_COMMAND_LENGTH] = {'\0'};
int gArgc = 0;

int gboot_main()
{
	char receive_string[MAX_COMMAND_LENGTH * MAX_COMMAND_NUMBER] = {'\0'};
	int a = 10;
	int index = 0;
	char get_data = 0;
	
	Uart0Init();
	
	EnableMasterIrq();
	
//	touch_init();
	
//	dma_init();
	
	print_string("\n---------------- Mini2440 Diags ----------------\n");
	printf_string("bss_start = 0x%x\n", bss_start);
	printf_string("bss_end = 0x%x\n", bss_end);
	print_string(":-) ");

	while(1)
	{
		int index = 0;
	
		memset(receive_string, 0, MAX_COMMAND_LENGTH * MAX_COMMAND_NUMBER); 	// Clear command line buffer
		uart0_recv_string(receive_string);										// Receive command string from terminal

		print_string(receive_string);
		GenerateArgv(&gArgc, &gArgv, receive_string);
#ifdef DEBUG_ON
		printf_string("\ngArgc = %d\n", gArgc);
		
		for(index = 0; index < gArgc; index++)
		{
			printf_string("gArgv[%d] = %s\n", index, gArgv[index]);
		}
#endif
		handle_event_commond(receive_string, gArgc, gArgv);
		
		print_string(":-) ");
	}

    return 0;    
}
