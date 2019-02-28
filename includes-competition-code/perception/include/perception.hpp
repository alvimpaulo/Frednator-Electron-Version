#ifndef PERCEPTION_HPP
#define PERCEPTION_HPP

#include <basemodule.hpp>
#include <ballDetector.hpp>
#include <ballCandidateANN.hpp>

#include <fieldDetector.hpp>
#include <goalDetector.hpp>
#include <lineDetector.hpp>
#include <yellowDetector.hpp>
#include <ellipseDetector.hpp>
#include <alproxies/alvideodeviceproxy.h>
#include <alvision/alvisiondefinitions.h>
#include <time.h>
#include <cstdio>
#include <vector>
#include <cmath>

#include <opencv2/core/core.hpp>
#include "opencv2/objdetect/objdetect.hpp"

#define annNameXml "annBall1camada8.xml"

class perception : public baseModule
{
public:
    perception(void*);
    ~perception();
    void tick();
    void retrieveImage();
   // void getField(cv::Mat image);
   // void getGoal(cv::Mat image);
    void updateUnboard();
private:
    //int goalCenter;
    std::list<bool> buffer;
    std::list<int> distanceBuffer;
    PerceptionData visionData;
    AL::ALVideoDeviceProxy *cameraProxy;
    std::string subscriberID;
    cv::Mat raw_TopImg, raw_BotImg;
    cv::VideoCapture cap;
    cv::Point2i _ballDistance;
    BallDetector ballDetector;
    ballAnnDetector *ballAnn;


    FieldDetector fieldDetector;
    GoalDetector goalDetector;
    LineDetector lineDetector;
    EllipseDetector ellipseDetector;
    YellowDetector yellowDetector;

    bool oldCamera;
};

#endif // PERCEPTION_HPP
