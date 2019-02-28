#include "fieldDetector2.hpp"


void FieldDetector2::run(cv::Mat imgTop, cv::Mat imgBot, PerceptionData *data)
{
    cv::Mat src_HLS;

    //Color transformations
    cv::cvtColor(imgTop, src_HLS, cv::COLOR_BGR2HLS);
    cv::blur(src_HLS, src_HLS, cv::Size(2,2));
    cv::inRange(src_HLS, cv::Scalar(iLowH, iLowL, iLowS), cv::Scalar(iHighH, iHighL, iHighS), src_HLS);


#ifdef DEBUG_PERCEPTION
    //Create an image vector, put the desired images inside it and atualize the perception data debugImages with it.
    debugImgVector.assign(1, imgTop); //0
    debugImgVector.push_back(imgBot); // 1
    cv::cvtColor( src_HLS, src_HLS, CV_GRAY2BGR);
    debugImgVector.push_back(src_HLS); //2
    cv::cvtColor( src_HLS, src_HLS, CV_BGR2GRAY);
#endif

    int whiteCount = cv::countNonZero(src_HLS);
    float fieldpercentage = (float)whiteCount/(src_HLS.rows*src_HLS.cols);

    int y = fieldpercentage*src_HLS.rows*factor;

    cv::Mat lineOutput;
    cv::cvtColor(src_HLS, lineOutput, CV_GRAY2BGR);

    horizont = lineOutput.rows-y;
    horizont = horizont;

    std::cout<<horizont<<std::endl;

    if(horizont<lineOutput.rows && horizont>0){
        cv::line(lineOutput, cv::Point(0,horizont), cv::Point(lineOutput.cols-1, horizont), cv::Scalar(0,0,255), 2,8,0);
    }

    cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(3,3), cv::Point(-1,-1));
    cv::Mat erodeOutput,dilateOutput;

    if(kernel>0){
        element = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(kernel,kernel), cv::Point(-1,-1));
    }
    cv::erode(src_HLS, erodeOutput,element, cv::Point(-1,-1),1, cv::BORDER_CONSTANT);
    cv::dilate(erodeOutput, dilateOutput, element, cv::Point(-1,-1),1, cv::BORDER_CONSTANT);

    cv::Mat ballOutput = imgTop.clone();

    std::vector<cv::Vec3f> circles;

    cv::HoughCircles(dilateOutput, circles, CV_HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius);

    for( int i = 0; i < circles.size(); i++ )
    {
       cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
       int radius = cvRound(circles[i][2]);
       cv::circle( ballOutput, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
       // circle outline
       circle( ballOutput, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
     }



#ifdef DEBUG_PERCEPTION
    debugImgVector.push_back(lineOutput);

    cv::cvtColor(erodeOutput, erodeOutput, CV_GRAY2BGR);
    debugImgVector.push_back(erodeOutput); //2
    cv::cvtColor(erodeOutput,erodeOutput, CV_BGR2GRAY);

    cv::cvtColor( dilateOutput, dilateOutput, CV_GRAY2BGR);
    debugImgVector.push_back(dilateOutput); //2
    cv::cvtColor(dilateOutput, dilateOutput, CV_BGR2GRAY);

    debugImgVector.push_back(ballOutput);
#endif





#ifdef DEBUG_PERCEPTION
    // atualize the perception data debugImages with debugImgVector.
    std::pair<std::map<std::string,std::vector<cv::Mat> >::iterator, bool> debugInsertion;
    debugInsertion = data->debugImages.insert(std::make_pair("fieldDetector2", debugImgVector));
    if(!debugInsertion.second){
        data->debugImages["fieldDetector2"] = debugImgVector;
    }
#endif

    updateData(data);
}

void FieldDetector2::updateData(PerceptionData *data)
{
    data->fieldDistance = this->horizont;

}

