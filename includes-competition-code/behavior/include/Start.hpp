/*
* Start inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class Start : public BehaviorBase
{
private:
   
    static Start *instance;
    bool standing;
    int sonarOutput;
    std::vector<int> sonarVec;
    int sizeSonarLeft, sonarAverageLeft, sizeSonarRight, sonarAverageRight;
    bool kickOff;
    
public:
    Start();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
