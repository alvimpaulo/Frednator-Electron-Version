#include <communication.hpp>

communication::communication(void* _ub) : udpGmCtrl(GAMECONTROLLER_DATA_PORT), udpSend(TEAM_PORT)
{
    this->unBoard = static_cast<UnBoard*>(_ub);

    addrlen = sizeof(targetGmCtrl);
    FD_ZERO(&socks);
    FD_SET(udpGmCtrl.fd, &socks);
    tmp = socks;
    nsocks = std::max(udpGmCtrl.fd,0) + 1;
    int broadcastEnable = 1;
    ret = setsockopt(udpSend.fd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
    gmCtrl.version = GAMECONTROLLER_RETURN_STRUCT_VERSION;
    gmCtrl.team = TEAM_NUM;
    gmCtrl.player = PLAYER_NUM;
    gmCtrl.message = GAMECONTROLLER_RETURN_MSG_ALIVE;

    SPLsend.playerNum = PLAYER_NUM;
    SPLsend.teamNum = TEAM_NUM;
    SPLsend.fallen = 0;
    SPLsend.ballAge = 0;
    SPLsend.intention = 0;
    SPLsend.averageWalkSpeed = 0;
    SPLsend.maxKickDistance = 0;
    SPLsend.currentPositionConfidence = 0;
    SPLsend.currentSideConfidence = 0;

    memset((char *)&targetReceive, 0, sizeof(targetReceive));
    targetReceive.sin_family = AF_INET;
    targetReceive.sin_addr.s_addr = inet_addr("10.0.255.255");
    targetReceive.sin_port = htons(TEAM_PORT);
}

void communication::tick()
{
    udpSend.send((char *)&SPLsend, sizeof(SPLStandardMessage), &targetReceive, TEAM_PORT);
    tv.tv_sec = 0;
    tv.tv_usec = 200000;
    if (select(nsocks, &socks, NULL, NULL, &tv) >= 0)
    {
        ////qiLogInfo("Communication") << "Entre If 1 e 2" << std::endl;
        if (FD_ISSET(udpGmCtrl.fd, &socks))
        {
            ////qiLogInfo("Communication") << "Entre If 2 e 3" << std::endl;
            recvlen = udpGmCtrl.receive((char *)&gmCtrlData, sizeof(RoboCupGameControlData), (struct sockaddr *)&targetGmCtrl, &addrlen);
            if(recvlen > 0)
            {


                ////qiLogInfo("Communication") << "Entre If 3 e 4" << std::endl;

                udpGmCtrl.send((char *)&gmCtrl, sizeof(gmCtrl), &targetGmCtrl, GAMECONTROLLER_RETURN_PORT);


                if (!(ComparaGmCtrl(gmCtrlData, gmCtrlOld))) {
//                    gmCtrlSaver.save(gmCtrlData);
                    writeTo(communication, gmCtrlData, gmCtrlData);

                }
                gmCtrlOld = gmCtrlData;
            }
        }
        socks = tmp;
    }
}

int communication::ComparaGmCtrl (RoboCupGameControlData gmCtrl1, RoboCupGameControlData gmCtrl2) {
    int i;

    if ((gmCtrl1.state==gmCtrl2.state)&&
        (gmCtrl1.firstHalf==gmCtrl2.firstHalf)&&
        (gmCtrl1.kickOffTeam==gmCtrl2.kickOffTeam)&&
        (gmCtrl1.secondaryState==gmCtrl2.secondaryState)&&
        (gmCtrl1.dropInTeam==gmCtrl2.dropInTeam)&&
        (gmCtrl1.teams[TEAM_NUM_VECTOR].players[PLAYER_NUM-1].penalty==gmCtrl2.teams[TEAM_NUM_VECTOR].players[PLAYER_NUM-1].penalty)&&
        (gmCtrl1.teams[TEAM_NUM_VECTOR].players[PLAYER_NUM-1].secsTillUnpenalised==gmCtrl2.teams[TEAM_NUM_VECTOR].players[PLAYER_NUM-1].secsTillUnpenalised))
    {
        i = 1;
    }
    else{
        i = 0;
    }
    return(i);
}
