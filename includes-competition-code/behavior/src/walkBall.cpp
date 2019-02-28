#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

// This Threshold defines if ball is close enough for kicking
#define THRESH_BALL_CLOSE 110
#define CAMERA_BOT false

walkBall* walkBall::instance = NULL;

walkBall::walkBall()
{

    ballX = -1;
    ballY = -1;
    lostCount = 0;
    isLost = true;
    camera = true;
    walkCounter = 0;
}
// TODO:
// timeout do lookforball
// metodo velho detecao de bola nao detecta porra nenhuma
// decidir estategia pra mudar de camera
// melhorar candidatos ann

BehaviorBase* walkBall::Behavior()
{
    if(! instance)
    {
        instance = new walkBall();
    }
    return instance;
}

void walkBall::action(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //qiLogInfo("Behavior") << "Walk to Ball" << std::endl;

    // Construct Sonar Class
    sonarStatic->getSonar(unBoard);

    //*******************************
    //
    //     Behaviour Variables
    //
    //*******************************
    //Walk Transition and Turn Finish indicates that a movement has finished. Start Behavior will set this to initial values
    //*******************************

    writeTo(behavior, walkTransition, true);
    writeTo(behavior, turnFinished, false); //setting this for lookAround

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
    action.leds.rightEye.green = 0;

    //*******************************

    ballX = readFrom(perception, ballX);
    ballY = readFrom(perception, ballY);

    camera = readFrom(perception, isTopCamera);

    if(ballX == -1 && ballY == -1)
    {
        isLost = true;

        action.leds.leftEye.red = 1;
        action.leds.leftEye.blue = 0;
        action.leds.leftEye.green = 0;
    }
    else
    {
        isLost = false;
        action.leds.leftEye.red = 0;
        action.leds.leftEye.blue = 0;
        action.leds.leftEye.green = 1;
    }

    //*******************************
    //
    //          Motion
    //
    //*******************************
    //  Will make robot walk ahead while aligning to ball
    //  Also, reajust Yaw
    //*******************************

    action.body.actionType = ActionCommand::Body::WALK;

    action.body.forward = 100.0;
    action.body.left = 0.0;
    action.body.turn = 0.4*(160-ballX)/160;
    action.body.power = 1.0;
    action.body.bend = 15.0;
    action.body.isFast = true;

    action.head.isRelative = false;
    action.head.yawSpeed = 0.5;
    action.head.yaw = 0.0;

    writeCommand(action, unBoard);

}

BehaviorBase* walkBall::transition(void* _ub)
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

    const int Y_SWITCH_TO_BOTCAMERA = 200;

    // If ball is not lost ...
    if(!isLost){
        //walkCounter = 0;
        if (ballY >= Y_SWITCH_TO_BOTCAMERA && camera){
            writeTo(behavior, ballXApproach, ballX);
            return approachBall::Behavior();
        }
        // If Ball is close and found on bottom camera robot will kick
        else if(ballY > THRESH_BALL_CLOSE && camera == CAMERA_BOT){            
            int goleiro = readFrom(perception, approxDistance);
            if (goleiro != -1)
                return pickUp::Behavior();
            else
                return Kick::Behavior();
        }
        else
            return this;
    }
    // If Ball is Lost will start looking again
    else{
        return LookForBall::Behavior();
        //        walkCounter++;
        //        if(walkCounter > 5){
        //            walkCounter = 0;
        //            return LookForBall::Behavior();
        //        }
    }

    return this;
}
