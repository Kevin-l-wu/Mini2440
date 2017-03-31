#include "GlobalDefine.h"
#include "Interrupt.h"
#include "Error.h"
#include "Button.h"


void InitButtonIrq()
{
	unsigned cpsr_value = 0;
	
	print_string("Button Irq Init\n");
	
	/* Configure GPIOF */
	GPFCON &= ~(0x03 << 0) & (0x03 << 6) & (0x03 << 10) & (0x03 << 12) & (0x03 << 14) & (0x03 << 22);
	GPFCON |= (0x02 << 0) | (0x02 << 6) | (0x02 << 10) | (0x02 << 12) | (0x02 << 14) | (0x02 << 22);
   
   // Do not mask in EINTMASK
	(EINTMASK) &= (~(1<<8)) & (~(1<<11)) & (~(1<<13)) & (~(1<<14) & (~(1<<15)) & (~(1<<19))) ;
	
    // Do not set EINT8_23
    (INTMSK)   &= ~(1<<5);
    
    __asm__( 
    /*IRQ enable*/  
    "mrs r0, cpsr\n"
    "bic r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"
	"mov %[cpsr_value1], r0\n"
    : [cpsr_value1] "=r" (cpsr_value)
    :
    );
	printf_string("Cpsr = 0x%x\n", cpsr_value);
	
	EINTPEND = ~(0x00);
}

void ClearButtonIrq()
{	
	print_string("Clear Button Irq\n");
	
	/* Configure GPIOF */
	GPFCON &= ~(0x03 << 0) & (0x03 << 6) & (0x03 << 10) & (0x03 << 12) & (0x03 << 14) & (0x03 << 22);
   
   // Do not mask in EINTMASK
	(EINTMASK) |= (1 << 8) | (1 << 11) | (1 << 13) | (1 << 14) | (1 << 15) | (1 << 19) ;
	
    // Do not set EINT8_23
    (INTMSK) |= (1<<5);
	
	EINTPEND = ~(0x00);
}

void button_irq()
{
	unsigned value = 0;
	
	value = (EINTPEND);
	
	switch(value)
	{
		case (1 << 8):
			light_led(0);
			break;
		
		case (1 << 11):
			put_out_led(0);
			break;
		
		case (1 << 13):
			light_led(1);
			break;
		
		case (1 << 14):
			put_out_led(1);
			break;
		
		case (1 << 15):
			light_led(2);
			break;
		
		case (1 << 19):
			put_out_led(2);
			break;
			
		default:
			break;
	}
}

/*******************************************************************
 * Function Name: 	 	button_test
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Button Test
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS button_test(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	unsigned wait_times = 100;
	print_string("\n--------------- Button Test ----------------\n");
	
	InitButtonIrq();
	
	while(wait_times--)
	{
		delay(500000);
		print_string(".");
	}
	
	ClearButtonIrq();
	
	print_string("\n--------------- Button Test Over ----------------\n");
	
	return status;
}