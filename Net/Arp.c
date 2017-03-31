#include <stdio.h>
#include "GlobalDefine.h"
#include "Error.h"
#include "Interrupt.h"
#include "Dm9000.h"
#include "Uart.h"
#include "Network.h"
#include "Arp.h"

//#define ADJUST_ENDIAN(val) (((val) & 0x00ff) << 8 | ((val) & 0xff00) >> 8) 


void arp_request()
{
	ARP_HDR arp_send_buf;
	u8 dst_ip_addr[4] = {192, 168, 1, 104}; 
	u8 src_ip_addr[4] = {192, 168, 1, 20};
	u8 dst_mac_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	u8 isr_status = 0;
	u8 dev_mac_addr[6] = {9, 8, 7, 6, 5, 4};
	
//	print_string("arp_request\n");
	
	/* 1. Construct arp request packet */
	memcpy(arp_send_buf.eth_hdr.d_mac, dst_mac_addr, 6);  	//Ethernet packet Destination mac addr, not known, should all 0xff
	memcpy(arp_send_buf.eth_hdr.s_mac, dev_mac_addr, 6);			//Ethernet packet Source mac addr
	arp_send_buf.eth_hdr.type = ADJUST_ENDIAN(0x0806);		//Ethernet packet type: Arp 0x0806

	arp_send_buf.hw_type = ADJUST_ENDIAN(0x01);				//Hardware type, ethernet = 1;
	arp_send_buf.protocol = ADJUST_ENDIAN(0x0800);			//IP protocol type
	arp_send_buf.hw_len = 6;									//Ethernet address(Mac addr) is 6
	arp_send_buf.protocol_len = 4;							//Ip address length is 4
	arp_send_buf.opcode = ADJUST_ENDIAN(0x01);				//Opcode = 1, request
	
	memcpy(arp_send_buf.src_mac, dev_mac_addr, 6);				//Send mac addr
	memcpy(arp_send_buf.src_ip_addr, src_ip_addr, 4);
	memcpy(arp_send_buf.dst_mac, dst_mac_addr, 6);				
	memcpy(arp_send_buf.dst_ip_addr, dst_ip_addr, 4);		
	
	/* 2. Send request packet*/
	dm9000_send((u8*)&arp_send_buf, (14 + 28));
}

void arp_process(u8* arp_buff, int length)
{
	u32 i = 0;
	u8* ptr = NULL;
	ARP_HDR* arp_buff_ptr = (ARP_HDR*)arp_buff;
	u8 dev_mac_addr[6] = {9, 8, 7, 6, 5, 4};

	u8 src_ip_addr[4] = {192, 168, 1, 20};
	ARP_HDR arp_send_buf;
	u8 host_ip_addr[4];
	u8 host_mac_addr[6];
	
	switch(ADJUST_ENDIAN(arp_buff_ptr->opcode))
	{
//		printf_string("arp_buff_ptr->opcode = 0x%x\n", ADJUST_ENDIAN(arp_buff_ptr->opcode));
		case 1:	/* Arp request packet */
			/* 1. Construct arp ack packet */
			memcpy(arp_send_buf.eth_hdr.d_mac, arp_buff_ptr->src_mac, 6);
			memcpy(arp_send_buf.eth_hdr.s_mac,arp_buff_ptr->dst_mac, 6);
			arp_send_buf.eth_hdr.type = ADJUST_ENDIAN(0x0806);

			arp_send_buf.hw_type = ADJUST_ENDIAN(0x01);
			arp_send_buf.protocol = ADJUST_ENDIAN(0x0800);
			arp_send_buf.hw_len = 6;
			arp_send_buf.protocol_len = 4;
			arp_send_buf.opcode = ADJUST_ENDIAN(0x02);
			
			memcpy(arp_send_buf.src_mac, dev_mac_addr, 6);
			memcpy(arp_send_buf.src_ip_addr, src_ip_addr, 4);
			
			memcpy(arp_send_buf.dst_mac, arp_buff_ptr->src_mac, 6);				
			memcpy(arp_send_buf.dst_ip_addr, arp_buff_ptr->src_ip_addr, 4);		
			
			/* 2. Send request packet*/			
			dm9000_send((u8*)&arp_send_buf, (14 + 28));
			
			break;

		case 2: /* Arp ack packet */
			memcpy(host_ip_addr, (*arp_buff_ptr).src_ip_addr, 4);
			memcpy(host_mac_addr, (*arp_buff_ptr).src_mac, 6);
			
			print_string("\nHost ip = ");
			for(i = 0; i < 4; i++)
			{
				printf_string("%d ", host_ip_addr[i]);
			}
			
			print_string("\nHost mac = ");
			for(i = 0; i < 6; i++)
			{
				printf_string("%C ", host_mac_addr[i]);
			}
			print_string("\n");
			
			break;
			
		default:
			break;
	}
}

MINI2440_STATUS arp_test(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	
	u32 isr_status = 0;
	char recv_char[200] = {'\0'};
	int test_times = 10;
	
	dm9000_init();
	
	print_string("arp_test\n");
	
	delay(1000000);
	
	while(test_times--)
	{
		arp_request();
		
		delay(200000);
	}

	dm9000_int_clear();
	
	return status;
}



