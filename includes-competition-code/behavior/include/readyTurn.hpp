#pragma once
/*
* readyTurn inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class readyTurn : public BehaviorBase
{
private:
    static readyTurn *instance;

public:
    readyTurn();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
