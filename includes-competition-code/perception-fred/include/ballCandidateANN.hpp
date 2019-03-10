#include <iostream>
#include <unBoard.hpp>
#include <opencv2/opencv.hpp>

#define trainingSize 15

//tirar debugs - //qiLogInfo e cout
//colocar sonar em approach e outros estados q anda. --X--
//diminuir velocidade --x--
//melhorar metodo do bruno
//chutar penalti
//melhorar rede para detectar com branco em volta

typedef struct ballCandidateANN {
    cv::Rect rect;
    cv::Mat roi;
    cv::Point center;
    ballCandidateANN(cv::Rect rect, cv::Mat image) {
        this->rect = rect;
        //this->roi = image.clone();
        cv::resize(image, this->roi, cv::Size(trainingSize, trainingSize),0,0,cv::INTER_CUBIC);;
        this->center = cv::Point(rect.x + rect.width/2, rect.y + rect.height/2);
    }
    ballCandidateANN(){}
} ballCandidateANN;

class ballAnnDetector{

public:

    ballAnnDetector(char *xml);
    void getBestBallANN(std::vector<ballCandidateANN> candidates,PerceptionData* data);
    std::vector<ballCandidateANN> findBallANN(cv::Mat image, cv::Mat field, bool isTopCamera);
    void setNotFound(int);


private:

    cv::Mat debugImage;

    CvANN_MLP annTrained;
    cv::KalmanFilter KF;
    bool firstTime;
    int notFound;

    int predictAnn(int i, cv::Mat imgTreino, float * maxActivationValue, int * candidatePos);
    void correctKalman(PerceptionData* data);
    cv::Point predictKalman();
    void initKalman();
    void drawRect(cv::Rect, int);
    u_int8_t isDistanceCompatible(int diameter, int y);

};

