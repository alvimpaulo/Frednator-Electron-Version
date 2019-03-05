#include <unBoard.hpp>
#ifndef FREDNATOR
#include <BehaviorBase.hpp>
#endif
UnBoard::UnBoard()
{
}

UnBoard::~UnBoard()
{
}

MotionData::MotionData()
{
    kickFinished = 0;
    walkFinished = false;
    isStanding = false;
}

PerceptionData::PerceptionData()
{
    ballX = -1;
    ballY = -1;
    fieldDistance = -1;
    isTopCamera = 1;
    lostCounter = 0;
    approxDistance = -1;
}

CommunicationData::CommunicationData()
{
}
#ifndef FREDNATOR
BehaviorData::BehaviorData()
{
    requestIndex = 0;
    walkTransition = true;
    turnFinished = false;
    ballXApproach = 0;
    isKick = false;
    searchGoal = false;

    //sonarPos = 0;
    //sonarFilter = 0;

    leftSonar.reserve(10);
    rightSonar.reserve(10);
    //sonarPos.reserve(10);

    sonarPos = 0;
    l = 0;
    r = 0;

    for (int i = 0; i < 10; i++)
    {
        leftSonar.push_back(0.0);
        rightSonar.push_back(0.0);
        //sonarPos.push_back(0);
    }

    for (int i = 0; i < 20; i++)
    {
        linesBuffer.push_back(cv::Point2f());
        linesBuffer[i].y = CV_PI;
        linesBuffer[i].x = 120;
    }

    getUp = false;

    play_chest = false;
    gameState = GAME_STATE::INITIAL;
    wasSet = false;

    countAr = 0;
}
#endif
SynchronisationData::SynchronisationData()
{
    locker = new boost::mutex();
}
