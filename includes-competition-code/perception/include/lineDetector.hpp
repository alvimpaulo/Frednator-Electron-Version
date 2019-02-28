#ifndef LINEDETECTOR_HPP
#define LINEDETECTOR_HPP

#include "featureDetector.hpp"

class LineDetector : public FeatureDetector
{
private:
    std::vector<cv::Point2f> lines, filteredLines;
    std::vector<cv::Point> corners;

#ifdef DEBUG_PERCEPTION
    std::vector<cv::Mat> debugImgVector;
#endif


public:
    LineDetector():
        line_threshold_theta_low(0.1),
        line_threshold_theta_high(0.4),
        line_threshold_rho(6),
        houghLinesThreshold(70)
    {}
    virtual void run(cv::Mat topImg, cv::Mat greenFrame, PerceptionData *data);
    virtual void updateData(PerceptionData *data);

    // Thresholds for angles between lines and hough lines to limit the ammount of lines
    float line_threshold_theta_low;
    float line_threshold_theta_high;
    int line_threshold_rho;
    int houghLinesThreshold;

};

#endif // LINEDETECTOR_HPP
