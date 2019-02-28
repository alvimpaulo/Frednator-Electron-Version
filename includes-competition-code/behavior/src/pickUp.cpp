#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

pickUp* pickUp::instance = NULL;

pickUp::pickUp()
{

}

BehaviorBase* pickUp::Behavior()
{
    if(! instance)
    {
        instance = new pickUp();
    }
    return instance;
}

void pickUp::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Do something
    //qiLogInfo("Behavior") << "Fuciona!" << std::endl;

    ActionCommand::All action;
    action.body.actionType = ActionCommand::Body::STAND;

    action.leds.rightEye.red = 1;
    action.leds.rightEye.blue = 0;
    action.leds.rightEye.green = 0;
    action.leds.leftEye.red = 1;
    action.leds.leftEye.blue = 0;
    action.leds.leftEye.green = 0;

    action.body.forward = 0.0;
    action.body.left = 0.0;
    action.body.turn = 0.0;
    action.body.power = 1.0;
    action.body.bend = 15.0;

    writeCommand(action, unBoard);

}

BehaviorBase* pickUp::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);
    
    return this;
}
