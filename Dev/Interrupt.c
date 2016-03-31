#include "Interrupt.h"

void init_irq()
{

    // 在EINTMASK寄存器中使能它们
    *(EINTMASK) &= (~(1<<8)) & (~(1<<11)) & (~(1<<13)) & (~(1<<14));

    // EINT8_23使能
    *(INTMSK)   &= ~(1<<5);
    
    __asm__( 
    /*开中断*/  
    "mrs r0,cpsr\n"
    "bic r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"            
    : 
    : 
  );
}