#ifndef UART_H_
#define UART_H_

/*Uart relevant register define*/
#define GPHCON (*(volatile unsigned long*)0x56000070)
#define CLKCON ((volatile unsigned long*)0x4c00000c)
#define ULCON0 ((volatile unsigned long*)0x50000000)
#define UCON0 ((volatile unsigned long*)0x50000004)
#define UFCON0 ((volatile unsigned long*)0x50000008)
#define UMCON0 ((volatile unsigned long*)0x5000000c)
#define UTRSTAT0 (*(volatile unsigned long*)0x50000010)
#define UTXH0 (*(volatile unsigned long*)0x50000020)
#define URXH0 (*(volatile unsigned long*)0x50000024) 
#define UBRDIV0 (*(volatile unsigned long*)0x50000028)

#define MAX_COMMOND 10

void uart0_init();
void uart0_send_data(char data);
void uart0_recv_data(char* data);
void print_string(const char* string);
void printf_string(const char* string, ...);

#endif
