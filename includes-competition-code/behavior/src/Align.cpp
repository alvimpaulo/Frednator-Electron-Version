#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

Align* Align::instance = NULL;

Align::Align()
{

}

BehaviorBase* Align::Behavior()
{
    if(! instance)
    {
        instance = new Align();
    }
    return instance;
}

void Align::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    ActionCommand::All action;


    //Do something
    qiLogInfo("Behavior") << "Align!" << std::endl;

    float angleTmp;
    angleTmp = readFrom(motion, sensors.joints.angles[Joints::HeadYaw]);
    qiLogInfo("Behavior") << "Angle " << angleTmp << std::endl;


    action.leds.leftEye.red = 0;
    action.leds.leftEye.blue = 0;
    action.leds.leftEye.green = 1;

    action.leds.rightEye.red = 1;
    action.leds.rightEye.blue = 1;
    action.leds.rightEye.green = 0;

    action.body.actionType = ActionCommand::Body::WALK;
    action.body.forward = 0.0;
    action.body.left = 0.0;

    action.body.turn = angleTmp;
    if(abs(angleTmp) < 0.3)
    {
        action.body.turn = 0.0;
    }

    action.body.power = 1.0;
    action.body.bend = 15.0;

    action.head.yaw = 0.0;

    bool walkTmp = readFrom(behavior, walkTransition);
    if(walkTmp == true)
    {

        writeCommand(action, unBoard);
        writeTo(behavior, walkTransition, false);
    }
    bool finTmp = readFrom(motion, walkFinished);
    writeTo(behavior, turnFinished, finTmp);
}

BehaviorBase* Align::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);
    bool finTmp = readFrom(behavior, turnFinished)
    if(finTmp == false)
    {
        return this;
    }
    else
    {
        writeTo(behavior, walkTransition, true);
        writeTo(behavior, turnFinished, false);
        writeTo(motion,walkFinished, false);
        return walkBall::Behavior();
    }
}
