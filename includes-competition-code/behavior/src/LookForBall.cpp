#include <includeMapBehavior.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <qi/log.hpp>

#define MAX_HEAD_YAW 0.7

LookForBall* LookForBall::instance = NULL;

LookForBall::LookForBall()
{
    topOrDown = false;
    nextHeadPos = LEFT;
    timeOut = 0;
    sign = 1;
    headCount = 0;
    sonarOutput = SonarData::NONE;

    ballX = -1;
    ballY = -1;
    lostCount = 0;
    isLost = true;
    camera = 0;


}

BehaviorBase* LookForBall::Behavior()
{
    if(! instance)
    {
        instance = new LookForBall();
    }
    return instance;
}



void LookForBall::action(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    lastBehavior = this->instance;


    ////qiLogInfo("Behavior") << "Look For Ball!" << std::endl;


    ActionCommand::All action;

    //*******************************
    //
    //          Leds
    //
    //*******************************
    // Look For ball State is Blue
    // Ball Lost Red
    // Ball Found Green
    //*******************************

    action.leds.rightEye.red = 0;
    action.leds.rightEye.blue = 1;
    action.leds.rightEye.green = 0;

    // Sets Head Pitch Back to Front
    action.head.isRelative = false;

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

    if (ballX == -1)
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
    //  Will set robot to stand in same position
    //  Head Will move 45 degrees left or right
    //*******************************

    action.body.actionType = ActionCommand::Body::WALK;

    action.body.forward = 0.0;
    action.body.left = 0.0;
    action.body.turn = 0.0;
    action.body.power = 1.0;
    action.body.bend = 15.0;

    ////qiLogInfo("Behavior") << "Head POS!" << nextHeadPos << std::endl;

    // Only if Ball is lost
    if(isLost == true)
    {
        action.head.isRelative = true;
        action.head.yawSpeed = 0.1;
        action.head.yaw = sign*0.05;


        float headTmp = readFrom(motion, sensors.joints.angles[Joints::HeadYaw] );

        //Logic for moving head

        if(headTmp >= MAX_HEAD_YAW && nextHeadPos == LEFT)
        {
            nextHeadPos = RIGHT;
            sign = -1;

        }
        else if(headTmp <= -MAX_HEAD_YAW && nextHeadPos == RIGHT)
        {
            nextHeadPos = LEFT;
            sign = 1;
            ////qiLogInfo("Behavior") << "timeout: " << timeOut << std::endl;

            //Means Robot has Looked to both sides
            //topOrDown = !topOrDown;
            timeOut++;
            camera = readFrom(perception, isTopCamera);
            writeTo(perception, ballX, -1);
            writeTo(perception, ballY, -1);
            writeTo(perception,isTopCamera,!camera);
        }

    }
    else
    {
        action.leds.leftEye.red = 0;
        action.leds.leftEye.blue = 0;
        action.leds.rightEye.green = 1;
    }

    //action.head.pitch = topOrDown ? 0.0 : 0.35;
    action.head.pitch = 0.0;
    action.head.pitchSpeed = 1.0;
    writeCommand(action, unBoard);

    //???
    //boost::this_thread::sleep(boost::posix_time::milliseconds(1000));


}

BehaviorBase* LookForBall::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);


    // If ball is found
    if(!isLost) {
        timeOut = 0;
        return faceBall::Behavior();
    }

    // Only when full motion is finished that transition will be called for
    else if (timeOut == 2)
    {
        timeOut = 0;
        writeTo(motion, walkFinished, false);
        return LookAround::Behavior();
    }
    else if(timeOut < 2)
    {
        return this;
    }
}
