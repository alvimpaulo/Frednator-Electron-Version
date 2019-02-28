#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

WalkRandom* WalkRandom::instance = NULL;

WalkRandom::WalkRandom()
{
    ballX = -1;
    ballY = -1;
    lostCount = 0;
    isLost = true;
    camera = true;

    motionFinished = false;
}

BehaviorBase* WalkRandom::Behavior()
{
    if(! instance)
    {
        instance = new WalkRandom();
    }
    return instance;
}

void WalkRandom::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    ////qiLogInfo("Behavior") << "Walk Random!" << std::endl;

    //Adds Sonar
    sonarStatic->getSonar(unBoard);

    ActionCommand::All action;

    //*******************************
    //
    //          Leds
    //
    //*******************************
    //  State Led - Right - Green
    //  Ball Lost - Left - Red
    //  Ball Found - Right - Green
    //*******************************

    action.leds.rightEye.red = 0;
    action.leds.rightEye.blue = 0;
    action.leds.rightEye.green = 1;

    //*******************************
    //
    //          Vision
    //
    //*******************************
    // First Reads From Perception, then creates a buffer with value 10 for lost ball count.
    // Ball will only truly be lost when it is lost for 10 behaviour cycles.
    //*******************************

    ballX = readFrom(perception, ballX);
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
    }

    //*******************************
    //
    //          Motion
    //
    //*******************************
    //  Will make robot walk something
    //*******************************


    action.head.isRelative = false;
    action.head.yawSpeed = 0.5;
    action.head.yaw = 0.0;

    action.body.actionType = ActionCommand::Body::WALK;
    action.body.turn = 0.0;

    action.body.power = 1.0;
    action.body.bend = 15.0;

    action.body.forward = 100.0;
    action.body.left = 0.0;


    //*******************************
    // We want the behaviour only to be executed once, but the state must be in its loop until it finishes
    // to do so, we set the walkTmp variable to indicate wether or not the command was called already.
    // From motion we read weather or not walk has finished, and write it on the behavior Variable indicating so.
    //*******************************

    bool walkTmp = readFrom(behavior, walkTransition);
    if(walkTmp == true)
    {
        writeCommand(action, unBoard);
        writeTo(behavior, walkTransition, false);


    }

    motionFinished = readFrom(motion, walkFinished);

}

BehaviorBase* WalkRandom::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    // When It Goes to Sonar Returns to last class
    lastBehavior = this;

    //*******************************
    //
    //          Sonar
    //
    //*******************************
    // Highest Priority Transition, if sonar is active will go to avoid state
    //*******************************

    int sonarTmp;
    sonarTmp = readFrom(behavior, sonarPos);

    //std::cout << "Sonar" << sonarTmp << std::endl;
    if(sonarTmp != 0)
    {
        return Avoid::Behavior();
    }

    // If Ball is found, leaves state
    if(!isLost)
    {
        return walkBall::Behavior();
    }
    //If movement is Finished and ball is still lost goes back to look for ball
    else if(motionFinished == true && isLost)
    {
        writeTo(behavior, walkTransition, true);
        writeTo(behavior, turnFinished, false);
        writeTo(motion,walkFinished, false);

        return LookForBall::Behavior();

    }
    //If Motion is not Finished keeps returning to same state
    else if(motionFinished == false && isLost)
    {
       return this;
    }
}
