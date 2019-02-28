/*
 *
* Base Class to All Behavior
* All Behavior Should Inherite From this class and override this methods!
*/
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>


#include <unBoard.hpp>
#include <Sonar.hpp>
//#include <types/SensorValues.hpp>

namespace GAME_STATE {
    enum {
        INITIAL = 0,
        READY,
        SET,
        PLAYING,
        FINISHED,
        NUM_OF_STATES
    };
}


class BehaviorBase
{

public:

    static BehaviorBase* lastBehavior;
    static BehaviorBase* Behavior();
    static SonarClass *sonarStatic;
    virtual BehaviorBase* transition(void*){ return NULL;};
    virtual void action(void*){};
    void writeCommand(ActionCommand::All action, void* _ub);

protected:
    bool ButtonPressed(void*);
    ActionCommand::rgb getChestColor(void*);
};


enum{ WHITE = 0,
       RED,
       GREEN,
       BLUE
     };

