#include <napi.h>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <chrono>

#include "../../includes-competition-code/perception/include/yellowDetector.hpp"

void vectorFinalizer(Napi::Env env, void *vec)
{
    //std::cout << "deleting vec" << std::endl;
    delete (uchar *)vec; // Probably has leak, deleting only the vector pure data, not the object itself
}

Napi::Value typedArrayFromCvMat(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    cv::Mat *img;
    img = info[0].As<Napi::External<cv::Mat>>().Data();
    //cv::resize((*img), (*img), cv::Size(320, 240)); //only needed if capture cant be set
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

void cvMatFinalizer(Napi::Env env, cv::Mat *mat)
{
    //std::cout << "deleting mat" << std::endl;
    delete mat;
}

Napi::Value imgFromVideo(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    //auto started = std::chrono::high_resolution_clock::now();
    if (info.Length() > 0 && info[0].IsExternal())
    {
        ////std::cout << "0" << std::endl;
        cv::VideoCapture *img2;
        // auto newCapture = std::chrono::high_resolution_clock::now();
        // std::cout << "new capture = " << std::chrono::duration_cast<std::chrono::milliseconds>(newCapture - started).count() << std::endl;
        ////std::cout << "1" << std::endl;
        img2 = info[0].As<Napi::External<cv::VideoCapture>>().Data();
        // auto img2Converted = std::chrono::high_resolution_clock::now();
        // std::cout << "img2Converted = " << std::chrono::duration_cast<std::chrono::milliseconds>(img2Converted - started).count() << std::endl;
        ////std::cout << "2" << std::endl;
        if (img2->isOpened())
        {
            //std::cout << "a";
            cv::Mat *tempImg = new cv::Mat();
            bool grabStatus = img2->grab();
            if (!grabStatus) //If it reached end of the video
            {
                std::cout << "end of the video / couldn't grab" << std::endl;
                return Napi::Boolean::New(env, false);
            }
            // auto imgGrabbed = std::chrono::high_resolution_clock::now();
            // std::cout << "imgGrabbed = " << std::chrono::duration_cast<std::chrono::milliseconds>(imgGrabbed - started).count() << std::endl;
            img2->retrieve(*tempImg);
            // auto imgRetrieved = std::chrono::high_resolution_clock::now();
            // std::cout << "imgRetrieved = " << std::chrono::duration_cast<std::chrono::milliseconds>(imgRetrieved - started).count() << std::endl;
            ////std::cout << "c" << std::endl;
            //cv::imshow("a", *tempImg);
            //cv::waitKey(25);
            //cv::destroyAllWindows();

            return Napi::External<cv::Mat>::New(env, tempImg, cvMatFinalizer);
        }
    }
    return Napi::Boolean::New(env, false);
}

Napi::Value videoOpener(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    ////std::cout << "-1" << std::endl;

    cv::VideoCapture *cap;

    //open video from a file
    if (info.Length() == 1 && info[0].IsString())
    {
        std::cout << info[0].ToString().Utf8Value() << std::endl;
        cap = new cv::VideoCapture(info[0].ToString().Utf8Value(), cv::CAP_GSTREAMER); //may crash in windows
    }
    else if (info.Length() == 1 && info[0].IsNumber())
        cap = new cv::VideoCapture(info[0].ToNumber().Int64Value(), cv::CAP_V4L2); //may crash in windows

    //std::cout << "D" << std::endl;
    //(*cap).set(CV_CAP_PROP_FRAME_WIDTH, 1280); //not working
    //(*cap).set(CV_CAP_PROP_FRAME_HEIGHT, 720); //not working

    if (!cap->isOpened())
    {
        //std::cout << "cap not opened" << std::endl;
        return Napi::Boolean::New(env, false);
    }
    //std::cout << "cap opened" << std::endl;
    Napi::External<cv::VideoCapture> externalData = Napi::External<cv::VideoCapture>::New(env, cap);
    return externalData;
}

Napi::Value videoCloser(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    cv::VideoCapture *cap;

    //if used as intended
    if (info.Length() == 1 && info[0].IsExternal())
    {
        cap = info[0].As<Napi::External<cv::VideoCapture>>().Data();
    }
    else
    {
        return Napi::Boolean::New(env, false);
    }

    cap->release();

    if (cap->isOpened()) //couldn't close capture
    {
        //std::cout << "cap not closed" << std::endl;
        return Napi::Boolean::New(env, false);
    }
    //std::cout << "cap closed" << std::endl;

    return Napi::Boolean::New(env, true);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "videoOpener"), Napi::Function::New(env, videoOpener));
    exports.Set(Napi::String::New(env, "imgFromVideo"), Napi::Function::New(env, imgFromVideo));
    exports.Set(Napi::String::New(env, "videoCloser"), Napi::Function::New(env, videoCloser));
    exports.Set(Napi::String::New(env, "typedArrayFromCvMat"), Napi::Function::New(env, typedArrayFromCvMat));

    return exports;
}

NODE_API_MODULE(videoUtil, Init)
