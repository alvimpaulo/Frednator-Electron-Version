#pragma once
/*
* BallClose inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class BallClose : public BehaviorBase
{
private:
    static BallClose *instance;

public:
    BallClose();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
