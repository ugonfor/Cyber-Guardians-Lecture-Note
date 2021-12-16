#include "header/arphdr.h"
#include "header/ethhdr.h"
#include "header/ip.h"
#include "header/mac.h"

#include <cstdio>
#include <pcap.h>

#include <linux/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <iostream>


#pragma pack(push, 1)
struct EthArpPacket final {
	EthHdr eth_;
	ArpHdr arp_;
};
#pragma pack(pop)

Mac GetMacfromIp(pcap_t* handle, Ip tip, Mac myMac, Ip myip);
Mac GetMyMac(std::string dev);
Ip GetMyIp(std::string dev);

bool MakeEthPacket(EthArpPacket* lpPacket, bool isRequest, const char* eth_smac, const char* eth_dmac,
					const char* arp_smac, const char* arp_tmac, const char* arp_sip, const char* arp_tip);