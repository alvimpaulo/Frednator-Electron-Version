#pragma once

#include <iostream>
#include <vector>

class movingAverage
{
public:
    movingAverage();
    float mean(float oldAverage, float newData, float lastData, float size);
    float delData(float oldAverage, float newData);
    float expoAverage(float oldAverage, float newData, float lastData, float size);
    float medianFilter( std::vector<float> data);
private:

};
