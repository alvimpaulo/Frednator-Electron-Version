#pragma once
/*
* GmCtrlPenalized inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class GmCtrlPenalized : public BehaviorBase
{
private:
    static GmCtrlPenalized *instance;
    RoboCupGameControlData gmCtrlData;
    int penalized;
    int button_pressed;
    bool play_chest;

public:
    GmCtrlPenalized();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
