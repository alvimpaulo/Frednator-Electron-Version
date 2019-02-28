#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace cv;

typedef struct {
  vector<float> distancia;
  float theta, y_med;
  float melhor_rho;
}candidatos;

int main( int argc, char** argv ) {

  Mat img;
  VideoCapture cap(argv[1]);
  cap.read(img);

  float x_med = img.cols/2;

  //Mat img = imread( argv[1] , CV_LOAD_IMAGE_COLOR );

  Mat normalizada;
  int i, j;


  if (img.type() == CV_8UC3){

    normalizada.create(img.rows, img.cols,CV_8UC1);
    for ( i = 0; i  < img.rows; i++) {
      for ( j = 0; j < img.cols ; j ++) {

        Vec3b *pixel = img.ptr<Vec3b>(i,j);
        uchar blue = pixel->val[0];
        uchar green = pixel->val[1];
        uchar red = pixel->val[2];
        if (red + blue + green != 0 )
          normalizada.at<uchar>(i,j) = 100*green/(red+blue+green);
        else{
          normalizada.at<uchar>(i,j) = 0;
        }
      }
    }
  }

  Mat histograma;
  int histSize = 255; //quantidade de bins
  float range[] = { 0, 256 } ; //the upper boundary is exclusive
  const float* histRange = { range };

  //calcula histograma da imagem normalizada
  calcHist(&normalizada, 1, 0, Mat(), histograma, 1 ,&histSize, &histRange, true, false);
  std::cout << "oi" << std::endl;

  // int max_g = 0, i_max_g = 0, min_g;
  // for (i = 0; i < histograma.rows; i++) {
  //     histograma.at<int>(i,0) > max_g ? max_g = histograma.at<int>(i,0), i_max_g = i : 0;
  // }
  // min_g = (i_max_g * 256/(histSize)) - 8;
  // max_g = min_g + 16;
  // threshold(normalizada,normalizada,max_g,255,CV_THRESH_TOZERO_INV);
  // threshold(normalizada,normalizada,min_g,255,CV_THRESH_TOZERO);

  //faz o calculo para verificar qual a intensidade predominante na imagem (verde)
  float maior = -1;
  int bin;
  for ( i = 0; i < histSize; i++) {
    if (histograma.at<float>(i) > maior){
      maior = histograma.at<float>(i);
      bin = i;
      std::cout << "bin: "<< bin << std::endl;
    }
  }

  Mat binario(normalizada.rows, normalizada.cols, CV_8UC1);

  for ( i = 0; i  < normalizada.rows; i++) {
    for ( j = 0; j < normalizada.cols ; j ++) {
      if ((normalizada.at<uchar>(i,j) < bin + 8) && (normalizada.at<uchar>(i,j) > bin - 8)){
        binario.at<uchar>(i,j) = 255;
      }
      else{
        binario.at<uchar>(i,j) = 0;
      }
    }
  }

//PROCURA POR ARESTAS (CANNY)

Mat arestas;
int lowThreshold = 100;

Mat element1 = getStructuringElement(MORPH_ELLIPSE, Size(10 , 10));
erode(binario, binario, element1);
dilate(binario, binario, element1);

bilateralFilter(binario, arestas, 5, 500, 500);

Canny( arestas, arestas, lowThreshold, lowThreshold*3, 3 );




//PROCURA POR LINHAS


unsigned int flag = 0;
vector<Vec2f> lines;
//struct candidatos
vector<candidatos> best_lines;
int CONST_THETA = 15;
HoughLines(arestas, lines, 1, CV_PI/180, 80, 0, 0 );
for( size_t i = 0; i < lines.size(); i++ )
{
   float rho = lines[i][0], theta = lines[i][1];

   Point pt1, pt2;
   double a = cos(theta), b = sin(theta);
   double x0 = a*rho, y0 = b*rho;
   pt1.x = cvRound(x0 + 1000*(-b));
   pt1.y = cvRound(y0 + 1000*(a));
   pt2.x = cvRound(x0 - 1000*(-b));
   pt2.y = cvRound(y0 - 1000*(a));

   //DEBUG
   //line( img, pt1, pt2, Scalar(0,0,255), 3, CV_AA);

   if (i>0)
   for( size_t j = 0; j < i; j++)
   {
     if(theta >= lines[j][1] - CONST_THETA*(CV_PI/180) && theta <= lines[j][1] + CONST_THETA*(CV_PI/180))
      flag = 1;
      //se essa linha já tiver um theta parecido, ela é adicionada no vetor de candidatos para melhor linha
      //para isso, é necessário fazer um for para saber qual é o theta parecido já existente no vetor de candidatos
      for(size_t z = 0; z < best_lines.size(); z++){
        if (theta >= best_lines[z].theta - CONST_THETA*(CV_PI/180) && theta <= best_lines[z].theta + CONST_THETA*(CV_PI/180)) {
          best_lines[z].distancia.push_back(rho);
          z = best_lines.size();
        }
      }

   }

   if (flag == 0) {
    //line( img, pt1, pt2, Scalar(0,0,0), 3, CV_AA);
    //adiciona novo vetor que conterá theta diferente
    best_lines.push_back(candidatos());
    best_lines.back().distancia.push_back(rho);
    best_lines.back().theta = theta;
   }
   flag = 0;
}
for (size_t h = 0; h < best_lines.size(); h++) {
  if(best_lines[h].theta >= 0){
    best_lines[h].melhor_rho = best_lines[h].distancia[0];
    for (size_t l = 1; l < best_lines[h].distancia.size(); l++) {
      if (best_lines[h].melhor_rho > best_lines[h].distancia[l]){
        best_lines[h].melhor_rho = best_lines[h].distancia[l];
      }
    }
  }
  else{
    best_lines[h].melhor_rho = best_lines[h].distancia[0];
    for (size_t l = 1; l < best_lines[h].distancia.size(); l++) {
      if (best_lines[h].melhor_rho < best_lines[h].distancia[l]){
        best_lines[h].melhor_rho = best_lines[h].distancia[l];
      }
  }
}
  Point pt1, pt2, pt3;
  double a = cos(best_lines[h].theta), b = sin(best_lines[h].theta);
  double x0 = a*(best_lines[h].melhor_rho), y0 = (b*best_lines[h].melhor_rho);
  pt1.x = cvRound(x0 + 1000*(-b));
  pt1.y = cvRound(y0 + 1000*(a));
  pt2.x = cvRound(x0 - 1000*(-b));
  pt2.y = cvRound(y0 - 1000*(a));
  std::cout << "theta: " << best_lines[h].theta << std::endl;
  //line( img, pt1, pt2, Scalar(255,0,0), 3, CV_AA);



  pt3.x = cvRound(x_med);
  pt3.y = cvRound(tan(best_lines[h].theta - CV_PI/2)*(x_med-pt1.x)+pt1.y);
  std::cout << "Distância de saída: " << pt3.x << ", " << pt3.y << std::endl;
  circle(img, pt3, 1, Scalar(0,0,255), 3, 8, 0);
}


//ROI FIELD DETECTION

double a, b, x0, y0;
int y_max;
Mat roi_goal, roi_field(img.rows, img.cols, CV_8UC1);
roi_field = Scalar(0);
roi_goal = roi_field.clone();

  for (size_t i = 0; i < img.cols; i++) {
    a = cos(best_lines[0].theta), b = sin(best_lines[0].theta);
    x0 = a*(best_lines[0].melhor_rho), y0 = (b*best_lines[0].melhor_rho);
    y_max = tan(best_lines[0].theta - CV_PI/2)*(i - x0)+ y0;
    std::cout << "y_max: " << y_max << std::endl;
    for (size_t j = 1; j < best_lines.size() ; j++) {
        a = cos(best_lines[j].theta), b = sin(best_lines[j].theta);
        x0 = a*(best_lines[j].melhor_rho), y0 = (b*best_lines[j].melhor_rho);
        if (y_max <= (tan(best_lines[j].theta - CV_PI/2)*(i - x0)+ y0)) {
          y_max = tan(best_lines[j].theta - CV_PI/2)*(i - x0)+ y0;
        }
    }
    if (y_max <= img.rows) {
      for (int k = y_max >= 0 ? y_max : 0; k < img.rows; k++) {
          roi_field.at<uchar>(k,i) = 255;
          if (k < y_max + 20)
            roi_goal.at<uchar>(k,i) = 255;
      }
    }
  }

  //GOAL DETECTION

  Mat imgGray, whiteMasses, thresholdedMask;
  cvtColor(img, imgGray, CV_BGR2GRAY, 0); //Convert the captured frame from BGR to HSV
  threshold(imgGray, whiteMasses, 170, 255, CV_THRESH_BINARY);
  bitwise_and(whiteMasses, roi_goal, thresholdedMask);
  element1 = getStructuringElement(MORPH_ELLIPSE, Size(7 , 7));
  erode(thresholdedMask, thresholdedMask, element1);
  dilate(thresholdedMask, thresholdedMask, element1);

  // Plot the histogram
  int hist_w = 512; int hist_h = 400;
  int bin_w = round( (double) hist_w/histSize );

  Mat histImage( hist_h, hist_w, CV_8UC1, Scalar( 0,0,0) );
  normalize(histograma, histograma, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

  for( int i = 1; i < histSize; i++ )
  {
    line( histImage, Point( bin_w*(i-1), hist_h - cvRound(histograma.at<float>(i-1)) ) ,
                     Point( bin_w*(i), hist_h - cvRound(histograma.at<float>(i)) ),
                     Scalar( 255, 0, 0), 2, 8, 0  );
  }

  //Mostra imagens
  namedWindow( "img", CV_WINDOW_AUTOSIZE );
  imshow("img",img);

  namedWindow( "normalizada", CV_WINDOW_AUTOSIZE );
  imshow("normalizada",normalizada);

   namedWindow( "binarizada", CV_WINDOW_AUTOSIZE );
   imshow("binarizada",binario);

   namedWindow( "arestas", CV_WINDOW_AUTOSIZE );
   imshow("arestas",arestas);

   imshow("white in field", thresholdedMask);

   waitKey(0);



}
