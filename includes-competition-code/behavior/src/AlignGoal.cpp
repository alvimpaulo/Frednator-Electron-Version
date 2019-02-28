#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

AlignGoal* AlignGoal::instance = NULL;

AlignGoal::AlignGoal()
{

}

BehaviorBase* AlignGoal::Behavior()
{
    if(! instance)
    {
        instance = new AlignGoal();
    }
    return instance;
}

void AlignGoal::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Do something
    //qiLogInfo("Behavior") << "Align Goal!" << std::endl;

    int goalX;
    //goalX = readFrom(perception, centre x)
    //esquerda ou - direita ou thresh centro
    ActionCommand::All action;

    action.body.actionType = ActionCommand::Body::WALK;
    action.body.forward = 0.0;
    action.body.left = 300.0;
    action.body.turn = 0.1;
    action.body.power = 1.0;
    action.body.bend = 15.0;

    action.head.isRelative = false;
    action.head.yawSpeed = 0.5;
    action.head.yaw = 0.0;

    writeCommand(action, unBoard);

}

BehaviorBase* AlignGoal::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);
    //thresh centro
    //return Kick::Behavior();
    return this;
}
