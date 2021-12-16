# pcap programming
Packet Capture Application

## usage
```shell
ugonfor@gongon:~/ugonfor/pcap-test$ make
g++    -c -o pcap-test.o pcap-test.cpp
g++   pcap-test.o main.o  -lpcap -o pcap-test
ugonfor@gongon:~/ugonfor/pcap-test$ sudo ./pcap-test eth0
```

## example
```shell
==================================================
ether_type: 0x0800
ip_protocol: 0x06
TCP Packet Captured

Ethernet II
        Src MAC: aa:bb:cc:dd:ee:ff
        Dst MAC: aa:bb:cc:dd:ee:ff
IP Datagram
        Src IP: 1.2.3.4
        Dst IP: 1.2.3.4
TCP Segment
        Src Port: 443
        Dst Port: 443

Data size: 44
Memory Dump
      00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f  0123456789ABCDEF
0x00  82 2a 01 00 02 ed a7 00 02 3a f5 00 00 00 05 05  .*.......:......
0x10  00 01 3b 6e 01 00 02 ed a8 00 02 3a f5 00 00 00  ..;n.......:....
0x20  0b 09 00 01 3b 6e 00 00 00 02 5b 5d              ...;n....[].

98 bytes captured
```