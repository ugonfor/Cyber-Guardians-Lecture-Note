#include "send-arp.h"

bool MakeEthPacket(EthArpPacket* lpPacket, bool isRequest, const char* eth_smac, const char* eth_dmac,
					const char* arp_smac, const char* arp_tmac, const char* arp_sip, const char* arp_tip){
	lpPacket->eth_.dmac_ = Mac(eth_dmac);
	lpPacket->eth_.smac_ = Mac(eth_smac);
	lpPacket->eth_.type_ = htons(EthHdr::Arp);

	lpPacket->arp_.hrd_ = htons(ArpHdr::ETHER);
	lpPacket->arp_.pro_ = htons(EthHdr::Ip4);
	lpPacket->arp_.hln_ = Mac::SIZE;
	lpPacket->arp_.pln_ = Ip::SIZE;
	if (isRequest) lpPacket->arp_.op_ = htons(ArpHdr::Request);
	else lpPacket->arp_.op_ = htons(ArpHdr::Reply);
	lpPacket->arp_.smac_ = Mac(arp_smac);
	lpPacket->arp_.sip_ = htonl(Ip(arp_sip));
	lpPacket->arp_.tmac_ = Mac(arp_tmac);
	lpPacket->arp_.tip_ = htonl(Ip(arp_tip));

	return true;
}

Mac GetMacfromIp(pcap_t* handle, Ip tip, Mac myMac, Ip myip){
	Mac tMac;
	EthArpPacket ArpPacket;
	MakeEthPacket(&ArpPacket, true, std::string(myMac).c_str(), "FF:FF:FF:FF:FF:FF", 
				std::string(myMac).c_str(), "00:00:00:00:00:00", std::string(myip).c_str(), std::string(tip).c_str());
	
	int res = pcap_sendpacket(handle, reinterpret_cast<const u_char*>(&ArpPacket), sizeof(EthArpPacket));
	if (res != 0) {
		fprintf(stderr, "pcap_sendpacket return %d error=%s\n", res, pcap_geterr(handle));
	}

	while (true)
	{
		struct pcap_pkthdr* header;
		const u_char* recv_packet;
		int res = pcap_next_ex(handle, &header, &recv_packet);
		if (res == 0) continue;
		if (res == PCAP_ERROR || res == PCAP_ERROR_BREAK) {
			printf("pcap_next_ex return %d(%s)\n", res, pcap_geterr(handle));
			break;
		}
		
		EthArpPacket* _recv_packet =(EthArpPacket*) recv_packet;
		if (_recv_packet->eth_.type() == EthHdr::Arp){
			if (_recv_packet->arp_.op() == ArpHdr::Reply && _recv_packet->arp_.sip() == tip){
				tMac = _recv_packet->arp_.smac();
				break;
			}
		}
	}

	return tMac;
}

Mac GetMyMac(std::string dev){
	struct ifreq s;
	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

	strcpy(s.ifr_ifrn.ifrn_name, dev.c_str());

	if(ioctl(fd, SIOCGIFHWADDR, &s) != 0) {
		perror("[!] ERROR on ioctl\n");
		exit(-1);
	}
	
	Mac mac = Mac( (uint8_t *) s.ifr_hwaddr.sa_data);

	return mac;
}

Ip GetMyIp(std::string dev){
	struct ifreq ifr; 
	char ipstr[40]; 
	int s; 
	s = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP); 
	strncpy(ifr.ifr_name, dev.c_str(), IFNAMSIZ); 

	if (ioctl(s, SIOCGIFADDR, &ifr) < 0){
		perror("[!] ERROR on ioctl\n");
		exit(-1);
	}
	else { 
		inet_ntop(AF_INET, ifr.ifr_addr.sa_data+2, ipstr,sizeof(struct sockaddr));
		return Ip(ipstr);
	} 
}

