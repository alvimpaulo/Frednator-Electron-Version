#include <perception.hpp>
#include <cstdio>
#include <vector>
#include <cmath>
//#define VIDEO_SOURCE
#define BUFFER_LENGTH 15
#define DISTANCE_BUFFER_LENGTH 15

//#define DEBUG_PERCEPTION 1

perception::perception(void* _ub)
{

    cameraProxy = new AL::ALVideoDeviceProxy("127.0.0.1");
    AL::ALValue cameras, camerasColor, camerasRes;
    cameras.arraySetSize(2);
    camerasColor.arraySetSize(2);
    camerasRes.arraySetSize(2);

    cameras[0] = AL::kTopCamera;
    cameras[1] = AL::kBottomCamera;

    camerasColor[0] = AL::kBGRColorSpace;
    camerasColor[1] = AL::kBGRColorSpace;

    camerasRes[0] = AL::kQVGA;
    camerasRes[1] = AL::kQVGA;
    // Subscribe a Vision Module to ALVideoDevice, starting the
    // frame grabber if it was not started before.
    int fps = 10;
    // The subscriberID can be altered if other instances are already running
    subscriberID = cameraProxy->subscribeCameras("camera", cameras, camerasRes, camerasColor, fps);

    this->unBoard = static_cast<UnBoard*>(_ub);

    ballAnn =  new ballAnnDetector(annNameXml);

    for (int i=0; i<BUFFER_LENGTH; i++) buffer.push_back(0);
    for (int i=0; i<DISTANCE_BUFFER_LENGTH; i++) distanceBuffer.push_back(0);
    visionData.approxDistance=0;

    oldCamera = 0; //limpa o buffer na primeira vez que roda perception

#ifdef VIDEO_SOURCE
    cap = cv::VideoCapture("larc_day11_bot_kick_01.avi");
#endif

}

perception::~perception()
{
    // Unsubscribe the V.M.
    cameraProxy->unsubscribe(subscriberID);
}


void perception::tick()
{
    visionData.ballX = -1;
    visionData.ballY = -1;
    visionData.isTopCamera = readFrom(perception, isTopCamera);
    qiLogInfo("Perception") << visionData.isTopCamera;

    if(oldCamera != visionData.isTopCamera){
        buffer.clear();
        for (int i=0; i<BUFFER_LENGTH; i++) buffer.push_back(0);
        oldCamera = visionData.isTopCamera;
    }

#ifndef VIDEO_SOURCE
    retrieveImage();
#else
    //std::cout << "video" << std::endl;
    if (!cap.read(raw_BotImg))
    {
        //std::cout << "ACABOU!!!!" << std::endl;
        throw 1;
    }
    else
    {
        cv::resize(raw_BotImg,raw_BotImg,cv::Size(320,240));
        //std::cout << "Lendo!!!" << std::endl;
        getchar();
    }

#endif


    fieldDetector.run(raw_TopImg, raw_BotImg, &visionData);
    int fieldDistance = readFrom(perception,fieldDistance);

    yellowDetector.run(raw_TopImg,raw_BotImg,&visionData);



    //bool searchGoal = readFrom(behavior, searchGoal);
    //if(searchGoal){
    //    goalDetector.setGreenVerticalAvg(fieldDistance);
    //    goalDetector.run(raw_TopImg, fieldDetector.getRoiField().fieldImg, &visionData);
    //}


    //    distanceBuffer.push_back( yellowDetector.getDistance() );
    //    distanceBuffer.pop_front();
    //    //visionData.approxDistance += ( yellowDetector.getDistance() - distanceBuffer.front() ) / DISTANCE_BUFFER_LENGTH;
    //    visionData.approxDistance = 0;
    //    for (std::list<int>::iterator it=distanceBuffer.begin(); it!=distanceBuffer.end(); ++it)
    //        visionData.approxDistance += (*it);
    //    visionData.approxDistance = visionData.approxDistance / DISTANCE_BUFFER_LENGTH;
#ifdef DEBUG_PERCEPTION
    //std::cout << "teste "<<"getDistance: " << yellowDetector.getDistance() << std::endl;
    //std::cout << "teste "<<"approxDistance: " << visionData.approxDistance << std::endl;
#endif
    //ellipseDetector.run(raw_TopImg,fieldDetector.getRoiField().fieldImg,&visionData);
    //    //lineDetector.run(raw_TopImg, fieldDetector.getGreenRegion(),&visionData);



    bool isKick = readFrom(behavior, isKick);


    if (!isKick) {
        if (visionData.isTopCamera){
            ////std::cout << "Bola Cima" << std::endl;
            ////qiLogInfo("Perception") << "Cima";
            ballAnn->getBestBallANN(ballAnn->findBallANN(raw_TopImg, fieldDetector.getRoiField().fieldImg, visionData.isTopCamera),&visionData);

        }
        else{
            ////std::cout << "Bola Baixo" << std::endl;
            ////qiLogInfo("Perception") << "Baixo";
            //ballAnn->getBestBallANN(ballAnn->findBallANN(raw_BotImg, fieldDetector.getRoiField().fieldImg, visionData.isTopCamera),&visionData);
            //ballAnn->setNotFound(10); //sair do kalman!
            ballDetector.run(raw_BotImg, raw_BotImg, &visionData);
        }

    }
    else
    {
        ballDetector.run(raw_BotImg, raw_BotImg, &visionData);
    }

    if (visionData.ballX == -1){
        buffer.push_back(0);
        buffer.pop_front();
    }
    else {
        buffer.push_back(1);
        buffer.pop_front();

    }


    ////std::cout << "Perception: " << buffer << std::endl;

    //for (std::list<bool>::iterator it=buffer.begin(); it!=buffer.end(); ++it)
    //std::cout << "Perception: "<< *it;

    //cv::imwrite("cima.jpg",raw_TopImg);
    //cv::imwrite("baixo.jpg",raw_BotImg);

    //visionData.isTopCamera = 1;

    //so permite que troque a camera se ela for achada. A troca de camera por timeout ocorre no behavior
    //if (visionData.ballX >= 0) switchCamera(framesToSkip); //lostCounter eh incrementado nessa funcao
    updateUnboard();

#ifdef DEBUG_PERCEPTION
    cv::imwrite("cima.jpg",raw_TopImg);
    cv::imwrite("baixo.jpg",raw_BotImg);
#endif

}



void perception::retrieveImage()
{
    // Retrieve the current image.
    AL::ALValue results;
    results = cameraProxy->getImagesRemote(subscriberID);
    cv::Mat inputTop, inputBot;
    inputTop = cv::Mat(cv::Size(320,240),CV_8UC3);
    inputTop.data = (uchar*) results[0][6].GetBinary();
    inputBot = cv::Mat(cv::Size(320,240),CV_8UC3);
    inputBot.data = (uchar*) results[1][6].GetBinary();
    inputBot.copyTo(raw_BotImg);
    inputTop.copyTo(raw_TopImg);
    cameraProxy->releaseImages(subscriberID);
}

void perception::updateUnboard()
{
    //writeTo(perception, fieldDistance, visionData.fieldDistance);

    // writeTo(perception, lostCounter, visionData.lostCounter);


    std::list<bool>::iterator it1 = buffer.end();
    it1--;
    if(*it1 == 1){ //se o ultimo valor inserido for bola, entao escreve unboard
        writeTo(perception, ballX, visionData.ballX);
        writeTo(perception, ballY, visionData.ballY);
        ////qiLogInfo("Perception") << "x: " << visionData.ballX << "y: " << visionData.ballY << std::endl;
    }
    else{
        for (std::list<bool>::iterator it=buffer.begin(); it!=buffer.end(); ++it){
            if(*it == 1){
                ////qiLogInfo("Perception") << "Sem escrita unboard" << std::endl;
                return;
            }
        }

        //caso ache cinco -1, escreve que perdeu a bola na unboard
        writeTo(perception, ballX, visionData.ballX);
        writeTo(perception, ballY, visionData.ballY);
        ////qiLogInfo("Perception") << "x: " << visionData.ballX <<  "y: " << visionData.ballY << std::endl;
    }
    writeTo(perception, fieldDistance, visionData.fieldDistance);
    //writeTo(perception, isTopCamera, visionData.isTopCamera);
    //writeTo(perception, lines, visionData.lines);
    //writeTo(perception, corners, visionData.corners);
    writeTo(perception, approxDistance, visionData.approxDistance);
}


