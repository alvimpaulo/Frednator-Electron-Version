#include "ballDetector.hpp"


/*
 *
 * PARTE DE SEGMENTACAO DA BOLA (PARA PERTO)
 *
 */

ballCandidate BallDetector::getBestBallCandidate(std::vector<ballCandidate> candidates)
{
    if (candidates.size() == 0)
        return ballCandidate(cv::Point(-1,-1),0);
    int bestI = 0, maxArea = 0, nSpots = 0;
    for (unsigned int i = 0; i < candidates.size(); i++)
    {
        int blackArea = 0;
        for (unsigned int j = 0; j < candidates[i].blackSpots.size(); j++)
        {
            blackArea += candidates[i].blackSpots[j].width * candidates[i].blackSpots[j].height;
        }
        if (blackArea > 100 && candidates[i].blackSpots.size() > nSpots)
        {
            nSpots = candidates[i].blackSpots.size();
            maxArea = blackArea;
            bestI = i;
        }
    }
    if (maxArea == 0)
        return ballCandidate(cv::Point(-1,-1),0);
    return candidates[bestI];
}

bool isCircleWhiteFilled(cv::Mat img, cv::Point2f center, float radius, float whitePercentage = 0.35)
{
    if (img.cols == 0 or img.rows == 0)
        return false;
    cv::Rect rect(center.x-radius, center.y-radius, 2*radius, 2*radius);
    if(rect.x < 0) rect.x = 0;
    if(rect.y < 0) rect.y = 0;
    if(rect.x + rect.width > img.cols) {
        rect.width = img.cols - rect.x;
    }
    if(rect.y + rect.height > img.rows) {
        rect.height = img.rows - rect.y;
    }
    cv::Mat roi(img, rect);
    float roiSize = roi.cols * roi.rows;
    if (cv::countNonZero(roi)/roiSize > whitePercentage)
        return true;
    else
        return false;
}

std::vector<ballCandidate> BallDetector::findBallCandidates(cv::Mat image)
{
    const int MIN_BALL_SIZE = 15;//30
    const int MAX_BALL_SIZE = 80;//80
    const int MAX_BLACK_AREA = 1200;
    const int NEIGHBORHOOD_DISTANCE = 30;
    const int ILOWY = 0;
    const int IHIGHY = 70;

    const int ILOWCR = 110;
    const int IHIGHCR = 150;

    const int ILOWCB = 90;
    const int IHIGHCB = 130;

    //cv::Mat image = isTopCamera? raw_TopImg:raw_BotImg;
    std::vector<ballCandidate> candidates;

    cv::Mat blackMasses, whiteMasses, rawImg;

    image.copyTo(rawImg);

    // get black masses
    cv::cvtColor(image, blackMasses, CV_BGR2YCrCb);
#ifdef DEBUG_PERCEPTION
    cv::cvtColor( blackMasses, blackMasses, CV_BGR2YCrCb);
    debugImgVector.push_back(blackMasses);
    cv::cvtColor( blackMasses, blackMasses, CV_YCrCb2BGR);
#endif
    cv::inRange(blackMasses, cv::Scalar(ILOWY, ILOWCR, ILOWCB), cv::Scalar(IHIGHY, IHIGHCR, IHIGHCB), blackMasses);
    cv::GaussianBlur(blackMasses, blackMasses, cv::Size(5,5), 0, 0);

#ifdef DEBUG_PERCEPTION
    cv::cvtColor( blackMasses, blackMasses, CV_GRAY2BGR);
    debugImgVector.push_back(blackMasses);
    cv::cvtColor( blackMasses, blackMasses, CV_BGR2GRAY);
#endif
    cv::erode(blackMasses, blackMasses, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3)));

    // get white masses
    cv::cvtColor(image, whiteMasses, CV_BGR2GRAY);
    cv::threshold(whiteMasses, whiteMasses, 210, 255, CV_THRESH_BINARY);
#ifdef DEBUG_PERCEPTION
    cv::cvtColor( whiteMasses, whiteMasses, CV_GRAY2BGR);
    debugImgVector.push_back(blackMasses);
    cv::cvtColor( whiteMasses, whiteMasses, CV_BGR2GRAY);
#endif

    cv::Mat wContours, bContours;
    whiteMasses.copyTo(wContours);
    blackMasses.copyTo(bContours);

    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point> > blackContours, whiteContours;
    cv::Rect blackArea;
    //get masses contours
    cv::findContours(bContours, blackContours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    cv::findContours(wContours, whiteContours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    std::vector<cv::Point2f> center(whiteContours.size());
    std::vector<float> radius(whiteContours.size());
    // fit white masses with expected ball radius
    // TODO: find the relation between ball radius and its respective position on the image
    for (unsigned int i = 0; i < whiteContours.size(); i++)
    {
        cv::minEnclosingCircle(whiteContours[i], center[i], radius[i]);
        if ((radius[i] > MIN_BALL_SIZE && radius[i] < MAX_BALL_SIZE) && (isCircleWhiteFilled(whiteMasses,center[i],radius[i])))
        {
            candidates.push_back(ballCandidate(center[i],radius[i]));
            //cv::circle(image,center[i],radius[i],cv::Scalar(0,255,0));
        }
    }

    for (unsigned int i = 0; i < blackContours.size();i++)
    {
        //create a rectange containg the black mass
        blackArea = cv::boundingRect(blackContours[i]);
        //fit black masses with expected size
        //TODO: find the relation between black area and its respective position on the image
        //cv::rectangle(image,blackArea,cv::Scalar(0,0,255));

        if (blackArea.width * blackArea.height < MAX_BLACK_AREA)
        {
        #ifdef DEBUG_PERCEPTION
            cv::rectangle(image,blackArea,cv::Scalar(0,255,255));
        #endif
            //check if the black mass is near or inside a white mass
            for (unsigned int j = 0; j < candidates.size(); j++)
            {
                if (pow(blackArea.x + blackArea.width/2 - candidates[j].center.x, 2) +
                        pow(blackArea.y + blackArea.height/2 - candidates[j].center.y, 2) <= pow(candidates[j].radius + NEIGHBORHOOD_DISTANCE,2))
                {
                    candidates[j].blackSpots.push_back(blackArea);
                #ifdef DEBUG_PERCEPTION
                    cv::rectangle(image,blackArea,cv::Scalar(0,255,255));
                #endif

                }
            }
        }
    }
    if (candidates.size() < 1)
    {
        cv::Mat grad_x, grad_y, grad, highContrastAreas, img;
        cv::GaussianBlur(rawImg,img,cv::Size(5,5),1);
        cv::cvtColor(img, grad, CV_BGR2GRAY);
        cv::Scharr(grad,grad_x,CV_16S,1,0,3);
        cv::Scharr(grad,grad_y,CV_16S,0,1,3);
        cv::convertScaleAbs(grad_x,grad_x);
        cv::convertScaleAbs(grad_y,grad_y);
        cv::addWeighted(grad_x,0.5,grad_y,0.5,0,grad);
        cv::threshold(grad,highContrastAreas,254,255,CV_THRESH_TOZERO);
        cv::bitwise_and(blackMasses, highContrastAreas, highContrastAreas);
        cv::Moments mu = moments(highContrastAreas, true);
        cv::Point2f mc = cv::Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );
        if (mc.x > 0 && mc.y > 0)
        {
            if (isCircleWhiteFilled(whiteMasses, mc, 30, 0.2))
            {
                candidates.push_back(ballCandidate(mc,30));
                candidates[0].blackSpots.push_back(cv::Rect(mc, cv::Size(11,11)));
                //cv::circle(image, mc, 5, cv::Scalar(0,0,255),5);
            }
        }
    }

    return candidates;
}


void BallDetector::run(cv::Mat botImg, cv::Mat topImg,PerceptionData* data)
{
#ifdef DEBUG_PERCEPTION
    debugImgVector.assign(1, botImg);
#endif
    //this->bestCandidate = getBestBallCandidate(findBallCandidates(data->isTopCamera,
    //                                                            data->isTopCamera?topImg:botImg));

    this->bestCandidate = getBestBallCandidate(findBallCandidates(botImg));

//    if (bestCandidate.center.x == -1)
//    {
//        if (this->getLastSeen() > 10)
//        {
//            foundFeature();
//            data->isTopCamera = !data->isTopCamera;
//        }
//    else
//        featureNotFound();
//    }
//    else
//    {
//        foundFeature();
//        if (data->ballY >= Y_SWITCH_TO_TOPCAMERA && data->isTopCamera)
//            data->isTopCamera = !data->isTopCamera;
//        if (data->ballY <= Y_SWITCH_TO_BOTCAMERA && !data->isTopCamera)
//            data->isTopCamera = !data->isTopCamera;
//    }
    if (data->ballX != -1)
        cv::circle(botImg,cv::Point(data->ballX, data->ballY),10,cv::Scalar(0));

    updateData(data);
#ifdef DEBUG_PERCEPTION
    //Create an image vector, put the desired images inside it and atualize the perception data debugImages with it.
            debugImgVector.push_back(topImg);
            std::pair<std::map<std::string,std::vector<cv::Mat> >::iterator, bool> debugInsertion;
            debugInsertion = data->debugImages.insert(std::make_pair("ballDetector", debugImgVector));
            if(!debugInsertion.second){
                data->debugImages["ballDetector"] = debugImgVector;
            }
#endif
}

void BallDetector::updateData(PerceptionData *data)
{
    data->ballX = this->bestCandidate.center.x;
    data->ballY = this->bestCandidate.center.y;
}
