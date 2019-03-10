/**
    \file featureDetector.hpp
    \author UnBeatables
    \date LARC2018
    \name featureDetector
*/

#ifndef FEATUREDETECTOR_HPP
#define FEATUREDETECTOR_HPP
#include <opencv2/opencv.hpp>
#include "../../unboard/unBoard.hpp"

class FeatureDetector
{
  protected:
    int lastSeen;

  public:
    /**
        \brief Class constructor
    */
    FeatureDetector() : lastSeen(0) {}

    /**
        \brief Class destructor
    */
    ~FeatureDetector() {}

    /**
        \brief Updates class lastSeen variable
    */
    inline int getLastSeen()
    {
        return this->lastSeen;
    }
    /**
        \brief Anull class lastSeen variable
    */
    virtual void foundFeature()
    {
        this->lastSeen = 0;
    }
    /**
        \brief Increments class lastSeen variable
    */
    virtual void featureNotFound()
    {
        this->lastSeen++;
    }
#ifndef FREDNATOR
    virtual void run(cv::Mat topImg, cv::Mat goalImg, PerceptionData *data) = 0;
#else
    virtual cv::Mat run(cv::Mat topImg, cv::Mat goalImg, PerceptionData *data) = 0;
#endif
    virtual void updateData(PerceptionData *) = 0;
};

#endif // FEATUREDETECTOR_HPP
