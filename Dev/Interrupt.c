#include "Interrupt.h"

void init_irq()
{

    // ��EINTMASK�Ĵ�����ʹ������
    *(EINTMASK) &= (~(1<<8)) & (~(1<<11)) & (~(1<<13)) & (~(1<<14));

    // EINT8_23ʹ��
    *(INTMSK)   &= ~(1<<5);
    
    __asm__( 
    /*���ж�*/  
    "mrs r0,cpsr\n"
    "bic r0, r0, #0x80\n"
    "msr cpsr_c, r0\n"            
    : 
    : 
  );
}