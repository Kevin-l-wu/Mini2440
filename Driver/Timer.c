#include "GlobalDefine.h"
#include "Error.h"
#include "Common.h"
#include "Interrupt.h"
#include "Led.h"
#include "Timer.h"

/* Timer input clock Frequency = PCKL / (prescaler value + 1) / (divider value) */
static int timer0IntCount = 0;
static int timer0TimeOut = 0;
static int timer0TimeOutFlag = 0;

static int timer1IntCount = 0;
static int timer1TimeOut = 0;
static int timer1TimeOutFlag = 0;

static int timer2IntCount = 0;
static int timer2TimeOut = 0;
static int timer2TimeOutFlag = 0;

static int timer3IntCount = 0;
static int timer3TimeOut = 0;
static int timer3TimeOutFlag = 0;

static int timer4IntCount = 0;
static int timer4TimeOut = 0;
static int timer4TimeOutFlag = 0;

void TimerRegDump()
{
	printf_string("---------------------- Timer register -----------------------\n");
	
	printf_string("TCFG0 = 0x%x\n", TCFG0);
	printf_string("TCFG1 = 0x%x\n", TCFG1);
	printf_string("TCON = 0x%x\n", TCON);
//	printf_string("TCMPB0 = 0x%x\n", TCMPB0);
	printf_string("TCNTB4 = 0x%x\n", TCNTB4);
	
	printf_string("-------------------------------------------------------------\n");
}


static void DeadZoneSet(unsigned char value)
{
	TCFG0 &= (~(0xff << 16));
	TCFG0 |= (value);
}

/* Prescaler Set */
static void PrescalerSet(PRESCALER prescaler, unsigned char prescalerVal)
{
	if(prescaler == PRESCALER0)
	{
		TCFG0 &= (~(0xff));
		TCFG0 |= (0xff & prescalerVal);
	}
	else if(prescaler == PRESCALER1)
	{
		TCFG0 &= (~(0xff << 8));
		TCFG0 |= ((0xff << 8) & (prescalerVal << 8));
	}
	else
	{
		printf_string("Timer: Invalid prescaler No.\n");
	}
}

/* Divider Set */
static void DividerSet(TIMER timer, unsigned char dividerVal)
{
	switch(timer)
	{
		case TIMER0:
			TCFG1 = ((dividerVal & 0x0f) << 0);
			break;
			
		case TIMER1:
			TCFG1 = ((dividerVal & 0x0f) << 4);
			break;
			
		case TIMER2:
			TCFG1 = ((dividerVal & 0x0f) << 8);
			break;
			
		case TIMER3:
			TCFG1 = ((dividerVal & 0x0f) << 12);
			break;
			
		case TIMER4:
			TCFG1 = ((dividerVal & 0x0f) << 16);
			break;
			
		default:
			printf_string("Timer: Invalid timer No.\n");
			break;
	}
}

/* DMA mode set */
static void DmaModeSet(DMA_TIMER value)
{
	TCFG1 &= (~(0x0f << 20));
	TCFG1 |= (value & 0x0f);
}

void TimerSetOriginalVal(TIMER timer, unsigned short tcmpb, unsigned tcnpb)
{
	switch(timer)
	{
		case TIMER0:
			TCMPB0 = tcmpb;
			TCNTB0 = tcnpb;
			TCON &= (~(0x01 << 1));
			TCON |= (0x01 << 1);
			break;
			
		case TIMER1:
			TCMPB1 = tcmpb;
			TCNTB1 = tcnpb;
			TCON &= (~(0x01 << 9));
			TCON |= (0x01 << 9);
			break;
			
		case TIMER2:
			TCMPB2 = tcmpb;
			TCNTB2 = tcnpb;
			TCON &= (~(0x01 << 13));
			TCON |= (0x01 << 13);
			break;
			
		case TIMER3:
			TCMPB3 = tcmpb;
			TCNTB3 = tcnpb;
			TCON &= (~(0x01 << 17));
			TCON |= (0x01 << 17);
			break;
			
		case TIMER4:
			TCNTB4 = tcnpb;
			TCON &= (~(0x01 << 21));
			TCON |= (0x01 << 21);
			TCON &= (~(0x01 << 21));
			break;
			
		default:
			printf_string("Timer: Invalid timer No.\n");
			break;
	}
}

static void TimerIsReloadSet(TIMER timer, unsigned char isReload)
{
	switch(timer)
	{
		case TIMER0:			
			isReload ? (TCON |= (0x01 << 3)) : (TCON &= (~(0x01 << 3)));
			break;
			
		case TIMER1:
			isReload ? (TCON |= (0x01 << 11)) : (TCON &= (~(0x01 << 11)));
			break;
			
		case TIMER2:
			isReload ? (TCON |= (0x01 << 15)) : (TCON &= (~(0x01 << 15)));
			break;
			
		case TIMER3:
			isReload ? (TCON |= (0x01 << 19)) : (TCON &= (~(0x01 << 19)));
			break;
			
		case TIMER4:
			isReload ? (TCON |= (0x01 << 22)) : (TCON &= (~(0x01 << 22)));
			break;
			
		default:
			printf_string("Timer: Invalid timer No.\n");
			break;
	}
}

static void TimerOutputIsRevert(TIMER timer, unsigned char isRevert)
{
	switch(timer)
	{
		case TIMER0:			
			isRevert ? (TCON |= (0x01 << 2)) : (TCON &= (~(0x01 << 2)));
			break;
			
		case TIMER1:
			isRevert ? (TCON |= (0x01 << 10)) : (TCON &= (~(0x01 << 10)));
			break;
			
		case TIMER2:
			isRevert ? (TCON |= (0x01 << 14)) : (TCON &= (~(0x01 << 14)));
			break;
			
		case TIMER3:
			isRevert ? (TCON |= (0x01 << 18)) : (TCON &= (~(0x01 << 18)));
			break;
			
		case TIMER4:
			printf_string("Timer: Not support\n");
			break;
			
		default:
			printf_string("Timer: Invalid timer No.\n");
			break;
	}
}

void TimerStartStop(TIMER timer, unsigned char isStart)
{
	switch(timer)
	{
		case TIMER0:			
			isStart ? (TCON |= (0x01 << 0)) : (TCON &= (~(0x01 << 0)));
			break;
			
		case TIMER1:
			isStart ? (TCON |= (0x01 << 8)) : (TCON &= (~(0x01 << 8)));
			break;
			
		case TIMER2:
			isStart ? (TCON |= (0x01 << 12)) : (TCON &= (~(0x01 << 12)));
			break;
			
		case TIMER3:
			isStart ? (TCON |= (0x01 << 16)) : (TCON &= (~(0x01 << 16)));
			break;
			
		case TIMER4:
			isStart ? (TCON |= (0x01 << 20)) : (TCON &= (~(0x01 << 20)));
			break;
			
		default:
			printf_string("Timer: Invalid timer No.\n");
			break;
	}
}

/* Use Timer4 delay n seconds*/
void DelaySeconds(TIMER timer, int seconds)
{
	/* 0. Some data initialize */
	timer4IntCount = 0;
	timer4TimeOut = seconds;
	timer4TimeOutFlag = 0;
	
	/* 1. Set prescaler value = 49 */
	PrescalerSet(PRESCALER1, 49);
	
	/* 2. Set divider value = 0x03 */
	DividerSet(TIMER4, 0x03);
	
	/* 3. Set original value */
	TimerSetOriginalVal(TIMER4, 0, 62500);
	
	/* 4. Set if auto reload */
	TimerIsReloadSet(TIMER4, 1);
	
	/* 5. Start timer */
	TimerStartStop(TIMER4, 1);
	
	/* 6. Enable Interrupt */
	ClearPending(INT_TIMER4_IRQ_INDEX);
	EnableIrq(INT_TIMER4_IRQ_INDEX);
}

/* Use Timer4 delay n seconds*/
void DelayMs(TIMER timer, int ms)
{
	/* 0. Some data initialize */
	timer4IntCount = 0;
	timer4TimeOut = ms;
	timer4TimeOutFlag = 0;
	
	/* 1. Set prescaler value = 49 */
	PrescalerSet(PRESCALER1, 49);
	
	/* 2. Set divider value = 0x03 */
	DividerSet(TIMER4, 0x03);
	
	/* 3. Set original value */
	TimerSetOriginalVal(TIMER4, 0, 63);
	
	/* 4. Set if auto reload */
	TimerIsReloadSet(TIMER4, 1);
	
	/* 5. Start timer */
	TimerStartStop(TIMER4, 1);
	
	/* 6. Enable Interrupt */
	ClearPending(INT_TIMER4_IRQ_INDEX);
	EnableIrq(INT_TIMER4_IRQ_INDEX);
}

int isTimeOut(TIMER timer)
{
	int ret = 0;
	
	if(timer4TimeOutFlag != 0)
	{
		ret = 1;
	}
	else
	{
		ret = 0;
	}
	
	return ret;
}

void Timer0Interrupt()
{
	ClearPending(INT_TIMER0_IRQ_INDEX);
	
	if((timer0IntCount % 2) == 0)
	{
		light_led(0);
	}
	else
	{
		put_out_led(0);
	}
	
	timer0IntCount++;
}

void Timer1Interrupt()
{
	
}

void Timer2Interrupt()
{
	
}

void Timer3Interrupt()
{
	
}

void Timer4Interrupt()
{
	ClearPending(INT_TIMER4_IRQ_INDEX);
	
	if((timer4IntCount % 2) == 0)
	{
		light_led(0);
	}
	else
	{
		put_out_led(0);
	}
	
	if(timer4IntCount >= timer4TimeOut)
	{
		TimerStartStop(TIMER4, 0);
		timer4TimeOutFlag = 1;
		timer4TimeOut = 0;
		timer4IntCount = 0;
	}
	else
	{
		timer4TimeOutFlag = 0;
		timer4IntCount++;
	}
}

MINI2440_STATUS TimerTest(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	int delayTimes = 20;
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	DelaySeconds(TIMER4, 10);
	
	while(!isTimeOut(TIMER4))
	{
		delay(1000);
		printf_string("Timer4 start: Interrupt reg dump----\n");
	}

	TimerStartStop(TIMER4, 0);
	
	DelayMs(TIMER4, 5000);
	
	while(!isTimeOut(TIMER4))
	{
		printf_string("timer4IntCount = %d\n", timer4IntCount);
		printf_string("timer4TimeOut = %d\n", timer4TimeOut);
		printf_string("timer4TimeOutFlag = %d\n", timer4TimeOutFlag);
	}

	TimerStartStop(TIMER4, 0);

	return status;
}