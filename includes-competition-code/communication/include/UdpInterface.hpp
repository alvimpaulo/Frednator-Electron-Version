/*
* Udp Interface
*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cstdio>
#include <arpa/inet.h>

class UdpInterface{
private:
	int Team_Port;
	struct sockaddr_in myaddr;

public:
	int fd;
	UdpInterface(int);
	~UdpInterface();
	int send(char *msg, size_t len, struct sockaddr_in *target, int port);
	int receive(char *buf, size_t BUFSIZE, struct sockaddr *target, socklen_t *addrlen);
};
