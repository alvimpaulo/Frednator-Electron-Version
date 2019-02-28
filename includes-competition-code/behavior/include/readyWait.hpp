#pragma once
/*
* readyWait inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class readyWait : public BehaviorBase
{
private:
    static readyWait *instance;

public:
    readyWait();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
