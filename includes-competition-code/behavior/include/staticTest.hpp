#pragma once
/*
* staticTest inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class staticTest : public BehaviorBase
{
private:
    static staticTest *instance;

public:
    staticTest();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
