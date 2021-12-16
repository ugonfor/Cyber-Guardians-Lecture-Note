LDLIBS=-lpcap

all: send-arp

send-arp: main.o send-arp.o header/arphdr.o header/ethhdr.o header/ip.o header/mac.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f send-arp *.o header/*.o
