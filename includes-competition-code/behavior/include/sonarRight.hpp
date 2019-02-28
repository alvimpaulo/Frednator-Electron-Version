#pragma once
/*
* sonarRight inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class sonarRight : public BehaviorBase
{
private:
    static sonarRight *instance;
    int sonarOutput;

public:
    sonarRight();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
