#include "pcap-test.h"

Param param  = {
	.dev_ = NULL
};

int main(int argc, char* argv[]) {
	if (!parse(&param, argc, argv))
		return -1;

	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* pcap = pcap_open_live(param.dev_, BUFSIZ, 1, 1000, errbuf);
	if (pcap == NULL) {
		fprintf(stderr, "pcap_open_live(%s) return null - %s\n", param.dev_, errbuf);
		return -1;
	}

	while (true) {
		struct pcap_pkthdr* header;
		const u_char* packet;
		int res = pcap_next_ex(pcap, &header, &packet);
		if (res == 0) continue;
		if (res == PCAP_ERROR || res == PCAP_ERROR_BREAK) {
			printf("pcap_next_ex return %d(%s)\n", res, pcap_geterr(pcap));
			break;
		}

		// for debug
		// memdump((uint8_t*)packet, 0x80);
		printf("==================================================\n");
		if(isTCP((PacketHeader*) packet)){
			PrintEtherHeader((PacketHeader*) packet);
			PrintIPDatagram((PacketHeader*) packet);
			PrintTCPSegment((PacketHeader*) packet, header->caplen);
		}


		printf("%u bytes captured\n", header->caplen);
	}

	pcap_close(pcap);
}
