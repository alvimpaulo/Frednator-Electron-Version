#ifndef YELLOWDETECTOR_HPP
#define YELLOWDETECTOR_HPP

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifndef FREDNATOR
#include "featureDetector.hpp"
#include <unBoard.hpp>
#endif
#include <time.h>

class YellowDetector : public FeatureDetector
{
  private:
    int distance;
    float angle;

  public:
#ifdef DEBUG_PERCEPTION
    std::vector<cv::Mat> debugImgVector;
#endif

    YellowDetector() : //PARÂMETROS DO BLOB
                       minThreshold(150),
                       maxThreshold(255),
                       filterByArea(true),
                       minArea(250),
                       filterByCircularity(true),
                       minCircularity(0.1),
                       filterByConvexity(true),
                       minConvexity(0.01),
                       filterByInertia(true),
                       minInertiaRatio(0.1),
                       iLowH(30),
                       iHighH(45),
                       iLowS(100),
                       iHighS(255),
                       iLowV(50),
                       iHighV(255)
    {
    }

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
    int iLowH;
    int iHighH;
    int iLowS;
    int iHighS;
    int iLowV;
    int iHighV;

    virtual void run(cv::Mat topImg, cv::Mat goalImg, PerceptionData *data);
#ifndef FREDNATOR
    virtual void updateData(PerceptionData *data);
#endif

    virtual int getDistance();
};

#endif // YELLOWDETECTOR_HPP
