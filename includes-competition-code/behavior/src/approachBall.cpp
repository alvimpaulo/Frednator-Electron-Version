#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

approachBall* approachBall::instance = NULL;

approachBall::approachBall()
{
    motionFinished = 0;
    cont = 0;
}

BehaviorBase* approachBall::Behavior()
{
    if(!instance)
    {
        instance = new approachBall();
    }
    return instance;
}

void approachBall::action(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    // Construct Sonar Class
    sonarStatic->getSonar(unBoard);



    //Do something
    ////qiLogInfo("Behavior") << "approach" << std::endl;

    ActionCommand::All action;


    //amarelo
    action.leds.leftEye.red = 1;
    action.leds.leftEye.blue = 0;
    action.leds.leftEye.green = 1;

    action.body.actionType = ActionCommand::Body::WALK;

    int ballX = readFrom(behavior, ballXApproach);

    action.body.forward = 200.0;
    action.body.left = 0.0;
    action.body.turn = 0.0;
    action.body.power = 1.0;
    action.body.bend = 15.0;
    action.body.isFast = false;

    action.head.isRelative = false;
    action.head.yawSpeed = 0.5;
    action.head.yaw = 0.0;

    bool walkTmp = readFrom(behavior, walkTransition);
    if(walkTmp == true)
    {
        writeCommand(action, unBoard);
        writeTo(behavior, walkTransition, false);

    }

    motionFinished = readFrom(motion, walkFinished);
}

BehaviorBase* approachBall::transition(void* _ub)
{

    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

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

    if(motionFinished == false)
        return this;
    else{
        cont++;
        writeTo(behavior, walkTransition, true);
        writeTo(motion,walkFinished, false);

        if (cont == 4){
            cont = 0;
            writeTo(perception, ballX, -1);
            writeTo(perception, ballY, -1);
            writeTo(perception, isTopCamera, false); //isTopCamera = 0
            return walkBall::Behavior();
        }
        else
            return this;

    }
}
