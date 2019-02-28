#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

readyTurn* readyTurn::instance = NULL;

readyTurn::readyTurn()
{

}

BehaviorBase* readyTurn::Behavior()
{
    if(! instance)
    {
        instance = new readyTurn();
    }
    return instance;
}

void readyTurn::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Do something
    ////qiLogInfo("Behavior") << "Ready Turn!" << std::endl;
    ActionCommand::All action;

    action.body.actionType = ActionCommand::Body::WALK;

    action.body.forward = 0.0;
    action.body.left = 0.0;
    action.body.turn = -1.57;
    action.body.power = 1.0;
    action.body.bend = 15.0;

    action.head.isRelative = false;
    action.head.yawSpeed = 0.5;
    action.head.yaw = 0.0;

    bool walkTmp = readFrom(behavior, walkTransition);
    if(walkTmp == true)
    {
        writeCommand(action, unBoard);
        writeTo(behavior, walkTransition, false);
        ////qiLogInfo("Behavior") << "turn trans " << std::endl;
    }
    else
    {
        bool finTmp = readFrom(motion, walkFinished);
        writeTo(behavior, turnFinished, finTmp);
        ////qiLogInfo("Behavior") << "fin trans " << finTmp << std::endl;

    }



}

BehaviorBase* readyTurn::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    bool finTmp = readFrom(behavior, turnFinished)
    if(finTmp == true)
    {
        writeTo(behavior, walkTransition, true);
        writeTo(behavior, turnFinished, false);
        return readyWait::Behavior();

    }
    
    return this;
}
