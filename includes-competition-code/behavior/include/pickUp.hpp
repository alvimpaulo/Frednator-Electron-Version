#pragma once
/*
* pickUp inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class pickUp : public BehaviorBase
{
private:
    static pickUp *instance;

public:
    pickUp();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
