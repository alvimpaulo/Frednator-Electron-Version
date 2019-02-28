#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

TestBehavior* TestBehavior::instance = NULL;

TestBehavior::TestBehavior()
{

}

BehaviorBase* TestBehavior::Behavior()
{
    if(! instance)
    {
        instance = new TestBehavior();
    }
    return instance;
}

void TestBehavior::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Do something
    qiLogInfo("Behavior") << "Fuciona!" << std::endl;

    ActionCommand::All action;

    action.body.actionType = ActionCommand::Body::KICK;
    action.body.foot = ActionCommand::Body::RIGHT;
    action.body.ballY = -45.0;
    action.body.ballX = 160.0;
    action.body.kickDirection = 0.0;
    action.body.misalignedKick = false;
    action.body.power = 1.0;
    action.body.speed = 1.0;
    action.body.bend = 15.0;
    writeCommand(action, unBoard);
}

BehaviorBase* TestBehavior::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);
    
    return this;
}
