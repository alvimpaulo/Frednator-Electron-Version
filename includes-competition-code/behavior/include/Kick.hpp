#pragma once
/*
* Kick inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class Kick : public BehaviorBase
{
private:
    static Kick *instance;
    int finished;
    bool isKickable;
    bool isLeft;
    int oldBallX, oldBallY;
    int lostCount;
    bool lastMoveFinished;
    int kickBallX;
    int countOnce;
    int ticksCount;
public:
    Kick();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
