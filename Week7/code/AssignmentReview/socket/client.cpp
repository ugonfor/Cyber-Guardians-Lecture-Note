#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <iostream>
#include <thread>

using namespace std;

#define BUF_SIZE 1024

void recvThread(int cli_sd){
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
    // client MUST take two command-line arguments
    if (argc != 3) {
        printf("usage: %s <IP address> <port>\n", argv[0]);
        return 1;
    }

    // Argument1 : Server IP address 
    const char* server_ip = argv[1];
    // Argument2 : Server Port
    const char* server_port = argv[2];

    int socket_fd; // socket descriptor
    struct sockaddr_in server_addr; // server socket address

    // create socket 
    // AF_INET : IPv4
    // SOCK_STREAM : TCP
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        // if occur creation error
        puts("socket creation error");
        return 1;
    }

    // set the server address from arguments
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_addr.s_addr = inet_addr(server_ip); // server ip
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(atoi(server_port)); // port number

    // TCP connection with server
    if(connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        // if occur connection error
        puts("connect error\n");
        close(socket_fd);
        return 1;
    }

    thread* t = new thread(recvThread, socket_fd);
    t->detach();
    
    while (true)
    {
        string s;
        getline(cin, s);
        s += "\r\n";
        
        // sends the message to the server
        ssize_t res = send(socket_fd, s.c_str(), s.size(), 0);
		if (res == 0 || res == -1) {
			cerr << "send return " << res;
			perror(" ");
			break;
		}
    }
        
    // close socket descriptor
    close(socket_fd);

    return 0;
}