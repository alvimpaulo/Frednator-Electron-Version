#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP

#include <basemodule.hpp>
#include <cstring>			//Needed for memset
#include <sys/socket.h>	//Needed for the socket functions
#include <netdb.h>			//Needed for the socket functions
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <UdpInterface.hpp>
#include <RoboCupGameControlData.h>
#include <SPLStandardMessage.h>
#include <commData.h>
#include <fstream>
#include <cmath>
#include <iostream>

#include <qi/log.hpp>

#include <Settings.hpp> // Information about connection

class communication : public baseModule
{
public:
    communication(void*);
    void tick();
private:
    struct sockaddr_in targetReceive;
    UdpInterface udpSend;
    UdpInterface udpGmCtrl;
    RoboCupGameControlReturnData gmCtrl;
    RoboCupGameControlData gmCtrlData, gmCtrlOld;
    SPLStandardMessage SPLsend;
    struct sockaddr_in targetGmCtrl;
    socklen_t addrlen;
    fd_set socks;
    fd_set tmp;
    int recvlen;
    struct timeval tv;
    int nsocks;
    int ret;

    int ComparaGmCtrl(RoboCupGameControlData, RoboCupGameControlData);
};

#endif // COMMUNICATION_HPP
