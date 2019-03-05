#pragma once

#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <signal.h>
#include <string>
#include <map>
#include <vector>
#include <deque>
#include <opencv2/opencv.hpp>

#include <BehaviorRequest.hpp>
#include <types/ButtonPresses.hpp>
#include <Odometry.hpp>
#include <types/SensorValues.hpp>
#include <RoboCupGameControlData.h>

/**
 * Macro to wrap reads to module's UnBoard.
 * @param module which module's UnBoard to read
 * @param component the component to read
 */
#define readFrom(module, component) \
    unBoard->read(&(unBoard->module.component));
/**
 * Macro to wrap array reads from module's UnBoard.
 * Performs a memcpy on the provided arguments.
 * @param module which module's UnBoard to read from
 * @param component while component to be written
 * @param dest where to write to
 */
#define readArray(module, component, dest)  \
    memcpy(dest, unBoard->module.component, \
           sizeof(unBoard->module.component));

/**
 * Macro to wrap writes to module's UnBoard.
 * @param module which module's UnBoard to write
 * @param component while component to write
 * @param value the value to be written
 */
#define writeTo(module, component, value) \
    unBoard->write(&(unBoard->module.component), value);

/**
 * Macro to wrap array writes to module's UnBoard.
 * Performs a memcpy on the provided arguments.
 * @param module which module's UnBoard to write
 * @param component while component to write
 * @param value the value to be written
 */
#define writeArray(module, component, value) \
    memcpy(unBoard->module.component, value, \
           sizeof(unBoard->module.component));

/**
 * Macro to wrap acquiring a mutex on a the UnBoard.
 * @param name which module's lock to acquire
 */
#define acquireLock(name) \
    (unBoard->locks.name)->lock();

/**
 * Macro to wrap releasing a mutex on the UnBoard.
 * @param name which module's lock to release
 */
#define releaseLock(name) \
    (unBoard->locks.name)->unlock();

/**
 * UnBoard shared memory class, used for inter-module communication.
 * The UnBoard is friends with each of the module adapters
 * so that they can access UnBoard privates (and nobody else can)
 * The safeRun templated function has access to the UnBoard
 * to look up thread timings and things like that
 */

//Data Motion will share with other modules
struct MotionData
{
    explicit MotionData();
    ButtonPresses button;
    Odometry odometry;
    SensorValues sensors;
    int kickFinished;
    bool walkFinished;
    bool isStanding;
};

//Data Perception will share with other modules
struct PerceptionData
{
    explicit PerceptionData();
    int ballX, ballY;
    int fieldDistance;
    bool isTopCamera;
    int goalCenter;
    int lostCounter;

    std::vector<cv::Point2f> lines;
    std::vector<cv::Point> corners;
    cv::Point2f ellipseAverage;
    float ellipseAngle;
    //cv::Point2f yellowCenter;
    int approxDistance;
    float approxAngle;

#ifdef DEBUG_PERCEPTION
    //Vector containing the debug images to be used with frednator
    std::map<std::string, std::vector<cv::Mat>> debugImages;
#endif
};

//Data Communication will share with others
struct CommunicationData
{
    explicit CommunicationData();
    RoboCupGameControlData gmCtrlData;
};

//Data Behavior will share with others
struct BehaviorData
{
    explicit BehaviorData();
    BehaviorRequest request[2];
    int requestIndex;

    //Goallie

    bool searchGoal;

    // Turn
    bool walkTransition;
    bool turnFinished;
    int ballXApproach;

    // Kick
    bool isKick;

    //sonar
    //std::vector<int>
    int sonarPos;
    int sonarSize;
    std::vector<float> leftSonar, rightSonar;
    float l, r;

    //GameControlBehaviorData
    bool play_chest;
    int gameState;
    bool wasSet;
    int countAr;
    bool getUp;

    //Goalie buffer
    std::vector<cv::Point2f> linesBuffer;
};

struct SynchronisationData
{
    explicit SynchronisationData();
    boost::mutex *locker;
};

/*struct ThreadData {
   explicit ThreadData();
   std::map<std::string, boost::function<void(const boost::program_options::variables_map &)> > configCallbacks;
};*/

class UnBoard
{
    //Add friend class here

  public:
    explicit UnBoard();
    ~UnBoard();

    /* Function to read a component from the UnBoard */
    template <class T>
    const T &read(const T *component);

    /* Write a component to the UnBoard */
    template <class T>
    void write(T *component, const T &value);

//private:
#ifndef FREDNATOR
    //
    MotionData motion;
#endif
    //
    PerceptionData perception;

    //
    CommunicationData communication;

    //
    BehaviorData behavior;

    /* Data ThreadWatcher will be sharing with others 
	ThreadBlackboard thread;*/

    // Locks used for inter-thread synchronisation
    SynchronisationData locks;
};

template <class T>
const T &UnBoard::read(const T *component)
{
    return *component;
}

template <class T>
void UnBoard::write(T *component, const T &value)
{
    locks.locker->lock();
    *component = value;
    locks.locker->unlock();
}
