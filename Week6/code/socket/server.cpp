#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>
#include <thread>

#define BUF_SIZE 1024
using namespace std;

void receiverThread(int cli_sd){
    // For debug, print the client ip address.
    char recv_data[BUF_SIZE] = {0};
    while (true)
    {
        // recv the message from client.
        ssize_t res = recv(cli_sd, recv_data, BUF_SIZE, 0);
        if (res == 0 || res == -1){
            cerr << "recv return " << res;
            perror(" ");
            break;
        }
        recv_data[res] = '\0';
        cout << recv_data;
        cout.flush();
    }

    printf("disconnected.\n");
    // after the relay, close the socket descriptors.
    close(cli_sd);
}


int main(int argc, char const *argv[])
{
    // server MUST take one command-line arguments
    if (argc != 2) {
        printf("usage: %s <port>\n", argv[0]);
        return 1;
    }

    // Argument1 : Server port number 
    const char* server_port = argv[1];

    // server permits two TCP clients.
    int server_fd, client1_fd; // socket descriptor of server, client1
    struct sockaddr_in server_addr, client1_addr;  // socket address of server, client1
    
    // accept ft needs address size pointer
    int client1_len = sizeof(client1_addr); // address size

    // create socket 
    // AF_INET : IPv4
    // SOCK_STREAM : TCP
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        // if occur creation error
        puts("socket creation error");
        return 1;
    }

	int optval = 1;
	int res = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (res == -1) {
		perror("setsockopt");
		return -1;
	}

    // set the server address from arguments
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // server ip. INADDR_ANY : use ALL LAN card.
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(atoi(server_port)); // port number

    // bind socket to server_addr
    // server_addr's IP : INADDR_ANY. so It binds the socket to all available interfaces.
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        // if occur bind error
        puts("bind error");
        return 1;
    }

    // listen connection from clients
    if(listen(server_fd, 5) < 0)
    {
        // if occur listen error
        puts("listen error");
        return 1;
    }

    while (true)
    {
        // accept connection from clients
        client1_fd = accept(server_fd,(struct sockaddr *)&client1_addr,(socklen_t *)&client1_len);
        // handle the accept error
        if(client1_fd < 0)
        {
            puts("client 1 accept failed\n");
            exit(0);
        }
        char ip_addr_clients[20];
        inet_ntop(AF_INET, &client1_addr.sin_addr.s_addr, ip_addr_clients, sizeof(ip_addr_clients));
        printf("%s client connect.\n", ip_addr_clients);
        
        thread* t = new thread(receiverThread, client1_fd);
        t->detach();
    }
    

    // close the server's socket descriptors
    close(server_fd);

	return 0;
}