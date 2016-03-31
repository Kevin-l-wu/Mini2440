#ifndef LED_H_
#define LED_H_

#define LED_MAX_NUMBER 4

/*Led relevant GPIO define*/
#define GPBCON (*(volatile unsigned long*)0x56000010)
#define GPBDAT (*(volatile unsigned long*)0x56000014)

void light_led(int led_number);
void led_test();

#endif
