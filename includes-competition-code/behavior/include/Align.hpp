#pragma once
/*
* Align inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class Align : public BehaviorBase
{
private:
    static Align *instance;

public:
    Align();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
