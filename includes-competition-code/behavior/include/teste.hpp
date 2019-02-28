/*
* teste inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>

class teste : public BehaviorBase
{
private:
    static teste *instance;

public:
    teste();
    static BehaviorBase* Behavior();
    BehaviorBase* transition();
    void action(void*);
};
