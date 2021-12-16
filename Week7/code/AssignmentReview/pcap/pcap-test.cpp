#include "pcap-test.h"

void usage() {
	printf("syntax: pcap-test <interface>\n");
	printf("sample: pcap-test wlan0\n");
}

bool parse(Param* param, int argc, char* argv[]) {
	if (argc != 2) {
		usage();
		return false;
	}
	param->dev_ = argv[1];
	return true;
}

bool memdump(uint8_t* mem, uint32_t len){
	if (0xff < len){
		printf("memdump : too long length(0x%u)\n",len);
		return false;
	}
	printf("Memory Dump\n");
	printf("      00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f  0123456789ABCDEF\n");

	int idx = 0;
	while (len != idx)
	{
		if(idx % 0x10 == 0) printf("0x%02x  ", idx);
		printf("%02x ", *(mem + idx));
		if(idx % 0x10 == 0xf) {
			printf(" ");
			for (int i = -15; i <= 0; i++) {
				if ( 31 < *(mem+idx+i) &&  *(mem+idx+i) < 127 ) printf("%c", *(mem+idx+i));
				else printf(".");
			}
			printf("\n");
		}

		idx++;
	}
	
	if(idx % 0x10 != 0){
		for (int i = -(0x10 - idx%0x10) + 1; i <= 0; i++) printf("   ");
		printf(" ");

		for (int i = -(idx%0x10) + 1; i <= 0; i++) {
			if ( 31 < *(mem+idx+i) &&  *(mem+idx+i) < 127 ) printf("%c", *(mem+idx+i));
			else printf(".");
		}
	}
	printf("\n");

	return true;
}

bool isTCP(PacketHeader* packet){
	printf("ether_type: 0x%04x\n", ntohs(packet->ether_type));
	printf("ip_protocol: 0x%02x\n", packet->ip_protocol);
	// is IPv4?
	if(ntohs(packet->ether_type) != 0x0800) return false;

	// is TCP?
	if(packet->ip_protocol != 0x06) return false;

	printf("TCP Packet Captured\n");
	printf("\n");
	return true;
}

bool PrintEtherHeader(PacketHeader* packet){
	printf("Ethernet II\n");
	printf("\tSrc MAC: %02x:%02x:%02x:%02x:%02x:%02x\n"	,packet->ether_shost[0]
													,packet->ether_shost[1]
													,packet->ether_shost[2]
													,packet->ether_shost[3]
													,packet->ether_shost[4]
													,packet->ether_shost[5]);
	printf("\tDst MAC: %02x:%02x:%02x:%02x:%02x:%02x\n"	,packet->ether_dhost[0]
													,packet->ether_dhost[1]
													,packet->ether_dhost[2]
													,packet->ether_dhost[3]
													,packet->ether_dhost[4]
													,packet->ether_dhost[5]);
	return true;
}

bool PrintIPDatagram(PacketHeader* packet){
	printf("IP Datagram\n");
	uint8_t* ip_saddr = (uint8_t*)&packet->ip_saddr;
	uint8_t* ip_daddr = (uint8_t*)&packet->ip_daddr;
	printf("\tSrc IP: %d.%d.%d.%d\n",	*(ip_saddr+0)
							,*(ip_saddr+1)
							,*(ip_saddr+2)
							,*(ip_saddr+3));
	printf("\tDst IP: %d.%d.%d.%d\n",	*(ip_daddr+0)
							,*(ip_daddr+1)
							,*(ip_daddr+2)
							,*(ip_daddr+3));
	
	return true;
}

bool PrintTCPSegment(PacketHeader* packet, uint32_t cap_len){
	printf("TCP Segment\n");
	printf("\tSrc Port: %d\n", ntohs(packet->tcp_sport));
	printf("\tDst Port: %d\n", ntohs(packet->tcp_sport));
	printf("\n");

	printf("Data size: %ld\n", cap_len - sizeof(PacketHeader));
	
	if (cap_len - sizeof(PacketHeader) <= 8){
		memdump((uint8_t*) (packet+1), 8);
	}
	else{
		memdump((uint8_t*) (packet+1), cap_len - sizeof(PacketHeader));
	}
	printf("\n");
	return true;
}



