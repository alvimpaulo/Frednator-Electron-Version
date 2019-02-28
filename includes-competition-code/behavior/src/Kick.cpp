#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

#define THRESH_LEFT 110//114
#define THRESH_RIGHT 210//221
#define THRESH_FRONT 180//170
#define ERROR_MARGIN 10
#define LEFT_OR_RIGHT 160


Kick* Kick::instance = NULL;

Kick::Kick() : ticksCount(0), isKickable(false), isLeft(true), oldBallX(-1), oldBallY(-1), lostCount(0), lastMoveFinished(true), kickBallX(0), countOnce(0)
{

}

BehaviorBase* Kick::Behavior()
{
    if(! instance)
    {
        instance = new Kick();
    }
    return instance;
}

void Kick::action(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);


    //qiLogInfo("Behavior") << "Kick!" << std::endl;

    //*******************************
    //
    //     Behaviour Variables
    //
    //*******************************
    // Indicates if kick is finished
    //*******************************

    finished = readFrom(motion, kickFinished);
    ticksCount++;

    ActionCommand::All action;

    //*******************************
    //
    //          Leds
    //
    //*******************************
    //  State Led - Right - Yellow
    //  Ball Lost - Left - Red
    //  Ball Found - Right - Green
    //*******************************

    action.leds.rightEye.red = 1;
    action.leds.rightEye.blue = 0;
    action.leds.rightEye.green = 1;

    //*******************************
    //
    //          Vision
    //
    //*******************************
    // First Reads From Perception
    //*******************************

    int ballX, ballY;
    float headPitch;

    ballX = readFrom(perception, ballX);
    ballY = readFrom(perception, ballY);
    headPitch = readFrom(motion, sensors.joints.angles[Joints::HeadPitch]);
    writeTo(behavior, isKick, true);

    //*******************************
    //
    //          Motion
    //
    //*******************************
    //  Will make robot kick. Checks First if head is down, then goes to kick logic
    //*******************************


    if(headPitch > 0.20 && ticksCount > 15)
    {
        //qiLogInfo("Behavior") << "Cabeca Baixa!" << ballX << std::endl;
        if(ballX == -1)
        {
            // If ball is not found adds on lost buffer
            // Also saves old position, so that parameters for kicking stay ok

            ballX = oldBallX;
            ballY = oldBallY;
            lostCount++;

            //qiLogInfo("Behavior") << "Ball Lost!" << std::endl;
        }
        // Updated Old Ball
        oldBallX = ballX;
        oldBallY = ballY;

        //If position is ok, and ball has not been lost for three cycles sets kick
        if(isKickable || lostCount > 1)
        {
            isKickable = true;
            //qiLogInfo("Behavior") << "Kickable!" << std::endl;
            lostCount = 0;
            action.body.actionType = ActionCommand::Body::KICK;

            //For choosing Leg
            /*if(!isLeft)
            {
                action.body.foot = ActionCommand::Body::RIGHT;
                action.body.ballY = -45.0;
            }
            else
            {*/
                action.body.foot = ActionCommand::Body::LEFT;
                action.body.ballY = 45.0;
            //}

            action.body.ballX = 160.0;
            action.body.kickDirection = 0.0;
            action.body.misalignedKick = true;
            action.body.power = 1.0;
            action.body.bend = 15.0;
        }
        // In case ball is not kickable or is lost Robot Will Align
        else
        {
            // Lateral alignment
            if(ballY > (THRESH_FRONT - ERROR_MARGIN))// && ballY < (THRESH_FRONT + ERROR_MARGIN))
            {
                //qiLogInfo("Behavior") << "Lateral!" << std::endl;

                // Align If ball is too much too the left, robot will go right
                if((ballX > (THRESH_LEFT + ERROR_MARGIN))) //&& ballX <= LEFT_OR_RIGHT) || ballX > (THRESH_RIGHT + ERROR_MARGIN))
                {
                    action.body.actionType = ActionCommand::Body::WALK;
                    action.body.forward = 0.0;
                    action.body.left = -50.0;
                    action.body.turn = 0.0;
                    action.body.power = 1.0;
                    action.body.bend = 15.0;
                }
                // Align If ball is too much to the right, robot will go left
                else if((ballX < (THRESH_LEFT - ERROR_MARGIN)))// || (ballX < (THRESH_RIGHT - ERROR_MARGIN) && ballX > LEFT_OR_RIGHT))
                {
                    action.body.actionType = ActionCommand::Body::WALK;
                    action.body.forward = 0.0;
                    action.body.left = 50.0;
                    action.body.turn = 0.0;
                    action.body.power = 1.0;
                    action.body.bend = 15.0;
                }
                // Will only update isLeft once
                else if(countOnce == 0)
                {
                    countOnce = 1;
                    kickBallX = ballX;
                    if(ballX > LEFT_OR_RIGHT)
                        isLeft = false;
                    else
                        isLeft = true;
                    isKickable = true;
                }
            }
            //Goes Forward to Ball
            else
            {
                //qiLogInfo("Behavior") << "Front!" << std::endl;
                action.body.actionType = ActionCommand::Body::WALK;
                action.body.forward = 120.0;
                action.body.left = 0.0;
                action.body.turn = 0.0;
                action.body.power = 1.0;
                action.body.bend = 15.0;
            }
        }
    }
    else
    {
        //qiLogInfo("Behavior") << "Wait Head!" << std::endl;
        action.body.actionType = ActionCommand::Body::WALK;
        action.body.forward = 0.0;
        action.body.left = 0.0;
        action.body.turn = 0.0;
        action.body.power = 1.0;
        action.body.bend = 15.0;
    }

    action.head.yawSpeed = 1.0;
    action.head.yaw = 0.0;
    action.head.isRelative = false;
    if(lastMoveFinished || isKickable)
    {
        //qiLogInfo("Behavior") << "New Command!" << std::endl;
        writeTo(motion, walkFinished, false);
        //action.body.speed = 1.0;
        action.head.yawSpeed = 0.5;
        action.head.yaw = 0.0;
        action.head.pitch = 0.25;
        action.head.pitchSpeed = 1.0;


        writeCommand(action, unBoard);
    }
    lastMoveFinished = readFrom(motion, walkFinished);
}

BehaviorBase* Kick::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);


    finished = readFrom(motion, kickFinished);

    if(finished == 1 && isKickable)
    {
        countOnce = 0;
        ticksCount = 0;
        isKickable = false;
        writeTo(perception, ballX, -1);
        writeTo(perception, ballY, -1);
        writeTo(behavior, isKick, false);
        return postKick::Behavior();
    }

    return this;
}
