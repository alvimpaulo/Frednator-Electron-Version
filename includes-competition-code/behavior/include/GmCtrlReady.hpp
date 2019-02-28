#pragma once
/*
* GmCtrlReady inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class GmCtrlReady : public BehaviorBase
{
private:
    static GmCtrlReady *instance;
    RoboCupGameControlData gmCtrlData;
    int penalized;
    int readyNumber;

public:
    GmCtrlReady();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
    bool getUpIfFalling(void*);
    void getUpFront(void*);
    void getUpBack(void*);
    void deadStiff(void*);
    void refPickUp(void*);
};
