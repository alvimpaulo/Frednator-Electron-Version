#ifndef FIELDDETECTOR_HPP
#define FIELDDETECTOR_HPP

#include <featureDetector.hpp>

typedef struct fieldCandidate
{
    int horizonHeight;
    cv::Mat fieldImg;
    fieldCandidate():horizonHeight(-1){}
} fieldCandidate;


class FieldDetector : public FeatureDetector
{
private:
    fieldCandidate bestField;
    cv::Mat greenRegion;
#ifdef DEBUG_PERCEPTION
    std::vector<cv::Mat> debugImgVector;
#endif
public:
    FieldDetector(){}
    cv::Mat getGreenRegion();
    fieldCandidate getRoiField();
    virtual void run(cv::Mat imgTop, cv::Mat imgBot, PerceptionData *data);
    virtual void updateData(PerceptionData* data);
};

#endif // FIELDDETECTOR_HPP
