#pragma once
/*
* approachBall inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class approachBall : public BehaviorBase
{
private:
    static approachBall *instance;
    bool motionFinished;
    int cont;

public:
    approachBall();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
