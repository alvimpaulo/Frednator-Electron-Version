#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

tempBehaviorClass* tempBehaviorClass::instance = NULL;

tempBehaviorClass::tempBehaviorClass()
{

}

BehaviorBase* tempBehaviorClass::Behavior()
{
    if(! instance)
    {
        instance = new tempBehaviorClass();
    }
    return instance;
}

void tempBehaviorClass::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Do something
    qiLogInfo("Behavior") << "Fuciona!" << std::endl;
}

BehaviorBase* tempBehaviorClass::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);
    
    return this;
}
