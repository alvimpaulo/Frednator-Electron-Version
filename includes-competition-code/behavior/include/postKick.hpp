#pragma once
/*
* postKick inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class postKick : public BehaviorBase
{
private:
    static postKick *instance;
    int ticks;
    int ballX;
public:
    postKick();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
