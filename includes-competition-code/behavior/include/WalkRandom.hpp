#pragma once
/*
* WalkRandom inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class WalkRandom : public BehaviorBase
{
private:
    static WalkRandom *instance;

    int ballX, ballY, lostCount;
    bool isLost, camera;

    bool motionFinished;


public:
    WalkRandom();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
