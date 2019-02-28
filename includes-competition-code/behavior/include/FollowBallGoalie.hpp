#pragma once
/*
* FollowBallGoalie inherited from BehaviorBase.
*/
#include <BehaviorBase.hpp>
#include <opencv2/opencv.hpp>

class FollowBallGoalie : public BehaviorBase
{
private:
    static FollowBallGoalie *instance;
    bool camera;
    int ballX;
    int ballY;
    float directionX;
    std::vector<cv::Point2f> lines;
    std::vector<cv::Point2f> linesBuffer;
    cv::Point2f bufferAvg;

public:
    FollowBallGoalie();
    static BehaviorBase* Behavior();
    BehaviorBase* transition(void*);
    void action(void*);
};
