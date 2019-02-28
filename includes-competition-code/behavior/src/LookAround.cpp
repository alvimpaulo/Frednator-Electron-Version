#include <includeMapBehavior.hpp>
#include <qi/log.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

LookAround* LookAround::instance = NULL;

LookAround::LookAround()
{
    sonarOutput = SonarData::NONE;
    ballX = -1;
    ballY = -1;
    lostCount = 0;
    isLost = true;

    countAround = 0;

    motionFinished = 0;
}

BehaviorBase* LookAround::Behavior()
{
    if(! instance)
    {
        instance = new LookAround();
    }
    return instance;
}

void LookAround::action(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);


    ////qiLogInfo("Behavior") << "Look Around!" << std::endl;


    ActionCommand::All action;

    //*******************************
    //
    //          Leds
    //
    //*******************************
    //  State Led - Right - Cyan
    //  Ball Lost - Left - Red
    //  Ball Found - Right - Green
    //*******************************

    action.leds.rightEye.red = 0;
    action.leds.rightEye.blue = 1;
    action.leds.rightEye.green = 1;


    //*******************************
    //
    //          Motion
    //
    //*******************************
    //  Will make robot turn around itself 90 degrees
    //  Also Reajust Yaw
    //*******************************

    action.head.isRelative = false;
    action.head.yawSpeed = 0.5;
    action.head.yaw = 0.0;

    action.body.actionType = ActionCommand::Body::WALK;
    action.body.forward = 0.0;
    action.body.left = 0.0;

    action.body.power = 1.0;
    action.body.bend = 15.0;
    action.body.turn = 1.57;

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
        countAround++; //Counts Times Action Has been executed

    }

    motionFinished = readFrom(motion, walkFinished);

    //bool finTmp = readFrom(motion, walkFinished);
    //writeTo(behavior, turnFinished, finTmp);
    //boost::this_thread::sleep(boost::posix_time::milliseconds(10000));

}

BehaviorBase* LookAround::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //bool finTmp = readFrom(behavior, turnFinished);

    //If Ball is Found Goes after it
    if(motionFinished == false)
    {
        return this;
    }
    else
    {
        ////qiLogInfo("Behavior") << "LookAround transition count" << countAround << std::endl;
        if(countAround < 3){

            writeTo(behavior, walkTransition, true);
            writeTo(behavior, turnFinished, false);
            writeTo(motion,walkFinished, false);
            ////qiLogInfo("Behavior") << "LookAround transition count" << std::endl;
            return LookForBall::Behavior();
        }
        else{     // If Has done three turns walks ramdonly and reset behaviour variables

            countAround = 0;
            writeTo(motion,walkFinished, false);
            writeTo(behavior, walkTransition, true);
            writeTo(behavior, turnFinished, false);

            ////qiLogInfo("Behavior") << "LookAround transition count WALKRANDOM" << std::endl;
            return WalkRandom::Behavior(); //mudar esse estado dps
        }
    }
}
