#include "GlobalDefine.h"
#include "Error.h"
#include "Common.h"
#include "I2c.h"

#define INTPND (*(volatile unsigned long*)0x4a000010)
#define SRCPND (*(volatile unsigned long*)0x4a000000)
#define INTMSK (*(volatile unsigned long*)0x4a000008)
#define GPECON (*(volatile unsigned long*)0x56000040)
#define GPEUP  (*(volatile unsigned long*)0x56000048)

#define IICCON    (*(volatile unsigned char*)0x54000000)
#define IICSTAT   (*(volatile unsigned char*)0x54000004)
#define IICDS     (*(volatile unsigned char*)0x5400000C)

#define SLAVE_WRITE_ADD 0xa0
#define SLAVE_READ_ADD 0xa1


void i2c_delay(int i)
{
   int j = 0;
   while (i--)	
   {
       for (j=0;j<100;j++)
       {	
           ;
       }  
   }	
}


void i2c_init()
{
    //1.a 初始化中断
    INTPND |= (1 << 27);
    SRCPND |= (1 << 27);  
    INTMSK &= ~(1 << 27);
     
    IICCON |= (1 << 5); 
    
    //1.b 设置scl时钟
    IICCON &= ~(1 << 6);
    IICCON &= ~(0xf << 0);
    IICCON |= (0x5 << 0);
    
    //2. 设置IICSTAT	
    IICSTAT |= (1 << 4);
    
    //3.设置引脚功能
    GPECON |= (0x2 << 28) | (0x2 << 30);
    GPEUP |= (0x3 << 14);
    
    //4.允许产生ACK
    IICCON |= (1 << 7);
}


void write_byte(unsigned char xchar, unsigned char daddr)
{
    
    //1. 设置处理器为主设备+发送模式
    IICSTAT |= (3 << 6);
    
    //2. 将从设备的地址写入到IICDS寄存器
    IICDS = SLAVE_WRITE_ADD;
    IICCON &= ~(1 << 4);
    
    //3. 写入0xF0写入IICSTAT
    IICSTAT = 0xF0;
    
    //4. 等待ACK的产生
    while ((IICCON & (1 << 4)) == 0 )
        i2c_delay(100);
    
    //5.1写入字节的地址到IICDS寄存器
    IICDS = daddr;
    IICCON &= ~(1 << 4);
    
    //5.2等待ACK的产生
    while ((IICCON & (1 << 4)) == 0 )
        i2c_delay(100);
    
    //6. 将要传输的字节数据写入IICDS寄存器
    IICDS = xchar;
    IICCON &= ~(1 << 4);   
    
    //8. 等待ACk的产生
    while ((IICCON & (1 << 4)) == 0 )
        i2c_delay(100);
    
    //9. 写入0xD0到IICSTAT
    IICSTAT = 0xD0;
    
    //10. 清除中断	
    IICCON &= ~(1 << 4);
    
    i2c_delay(100);
}

void read_data(unsigned char *buf, unsigned char daddr, int length)
{
    int j =0;
    unsigned char unusedata;
    
    //1. 设置为主设备发送模式
    IICSTAT |= (3<<6);
    
    //写入从设备地址
    IICDS = SLAVE_WRITE_ADD;
    IICCON &= ~(1<<4);
    
    //写入0xF0到IICSTAT
    IICSTAT = 0xF0;
    
    //等待ACK
    while ((IICCON & (1<<4)) == 0 )
        i2c_delay(100);
    
    //写入eeprom内部地址
    IICDS = daddr;
    IICCON &= ~(1<<4);
    
    //等待ACK
    while ((IICCON & (1<<4)) == 0 )
        i2c_delay(100);
            
    
    //设置为主设备接收模式
    IICSTAT &= ~(3<<6);
    IICSTAT |= (2<<6);
    
    
    //2.写入从设备地址到IICDS
    IICDS = SLAVE_READ_ADD;
    IICCON &= ~(1<<4);
    
    
    //3.写入0xB0到IICSTAT开始接收
    IICSTAT = 0xb0;
    while ((IICCON & (1<<4)) == 0 )
        i2c_delay(100);
        
    
    	/*写入设备内部地址*/
	IICDS = daddr;
	IICCON &= ~(1 << 4);
	while((IICCON & (1 << 4)) == 0)
	{
		i2c_delay(100);
	}	
	     
    
    //丢掉收到的第1个字节
 /*   unusedata = IICDS;
    IICCON &= ~(1<<4);
    while ((IICCON & (1<<4)) == 0 )
            i2c_delay(100);
 */   
    for(j=0;j<length;j++)
    {
        if(j == (length -1))
        {
           IICCON &= ~(1<<7); 		
        }
   
    //5.1 从IICDS里取出数据
        buf[j]=IICDS;
    
    //5.2 清除中断
        IICCON &= ~(1<<4);
    
    //4.等待中断
        while ((IICCON & (1<<4)) == 0 )
            i2c_delay(100);
    }
    	
    	
    //写入0x90到IICSTAT
    IICSTAT = 0x90;
    
 
    // 清除中断
    IICCON &= ~(1<<4);
}

MINI2440_STATUS I2cTest(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
    int i = 0;
    unsigned char sbuf[256] = {0};
    unsigned char dbuf[256] = {0};	
	
    i2c_init();
    
    for(i=0;i<256;i++)
    {
    	sbuf[i] = i;
    	dbuf[i] = 0;
    }
    
    printf_string("dbuf befor I2C read:\n");
    
    for(i =0; i<256;i++)
    {
       if(i%8==0)
           printf_string("\n");
           
       printf_string("%d\t",dbuf[i]);	
    }	
    
    for(i=0;i<256;i++)
        write_byte(sbuf[i], i);
        
    printf_string("i2c reading, plese wait!\n\r");
    
    read_data(dbuf, 0, 256);
    
    printf_string("dbuf after I2C read:\r\n");
    
    for(i =0; i<256;i++)
    {
       if(i%8==0)
           printf_string("\r\n");
           
       printf_string("%d\t",dbuf[i]);	
    }	
}
