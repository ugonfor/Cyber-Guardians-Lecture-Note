all: babyvm

babyvm : main.o babyvm.o
	g++ -o babyvm babyvm.o main.o

babyvm.o : babyvm.cpp babyvm.h

main.o : main.cpp babyvm.h

clean:
	rm *.o babyvm