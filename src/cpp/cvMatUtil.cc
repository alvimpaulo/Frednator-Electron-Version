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
  //std::cout << "deleting mat" << std::endl;
  delete mat;
}

void vectorFinalizer(Napi::Env env, void *vec)
{
  //std::cout << "deleting vec" << std::endl;
  delete (uchar *)vec; // Probably has leak, deleting only a vector pure data, not the structure itself
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

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "typedArrayFromCvMat"), Napi::Function::New(env, typedArrayFromCvMat));

  return exports;
}

NODE_API_MODULE(cvmatUtil, Init)