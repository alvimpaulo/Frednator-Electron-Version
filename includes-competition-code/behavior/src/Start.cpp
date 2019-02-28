#include <includeMapBehavior.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <qi/log.hpp>

#include <movingAverage.hpp>

Start* Start::instance = NULL;

Start::Start()
{
    standing = false; // has not stood up
    sonarOutput = SonarData::NONE;
    sonarVec.reserve(10);
    std::fill(sonarVec.begin(), sonarVec.end(), 0);
    kickOff = false;

}

BehaviorBase* Start::Behavior()
{
    if(! instance)
    {
        instance = new Start();
    }
    return instance;
}



void Start::action(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Read variable from Set State. If this is true, will go to WalkInit Satate

    kickOff = readFrom(communication, gmCtrlData.kickOffTeam);
    kickOff = !kickOff;

    ////qiLogInfo("Behavior") << "KickOff: " << kickOff << std::endl;

    //*******************************
    //
    //     Behaviour Variables
    //
    //*******************************
    //Walk Transition and Turn Finish indicates that a movement has finished. Start Behavior will set this to initial values
    //*******************************

    writeTo(behavior, walkTransition, true);
    writeTo(behavior, turnFinished, false); //setting this for lookAround

    lastBehavior = this->instance;

    ////qiLogInfo("Behavior") << "Starting Fred!" << std::endl;

    //*******************************
    //          Sonar
    //*******************************
    // Construct Sonar Class
    //*******************************

    sonarStatic->getSonar(unBoard);

    //*******************************
    //
    //      Motion Commands
    //
    //*******************************
    // Robot Will Stand and Set Head Ahead
    //*******************************

    ActionCommand::All action;

    // Both eyes White for Start
    action.leds.rightEye.red = 1;
    action.leds.rightEye.blue = 1;
    action.leds.rightEye.green = 1;
    action.leds.leftEye.red = 1;
    action.leds.leftEye.blue = 1;
    action.leds.leftEye.green = 1;


    action.body.actionType = ActionCommand::Body::STAND;
    action.head.isRelative = false;
    action.head.yawSpeed = 0.5;
    action.head.yaw = 0.0;
    writeCommand(action, unBoard);

    //Checks if Robot is Standing so that action is finished
    standing = readFrom(motion, isStanding);


}

BehaviorBase* Start::transition(void* _ub)
{

    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Transition to LookFor Ball If Standing Motion Has Finished
    if(standing == false && !kickOff)
    {
        return LookForBall::Behavior();

    }
    //During KickOff Robot Will Walk
    else if(standing == false && kickOff)
    {
        return WalkInit::Behavior();
    }

    return this;

}
