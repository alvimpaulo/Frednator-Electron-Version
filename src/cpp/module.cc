#include <node_api.h>
#include <napi.h>

// Opencv includes.
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>

void cvMatFinalizer(Napi::Env env, cv::Mat *mat)
{
  std::cout << "deleting mat" << std::endl;
  delete mat;
}
void vectorFinalizer(Napi::Env env, void *vec)
{
  std::cout << "deleting vec" << std::endl;
  delete (uchar *)vec;
}

Napi::Value typedArrayFromCvMat(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  cv::Mat *img;
  img = info[0].As<Napi::External<cv::Mat>>().Data();
  cv::resize((*img), (*img), cv::Size(320, 240));
  std::vector<cv::Mat> *rgbchannels = new std::vector<cv::Mat>(3);
  std::vector<u_char> *fourChannelVector = new std::vector<u_char>(img->cols * img->rows * 4);

  if (img->channels() == 3) // RGB
  {
    cv::cvtColor((*img), (*img), CV_BGR2RGB);
    cv::split((*img), (*rgbchannels));
    for (size_t i = 0; i < img->cols * img->rows; ++i)
    {
      (*fourChannelVector)[i * 4] = (*rgbchannels)[0].data[i];
      (*fourChannelVector)[i * 4 + 1] = (*rgbchannels)[1].data[i];
      (*fourChannelVector)[i * 4 + 2] = (*rgbchannels)[2].data[i];
      (*fourChannelVector)[i * 4 + 3] = 255;
    }
    //std::cout << " fourchannelvector data: " << (int)fourChannelVector->at(0) << "\t" << (int)fourChannelVector->at(1) << "\t" << (int)fourChannelVector->at(2) << "\t" << (int)fourChannelVector->at(3) << "\t" << std::endl;
    ////std::cout << rgbchannels[0] << std::endl;
  }

  if (img->channels() == 1) //B&W
  {
    for (size_t i = 0; i < (img->cols * img->rows); i++)
    {
      if (img->channels() == 1)
      {
        u_char grayPixel = img->data[i];
        (*fourChannelVector)[i * 4] = grayPixel;
        (*fourChannelVector)[i * 4 + 1] = grayPixel;
        (*fourChannelVector)[i * 4 + 2] = grayPixel;
        (*fourChannelVector)[i * 4 + 3] = 255;
      }
    }
  }

  /* cv::imshow("a", img);
  cv::waitKey(0);
  cv::destroyAllWindows();
 */
  // std::cout << "img data: " << img->cols << "\t" << img->rows << "\t" << img->channels() << "\t" << img->total() << "\t" << std::endl;

  Napi::ArrayBuffer arrayBuffer = Napi::ArrayBuffer::New(env, fourChannelVector->data(), fourChannelVector->size(), vectorFinalizer);

  // std::cout << " arraybuffer data: " << arrayBuffer.Data() << std::endl;
  //Four channel typed array of uchar
  Napi::TypedArrayOf<u_int8_t> typedArray = Napi::TypedArrayOf<uint8_t>::New(env, img->cols * img->rows * 4, arrayBuffer, 0);

  delete rgbchannels;
  return typedArray;
}

Napi::Value cvMatSenderReciever(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  ////std::cout << "-1" << std::endl;
  if (info.Length() > 0 && info[0].IsExternal())
  {
    ////std::cout << "0" << std::endl;
    cv::VideoCapture *img2;
    ////std::cout << "1" << std::endl;
    img2 = info[0].As<Napi::External<cv::VideoCapture>>().Data();
    ////std::cout << "2" << std::endl;
    if (img2->isOpened())
    {
      //std::cout << "a";
      cv::Mat *tempImg = new cv::Mat();
      (*img2) >> (*tempImg);
      ////std::cout << "c" << std::endl;
      //cv::imshow("a", tempImg);
      //cv::waitKey(0);
      //cv::destroyAllWindows();
      Napi::External<cv::Mat> externalData = Napi::External<cv::Mat>::New(env, tempImg, cvMatFinalizer);
      return externalData;
    }
    return info[0];
  }
  else
  {
    //std::cout << "D" << std::endl;
    cv::VideoCapture *cap;
    //(*cap).set(CV_CAP_PROP_FRAME_WIDTH, 320);
    //(*cap).set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    cap = new cv::VideoCapture(0);

    Napi::External<cv::VideoCapture> externalData = Napi::External<cv::VideoCapture>::New(env, cap);
    return externalData;
  }
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "typedArrayFromCvMat"), Napi::Function::New(env, typedArrayFromCvMat));
  exports.Set(Napi::String::New(env, "cvMatSenderReciever"), Napi::Function::New(env, cvMatSenderReciever));

  return exports;
}

NODE_API_MODULE(addon, Init)