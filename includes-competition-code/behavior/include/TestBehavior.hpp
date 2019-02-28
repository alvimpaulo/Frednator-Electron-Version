#pragma once
/*
* TestBehavior inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class TestBehavior : public BehaviorBase
{
private:
    static TestBehavior *instance;

public:
    TestBehavior();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
