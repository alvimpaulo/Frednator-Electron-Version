#include <includeMapBehavior.hpp>
#include <qi/log.hpp>

#define ballFrameMargin 20
#define cameraWidth 320

FollowBallGoalie* FollowBallGoalie::instance = NULL;

FollowBallGoalie::FollowBallGoalie()
{
    camera = true;
    ballX = -1;
    ballY = -1;
    directionX = 0;
}

BehaviorBase* FollowBallGoalie::Behavior()
{
    if(! instance)
    {
        instance = new FollowBallGoalie();
    }
    return instance;
}

void FollowBallGoalie::action(void* _ub)
{
	UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);

    //Do something
    qiLogInfo("Behavior") << "Fuciona!" << std::endl;

    //camera = readFrom(perception, isBottomCamera);

    //writeTo(behavior, walkTransition, true);
    //writeTo(behavior, turnFinished, false); //setting this for lookAround

    //ballX = readFrom(perception, ballX);
    //ballY = readFrom(perception, ballY);
    lines = readFrom(perception, lines);
    linesBuffer = readFrom(behavior, linesBuffer);

    std::cout << "lines: " << lines << std::endl;

    for(int i = 0 ; i < lines.size() ; i++){
        if(i == 20){
            break;
        }
        if(fabs(lines[i].y - CV_PI/2) < CV_PI/10){
            for(int j = 0; j < 19 ; j++){
                linesBuffer[j+1] = linesBuffer[j];
            }
            linesBuffer[0] = lines[i];
        }
    }

    bufferAvg.x = 0;
    bufferAvg.y = 0;
    for(int i = 0 ; i < 20; i++){
        bufferAvg.x += linesBuffer[i].x;
        bufferAvg.y += linesBuffer[i].y;
    }
    bufferAvg.x = bufferAvg.x/20;
    bufferAvg.y = bufferAvg.y/20;



    ActionCommand::All action;
    action.body.actionType = ActionCommand::Body::WALK;

    //Bola à esquerda
    if(ballX >= 0 && ballX <= ((cameraWidth-ballFrameMargin)/2)){


        action.leds.rightEye.red = 0;
        action.leds.rightEye.blue = 0;
        action.leds.rightEye.green = 0;
        action.leds.leftEye.red = 0;
        action.leds.leftEye.blue = 0;
        action.leds.leftEye.green = 1;

        directionX = 50;
    }

    //Bola à direita
    else if(ballX >= (cameraWidth+ballFrameMargin)/2 && ballX <= cameraWidth){

        action.leds.rightEye.red = 0;
        action.leds.rightEye.blue = 0;
        action.leds.rightEye.green = 1;
        action.leds.leftEye.red = 0;
        action.leds.leftEye.blue = 0;
        action.leds.leftEye.green = 0;

        directionX = -50;
    }

    //Bola sumiu
    else if(ballX == -1){

        action.leds.rightEye.red = 1;
        action.leds.rightEye.blue = 0;
        action.leds.rightEye.green = 0;
        action.leds.leftEye.red = 1;
        action.leds.leftEye.blue = 0;
        action.leds.leftEye.green = 0;
        directionX = 0;
    }

    //Bola no meio
    else {

        action.leds.rightEye.red = 0;
        action.leds.rightEye.blue = 0;
        action.leds.rightEye.green = 1;
        action.leds.leftEye.red = 0;
        action.leds.leftEye.blue = 0;
        action.leds.leftEye.green = 1;

        directionX = 0;
    }


    action.body.forward = 0; // compensa movimento pra trás
    action.body.left = directionX;
    action.body.turn = 0.0;
    action.body.power = 0; // Força do chute. Estava fazendo ele andar para tras e mantem ele se movendo mesmo quando nao tem direçao
    action.body.bend = 10; // não anda com bend 0
    //action.body.isFast = true;
    // diretionX = 100 == 100mm

    writeCommand(action, unBoard);

    /*
    action.head.isRelative = false;
    action.head.yawSpeed = 0.5;
    action.head.yaw = 0.0;*/


}

BehaviorBase* FollowBallGoalie::transition(void* _ub)
{
    UnBoard *unBoard;
    unBoard = static_cast<UnBoard*>(_ub);
    
    return this;
}
