/** 
    \file yellowDetector.cpp
    \author UnBeatables
    \date LARC2018
    \name yellowDetector
*/

#include "../include/yellowDetector.hpp"
void YellowDetector::run(cv::Mat topImg, cv::Mat goalImg, PerceptionData *data)
{
    debugImgVector.clear();
    cv::Mat src = topImg.clone();

    //Yellow HSL values range definitions
    int iLowH = 22;
    int iHighH = 29;
    int iLowS = 100;
    int iHighS = 255;
    int iLowV = 100;
    int iHighV = 255;

    //Variables initialization
    cv::Mat src_HSV;
    std::vector<std::vector<cv::Point>> contours;
    int thresh = 100;
    std::vector<cv::Vec4i> hierarchy;

    //Color transformations
    cv::cvtColor(src, src_HSV, cv::COLOR_BGR2HSV);
    cv::blur(src_HSV, src_HSV, cv::Size(2, 2));
    cv::inRange(src_HSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), src_HSV);

#ifdef DEBUG_PERCEPTION
    //Create an image vector, put the desired images inside it and atualize the perception data debugImages with it.
    debugImgVector.assign(1, topImg); //0

    debugImgVector.push_back(src_HSV); //1

#endif
    //Morphological transformations
    cv::Mat element = cv::getStructuringElement(0, cv::Size(3, 3), cv::Point(1, 1));
    cv::erode(src_HSV, src_HSV, element);
    cv::dilate(src_HSV, src_HSV, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::erode(src_HSV, src_HSV, element);

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

#ifdef FREDNATOR
    cv::Ptr<cv::SimpleBlobDetector> topDetector = cv::SimpleBlobDetector::create(params);
    std::vector<cv::KeyPoint>
        keypoints;
    topDetector->detect(topYellowMask, keypoints);
#else
    cv::SimpleBlobDetector topDetector(params);
    std::vector<cv::KeyPoint> keypoints;
    topDetector.detect(topYellowMask, keypoints);
#endif
    cv::Mat im_with_keypoints;
    cv::drawKeypoints(topYellowMask, keypoints, im_with_keypoints, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DEFAULT);

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
    //std::cout << "Distance: " << this->distance << std::endl;

#ifdef DEBUG_PERCEPTION

    //Create an image vector, put the desired images inside it and atualize the perception data debugImages with it.

    debugImgVector.push_back(topYellowMask); //2

    /*     cv::imshow("a", im_with_keypoints);
    cv::waitKey(0);
    cv::destroyAllWindows(); */

    debugImgVector.push_back(im_with_keypoints); //3

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

void YellowDetector::updateData(PerceptionData *data)
{
    //data->yellowCenter = this->mc;
    data->approxDistance = this->distance;
    //data->approxAngle = this->angle;
}

int YellowDetector::getDistance()
{
    return this->distance;
}
