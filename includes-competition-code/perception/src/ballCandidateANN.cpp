#include "ballCandidateANN.hpp"


ballAnnDetector::ballAnnDetector(char *xml){ //construtor
    annTrained.load(xml);
    //initKalman();
}

void ballAnnDetector::setNotFound(int value){
    this->notFound = value;
}

void ballAnnDetector::drawRect(cv::Rect ROI, int cor){ //debug
    cv::rectangle(debugImage, ROI, cv::Scalar(0,cor,255), 1, 8, 0);
    //cv::imwrite("bola.jpg",debugImage);
}

void ballAnnDetector::initKalman(){

    int stateSize = 4; //state size (variaveis que vc deseja obter: x, y, v_x, v_y)
    int measSize = 2; //measurement size (variaveis que vc mede: x, y)

    KF = cv::KalmanFilter(stateSize,measSize, 0);

    // [ 1 0 dT 0  0 0 ]
    // [ 0 1 0  dT 0 0 ]
    // [ 0 0 1  0  0 0 ]
    // [ 0 0 0  1  0 0 ]
    // [ 0 0 0  0  1 0 ]
    // [ 0 0 0  0  0 1 ]
    KF.transitionMatrix = *(cv::Mat_<float>(4, 4) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1);

    cv::setIdentity(KF.processNoiseCov, cv::Scalar(1e-2)); // -5
    //    KF.processNoiseCov.at<float>(2,2) = 5.0f;
    //    KF.processNoiseCov.at<float>(3,3) = 5.0f;

    cv::setIdentity(KF.measurementMatrix);
    cv::setIdentity(KF.measurementNoiseCov, cv::Scalar(1e-1));
    cv::setIdentity(KF.errorCovPost, cv::Scalar::all(1));

    firstTime = 1;
    notFound = 0;
}

void ballAnnDetector::correctKalman(PerceptionData* data){
    cv::Mat_<float> measurement(2,1);
    //measurement.setTo(Scalar(0));

    measurement(0) = data->ballX;
    measurement(1) = data->ballY;
    // The "correct" phase that is going to use the predicted value and our measurement
    KF.correct(measurement);

}

cv::Point ballAnnDetector::predictKalman(){

    if (!firstTime) {
        cv::Mat prediction = KF.predict();
        return cv::Point(prediction.at<float>(0),prediction.at<float>(1));
        //circle(this->img,predictPt,10,Scalar(0,255,0));
        //imshow("Imagem", this->img);
    }
}

int ballAnnDetector::predictAnn(int i,cv::Mat imgTreino, float* maxActivationValue, int* candidatePos){

    //limiar que determina o valor minimo da funcao de ativacao para dizer que eh bola!!!
    float threshold = 0.50; //aumentar ou diminuir conforme necessidade //0.65

    cv::resize(imgTreino, imgTreino, cv::Size(trainingSize, trainingSize),0,0,cv::INTER_CUBIC);
    cv::cvtColor(imgTreino,imgTreino, CV_BGR2GRAY);

    imgTreino = imgTreino.reshape(0,1);
    imgTreino.convertTo(imgTreino, CV_32FC1);

    cv::Mat responseMat;
    annTrained.predict(imgTreino,responseMat);

    cv::Point maxIdx;
    cv::minMaxLoc(responseMat,0,0,0,&maxIdx);

    if (maxIdx.x == 1){ //achou bola, entao eh necessario verificar o peso da funcao de ativacao
        float actValue = responseMat.at<float>(1);
        if (actValue >= threshold && *maxActivationValue < actValue){
            //cout << responseMat << endl;
            *candidatePos = i;
            *maxActivationValue = actValue;
            return 1;
        }
    }
    return 0; //nao achou bola
}
void ballAnnDetector::getBestBallANN(std::vector<ballCandidateANN> candidates, PerceptionData* data)
{

    int candidatePos;
    int findBall = 0;
    float maxActivationValue = -1;


    for (int i = 0; i < candidates.size(); i++)
    {
        clock_t timer = clock();

        //essa funcao retorna se achou-se bola ou nao
        findBall += predictAnn(i,candidates[i].roi, &maxActivationValue, &candidatePos);

        timer = clock() - timer;
        ////std::cout << "Executou em " << ((float)timer)/CLOCKS_PER_SEC*1000000 << " usegundos" << endl;

    }
    //    cv::Point ballPrediction = predictKalman();
    //    //caso a rede neural nao encontre a bola, a posicao dela eh determinada pela predicao do kalman
    //    data->ballX = ballPrediction.x;
    //    data->ballY = ballPrediction.y;

    if (!findBall){
        data->ballX = -1;
        data->ballY = -1;
        //        notFound++;
        //        if( notFound >= 10 ) {
        //            firstTime = 1; //se perder a bola por muitos frames, nao confiar mais na predicao
        //            data->ballX = -1;
        //            data->ballY = -1;
        //            //atribui a posicao x = -1, para indicar que a bola nao foi encontrada
        //        }
        //        else{
        //            //DEBUG do KALMAN!!!!!!!!!!!!!!!!!!!!!!!!
        //            //cv::circle(debugImage,ballPrediction,10,cv::Scalar(0,255,0));
        //            //cv::imwrite("bola.jpg",debugImage);

        //        }
        return; //nao achou a bola
    }

    notFound = 0;  //encontrou a bola
    data->ballX = candidates[candidatePos].center.x;
    data->ballY = candidates[candidatePos].center.y;

    /************************///KALMAN
    //    if (firstTime){ //atribuicao do estado corrigido sem adicao dos ruidos!
    //        KF.statePost.at<float>(0) = candidates[candidatePos].center.x;
    //        KF.statePost.at<float>(1) = candidates[candidatePos].center.y;
    //        KF.statePost.at<float>(2) = 0;
    //        KF.statePost.at<float>(3) = 0;
    //        firstTime = 0;
    //    }
    //    else
    //        correctKalman(data);
    /************************///KALMAN


    //debug
    //std::cout << "CANDIDATOS" << std::endl;
    drawRect(candidates[candidatePos].rect, 0);
}

u_int8_t ballAnnDetector::isDistanceCompatible(int diameter, int y){

    float diameterAux = 0.2481*y - 17.09;

    if(diameterAux <= diameter + 5 && diameterAux >= diameter - 5){
        return 1;
    }
    else{
        return 0;
    }
}

float distance(int Xi, int Xf, int Yi, int Yf){
    return sqrt(pow(Xf - Xi, 2) + pow(Yf - Yi, 2));
}

int checkRadius(std::vector<cv::Point2f> auxDistCenters, cv::Point2f finalCenter){
    if (auxDistCenters.size() == 1){ //apenas um ponto contendo a regiao preta
        //        float dist = distance(auxDistCenters[0].x, finalCenter.x,auxDistCenters[0].y,finalCenter.y);
        //        if (dist > 30.0){
        //            return dist;
        //        }
        return 30;
    }
    //mais de um ponto contendo a regiao preta
    float biggestDist = distance(auxDistCenters[0].x, finalCenter.x,auxDistCenters[0].y,finalCenter.y);
    //std::cout << "Biggest: "<< biggestDist << std::endl;
    for(int i = 1; i < auxDistCenters.size();i++){
        float dist = distance(auxDistCenters[i].x, finalCenter.x,auxDistCenters[i].y,finalCenter.y);
        //std::cout << "Dist: "<< dist << std::endl;
        if (dist > biggestDist){
            biggestDist = dist;
        }
    }
    //TODO: testar se eh necessario verificar se esse valor eh menor que 30!
    return biggestDist; //o raio eh o centro preto mais longe!

}


std::vector<ballCandidateANN> ballAnnDetector::findBallANN(cv::Mat image, cv::Mat field, bool isTopCamera)
{
    //const int MIN_BALL_SIZE = isTopCamera? 14:30;
    //const int MAX_BALL_SIZE = isTopCamera? 30:130;

    debugImage = image;

    std::vector<ballCandidateANN> candidates;
    cv::Mat grad_x, grad_y, grad, highContrastAreas, img;
    cv::GaussianBlur(image,img,cv::Size(5,5),1);
    cv::cvtColor(img, grad, CV_BGR2GRAY);

    if (cv::countNonZero(field) && isTopCamera) cv::bitwise_and(grad,field,grad); //procura apenas no verde da camera de cima
    //cv::imwrite("bola_xfield.jpg",grad);

    cv::Scharr(grad,grad_x,CV_16S,1,0,3);
    cv::Scharr(grad,grad_y,CV_16S,0,1,3);
    cv::convertScaleAbs(grad_x,grad_x);
    cv::convertScaleAbs(grad_y,grad_y);
    cv::addWeighted(grad_x,0.5,grad_y,0.5,0,grad);
    cv::threshold(grad,highContrastAreas,254,255,CV_THRESH_TOZERO);
    //cv::imwrite("thres.jpg", highContrastAreas);
    highContrastAreas.copyTo(grad);
    cv::dilate(highContrastAreas,highContrastAreas,cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3)));
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point> > contrastContours;
    cv::findContours(highContrastAreas, contrastContours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    std::vector<cv::Point2f> center(contrastContours.size());
    std::vector<float> radius(contrastContours.size());
    for (unsigned int i = 0; i < contrastContours.size(); i++)
    {
        cv::minEnclosingCircle(contrastContours[i], center[i], radius[i]);

        /*********************** Detecta regioes pretas em contrastes muito grandes!*********************/

        cv::RotatedRect rect = cv::minAreaRect(contrastContours[i]);

        // rotated rectangle
        cv::Point2f rect_points[4]; rect.points( rect_points );

        float lx = abs(rect_points[0].x -  rect_points[(1)%4].x);
        float ly = abs(rect_points[1].y -  rect_points[(2)%4].y);
        if (lx*ly > 2500){ //calculo da area!

            cv::Rect rect1 = rect.boundingRect();

            if(rect1.x < 0) rect1.x = 0;
            if(rect1.y < 0) rect1.y = 0;
            if(rect1.x + rect1.width > image.cols){
                rect1.width = image.cols - rect1.x;
            }
            if(rect1.y + rect1.height > image.rows){
                rect1.height = image.rows - rect1.y;
            }
            cv::Mat aux = image.clone(); //caso tenha dois retangulos sobrepostos
            cv::cvtColor(aux, aux, CV_BGR2GRAY);
            cv::Mat newSearch = aux(rect1);

            //cv::imwrite("blackSearch.jpg", newSearch);

            cv::threshold(newSearch,newSearch,50,255,CV_THRESH_BINARY_INV);

            cv::dilate(newSearch,newSearch,cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));


            std::vector<std::vector<cv::Point> > blackContours;
            std::vector<cv::Vec4i> hierarchy1;
            cv::findContours(newSearch, blackContours, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

            std::vector<cv::Point2f> centros, auxDistCentros;
            std::vector<float> raios;
            cv::Point2f centroAux;
            float raioAux;

            for(int ii = 0; ii < blackContours.size();ii++){
                cv::minEnclosingCircle(blackContours[ii], centroAux, raioAux);
                if (raioAux > 1 && raioAux < 50){ //se o raio for mt pequeno ou mt grande, descartar o centro dele
                    centroAux.x = centroAux.x + rect1.x;
                    centroAux.y = centroAux.y + rect1.y;
                    centros.push_back(centroAux);
                    raios.push_back(raioAux);
                    //cv::circle(image,centroAux, raioAux,cv::Scalar(0,0,255));
                }
            }
            //std::cout << "NEW" << std::endl;
            if (centros.size() > 0){
                bool continua = 0;
                while (!continua){
                    cv::Point2f finalCentro;
                    finalCentro = centros[0];
                    centros.erase(centros.begin());
                    auxDistCentros.clear();
                    auxDistCentros.push_back(finalCentro);
                    for (int ii = 0; ii < centros.size();ii++){
                        if(distance(centros[ii].x, finalCentro.x, centros[ii].y, finalCentro.y) < 50){
                            auxDistCentros.push_back(centros[ii]);
                            finalCentro += centros[ii];
                            finalCentro.x /= 2;
                            finalCentro.y /= 2;
                            centros.erase(centros.begin()+ii);
                            ii--;
                        }
                    }
                    int RAIO = checkRadius(auxDistCentros, finalCentro);
                    //estd::cout << RAIO << std::endl;
                    int pX = finalCentro.x - RAIO > 0? finalCentro.x - RAIO : 0,
                            pY = finalCentro.y - RAIO > 0? finalCentro.y - RAIO : 0,
                            width = pX + 2*RAIO > img.cols?  img.cols - pX : 2*RAIO,
                            height = pY + 2*RAIO > img.rows? img.rows - pY : 2*RAIO;
                    //if (field.at<uchar>(pX,pY) == 255){
                    cv::Rect r = cv::Rect(pX, pY, width, height);
                    candidates.push_back(ballCandidateANN(r,image(r)));
                    //cv::rectangle(image, r, cv::Scalar(255,0,0), 1, 8, 0);

                    //cv::circle(image,finalCentro,RAIO,cv::Scalar(0,255,0));
                    if(centros.size() == 0){
                        continua = 1;
                    }
                }
            }
        }


        /*****************************************/

        if (isTopCamera) {
            if (isDistanceCompatible(2*radius[i], center[i].y))
            {
                int pX = center[i].x - 0.7071*radius[i] > 0? center[i].x - 0.7071*radius[i] : 0,
                        pY = center[i].y - 0.7071*radius[i] > 0? center[i].y - 0.7071*radius[i] : 0,
                        width = center[i].x + 0.7071*radius[i] > img.cols?  img.cols - pX : 1.4142*radius[i],
                        height = center[i].y + 0.7071*radius[i] > img.rows? img.rows - pY : 1.4142*radius[i];
                //if(field.at<uchar>(pY,pX) == 255){
                //    //std::cout << "pxpy " << pX << ' '<< pY << std::endl;
                //so vira candidato se tiver dentro do campo!!!!
                cv::Rect r = cv::Rect(pX, pY, width, height);
                candidates.push_back(ballCandidateANN(r,image(r)));
                //cv::rectangle(image, r, cv::Scalar(255,0,0), 1, 8, 0);
            }
        }
        else
        {
            if (radius[i] > 10 && radius[i] < 80)
            {
                int pX = center[i].x - 0.7071*radius[i] > 0? center[i].x - 0.7071*radius[i] : 0,
                        pY = center[i].y - 0.7071*radius[i] > 0? center[i].y - 0.7071*radius[i] : 0,
                        width = center[i].x + 0.7071*radius[i] > img.cols?  img.cols - pX : 1.4142*radius[i],
                        height = center[i].y + 0.7071*radius[i] > img.rows? img.rows - pY : 1.4142*radius[i];

//                int pX = center[i].x - radius[i] > 0? center[i].x - radius[i] : 0,
//                        pY = center[i].y - radius[i] > 0? center[i].y - radius[i] : 0,
//                        width = center[i].x + 2*radius[i] > img.cols?  img.cols - pX : 2*radius[i],
//                        height = center[i].y + 2*radius[i] > img.rows? img.rows - pY : 2*radius[i];
                //if(field.at<uchar>(pY,pX) == 255){
                //    //std::cout << "pxpy " << pX << ' '<< pY << std::endl;
                //so vira candidato se tiver dentro do campo!!!!
                cv::Rect r = cv::Rect(pX, pY, width, height);
                candidates.push_back(ballCandidateANN(r,image(r)));
                //cv::rectangle(image, r, cv::Scalar(255,0,0), 1, 8, 0);
            }
        }
        //cv::rectangle(image, r, cv::Scalar(255,0,0), 1, 8, 0);

    }

    return candidates;
}



