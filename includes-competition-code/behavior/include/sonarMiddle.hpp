#pragma once
/*
* sonarMiddle inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class sonarMiddle : public BehaviorBase
{
private:
    static sonarMiddle *instance;
    int sonarOutput;

public:
    sonarMiddle();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
