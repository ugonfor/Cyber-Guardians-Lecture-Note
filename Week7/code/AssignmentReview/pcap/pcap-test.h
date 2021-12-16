#include <pcap.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>    // for tcp header
#include <netinet/ip.h>     // for ip header
#include <net/ethernet.h>   // for ethernet header

#define ETH_ALEN	6		/* Octets in one ethernet addr	 */

typedef struct {
	char* dev_;
} Param;

typedef struct packet
{
    //ethernet
    uint8_t  ether_dhost[ETH_ALEN];	/* destination eth addr	*/
    uint8_t  ether_shost[ETH_ALEN];	/* source ether addr	*/
    uint16_t ether_type;		        /* packet type ID field	*/

    //ip
//#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint32_t ip_ihl:4;
    uint32_t ip_version:4;
//#elif __BYTE_ORDER == __BIG_ENDIAN
//    unsigned int ip_version:4;
//    unsigned int ip_ihl:4;
//#else
//# error	"Please fix <bits/endian.h>"
//#endif

    uint8_t ip_tos;
    uint16_t ip_tot_len;
    uint16_t ip_id;
    uint16_t ip_frag_off;
    uint8_t ip_ttl;
    uint8_t ip_protocol;
    
    uint16_t ip_check1;
    //uint16_t ip_check2;

    uint32_t ip_saddr;
    uint32_t ip_daddr;

    // TCP
	uint16_t tcp_sport;	/* source port */
	uint16_t tcp_dport;	/* destination port */
	uint32_t tcp_seq;		/* sequence number */
	uint32_t tcp_ack;		/* acknowledgement number */
//# if __BYTE_ORDER == __LITTLE_ENDIAN
	uint8_t tcp_x2:4;	/* (unused) */
	uint8_t tcp_off:4;	/* data offset */
//# endif
//# if __BYTE_ORDER == __BIG_ENDIAN
//	uint8_t tcp_off:4;	/* data offset */
//	uint8_t tcp_x2:4;	/* (unused) */
//# endif
	uint8_t tcp_flags;
# define TH_FIN	0x01
# define TH_SYN	0x02
# define TH_RST	0x04
# define TH_PUSH	0x08
# define TH_ACK	0x10
# define TH_URG	0x20
	uint16_t tcp_win;	/* window */
	uint16_t tcp_sum;	/* checksum */
	uint16_t tcp_urp;	/* urgent pointer */
} __attribute__ ((__packed__)) PacketHeader;


void usage();
bool parse(Param* param, int argc, char* argv[]);
bool isTCP(PacketHeader* packet);
bool PrintEtherHeader(PacketHeader* packet);
bool PrintIPDatagram(PacketHeader* packet);
bool PrintTCPSegment(PacketHeader* packet, uint32_t cap_len);
bool memdump(uint8_t* mem, uint32_t len);