CC = g++
LDLIBS += -lpcap

all: pcap-test

pcap-test: pcap-test.o main.o

main.o: main.cpp pcap-test.h

pcap-test.o: pcap-test.cpp pcap-test.h

clean:
	rm -f pcap-test *.o
