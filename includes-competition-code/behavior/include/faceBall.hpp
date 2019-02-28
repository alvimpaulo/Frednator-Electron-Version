#pragma once
/*
* faceBall inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class faceBall : public BehaviorBase
{
private:
    static faceBall *instance;

public:
    faceBall();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
    bool motionFinished;
};
