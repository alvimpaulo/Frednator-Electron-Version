#pragma once


#include <alproxies/almemoryproxy.h>
#include <alproxies/alsonarproxy.h>
#include <string>

namespace SonarData
{
	enum{
		NONE = 0,
		LEFT,
		MIDDLE,
		RIGHT
	};
}

class SonarClass
{

private: 
	AL::ALMemoryProxy *memory;
    AL::ALSonarProxy *sonar;
    int sizeSonarLeft, sizeSonarRight, sizePos, Pos;
    float sonarAverageLeft, sonarAverageRight;
    int sonarAverageOut;
    float left, right;

public:
	SonarClass(std::string naoIP);
    void writeFunction(void*);
	~SonarClass();
    void getSonar(void*);


};
