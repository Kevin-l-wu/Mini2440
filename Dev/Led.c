#include "Led.h"
#include "Common.h"
#include "Uart.h"
#include "Framework.h"

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

void led_test(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	print_string("In function led_test\n");
	int index = 0;
	
	for(index = 0; index < LED_MAX_NUMBER; index++)
	{
		print_string("Logs\n");
		light_led(index);
		delay(500000);
		put_out_led(index);
	}
}
