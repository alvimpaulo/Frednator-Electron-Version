#pragma once
/*
* sonarLeft inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class sonarLeft : public BehaviorBase
{
private:
    static sonarLeft *instance;
    int sonarOutput;

public:
    sonarLeft();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
