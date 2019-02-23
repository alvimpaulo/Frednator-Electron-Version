#include "goalDetector.hpp"


//void GoalDetector::run(cv::Mat topImg, cv::Mat goalImg, PerceptionData *data)
void GoalDetector::run(cv::Mat topImg, cv::Mat goalImg, PerceptionData *data)
{
    #ifdef DEBUG_PERCEPTION
        //Create an image vector, put the desired images inside it and atualize the perception data debugImages with it.
        debugImgVector.assign(1, topImg);
    #endif
    // Goal detection on roi_field
    cv::Mat goal = topImg.clone();
    cv::Mat src_gray,src;

    cv::Mat campo = cv::Mat::zeros(goal.rows,goal.cols, CV_8UC1);
    cv::cvtColor( goal, src_gray, CV_BGR2GRAY );
    #ifdef DEBUG_PERCEPTION
        cv::cvtColor( src_gray, src_gray, CV_GRAY2BGR);
        debugImgVector.push_back(src_gray); //nao mostra
        cv::cvtColor( src_gray, src_gray, CV_BGR2GRAY);
    #endif
    //Field roi mask creation
    for (int w = 0; w < campo.rows; w++) {
        for (int q = 0; q < campo.cols ; q ++) {
            if(w < this->greenVerticalAvg /*&& w > (140 - 0.5*greenVerticalAvg)*/ ){
                campo.at<uchar>(w,q) = 255;
            }
        }}
    //Get goal ROI
    cv::bitwise_and(src_gray,campo,src);
    cv::GaussianBlur( src, src, cv::Size( 3,3 ), 0, 0 );
    #ifdef DEBUG_PERCEPTION
        cv::cvtColor( src, src, CV_GRAY2BGR);
        debugImgVector.push_back(src); //teve que converter
        cv::cvtColor( src, src, CV_BGR2GRAY);
    #endif

    int ddepth = CV_16S;

    cv::Mat grad;
    //Get contour
    cv::Sobel( src, grad, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );

    cv::convertScaleAbs( grad, grad);

    //cv::addWeighted( abs_grad_x, 1, cv::Mat::zeros(abs_grad_x.rows,abs_grad_x.cols,CV_8UC1)  , 0.5, 0, grad );
    cv::threshold( grad, grad, grad_threshold, 255,0 );
    //Noise canceling
    cv::dilate(grad,grad,cv::getStructuringElement(cv::MORPH_RECT,cv::Size(1,3)),cv::Point(-1,-1)); //3
    cv::erode(grad,grad,cv::getStructuringElement(cv::MORPH_RECT,cv::Size(1,9)),cv::Point(-1,-1)); //7
    #ifdef DEBUG_PERCEPTION
        cv::cvtColor( grad, grad, CV_GRAY2BGR);
        debugImgVector.push_back(grad); //teve que converter
        cv::cvtColor( grad, grad, CV_BGR2GRAY);
    #endif
    //Get line in image
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(grad,lines,1,CV_PI/180,10,10);

    float gol_x = 0.0, nLines = 0.0;

    for(size_t i = 0; i < lines.size(); i++){
    // If lines close to field its likely a goal
        cv::Vec4i aLine = lines[i];
        if(aLine[1] > this->greenVerticalAvg - 50 /*&& aLine[3] < (160 - 0.5*media)*/){
            gol_x = gol_x + (float)((float)aLine[0]/(fabs((float)aLine[1] - (float)this->greenVerticalAvg)));
            nLines = nLines + (float)(1/(fabs((float)aLine[1] - (float)this->greenVerticalAvg)));
    #ifdef DEBUG_PERCEPTION
            //std::cout << "linha_x:" << std::dec << aLine[0] << std::endl;
            cv::line(goal,cv::Point(aLine[0],aLine[1]),cv::Point(aLine[2],aLine[3]),cv::Scalar(0,0,255),3,CV_AA);
    #endif
        }
    }
    std::cout << "Perception: nlines " << nLines << std::endl;
    if(nLines){
        gol_x = gol_x/nLines;
    }
    else{
        gol_x = -1;
    }

    this->bestGoal.imgPosX = gol_x;

    updateData(data);

    #ifdef DEBUG_PERCEPTION
        cv::circle(goal,cv::Point(gol_x,this->greenVerticalAvg),4,cv::Scalar(255,0,0),2);
        cv::imwrite("gol.jpg",goal);
        cv::imwrite("gol_gradiente.jpg",grad);
        debugImgVector.push_back(goal);

        cv::cvtColor( grad, grad, CV_GRAY2BGR);
        debugImgVector.push_back(grad); //teve que converter
        cv::cvtColor( grad, grad, CV_BGR2GRAY);

        //atualize the perception data debugImages with debugImgVector
        std::pair<std::map<std::string,std::vector<cv::Mat> >::iterator, bool> debugInsertion;
        debugInsertion = data->debugImages.insert(std::make_pair("goalDetector", debugImgVector));
        if(!debugInsertion.second){
            data->debugImages["goalDetector"] = debugImgVector;
        }

        //std::cout << "gol_x = " << std::dec << gol_x << " " << this->greenVerticalAvg << std::endl;
    #endif
}

void GoalDetector::updateData(PerceptionData *data)
{
    data->goalCenter = this->bestGoal.imgPosX;
}
