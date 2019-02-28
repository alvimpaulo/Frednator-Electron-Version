#ifndef BALL_DETECTOR_HPP
#define BALL_DETECTOR_HPP

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "featureDetector.hpp"

typedef struct ballCandidate {
    cv::Point center;
    int radius;
    std::vector<cv::Rect> blackSpots;
    ballCandidate(cv::Point p, int r) {
        this->center = p;
        this->radius = r;
    }
    ballCandidate() {
        this->center = cv::Point(-1,-1);
    }
} ballCandidate;

class BallDetector : public FeatureDetector
{
private:
    const int Y_SWITCH_TO_BOTCAMERA;
    const int Y_SWITCH_TO_TOPCAMERA;
    ballCandidate bestCandidate;
#ifdef DEBUG_PERCEPTION
    std::vector<cv::Mat> debugImgVector;
#endif
public:
    BallDetector():Y_SWITCH_TO_BOTCAMERA(200), Y_SWITCH_TO_TOPCAMERA(30){}
    ballCandidate getBestBallCandidate(std::vector<ballCandidate> candidates);
    std::vector<ballCandidate> findBallCandidates(cv::Mat image);
    virtual void run(cv::Mat topImg, cv::Mat botImg, PerceptionData *data);
    virtual void updateData(PerceptionData* data);
};

#endif //BALL_DETECTOR_HPP
