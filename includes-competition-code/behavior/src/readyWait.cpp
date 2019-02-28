#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

readyWait* readyWait::instance = NULL;

readyWait::readyWait()
{

}

BehaviorBase* readyWait::Behavior()
{
    if(! instance)
    {
        instance = new readyWait();
    }
    return instance;
}

void readyWait::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Do something
    ////qiLogInfo("Behavior") << "Ready Wait!" << std::endl;

    ActionCommand::All action;
    action.body.actionType = ActionCommand::Body::STAND;
    writeCommand(action, unBoard);

}

BehaviorBase* readyWait::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);
    
    return this;
}
