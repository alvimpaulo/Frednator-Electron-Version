#ifndef FEATUREDETECTOR_HPP
#define FEATUREDETECTOR_HPP
#include <opencv2/opencv.hpp>
#include "../../unboard/unBoard.hpp"

class FeatureDetector
{
  protected:
    int lastSeen;

  public:
    FeatureDetector() : lastSeen(0) {}
    ~FeatureDetector() {}
    inline int getLastSeen()
    {
        return this->lastSeen;
    }
    virtual void foundFeature()
    {
        this->lastSeen = 0;
    }
    virtual void featureNotFound()
    {
        this->lastSeen++;
    }

    virtual void run(cv::Mat topImg, cv::Mat goalImg, PerceptionData *data) = 0;
    virtual void updateData(PerceptionData *) = 0;
};

#endif // FEATUREDETECTOR_HPP
