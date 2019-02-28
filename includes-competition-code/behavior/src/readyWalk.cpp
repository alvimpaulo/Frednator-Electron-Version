#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

readyWalk* readyWalk::instance = NULL;

readyWalk::readyWalk()
{
    vbOnce = false;
    counter = 0;
}

BehaviorBase* readyWalk::Behavior()
{
    if(! instance)
    {
        instance = new readyWalk();
    }
    return instance;
}

void readyWalk::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Do something
    ////qiLogInfo("Behavior") << "Ready Walk!" << std::endl;

    ActionCommand::All action;


    action.body.actionType = ActionCommand::Body::WALK;

    action.body.forward = 250.0;
    action.body.left = 0.0;
    action.body.turn = 0;
    action.body.power = 0.1;
    action.body.bend = 15.0;
    action.body.speed = 0.2;
    action.body.isFast = false;

    action.head.isRelative = false;
    action.head.yawSpeed = 0.5;
    action.head.yaw = 0.0;

    bool walkTmp = readFrom(behavior, walkTransition);
    if(walkTmp == true)
    {
        counter++;
        writeCommand(action, unBoard);
        writeTo(behavior, walkTransition, false);
    }

    walkFinished = readFrom(motion, walkFinished);
}

BehaviorBase* readyWalk::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);
    bool finTmp = readFrom(behavior, turnFinished)
    if(walkFinished == true)
    {
        if(counter == 10){
            writeTo(behavior, walkTransition, true);
            writeTo(behavior, turnFinished, false);
            writeTo(motion,walkFinished, false);
            return readyTurn::Behavior();
        }else{
            //qiLogInfo("Behavior") << "Ready Walk!" << counter << std::endl;
            writeTo(behavior, walkTransition, true);
            writeTo(behavior, turnFinished, false);
            writeTo(motion,walkFinished, false);
            return this;
        }
    }

    return this;
}
