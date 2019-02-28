#pragma once

#include <types/ActionCommand.hpp>

class BehaviorRequest {
public:
    //Actions we will send to motion
    ActionCommand::All actions;

    //Where walking to (for now, relative positions. Should be global after we have Localisation)
    int walkingToX; //mm
    int walkingToY; //mm

    //Where shooting to (for now, relative positions. Should be global after we have Localisation)
    int shootingToX; //mm
    int shootingToY; //mm

    //Ball position (relative)
    int ballX; //mm
    int ballY; //mm

    BehaviorRequest() {
        walkingToX = 0;
        walkingToY = 0;
        shootingToX = 0;
        shootingToY = 0;
        ballX = -1;
        ballY = -1;
    }
};
