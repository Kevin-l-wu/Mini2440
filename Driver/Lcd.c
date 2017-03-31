#include <stdio.h>
#include "GlobalDefine.h"
#include "Template.h"
#include "Error.h"
#include "Uart.h"
#include "Lcd.h"

/**************************************************************
The initial and control for 640×480 16Bpp TFT LCD----VGA
**************************************************************/
#define rGPCCON		(*(volatile unsigned *)0x56000020) //Port C control
#define rGPCDAT		(*(volatile unsigned *)0x56000024) //Port C data
#define rGPCUP		(*(volatile unsigned *)0x56000028) //Pull-up control C

#define rGPDCON		(*(volatile unsigned *)0x56000030) //Port D control
#define rGPDDAT		(*(volatile unsigned *)0x56000034) //Port D data
#define rGPDUP		(*(volatile unsigned *)0x56000038) //Pull-up control D

#define rGPGCON		(*(volatile unsigned *)0x56000060) //Port G control
#define rGPGDAT		(*(volatile unsigned *)0x56000064) //Port G data
#define rGPGUP		(*(volatile unsigned *)0x56000068) //Pull-up control G

// LCD CONTROLLER
#define rLCDCON1	(*(volatile unsigned *)0x4d000000) //LCD control 1
#define rLCDCON2	(*(volatile unsigned *)0x4d000004) //LCD control 2
#define rLCDCON3	(*(volatile unsigned *)0x4d000008) //LCD control 3
#define rLCDCON4	(*(volatile unsigned *)0x4d00000c) //LCD control 4
#define rLCDCON5	(*(volatile unsigned *)0x4d000010) //LCD control 5

#define rLCDSADDR1	(*(volatile unsigned *)0x4d000014) //STN/TFT Frame buffer start address 1
#define rLCDSADDR2	(*(volatile unsigned *)0x4d000018) //STN/TFT Frame buffer start address 2
#define rLCDSADDR3	(*(volatile unsigned *)0x4d00001c) //STN/TFT Virtual screen address set
#define rREDLUT		(*(volatile unsigned *)0x4d000020) //STN Red lookup table
#define rGREENLUT	(*(volatile unsigned *)0x4d000024) //STN Green lookup table 
#define rBLUELUT	(*(volatile unsigned *)0x4d000028) //STN Blue lookup table
#define rDITHMODE	(*(volatile unsigned *)0x4d00004c) //STN Dithering mode
#define rTPAL		(*(volatile unsigned *)0x4d000050) //TFT Temporary palette
#define rLCDINTPND	(*(volatile unsigned *)0x4d000054) //LCD Interrupt pending
#define rLCDSRCPND	(*(volatile unsigned *)0x4d000058) //LCD Interrupt source
#define rLCDINTMSK	(*(volatile unsigned *)0x4d00005c) //LCD Interrupt mask
#define rTCONSEL	(*(volatile unsigned *)0x4d000060) //LPC3600 Control --- edited by junon


#define U8 unsigned char
#define U16 unsigned short
#define U32 unsigned int


//------------Lcd controller 1 set-----------
#define CLKVAL 4

//------------Lcd controller 2 set-----------
#define VBPD 1
#define LINEVAL 320
#define VFPD 5
#define VSPW 1

//------------Lcd controller 3 set------------
#define HBPD 36
#define HOZVAL 240
#define HFPD 19

//------------Lcd controller 4 set------------

#define HSPW 5

#define SCR_XSIZE HOZVAL
#define SCR_YSIZE LINEVAL

#define LCD_ADDR ((U32)lcd_buffer)
#define LCD_CON5 ((1<<11) | (1 << 9) | (1 << 8) | (1 << 3) | (1 << 0))

unsigned short lcd_buffer[SCR_YSIZE][SCR_XSIZE];

/**************************************************************
320×240 TFT LCD数据和控制端口初始化
**************************************************************/
static void lcd_port_init()
{
    rGPCUP = 0xffffffff; // Disable Pull-up register
    rGPCCON = 0xaaaa02a8; //Initialize VD[7:0],VM,VFRAME,VLINE,VCLK

    rGPDUP = 0xffffffff; // Disable Pull-up register
    rGPDCON = 0xaaaaaaaa; //Initialize VD[15:8]
}

/**************************************************************
320×240 TFT LCD功能模块初始化
**************************************************************/
static void lcd_init()
{
	rLCDCON1 = (CLKVAL << 8) | (3 << 5) | (12 << 1);
	rLCDCON2 = (VBPD << 24) | ((LINEVAL - 1) << 14) | (VFPD << 6) | (VSPW << 0);
	rLCDCON3 = (HBPD << 19) | ((HOZVAL - 1) << 8) | (HFPD << 0);
	rLCDCON4 = (13 << 8) | (HSPW << 0);

    rLCDCON5 = LCD_CON5;

	//Frame buffer start address A[30:22] A[21:1]
    rLCDSADDR1 = ((LCD_ADDR >> 22) << 21) | (((LCD_ADDR >> 1) & 0x1fffff) << 0);
    
	//Frame buffer end address A[21:1]
	rLCDSADDR2 = ((LCD_ADDR + HOZVAL * LINEVAL * 2) & 0x1fffff) >> 1;
    
	rLCDSADDR3 = HOZVAL;       

    rLCDINTMSK |= 3;
	rTCONSEL &= (~7);

	rTPAL = 0;
	rTCONSEL &= ~((1<<4) | 1);
}

/**************************************************************
LCD视频和控制信号输出或者停止，1开启视频输出
**************************************************************/
static void lcd_output_enable(int onoff)
{
    if(onoff == 1)
	{
		rLCDCON1 |= 0x01; // ENVID=ON
    }
	else
	{
		rLCDCON1 &= (~0x01); // ENVID Off
	}
}

/**************************************************************
320×240 8Bpp TFT LCD 电源控制引脚使能
**************************************************************/
static void lcd_power_enable(int invpwren, int pwren)
{
    //GPG4 is set as LCD_PWREN
    rGPGUP = rGPGUP | (1 << 4); // Pull-up disable
    rGPGCON = rGPGCON | (3 << 8); //GPG4=LCD_PWREN
    
    //Enable LCD POWER ENABLE Function
    rLCDCON5 = rLCDCON5 & (~(1 << 3)) | (pwren << 3);   // PWREN
    rLCDCON5 = rLCDCON5 & (~(1 << 5)) | (invpwren << 5);   // INVPWREN
}


static void lcd_fill_pixel(int y, int x, U16 content)
{
	if(y < SCR_YSIZE && x < SCR_XSIZE)
	{
		lcd_buffer[y][x] = content ;
	}
}

static void lcd_fill_line(int line_number, U16 content)
{
	int index = 0;
	
	if(line_number < SCR_YSIZE)
	{
		for(index = 0; index < SCR_XSIZE; index++)
		{
			lcd_buffer[line_number][index] = content;
		}
	}
}

static void lcd_fill_column(int column_number, U16 content)
{
	int index = 0;
	
	if(column_number < SCR_XSIZE)
	{
		for(index = 0; index < SCR_YSIZE; index++)
		{
			lcd_buffer[index][column_number] = content;
		}
	}
}

/**************************************************************
320×240 TFT LCD全屏填充特定颜色单元或清屏
**************************************************************/
static void lcd_clear_screen(U16 color)
{
	unsigned int x, y;
  
    for(y = 0; y < (SCR_YSIZE); y++)
    {
		for(x = 0; x < (SCR_XSIZE); x++)
		{
			lcd_buffer[y][x] = color ;
		}
    }
}

void dump_register()
{
	print_string("Dump register:\n");
	printf_string("rLCDCON1 = %x\n", rLCDCON1);
	printf_string("rLCDCON2 = %x\n", rLCDCON2);
	printf_string("rLCDCON3 = %x\n", rLCDCON3);
	printf_string("rLCDCON4 = %x\n", rLCDCON4);
	printf_string("rLCDCON5 = %x\n", rLCDCON5);
	
	printf_string("rLCDSADDR1 = %x\n", rLCDSADDR1);
	printf_string("rLCDSADDR2 = %x\n", rLCDSADDR2);
	printf_string("rLCDSADDR3 = %x\n", rLCDSADDR3);
	
	printf_string("rLCDINTMSK = %x\n", rLCDINTMSK);
	
	printf_string("rTCONSEL = %x\n", rTCONSEL);
	printf_string("rTPAL = %x\n", rTPAL);
}

void lcd_line_test(int start_line, int end_line, U16 content)
{
	int line_index = 0;
	int column_index = 0;
	int color_index = 0;
	
	printf_string("\nlcd_line_test, start_line = %d, end_line = %d\n", start_line, end_line);
	
	printf_string("Color: %d\n\n", content);
	for(line_index = start_line; line_index < end_line; line_index++)
	{
		printf_string("Fill line: %d\n\n", line_index);
		lcd_fill_line(line_index, content);
		
		delay(1000);
	}
	delay(1500000);
}

void lcd_column_test(int start_column, int end_column, U16 content)
{
	int line_index = 0;
	int column_index = 0;
	int color_index = 0;
	
	printf_string("\nlcd_column_test, start_column = %d, end_column = %d\n", start_column, end_column);

	printf_string("Color: %d\n\n", content);
	for(column_index = start_column; column_index < end_column; column_index++)
	{
		printf_string("Fill column: %d\n\n", column_index);
		lcd_fill_column(column_index, content);
		
		delay(1000);
	}
	delay(1500000);
}

void display_char(int column_number, int line_number, unsigned short* template)
{
	int x = 0, y = 0;
	printf_string("\n\ndisplay_char()\n\n");
	
	for(y = line_number; y < (line_number + 16); y++)
	{
		for(x = column_number; x < (column_number + 8); x++)
		{
			printf_string("x = %d, y = %d\n\n", x, y);
			lcd_buffer[y][x] = template[(x - column_number) + (y - line_number) * 8];
		}
		delay(1000);
	}
}

void display_char_with_color(int column_number, int line_number, char ascii_char, unsigned short color)
{
	TEMPLATE8X16* ascii_template = NULL;
	
	get_template(ascii_char, color, &ascii_template);
	
	display_char(column_number, line_number, *ascii_template);
}

void display_string_with_color(int column_number, int line_number, char* ascii_string, unsigned short color)
{
	int char_no = 0;
	
	if(ascii_string != NULL)
	{
		while(*ascii_string != '\0')
		{
			display_char_with_color(column_number + 16 * char_no, line_number, *ascii_string, color);
			char_no++;
			ascii_string++;
		}
	}
}


MINI2440_STATUS lcd_test(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	int line_index = 0;
	int column_index = 0;
	int color_index = 0;
	TEMPLATE8X16* ascii_template;

	lcd_port_init();
	
	lcd_init();
	
	lcd_power_enable(0, 1);
	lcd_output_enable(1);
	
	print_string("Fill all screen test\n");
	
	for(color_index = 0; color_index < 15; color_index++)
	{
		printf_string("Color: %d\n\n", 0x03 << color_index);
		lcd_clear_screen(0x03 << color_index); 
		
		delay(500000);
	}
	
	lcd_clear_screen(0x0000);

	print_string("\nlcd line test\n");
	
	lcd_line_test(0, 80, 0x0f);
	lcd_line_test(80, 160, 0x0f << 4);
	lcd_line_test(168, 169, 0x0f << 8);
	lcd_line_test(240, 320, 0x0f << 12);
	
	lcd_clear_screen(0x0000);
	
	print_string("\nlcd column test\n");
	
	lcd_column_test(0, 60, 0x0f);
	lcd_column_test(60, 120, 0x0f << 4);
	lcd_column_test(125, 126, 0x0f << 8);
	lcd_column_test(180, 240, 0x0f << 12);
	
	lcd_clear_screen(0x0000);
	
	print_string("\nDisplay template\n");
	display_char(50 + 16 * 0, 120 + 0, global_ascii_template[0].ascii_template);
	display_char(50 + 16 * 1, 120 + 0, global_ascii_template[1].ascii_template);
	display_char(50 + 16 * 2, 120 + 0, global_ascii_template[2].ascii_template);
	display_char(50 + 16 * 3, 120 + 0, global_ascii_template[3].ascii_template);
	display_char(50 + 16 * 4, 120 + 0, global_ascii_template[4].ascii_template);
	display_char(50 + 16 * 5, 120 + 0, global_ascii_template[5].ascii_template);
	display_char(50 + 16 * 6, 120 + 0, global_ascii_template[6].ascii_template);
	display_char(50 + 16 * 7, 120 + 0, global_ascii_template[7].ascii_template);
	display_char(50 + 16 * 8, 120 + 0, global_ascii_template[8].ascii_template);
	display_char(50 + 16 * 9, 120 + 0, global_ascii_template[9].ascii_template);
	
	display_char(50 + 16 * 0, 120 + 32, global_ascii_template[10].ascii_template);
	display_char(50 + 16 * 1, 120 + 32, global_ascii_template[11].ascii_template);
	display_char(50 + 16 * 2, 120 + 32, global_ascii_template[12].ascii_template);
	display_char(50 + 16 * 3, 120 + 32, global_ascii_template[13].ascii_template);
	display_char(50 + 16 * 4, 120 + 32, global_ascii_template[14].ascii_template);
	display_char(50 + 16 * 5, 120 + 32, global_ascii_template[15].ascii_template);
	display_char(50 + 16 * 6, 120 + 32, global_ascii_template[16].ascii_template);
	display_char(50 + 16 * 7, 120 + 32, global_ascii_template[17].ascii_template);
	display_char(50 + 16 * 8, 120 + 32, global_ascii_template[18].ascii_template);
	display_char(50 + 16 * 9, 120 + 32, global_ascii_template[19].ascii_template);
	
	display_char(50 + 16 * 0, 120 + 64, global_ascii_template[20].ascii_template);
	display_char(50 + 16 * 1, 120 + 64, global_ascii_template[21].ascii_template);
	display_char(50 + 16 * 2, 120 + 64, global_ascii_template[22].ascii_template);
	display_char(50 + 16 * 3, 120 + 64, global_ascii_template[23].ascii_template);
	display_char(50 + 16 * 4, 120 + 64, global_ascii_template[24].ascii_template);
	display_char(50 + 16 * 5, 120 + 64, global_ascii_template[25].ascii_template);
	
	delay(2000000);
	
	lcd_clear_screen(0x0000);
	
	get_template('A', 0xf800, &ascii_template);
	
	display_char(50 + 16 * 0, 120 + 64, *ascii_template);
	
	get_template('B', 0x001f, &ascii_template);
	
	display_char(50 + 16 * 1, 120 + 64, *ascii_template);
	
	get_template('C', 0x07e0, &ascii_template);
	
	display_char(50 + 16 * 2, 120 + 64, *ascii_template);
	
	printf_string("ascii_template = 0x%x\n", ascii_template);
	printf_string("*ascii_template = 0x%x\n", *ascii_template);
	
	delay(2000000);
	
	lcd_clear_screen(0x0000);
	
	display_string_with_color(50 + 16 * 0, 20 + 32 * 0, "DIAGVERSION", 0x07e0);
	display_string_with_color(50 + 16 * 0, 20 + 32 * 1, "SN", 0xf800);
	display_string_with_color(50 + 16 * 0, 20 + 32 * 2, "LEDTEST", 0x001f);
	display_string_with_color(50 + 16 * 0, 20 + 32 * 3, "LCDTEST", 0x07e0);
	display_string_with_color(50 + 16 * 0, 20 + 32 * 4, "NANDTEST", 0xf800);
	
	delay(2000000);
	
	return status;
}
