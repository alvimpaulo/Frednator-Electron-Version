#pragma once
/*
* GmCtrlPlaying inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class GmCtrlPlaying : public BehaviorBase
{
private:
    static GmCtrlPlaying *instance;
    RoboCupGameControlData gmCtrlData;
    int penalized;

public:
    GmCtrlPlaying();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
    bool getUpIfFalling(void*);
    void getUpFront(void*);
    void getUpBack(void*);
    void deadStiff(void*);
    void refPickUp(void*);

};
