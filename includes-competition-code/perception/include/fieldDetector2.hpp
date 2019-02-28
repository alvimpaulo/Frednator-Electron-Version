#ifndef FIELDDETECTOR2_HPP
#define FIELDDETECTOR2_HPP

#include <featureDetector.hpp>


class FieldDetector2 : public FeatureDetector
{
private:
    cv::Mat greenRegion;
    int horizont;

#ifdef DEBUG_PERCEPTION
    std::vector<cv::Mat> debugImgVector;
#endif

public:
    FieldDetector2():
        iLowH(30),
        iHighH(90),
        iLowL(50),
        iHighL(200),
        iLowS(10),
        iHighS(255),
        factor(1.0),
        dp(1),
        minDist(30),
        param1(62),
        param2(5),
        maxRadius(30),
        minRadius(5),
        kernel(3)
    {}
    cv::Mat getGreenRegion();
    virtual void run(cv::Mat imgTop, cv::Mat imgBot, PerceptionData *data);
    virtual void updateData(PerceptionData* data);

    //Green HSL values range definitions
    int iLowH, iHighH, iLowS, iHighS, iLowL, iHighL;
    float factor;
    double dp, minDist, param1, param2;
    int maxRadius, minRadius, kernel;
};

#endif // FIELDDETECTOR2_HPP
