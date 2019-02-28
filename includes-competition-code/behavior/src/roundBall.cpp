#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

roundBall* roundBall::instance = NULL;

roundBall::roundBall()
{
    walkTurn = true; // walk = true, turn = false
    counter = 0;
}

BehaviorBase* roundBall::Behavior()
{
    if(! instance)
    {
        instance = new roundBall();
    }
    return instance;
}

void roundBall::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Do something
    //qiLogInfo("Behavior") << "Fuciona!" << std::endl;


    ActionCommand::All action;

    //*******************************
    //
    //          Leds
    //
    //*******************************
    //  State Led - Right -
    //*******************************

    action.leds.rightEye.red = 0;
    action.leds.rightEye.blue = 0;
    action.leds.rightEye.green = 1;

    action.leds.leftEye.blue = 0;


    action.head.isRelative = false;
    action.head.yawSpeed = 0.5;
    action.head.yaw = 0.0;

    action.body.actionType = ActionCommand::Body::WALK;
    action.body.forward = 0.0;
    //action.body.left = 1300; //depende de walkTurn

    action.body.power = 1.0;
    action.body.bend = 15.0;
    //action.body.turn = 0.0; //depende de walkTurn
    action.body.isFast = false;

    int goalCenter = readFrom(perception, goalCenter);

    if(goalCenter < 160)
    {
        if(walkTurn){
            action.leds.leftEye.green = 1;
            action.leds.leftEye.red = 0;

            action.body.left = 200.0;
            action.body.turn = 0.0;
        }else{
            action.leds.leftEye.green = 0;
            action.leds.leftEye.red = 1;

            action.body.left = 0.0;
            action.body.turn = -0.25;
        }
    }else{
        if(walkTurn){
            action.leds.leftEye.green = 1;
            action.leds.leftEye.red = 0;

            action.body.left = -200.0;
            action.body.turn = 0.0;
        }else{
            action.leds.leftEye.green = 0;
            action.leds.leftEye.red = 1;

            action.body.left = 0.0;
            action.body.turn = 0.25;
        }
    }

    //*******************************
    // We want the behaviour only to be executed once, but the state must be in its loop until it finishes
    // to do so, we set the walkTmp variable to indicate wether or not the command was called already.
    // From motion we read weather or not walk has finished, and write it on the behavior Variable indicating so.
    //*******************************

    bool walkTmp = readFrom(behavior, walkTransition);

    if(walkTmp == true)
    {
        walkTurn = !walkTurn;
        writeCommand(action, unBoard);
        writeTo(behavior, walkTransition, false);
    }

    motionFinished = readFrom(motion, walkFinished);

}

BehaviorBase* roundBall::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    int goalCenter = readFrom(perception, goalCenter);

    if (goalCenter == -1 || abs(goalCenter - 160) < 36)
    {
        counter = 0;
        walkTurn = true;
        writeTo(behavior, walkTransition, true);
        writeTo(motion,walkFinished, false);
        int goleiro = readFrom(perception, approxDistance);
        //std::cout << "Distancia behavior" << goleiro << std::endl;
        writeTo(behavior, searchGoal, false);
        if (goleiro != -1)
            return pickUp::Behavior();
        else
            return Kick::Behavior();
    }

    if(motionFinished == false)
    {
        return this;
    }
    else
    {
        if(counter > 10){
            counter = 0;
            walkTurn = true;
            writeTo(behavior, walkTransition, true);
            writeTo(motion,walkFinished, false);
            return LookForBall::Behavior();
        }else{
            writeTo(behavior, walkTransition, true);
            writeTo(motion,walkFinished, false);
            return this;
        }
    }

}
