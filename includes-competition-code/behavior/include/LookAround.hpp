/*
* LookAround inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class LookAround : public BehaviorBase
{
private:
    static LookAround *instance;
    int sonarOutput;

    int ballX, ballY, lostCount;
    bool isLost;

    int countAround;
    bool motionFinished;

public:
    LookAround();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
