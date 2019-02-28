#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

#define MAX_TICKS 1500
postKick* postKick::instance = NULL;

postKick::postKick()
{
    ticks = 0;
    ballX = -1;
}

BehaviorBase* postKick::Behavior()
{
    if(! instance)
    {
        instance = new postKick();
    }
    return instance;
}

void postKick::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //qiLogInfo("Behavior") << "Post";

    ActionCommand::All action;

    //*******************************
    //
    //          Leds
    //
    //*******************************
    // postKick State is Blue
    // Ball Lost Red
    // Ball Found Green
    //*******************************

    action.leds.rightEye.red = 1;
    action.leds.rightEye.blue = 0;
    action.leds.rightEye.green = 0;


    //*******************************
    //
    //          Vision
    //
    //*******************************
    // First Reads From Perception
    //*******************************


    ballX = readFrom(perception, ballX);
    ticks++;

    action.body.actionType = ActionCommand::Body::WALK;
    action.body.forward = 0.0;
    action.body.left = 0.0;
    action.body.turn = 0.0;
    action.body.power = 1.0;
    action.body.bend = 15.0;

    action.head.yawSpeed = 0.5;
    action.head.yaw = 0.0;
    action.head.pitch = 0.25;
    action.head.pitchSpeed = 1.0;

    writeCommand(action, unBoard);


}

BehaviorBase* postKick::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    if(ballX != -1) {
        ticks = 0;
        return Kick::Behavior();
    }
    else if(ticks > MAX_TICKS) {
        ticks = 0;
        return LookForBall::Behavior();
    }
    return this;
}
