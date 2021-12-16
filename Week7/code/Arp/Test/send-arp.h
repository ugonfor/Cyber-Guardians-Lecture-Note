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
