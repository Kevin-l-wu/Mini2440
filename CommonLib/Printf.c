//#include <stdarg.h>
#include "Printf.h"

//static char gOutputBuf[PRINTF_MAX_OUTPUT];

int Printf(const char* fmt, ...)
{
/*	int index = 0;
	int length = 0;
	va_list args;
	va_start(args, fmt);
	
	length = vsprintf((char*)gOutputBuf, fmt, args);
	
	va_end(args);
	
	for(index = 0; index < length; index++)
	{
		uart0_send_data(gOutputBuf[index]);
	}

	return length;
*/	
}