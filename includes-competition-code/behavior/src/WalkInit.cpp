#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

WalkInit* WalkInit::instance = NULL;

WalkInit::WalkInit()
{
    motionFinished = false;

    ballX = -1;
    ballY = -1;
    lostCount = 0;
    isLost = true;
    camera = true;

}

BehaviorBase* WalkInit::Behavior()
{
    if(! instance)
    {
        instance = new WalkInit();
    }
    return instance;
}

void WalkInit::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    ////qiLogInfo("Behavior") << "Walk Initial!" << std::endl;

    //After First KickOff Change Variable so that Robot wont Walk Ahead
    writeTo(behavior, wasSet, false);


    ActionCommand::All action;

    //*******************************
    //
    //          Vision
    //
    //*******************************
    // First Reads From Perception, then creates a buffer with value 10 for lost ball count.
    // Ball will only truly be lost when it is lost for 10 behaviour cycles.
    //*******************************

    /*ballX = readFrom(perception, ballX);
    ballY = readFrom(perception, ballY);

    if(ballX == -1 && ballY == -1)
    {
        lostCount++;
    }
    else
    {
        lostCount = 0;
        isLost = false;

        action.leds.leftEye.red = 0;
        action.leds.leftEye.blue = 0;
        action.leds.leftEye.green = 1;
    }

    if (lostCount > 10)
    {
        isLost = true;

        action.leds.leftEye.red = 1;
        action.leds.leftEye.blue = 0;
        action.leds.leftEye.green = 0;
    }*/

    //*******************************
    //
    //              Motion
    //
    //*******************************
    // Robot Will Walk Head 300mm
    //*******************************


    //Purple Purple for Walk Init
    action.leds.rightEye.red = 1;
    action.leds.rightEye.blue = 1;
    action.leds.rightEye.green = 0;
    action.leds.leftEye.red = 1;
    action.leds.leftEye.blue = 1;
    action.leds.leftEye.green = 0;


    action.body.actionType = ActionCommand::Body::WALK;
    action.body.forward = 300.0;
    action.body.left = 0.0;
    action.body.turn = 0.0;
    action.body.power = 1.0;
    action.body.bend = 15.0;

    action.head.isRelative = false;
    action.head.yawSpeed = 0.5;
    action.head.yaw = 0.0;
    //action.head.pitch = 0.35;
    action.head.pitch = 0.0;
    action.head.pitchSpeed = 1.0;

    // We want the behaviour only to be executed once, but the state must be in its loop until it finishes
    // to do so, we set the walkTmp variable to indicate wether or not the command was called already.
    // From motion we read weather or not walk has finished, and write it on the behavior Variable indicating so.

    bool walkTmp = readFrom(behavior, walkTransition);

    if(walkTmp == true)
    {
        writeCommand(action, unBoard);
        writeTo(behavior, walkTransition, false);
    }
    motionFinished = readFrom(motion, walkFinished);

    //finTmp = readFrom(motion, walkFinished);
    //writeTo(behavior, turnFinished, finTmp);
}

BehaviorBase* WalkInit::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //bool finTmp = readFrom(behavior, turnFinished)

    // If Ball is found
    /*if(!isLost)
    {

        ////qiLogInfo("Behavior") << "Transition Walk Ball!" << std::endl;
        return walkBall::Behavior();

    }*/
    // Will only leave this state if motion has finished executing
    if(motionFinished == true)
    {
        writeTo(behavior, walkTransition, true);
        writeTo(behavior, turnFinished, false);
        writeTo(motion,walkFinished, false);

        return LookForBall::Behavior();

    }
    return this;
}
