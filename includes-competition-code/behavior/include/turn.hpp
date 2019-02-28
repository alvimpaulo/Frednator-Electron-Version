#pragma once
/*
* turn inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class turn : public BehaviorBase
{
private:
    static turn *instance;
    bool motionFinished;

public:
    turn();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
