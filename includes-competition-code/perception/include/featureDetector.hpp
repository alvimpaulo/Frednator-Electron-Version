#ifndef FEATUREDETECTOR_HPP
#define FEATUREDETECTOR_HPP
#include <opencv2/opencv.hpp>
#ifndef FREDNATOR
#include "../../unboard/unBoard.hpp"
#endif
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
#ifndef FREDNATOR
  virtual void run(cv::Mat topImg, cv::Mat goalImg, PerceptionData *data) = 0;
  virtual void updateData(PerceptionData *) = 0;
#else
  virtual cv::Mat run(cv::Mat topImg, cv::Mat goalImg) = 0;
#endif
};

#endif // FEATUREDETECTOR_HPP
