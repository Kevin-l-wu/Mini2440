#include "GlobalDefine.h"
#include "Error.h"
#include "Interrupt.h"
#include "Dm9000.h"
#include "Uart.h"
#include "Network.h"

void udp_process(u8* buff, int length)
{
	UDP_HDR* udp_hdr_ptr = (UDP_HDR*)buff;
	
	tftp_process(buff, length, ADJUST_ENDIAN(udp_hdr_ptr->sport));
}

void ip_process(u8* arp_buff, int length)
{
	IP_HDR* ip_hdr_ptr = (IP_HDR*)arp_buff;
	
	switch(ip_hdr_ptr->proto)
	{
		case PROTO_UDP:
//			print_string("UDP IP packet received\n");
			udp_process(arp_buff, length);
			break;
			
		default:
//			print_string("Other IP packet received\n");
			break;
	}
}

int net_handle(char* rsv_buffer, int length)
{
	ETH_HDR* eth_ptr = (ETH_HDR*)rsv_buffer;
	
	switch(ADJUST_ENDIAN(eth_ptr->type))
	{
		case PROTO_ARP:
//			print_string("ARP packet received\n");
			arp_process(rsv_buffer, length);
			break;

		case PROTO_IP:
//			print_string("IP packet received\n");
			ip_process(rsv_buffer, length);
			break;

		default:
//			print_string("Other packet received\n");
			break;
	}
	
	return 0;
}