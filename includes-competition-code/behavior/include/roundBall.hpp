#pragma once
/*
* roundBall inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class roundBall : public BehaviorBase
{
private:
    static roundBall *instance;
    bool motionFinished;
    bool walkTurn;
    int counter;
    //int quarters; // faz o movimento em 4 partes

public:
    roundBall();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
