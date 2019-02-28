#pragma once
/*
* GmCtrlFinished inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class GmCtrlFinished : public BehaviorBase
{
private:
    static GmCtrlFinished *instance;
    RoboCupGameControlData gmCtrlData;
    int penalized;

public:
    GmCtrlFinished();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
