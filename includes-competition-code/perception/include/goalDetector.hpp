#ifndef GOALDETECTOR_HPP
#define GOALDETECTOR_HPP

#include "featureDetector.hpp"

typedef struct goalCandidate {
    int imgPosX;
    goalCandidate():imgPosX(-1){}
} goalCandidate;

class GoalDetector : public FeatureDetector
{
private:
    goalCandidate bestGoal;
    cv::Mat roiGoal;
    int greenVerticalAvg;

#ifdef DEBUG_PERCEPTION
    std::vector<cv::Mat> debugImgVector;
#endif



public:
    GoalDetector():
    grad_threshold(240),
    scale(1),
    delta(0)
    {}
    virtual void run(cv::Mat topImg, cv::Mat goalImg, PerceptionData *data);
    int getGreenVerticalAvg(){return this->greenVerticalAvg;}
    void setGreenVerticalAvg(int newGreenVerticalAvg){this->greenVerticalAvg = newGreenVerticalAvg;}
    virtual void updateData(PerceptionData *data);

    //treshold for binary image
    int grad_threshold;
    //Sobel arguments
    int scale;
    int delta;
};

#endif // GOALDETECTOR_HPP
