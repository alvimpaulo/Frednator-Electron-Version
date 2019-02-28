#pragma once
/*
* readyWalk inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class readyWalk : public BehaviorBase
{
private:
    static readyWalk *instance;
    bool vbOnce, walkFinished;
    int counter;

public:
    readyWalk();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
