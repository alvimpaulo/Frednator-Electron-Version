#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

turn* turn::instance = NULL;

turn::turn()
{
        motionFinished = false;
}

BehaviorBase* turn::Behavior()
{
    if(! instance)
    {
        instance = new turn();
    }
    return instance;
}

void turn::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Do something
    ////qiLogInfo("Behavior") << "Turn!" << std::endl;

    ActionCommand::All action;

    action.body.actionType = ActionCommand::Body::WALK;

    action.body.forward = 0.0;
    action.body.left = 0.0;
    action.body.turn = 3.14;
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
    }

    motionFinished = readFrom(motion, walkFinished);

    /*bool finTmp = readFrom(motion, walkFinished);
    writeTo(behavior, turnFinished, finTmp);*/

}

BehaviorBase* turn::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);
    
    //bool finTmp = readFrom(behavior, turnFinished)
    if(motionFinished == false)
    {
        ////qiLogInfo("Behavior") << "Transion Self!" << std::endl;
        return this;
    }
    else
        return walkBall::Behavior();

}
