#pragma once
/*
* GmCtrlInitial inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class GmCtrlInitial : public BehaviorBase
{
private:
    static GmCtrlInitial *instance;
    RoboCupGameControlData gmCtrlData;
    int penalized;

public:
    GmCtrlInitial();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
