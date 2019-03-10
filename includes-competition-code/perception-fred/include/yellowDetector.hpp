/** 
    \file yellowDetector.hpp
    \author UnBeatables
    \date LARC2018
    \name yellowDetector
*/

#ifndef YELLOWDETECTOR_HPP
#define YELLOWDETECTOR_HPP

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "featureDetector.hpp"



class YellowDetector : public FeatureDetector
{
  private:
    /**
        
    */
    int distance;
    float angle;

public:

#ifdef DEBUG_PERCEPTION
    std::vector<cv::Mat> debugImgVector;
#endif

/**
        \brief Class constructor
    */
    YellowDetector():
    //PARÂMETROS DO BLOB
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
    {}

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

/**
        \brief This function checks for golkeaper yellow shirt to prevent own goals.
        \details Using a yellow mask, this function looks for yellow masses and estimates its distance.
        \param topImg Top camera frame
        \param goalImg 
        \param data perception data to update
    */
#ifndef FREDNATOR
    virtual void run(cv::Mat topImg, cv::Mat goalImg, PerceptionData *data);
#else
    virtual cv::Mat run(cv::Mat topImg, cv::Mat goalImg, PerceptionData *data);
#endif

    /** \brief This function updates the perception data with information received from yellow detector.
        \param data perception data to update
    */
    virtual void updateData(PerceptionData *data);

    /** \brief Update distance variable
    */
    virtual int getDistance();
};

#endif // YELLOWDETECTOR_HPP
