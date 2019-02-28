#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

BallClose* BallClose::instance = NULL;

BallClose::BallClose()
{

}

BehaviorBase* BallClose::Behavior()
{
    if(! instance)
    {
        instance = new BallClose();
    }
    return instance;
}

void BallClose::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Do something
    ////qiLogInfo("Behavior") << "Ball Close!" << std::endl;

     ActionCommand::All action;

     action.body.actionType = ActionCommand::Body::WALK;

     action.body.forward = 300.0;
     action.body.left = 0.0;
     action.body.turn = 0.0;
     action.body.power = 1.0;
     action.body.bend = 15.0;

     action.head.isRelative = false;
     action.head.yawSpeed = 0.5;
     action.head.yaw = 0.0;
}

BehaviorBase* BallClose::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);
    
    return this;
}
