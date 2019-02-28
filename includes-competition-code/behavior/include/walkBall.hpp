#pragma once
/*
* walkBall inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class walkBall : public BehaviorBase
{
private:
    static walkBall *instance;

    int ballX, ballY, lostCount, walkCounter;
    bool isLost, camera;

public:
    walkBall();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
