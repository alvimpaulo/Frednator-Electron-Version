#include "ballDetector2.hpp"
#include <string>

void BallDetector2::run(cv::Mat imgTop, cv::Mat imgBot, PerceptionData* data)
{
    if(data->isTopCamera == true){

    }



    //******* Top Camera *******//

    cv::Mat top_HLS_green, top_HLS_white;

    //Color transformations
    cv::cvtColor(imgTop, top_HLS_green, cv::COLOR_BGR2HLS);
    top_HLS_white = top_HLS_green.clone();
    cv::blur(top_HLS_green, top_HLS_green, cv::Size(5,5));
    cv::inRange(top_HLS_green, cv::Scalar(iLowH, iLowL, iLowS), cv::Scalar(iHighH, iHighL, iHighS), top_HLS_green);
    cv::inRange(top_HLS_white, cv::Scalar(iLowH2, iLowL2, iLowS2), cv::Scalar(iHighH2, iHighL2, iHighS2), top_HLS_white);

    cv::Mat erodeTop_green, dilateTop_green, dilateTop_white;

    cv::Mat element, element2;
    element2 = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(5,5), cv::Point(-1,-1));

    if(kernel>0){
        element = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(kernel,kernel), cv::Point(-1,-1));
    }

    cv::erode(top_HLS_green, erodeTop_green, element2, cv::Point(-1,-1),1, cv::BORDER_CONSTANT);
    cv::dilate(erodeTop_green, dilateTop_green, element, cv::Point(-1,-1),1, cv::BORDER_CONSTANT);
    cv::dilate(top_HLS_white, dilateTop_white, element, cv::Point(-1,-1),1, cv::BORDER_CONSTANT);

#ifdef DEBUG_PERCEPTION
    //Create an image vector, put the desired images inside it and atualize the perception data debugImages with it.
    debugImgVector.assign(1, imgTop); //0

    cv::cvtColor( top_HLS_green, top_HLS_green, CV_GRAY2BGR);
    debugImgVector.push_back(top_HLS_green); //2
    cv::cvtColor( top_HLS_green, top_HLS_green, CV_BGR2GRAY);

    cv::cvtColor( top_HLS_white, top_HLS_white, CV_GRAY2BGR);
    debugImgVector.push_back(top_HLS_white); //3
    cv::cvtColor( top_HLS_white, top_HLS_white, CV_BGR2GRAY);

    cv::cvtColor( dilateTop_green, dilateTop_green, CV_GRAY2BGR);
    debugImgVector.push_back(dilateTop_green); //2
    cv::cvtColor(dilateTop_green, dilateTop_green, CV_BGR2GRAY);

    cv::cvtColor( dilateTop_white, dilateTop_white, CV_GRAY2BGR);
    debugImgVector.push_back(dilateTop_white); //3
    cv::cvtColor( dilateTop_white, dilateTop_white, CV_BGR2GRAY);

#endif

    cv::Mat topBallMask;
    cv::bitwise_not(dilateTop_green, topBallMask);
    cv::bitwise_and(topBallMask, dilateTop_white, topBallMask);
    cv::bitwise_not(topBallMask,topBallMask);

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
    topDetector.detect(topBallMask, keypoints);

    cv::Mat im_with_keypoints;
    cv::drawKeypoints(topBallMask, keypoints, im_with_keypoints, cv::Scalar(0,0,255), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);


    cv::Point2f coordenate;
    float radius;
    if(keypoints.size()!=0){
        coordenate = keypoints[0].pt;
        radius = keypoints[0].size;
    }

    float centerx = coordenate.x;
    float centery = coordenate.y;
    std::cout<< centerx <<" "<< centery<< std::endl;

    cv::SimpleBlobDetector topDetector2(params);
    std::vector<cv::KeyPoint> keypoints2;
    topDetector2.detect(dilateTop_green, keypoints2);

    cv::Mat im_with_keypoints2;
    cv::drawKeypoints(dilateTop_green, keypoints2, im_with_keypoints2, cv::Scalar(0,0,255), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

#ifdef DEBUG_PERCEPTION
//Create an image vector, put the desired images inside it and atualize the perception data debugImages with it.
    debugImgVector.push_back(im_with_keypoints);
    debugImgVector.push_back(im_with_keypoints2);
#endif



    //******** Bot Cameta *********//

    cv::Mat bot_HLS_green, bot_HLS_white;

    //Color transformations
    cv::cvtColor(imgBot, bot_HLS_green, cv::COLOR_BGR2HLS);
    bot_HLS_white = bot_HLS_green.clone();
    cv::blur(bot_HLS_green, bot_HLS_green, cv::Size(5,5));
    cv::inRange(bot_HLS_green, cv::Scalar(iLowH, iLowL, iLowS), cv::Scalar(iHighH, iHighL, iHighS), bot_HLS_green);
    cv::inRange(bot_HLS_white, cv::Scalar(iLowH2, iLowL2, iLowS2), cv::Scalar(iHighH2, iHighL2, iHighS2), bot_HLS_white);

    cv::Mat erodeBot_green, dilateBot_green, dilateBot_white;
    if(kernel>0){
        element = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(kernel,kernel), cv::Point(-1,-1));
    }

    cv::erode(bot_HLS_green, erodeBot_green, element2, cv::Point(-1,-1),1, cv::BORDER_CONSTANT);
    cv::dilate(erodeBot_green, dilateBot_green, element, cv::Point(-1,-1),1, cv::BORDER_CONSTANT);
    cv::dilate(bot_HLS_white, dilateBot_white, element, cv::Point(-1,-1),1, cv::BORDER_CONSTANT);

#ifdef DEBUG_PERCEPTION
    //Create an image vector, put the desired images inside it and atualize the perception data debugImages with it.
    debugImgVector.push_back(imgBot); // 1

    cv::cvtColor( bot_HLS_green, bot_HLS_green, CV_GRAY2BGR);
    debugImgVector.push_back(bot_HLS_green); //2
    cv::cvtColor( bot_HLS_green, bot_HLS_green, CV_BGR2GRAY);

    cv::cvtColor( bot_HLS_white, bot_HLS_white, CV_GRAY2BGR);
    debugImgVector.push_back(bot_HLS_white); //3
    cv::cvtColor( bot_HLS_white, bot_HLS_white, CV_BGR2GRAY);

    cv::cvtColor( dilateBot_green, dilateBot_green, CV_GRAY2BGR);
    debugImgVector.push_back(dilateBot_green); //2
    cv::cvtColor(dilateBot_green, dilateBot_green, CV_BGR2GRAY);

    cv::cvtColor( dilateBot_white, dilateBot_white, CV_GRAY2BGR);
    debugImgVector.push_back(dilateBot_white); //3
    cv::cvtColor( dilateBot_white, dilateBot_white, CV_BGR2GRAY);

#endif

    cv::Mat botBallMask;
    cv::bitwise_not(dilateBot_green, botBallMask);
    cv::bitwise_and(botBallMask, dilateBot_white, botBallMask);
    cv::bitwise_not(botBallMask,botBallMask);

    cv::SimpleBlobDetector botDetector(params);
    std::vector<cv::KeyPoint> bot_keypoints;
    botDetector.detect(botBallMask, bot_keypoints);

    cv::Mat im_with_bot_keypoints;
    cv::drawKeypoints(botBallMask, bot_keypoints, im_with_bot_keypoints, cv::Scalar(0,0,255), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);


    cv::Point2f coordenate2;
    float radius2;

    if(bot_keypoints.size()!=0){
        coordenate2 = bot_keypoints[0].pt;
        radius2 = bot_keypoints[0].size;
    }

    float centerx2 = coordenate2.x;
    float centery2 = coordenate2.y;
    std::cout<< centerx2 <<" "<< centery2 << std::endl;

    cv::SimpleBlobDetector botDetector2(params);
    std::vector<cv::KeyPoint> bot_keypoints2;
    botDetector2.detect(dilateBot_green, bot_keypoints2);

    cv::Mat im_with_bot_keypoints2;
    cv::drawKeypoints(dilateBot_green, bot_keypoints2, im_with_bot_keypoints2, cv::Scalar(0,0,255), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);


    //updateData(data);

#ifdef DEBUG_PERCEPTION
//Create an image vector, put the desired images inside it and atualize the perception data debugImages with it.
    debugImgVector.push_back(im_with_bot_keypoints);
    debugImgVector.push_back(im_with_bot_keypoints2);

    std::pair<std::map<std::string,std::vector<cv::Mat> >::iterator, bool> debugInsertion;
    debugInsertion = data->debugImages.insert(std::make_pair("ballDetector2", debugImgVector));
    if(!debugInsertion.second){
        data->debugImages["ballDetector2"] = debugImgVector;
    }

#endif
}

void BallDetector2::updateData(PerceptionData *data)
{
    data->ballX = this->bestCandidate.center.x;
    data->ballY = this->bestCandidate.center.y;
}
