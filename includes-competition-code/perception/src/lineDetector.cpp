#include "../include/lineDetector.hpp"

void LineDetector::run(cv::Mat topImg, cv::Mat greenFrame, PerceptionData *data)
{
#ifdef DEBUG_PERCEPTION
    debugImgVector.assign(1, greenFrame);
#endif


    // Mat for the edges in the green region
    cv::Mat edges, aux_edges/*,greenframe*/;

    // Find green region using hsv thresholds
    /*cv::cvtColor(topImg, greenFrame, CV_BGR2HSV);
    cv::blur(greenFrame, greenFrame, cv::Size(2,2));
	cv::inRange(greenFrame, cv::Scalar(35, 80, 0), cv::Scalar(85, 210, 210), greenFrame);
    greenFrame = 255 - greenFrame;*/

    cv::Canny(greenFrame, edges, 50, 150);  // Find the edges in the green region

    cv::HoughLines(edges, this->lines, 2, CV_PI/180, houghLinesThreshold, 0, 0);  // Find the lines on the edges

    cv::cvtColor(edges, edges, CV_GRAY2BGR);

    int flag = 0;

    // Filter the lines. Only parallel and perpendicular ones will be kept
    for( int i = 0; i < this->lines.size(); i++ )  // For each line
	{

        float rho = this->lines[i].x, theta = this->lines[i].y, a = cos(theta), b = sin(theta);
        float x0 = a*rho, y0 = b*rho;  // Find a point in the line
        if (i)  // All the lines except the first
		{

            for ( int j = 0; j < i; j++ )  // Compare line i with all the j other previous lines
            {
                // If the difference between the angles of the lines is between the thresholds, exclude the line
                if(fabs(this->lines[j].y - this->lines[i].y) < line_threshold_theta_high && fabs(this->lines[j].y - this->lines[i].y) > line_threshold_theta_low)
                {
                    flag = 1;
                    break;
                }           
                // If the difference between the angles of the lines is below the lower threshold (parallels) and if the difference between their rhos is too small, they might be the same line, so exclude the line
                if (fabs(this->lines[j].y - this->lines[i].y) <= line_threshold_theta_low && fabs(this->lines[j].x - this->lines[i].x) <= line_threshold_rho)
                {
                    flag = 1;
                    break;
                }
            }
		}

        if (!flag)  // Store the choosen lines, the ones that weren't excluded
		{
#ifdef DEBUG_PERCEPTION
           cv::line( edges, cv::Point(x0 + 1000*(-b), y0 + 1000*(a)), cv::Point(x0 - 1000*(-b), y0 - 1000*(a)), cv::Scalar(0,0,255), 2, CV_AA);
#endif
            this->filteredLines.push_back(this->lines[i]);
		}
		flag = 0;
	}
    for (int i=0; i < this->filteredLines.size(); i++ )  // For each of the choosen lines
	{
        for (int j=i+1; j <= this->filteredLines.size(); j++) // For every other line
		{
            // Find the linear and angular coefficients of each line
            float m1, n1, m2, n2, x_int, y_int;
            m1 = tan(this->filteredLines[j].y);
            n1 = this->filteredLines[j].x/cos(this->filteredLines[j].y);
            m2 = tan(this->filteredLines[i].y);
            n2 = this->filteredLines[i].x/cos(this->filteredLines[i].y);

            // Find the point of intersection of the two compared lines (corner)
            // TODO: only find the points between lines which their angle difference is above the lower angle threshold
            if (1/*fabs(lines[j].y-lines[i].y) > line_threshold_theta_low*/)
            {
                x_int = (m1*n2-n1*m2)/(m1-m2);
                y_int = (n1-n2)/(m1-m2);
                if(x_int>=0 && x_int<=360 && y_int>=0 && y_int<=240) // Only the corners inside the image will be stored
                {
                    this->corners.push_back(cv::Point(int(x_int),int(y_int)));
#ifdef DEBUG_PERCEPTION
                    cv::circle(edges,this->corners.back(),3,cv::Scalar(255,0,0),-1);
#endif
                }
            }
		}
	}

	updateData(data);

    #ifdef DEBUG_PERCEPTION
    //Create an image vector, put the desired images inside it and atualize the perception data debugImages with it.
        debugImgVector.push_back(edges);
        std::pair<std::map<std::string,std::vector<cv::Mat> >::iterator, bool> debugInsertion;
        debugInsertion = data->debugImages.insert(std::make_pair("lineDetector", debugImgVector));
        if(!debugInsertion.second){
            data->debugImages["lineDetector"] = debugImgVector;
        }

        cv::imwrite("green_lines.jpg", greenFrame);
        cv::imwrite("detected lines.jpg", edges);
    #endif
    }

void LineDetector::updateData(PerceptionData *data)
{
    data->lines.clear();
    data->lines = this->filteredLines;
    data->corners.clear();
    data->corners = this->corners;
}
