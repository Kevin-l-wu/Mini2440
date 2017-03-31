#include "Error.h"
#include "Common.h"
#include "Uart.h"
#include "Framework.h"
#include "Led.h"

/***********************************************************************
Function Name:  		led_init
Input Paramater:		None
Function Description:	Led initialize
************************************************************************/
void led_init()
{
	//Set GPIOB output 
	GPBCON &= ~(0xff << 10);
	GPBCON |= (0x55 << 10);
	
	//Set GPIOB led bit 1
	GPBDAT |= (0x0f << 5);
}

/***********************************************************************
Function Name:  		led_print_char
Input Paramater:		value
Function Description:	Display low 4 bit of value
************************************************************************/
void led_print_char(char value)
{
	GPBDAT = (~(value & 0x0f)) << 5;
}

/***********************************************************************
Function Name:  		light_led
Input Paramater:		led_number
Function Description:	Light the led_number led
************************************************************************/
void light_led(int led_number)
{
	unsigned gpbcon_set_value = GPBCON;
	unsigned gpbdat_set_value = GPBDAT;
	
	gpbcon_set_value = gpbcon_set_value & (~(0x00000003 << ((led_number + 5) * 2)));
	gpbcon_set_value = gpbcon_set_value | (0x00000001 << ((led_number + 5) * 2));
	
	gpbdat_set_value = gpbdat_set_value & (~(0x00000001 << (led_number + 5)));
	
	GPBCON = gpbcon_set_value;
	GPBDAT = gpbdat_set_value;
}

/***********************************************************************
Function Name:  		put_out_led
Input Paramater:		led_number
Function Description:	Put out the led_number led
************************************************************************/
void put_out_led(int led_number)
{
	unsigned gpbcon_set_value = GPBCON;
	unsigned gpbdat_set_value = GPBDAT;
	
	gpbcon_set_value = gpbcon_set_value & (~(0x00000003 << ((led_number + 5) * 2)));
	gpbcon_set_value = gpbcon_set_value | (0x00000001 << ((led_number + 5) * 2));
	
	gpbdat_set_value = gpbdat_set_value | (0x00000001 << (led_number + 5));
	
	GPBCON = gpbcon_set_value;
	GPBDAT = gpbdat_set_value;
}

/***********************************************************************
Function Name:  		led_test
Input Paramater:		argc, argv
Function Description:	Led function test
************************************************************************/
MINI2440_STATUS led_test(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	print_string("In function led_test\n");
	MINI2440_STATUS status = MINI2440_SUCCESS;
	int index = 0;
	
	for(index = 0; index < LED_MAX_NUMBER; index++)
	{
		print_string("Logs\n");
		light_led(index);
		delay(500000);
		put_out_led(index);
	}
	
	return status;
}
