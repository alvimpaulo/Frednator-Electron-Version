// DESATIVADO!!!

#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

faceBall* faceBall::instance = NULL;

faceBall::faceBall()
{
    motionFinished = false;
}

BehaviorBase* faceBall::Behavior()
{
    if(! instance)
    {
        instance = new faceBall();
    }
    return instance;
}

void faceBall::action(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Do something
    //qiLogInfo("Behavior") << "faceBall" << std::endl;

    float bodyCorrection = 0;
    bool camera = readFrom(perception, isTopCamera);

    if(camera){
        int ballX = readFrom(perception, ballX);
        int ballY = readFrom(perception, ballY);
        float deltaY = 687 - ballY;
        float deltaX = 160 - ballX;
        float ratio = deltaX / deltaY;
        bodyCorrection = 2 * (ratio - ratio*ratio*ratio/3 + ratio*ratio*ratio*ratio*ratio/5);
    }

    float headTmp = readFrom(motion, sensors.joints.angles[Joints::HeadYaw]);


    ActionCommand::All action;


    //*******************************
    //
    //          Leds
    //
    //*******************************
    //  State Led - Right - Magenta
    //  Ball Lost - Left - Red
    //  Ball Found - Right - Green
    //*******************************

    action.leds.rightEye.red = 1;
    action.leds.rightEye.blue = 1;
    action.leds.rightEye.green = 1;

    //*******************************

    action.head.isRelative = false;
    action.head.yawSpeed = 0.5;
    action.head.yaw = 0.0;

    action.body.actionType = ActionCommand::Body::WALK;
    action.body.forward = 0.0;
    action.body.left = 0.0;

    action.body.power = 1.0;
    action.body.bend = 15.0;
    action.body.turn = (headTmp + bodyCorrection);

    ////qiLogInfo("Behavior") << "faceBalll " << bodyCorrection*180/3.14 << "    " << headTmp*180/3.14 << std::endl;
    ////qiLogInfo("Behavior") << "faceBalll " << bodyCorrection*180/3.14 << "    " << headTmp*180/3.14 << std::endl;

    bool walkTmp = readFrom(behavior, walkTransition);

    if(headTmp + bodyCorrection > 0.35){

        if(walkTmp == true)
        {
            //qiLogInfo("Behavior") << "faceBalll " << bodyCorrection*180/3.14 << "    " << headTmp*180/3.14 << std::endl;
            writeCommand(action, unBoard);
            writeTo(behavior, walkTransition, false);
        }
        motionFinished = readFrom(motion, walkFinished);
    }
    else {
        motionFinished = 1;
    }
}

BehaviorBase* faceBall::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);
    

    // If it has finished, reset behaviour variables and and goes to walkBall
    if(motionFinished)
    {
        writeTo(behavior, walkTransition, true);
        writeTo(motion,walkFinished, false);
        return walkBall::Behavior();
    }

    return this;
}
