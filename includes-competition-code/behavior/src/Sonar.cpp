#include <Sonar.hpp>
#include <unBoard.hpp>
#include <movingAverage.hpp>


#define THRESHOLD_CLOSE 0.35


SonarClass::SonarClass(std::string naoIP)
{
	memory = new AL::ALMemoryProxy(naoIP);
    sonar = new AL::ALSonarProxy(naoIP);
    sonar->subscribe("myApplication");
    sizeSonarLeft = 0;
    sonarAverageLeft = 0;
    sizeSonarRight = 0;
    sonarAverageRight = 0;
    sizePos = 0;
    sonarAverageOut = 0;
    Pos = 0;
}

SonarClass::~SonarClass()
{
	  sonar->unsubscribe("myApplication");
}

void SonarClass::getSonar(void* _ub)
{

    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

	AL::ALValue ALleft, ALright;
    float oldLeft, oldRight, oldPos;

	ALleft = memory->getData("Device/SubDeviceList/US/Left/Sensor/Value");
    ALright = memory->getData("Device/SubDeviceList/US/Right/Sensor/Value");

    left = (float)ALleft;
    right = (float)ALright;

    /*oldLeft = readFrom(behavior, l);
    oldRight = readFrom(behavior, r);
    oldPos = readFrom(behavior, sonarPos);*/

    movingAverage filterRight, filterLeft, filterPos;


    std::vector<float> data;



    data = readFrom(behavior, leftSonar);
    data.erase(data.begin());
    data.push_back(left);

    sonarAverageLeft = filterLeft.delData(sonarAverageLeft, left);
    data.clear();

    data = readFrom(behavior, rightSonar);
    data.erase(data.begin());
    data.push_back(right);
    sonarAverageRight = filterRight.delData(sonarAverageRight, right);
    data.clear();


    ////std::cout << "SONAR: " << left << " " << right << std::endl;
    ////std::cout << "SONAR FILTERED MEAN: " << sonarAverageLeft << " " << sonarAverageRight << std::endl;



    writeFunction(unBoard);
    
    if(right < THRESHOLD_CLOSE && left < THRESHOLD_CLOSE)
    {
        //std::cout << "Perto centro" << std::endl;
        //writeTo(behavior, sonarPos, int(SonarData::MIDDLE));
         Pos = SonarData::MIDDLE;
        //return;
    }
    else if(left < THRESHOLD_CLOSE)
    {
        //std::cout << "Perto a esquerda" << std::endl;
        Pos = SonarData::LEFT;
        //writeTo(behavior, sonarPos, int(SonarData::LEFT));
        //return;
    }
    else if(right < THRESHOLD_CLOSE)
    {
        //std::cout << "Perto a direita" << std::endl;
        //writeTo(behavior, sonarPos, int(SonarData::RIGHT));
         Pos = SonarData::RIGHT;
        //return;
    }
    else
        Pos = SonarData::NONE;


    /*if(sizePos > 10)
    {
        data = readFrom(behavior, sonarPos);
        sonarAverageOut = filterPos.medianFilter(data);
    }*/


    /*writeTo(behavior, l, left);
    writeTo(behavior, r, right);*/

    writeTo(behavior, sonarPos, int(Pos));



}

void SonarClass::writeFunction(void *_ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);


        std::vector<float> tmp;

        tmp = readFrom(behavior, leftSonar);
        tmp.erase(tmp.begin());
        tmp.push_back(sonarAverageLeft);
        writeTo(behavior, leftSonar, tmp);



        tmp = readFrom(behavior, rightSonar);
        tmp.erase(tmp.begin());
        tmp.push_back(sonarAverageRight);
        writeTo(behavior, rightSonar, tmp);


       /* std::vector<int> tmpPos;
        tmpPos = readFrom(behavior, sonarPos);
        tmpPos.erase(tmpPos.begin());
        tmpPos.push_back(sonarAverageOut);
        writeTo(behavior, sonarPos, tmpPos);*/

}
