#pragma once
/*
* Avoid inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class Avoid : public BehaviorBase
{
private:
    static Avoid *instance;

public:
    Avoid();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
