#include <movingAverage.hpp>

movingAverage::movingAverage()
{

}

float movingAverage::mean(float oldAverage, float newData, float lastData, float size)
{
    float newAverage;
    newAverage = newData;
    if (newData > 1)
    {
        newAverage = oldAverage;
    }
    if(size > 1 && newData < 1)
    {
        newAverage = oldAverage +(newData - lastData)/(50);


    }
    return newAverage;
}

float movingAverage::expoAverage(float oldAverage, float newData, float lastData, float size)
{
    float newAverage;
    newAverage = newData;
    if (newData > 1)
    {
        newAverage = oldAverage;
    }
    if(size > 1 && newData < 1)
    {

        newAverage = (newData - oldAverage)*(2/(size+1))+oldAverage;

    }
    return newAverage;
}

float movingAverage::delData(float oldAverage, float newData)
{
    float newAverage;
    newAverage = newData;
    if (newData > 1)
    {
        newAverage = oldAverage;
    }
    return newAverage;
}

float movingAverage::medianFilter( std::vector<float> data)
{
    float sum = 0;
    float mean;
    for(int i = 0; i < 10; i++)
    {
        sum = sum + data[i];
    }

    mean = sum / 10;
    ////std::cout << "SONAR SUM " << mean << " " << data [9] << " " << data [8] << " " << data [0] << std::endl;

    return mean;
}
