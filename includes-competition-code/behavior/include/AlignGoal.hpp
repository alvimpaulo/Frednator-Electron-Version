#pragma once
/*
* AlignGoal inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class AlignGoal : public BehaviorBase
{
private:
    static AlignGoal *instance;

public:
    AlignGoal();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
