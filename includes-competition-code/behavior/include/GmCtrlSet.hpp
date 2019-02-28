#pragma once
/*
* GmCtrlSet inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class GmCtrlSet : public BehaviorBase
{
private:
    static GmCtrlSet *instance;
    RoboCupGameControlData gmCtrlData;
    int penalized;

public:
    GmCtrlSet();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
