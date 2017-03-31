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
    //1.a ��ʼ���ж�
    INTPND |= (1 << 27);
    SRCPND |= (1 << 27);  
    INTMSK &= ~(1 << 27);
     
    IICCON |= (1 << 5); 
    
    //1.b ����sclʱ��
    IICCON &= ~(1 << 6);
    IICCON &= ~(0xf << 0);
    IICCON |= (0x5 << 0);
    
    //2. ����IICSTAT	
    IICSTAT |= (1 << 4);
    
    //3.�������Ź���
    GPECON |= (0x2 << 28) | (0x2 << 30);
    GPEUP |= (0x3 << 14);
    
    //4.�������ACK
    IICCON |= (1 << 7);
}


void write_byte(unsigned char xchar, unsigned char daddr)
{
    
    //1. ���ô�����Ϊ���豸+����ģʽ
    IICSTAT |= (3 << 6);
    
    //2. �����豸�ĵ�ַд�뵽IICDS�Ĵ���
    IICDS = SLAVE_WRITE_ADD;
    IICCON &= ~(1 << 4);
    
    //3. д��0xF0д��IICSTAT
    IICSTAT = 0xF0;
    
    //4. �ȴ�ACK�Ĳ���
    while ((IICCON & (1 << 4)) == 0 )
        i2c_delay(100);
    
    //5.1д���ֽڵĵ�ַ��IICDS�Ĵ���
    IICDS = daddr;
    IICCON &= ~(1 << 4);
    
    //5.2�ȴ�ACK�Ĳ���
    while ((IICCON & (1 << 4)) == 0 )
        i2c_delay(100);
    
    //6. ��Ҫ������ֽ�����д��IICDS�Ĵ���
    IICDS = xchar;
    IICCON &= ~(1 << 4);   
    
    //8. �ȴ�ACk�Ĳ���
    while ((IICCON & (1 << 4)) == 0 )
        i2c_delay(100);
    
    //9. д��0xD0��IICSTAT
    IICSTAT = 0xD0;
    
    //10. ����ж�	
    IICCON &= ~(1 << 4);
    
    i2c_delay(100);
}

void read_data(unsigned char *buf, unsigned char daddr, int length)
{
    int j =0;
    unsigned char unusedata;
    
    //1. ����Ϊ���豸����ģʽ
    IICSTAT |= (3<<6);
    
    //д����豸��ַ
    IICDS = SLAVE_WRITE_ADD;
    IICCON &= ~(1<<4);
    
    //д��0xF0��IICSTAT
    IICSTAT = 0xF0;
    
    //�ȴ�ACK
    while ((IICCON & (1<<4)) == 0 )
        i2c_delay(100);
    
    //д��eeprom�ڲ���ַ
    IICDS = daddr;
    IICCON &= ~(1<<4);
    
    //�ȴ�ACK
    while ((IICCON & (1<<4)) == 0 )
        i2c_delay(100);
            
    
    //����Ϊ���豸����ģʽ
    IICSTAT &= ~(3<<6);
    IICSTAT |= (2<<6);
    
    
    //2.д����豸��ַ��IICDS
    IICDS = SLAVE_READ_ADD;
    IICCON &= ~(1<<4);
    
    
    //3.д��0xB0��IICSTAT��ʼ����
    IICSTAT = 0xb0;
    while ((IICCON & (1<<4)) == 0 )
        i2c_delay(100);
        
    
    	/*д���豸�ڲ���ַ*/
	IICDS = daddr;
	IICCON &= ~(1 << 4);
	while((IICCON & (1 << 4)) == 0)
	{
		i2c_delay(100);
	}	
	     
    
    //�����յ��ĵ�1���ֽ�
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
   
    //5.1 ��IICDS��ȡ������
        buf[j]=IICDS;
    
    //5.2 ����ж�
        IICCON &= ~(1<<4);
    
    //4.�ȴ��ж�
        while ((IICCON & (1<<4)) == 0 )
            i2c_delay(100);
    }
    	
    	
    //д��0x90��IICSTAT
    IICSTAT = 0x90;
    
 
    // ����ж�
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
