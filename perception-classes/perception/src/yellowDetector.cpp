#include "yellowDetector.hpp"
#ifndef FREDNATOR
#include <qi/log.hpp>
#endif
#define DEBUG_PERCEPTION 0

void YellowDetector::run(cv::Mat topImg, cv::Mat goalImg, PerceptionData *data)
{
    std::cout << "yellow" << std::endl;
    cv::Mat src = topImg.clone();

    cv::Mat src_HSV;
    std::vector<std::vector<cv::Point>> contours;
    int thresh = 100;
    std::vector<cv::Vec4i> hierarchy;

    cv::cvtColor(src, src_HSV, cv::COLOR_BGR2HSV);
    cv::blur(src_HSV, src_HSV, cv::Size(2, 2));
    cv::inRange(src_HSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), src_HSV);

#ifdef DEBUG_PERCEPTION
    //Create an image vector, put the desired images inside it and atualize the perception data debugImages with it.
    debugImgVector.assign(1, topImg); //0

    cv::cvtColor(src_HSV, src_HSV, CV_GRAY2BGR);
    debugImgVector.push_back(src_HSV); //2
    cv::cvtColor(src_HSV, src_HSV, CV_BGR2GRAY);

#endif

    cv::Mat element = cv::getStructuringElement(0, cv::Size(3, 3), cv::Point(1, 1));

    //cv::erode(src_HSV, src_HSV, element);
    cv::dilate(src_HSV, src_HSV, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

    //FAZ O BLOB
    cv::Mat topYellowMask = src_HSV;

    cv::bitwise_not(src_HSV, topYellowMask);

    cv::SimpleBlobDetector::Params params;

    params.minThreshold = minThreshold;
    params.maxThreshold = maxThreshold;
    params.filterByArea = filterByArea;
    params.minArea = minArea;
    params.filterByCircularity = filterByCircularity;
    params.minCircularity = minCircularity;
    params.filterByConvexity = filterByConvexity;
    params.minConvexity = minConvexity;
    params.filterByInertia = filterByInertia;
    params.minInertiaRatio = minInertiaRatio;

    cv::SimpleBlobDetector topDetector(params);
    std::vector<cv::KeyPoint> keypoints;
    topDetector.detect(topYellowMask, keypoints);

    cv::Mat im_with_keypoints;
    cv::drawKeypoints(topYellowMask, keypoints, im_with_keypoints, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    //float fieldpercentage = (float)keypoints.size()/(src.rows*src.cols);

    //TODO consertar a distância
    if (keypoints.size() != 0)
    {
        this->distance = keypoints[0].size; //retorna raio
    }
    else
    {
        this->distance = -1;
    }
    std::cout << "Distance: " << this->distance << std::endl;

#ifdef DEBUG_PERCEPTION

    //Create an image vector, put the desired images inside it and atualize the perception data debugImages with it.

    cv::cvtColor(topYellowMask, topYellowMask, CV_GRAY2BGR);
    debugImgVector.push_back(topYellowMask); //2
    cv::cvtColor(topYellowMask, topYellowMask, CV_BGR2GRAY);

    debugImgVector.push_back(im_with_keypoints);

    // atualize the perception data debugImages with debugImgVector.
    std::pair<std::map<std::string, std::vector<cv::Mat>>::iterator, bool> debugInsertion;
    debugInsertion = data->debugImages.insert(std::make_pair("yellowDetector", debugImgVector));
    if (!debugInsertion.second)
    {
        data->debugImages["yellowDetector"] = debugImgVector;
    }
#endif

    updateData(data);
}

#ifndef FREDNATOR

void YellowDetector::updateData(PerceptionData *data)
{
    //data->yellowCenter = this->mc;
    data->approxDistance = this->distance;
    //data->approxAngle = this->angle;
}

#endif

int YellowDetector::getDistance()
{
    return this->distance;
}
