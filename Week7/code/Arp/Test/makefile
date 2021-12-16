LDLIBS=-lpcap

all: arp-test

arp-test: main.o header/arphdr.o header/ethhdr.o header/ip.o header/mac.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f arp-test *.o header/*.o
