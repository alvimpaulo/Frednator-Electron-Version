/*
* Udp Interface
*/

#include <UdpInterface.hpp>

#define BUFSIZE 2048

UdpInterface::UdpInterface(int port)
{
    Team_Port = port;
  //  Create the socket with socket(domain, type, protocol)
  if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("Error creating the socket.");
  }

  // Preparing parameters to bind
  memset((char *)&myaddr, 0, sizeof(myaddr));
  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myaddr.sin_port = htons(Team_Port);

  // Bind the socket with bind(int socket, const struct sockaddr *address, socklen_t address_len);
  if( bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
  {
    //std::cout << "Error Binding the Socket" << std::endl;
  }
}

UdpInterface::~UdpInterface()
{
  close(fd);
}

int UdpInterface::send(char *msg, size_t len, struct sockaddr_in *target, int port)
{
    target->sin_port = htons(port);
    if(sendto(fd, msg, len, 0, (struct sockaddr*)target, sizeof(struct sockaddr)) < 0)
    {
        //std::cout << "Error Sending Message" << std::endl;
        return -1;
    }

    return 0;
}

int UdpInterface::receive(char *buf,  size_t size, struct sockaddr *target, socklen_t *addrlen)
{
  return recvfrom(fd, buf, size, 0, target, addrlen);
}
