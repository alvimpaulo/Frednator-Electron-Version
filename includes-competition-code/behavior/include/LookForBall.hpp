/*
* LookForBall inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class LookForBall : public BehaviorBase
{
private:
   
    static LookForBall *instance;
     int nextHeadPos;
     int timeOut;
     int sonarOutput;
     int sign;
     int headCount;
     bool topOrDown;
     int ballX, ballY, lostCount;
     bool isLost, camera;


public:
    LookForBall();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};

enum direction
{
    FRONT_FIRST = 1,
    LEFT,
    FRONT_SECOND,
    RIGHT,
};
