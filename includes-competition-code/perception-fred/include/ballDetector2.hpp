#ifndef BALL_DETECTOR2_HPP
#define BALL_DETECTOR2_HPP

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "featureDetector.hpp"
#include <unBoard.hpp>

typedef struct ball2Candidate {
    cv::Point center;
    int radius;
    std::vector<cv::Rect> blackSpots;
    ball2Candidate(cv::Point p, int r) {
        this->center = p;
        this->radius = r;
    }
    ball2Candidate() {
        this->center = cv::Point(-1,-1);
    }
} ball2Candidate;

class BallDetector2 : public FeatureDetector
{
private:
    ball2Candidate bestCandidate;
    PerceptionData vision;
#ifdef DEBUG_PERCEPTION
    std::vector<cv::Mat> debugImgVector;
#endif
public:

    BallDetector2():
        iLowH(55),
        iHighH(80),
        iLowL(20),
        iHighL(170),
        iLowS(30),
        iHighS(150),
        iLowH2(0),
        iHighH2(180),
        iLowL2(80),
        iHighL2(200),
        iLowS2(0),
        iHighS2(200),
        kernel(3),
        minThreshold(150),
        maxThreshold(250),
        filterByArea(true),
        minArea(400),
        filterByCircularity(true),
        minCircularity(0.4),
        filterByConvexity(true),
        minConvexity(0.01),
        filterByInertia(true),
        minInertiaRatio(0.01)
    {}

    int iLowH, iHighH, iLowS, iHighS, iLowL, iHighL;
    int iLowH2, iHighH2, iLowS2, iHighS2, iLowL2, iHighL2;
    int kernel;
    int minThreshold;
    int maxThreshold;
    bool filterByArea;
    int minArea;
    bool filterByCircularity;
    double minCircularity;
    bool filterByConvexity;
    double minConvexity;
    bool filterByInertia;
    double minInertiaRatio;


    virtual void run(cv::Mat topImg, cv::Mat botImg, PerceptionData *data);
    virtual void updateData(PerceptionData* data);
};

#endif //BALL_DETECTOR_HPP2
