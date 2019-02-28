#pragma once
/*
* WalkInit inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class WalkInit : public BehaviorBase
{
private:
    static WalkInit *instance;

    int ballX, ballY, lostCount;
    bool isLost, camera;


    bool motionFinished;

public:
    WalkInit();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
