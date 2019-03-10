#include <napi.h>

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <chrono>

#include "../../includes-competition-code/perception/include/yellowDetector.hpp"

// #define IMAGE_FROM_VIDEO_DEBUG
// #define YELLOW_DETECTOR_DEBUG
// #define VIDEO_OPENER_DEBUG
// #define VIDEO_CLOSER_DEBUG

void vectorFinalizer(Napi::Env env, void *vec)
{
    //std::cout << "deleting vec" << std::endl;
    delete (uchar *)vec; // Probably has leak, deleting only the vector raw data, not the object itself
}

void cvMatFinalizer(Napi::Env env, cv::Mat *mat)
{
    //std::cout << "deleting mat" << std::endl;
    delete mat;
}

Napi::Value yellowDetectorRun(const Napi::CallbackInfo &info)
{

#ifdef YELLOW_DETECTOR_DEBUG
    std::cout << "yd run" << std::endl;
#endif

    Napi::Env env = info.Env();
    if (info.Length() == 1 && info[0].IsExternal())
    { //recieved cv mat
        PerceptionData *data = new PerceptionData();

#ifdef YELLOW_DETECTOR_DEBUG
        std::cout << "yd run received cvmat" << std::endl;
#endif

        YellowDetector detector = YellowDetector();

#ifdef YELLOW_DETECTOR_DEBUG
        std::cout << "yd run created yd" << std::endl;
#endif

        cv::Mat *img = info[0].As<Napi::External<cv::Mat>>().Data();

#ifdef YELLOW_DETECTOR_DEBUG
        std::cout << "yd run converted cvmat" << std::endl;
#endif

        *img = detector.run(*img, *img, data);
#ifdef YELLOW_DETECTOR_DEBUG
        std::cout << "yd run runned" << std::endl;
#endif

        return Napi::External<cv::Mat>::New(env, img);
    }
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

Napi::Value imgFromVideo(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
#ifdef IMAGE_FROM_VIDEO_DEBUG
    auto started = std::chrono::high_resolution_clock::now();
#endif
    if (info.Length() > 0 && info[0].IsExternal())
    {
#ifdef IMAGE_FROM_VIDEO_DEBUG
        std::cout << "ImgFromVideo: 0" << std::endl;
#endif
        cv::VideoCapture *capture;
#ifdef IMAGE_FROM_VIDEO_DEBUG
        auto newCapture = std::chrono::high_resolution_clock::now();
        std::cout << "ImgFromVideo: new capture = " << std::chrono::duration_cast<std::chrono::milliseconds>(newCapture - started).count() << std::endl;
        std::cout << "ImgFromVideo: 1" << std::endl;
#endif
        capture = info[0].As<Napi::External<cv::VideoCapture>>().Data();
#ifdef IMAGE_FROM_VIDEO_DEBUG
        auto captureConverted = std::chrono::high_resolution_clock::now();
        std::cout << "ImgFromVideo: captureConverted = " << std::chrono::duration_cast<std::chrono::milliseconds>(captureConverted - started).count() << std::endl;
        std::cout << "ImgFromVideo: 2" << std::endl;
#endif
        if (capture->isOpened())
        {
#ifdef IMAGE_FROM_VIDEO_DEBUG
            std::cout << "ImgFromVideo: a" << std::endl;
#endif
            cv::Mat *tempImg = new cv::Mat();
            bool grabStatus = capture->grab();
            if (!grabStatus) //If it reached end of the video
            {
                std::cout << "end of the video / couldn't grab" << std::endl;
                return Napi::Boolean::New(env, false);
            }
#ifdef IMAGE_FROM_VIDEO_DEBUG
            auto imgGrabbed = std::chrono::high_resolution_clock::now();
            std::cout << "ImgFromVideo: imgGrabbed = " << std::chrono::duration_cast<std::chrono::milliseconds>(imgGrabbed - started).count() << std::endl;
#endif

            capture->retrieve(*tempImg);
#ifdef IMAGE_FROM_VIDEO_DEBUG
            auto imgRetrieved = std::chrono::high_resolution_clock::now();
            std::cout << "ImgFromVideo: imgRetrieved = " << std::chrono::duration_cast<std::chrono::milliseconds>(imgRetrieved - started).count() << std::endl;
            std::cout << "ImgFromVideo: c" << std::endl;

            std::cout << capture->get(CV_CAP_PROP_FRAME_WIDTH) << "\t" << capture->get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;
            std::cout << tempImg->channels() << "\t" << tempImg->cols << "\t" << tempImg->rows << std::endl;

            cv::imshow("tempimg", *tempImg);
            cv::waitKey(0);
            cv::destroyAllWindows();

            std::cout << "ImgFromVideo: Img shown / end of ImgFromVideo" << std::endl;
#endif

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
#ifdef VIDEO_OPENER_DEBUG
        std::cout << "capture from path " << info[0].ToString().Utf8Value() << std::endl;
#endif
        cap = new cv::VideoCapture(info[0].ToString().Utf8Value(), cv::CAP_GSTREAMER); //may crash in windows
    }
    else if (info.Length() == 1 && info[0].IsNumber())
    {
#ifdef VIDEO_OPENER_DEBUG
        std::cout << "capture from number " << info[0].ToNumber().Int64Value() << std::endl;
#endif
        cap = new cv::VideoCapture(info[0].ToNumber().Int64Value(), cv::CAP_V4L2); //may crash in windows
    }
    cv::Mat tempimg;

#ifdef VIDEO_OPENER_DEBUG
    std::cout << "D" << std::endl;
#endif
    (*cap).set(CV_CAP_PROP_FRAME_WIDTH, 640);  //not working
    (*cap).set(CV_CAP_PROP_FRAME_HEIGHT, 480); //not working

    if (!cap->isOpened())
    {
        //std::cout << "cap not opened" << std::endl;
        return Napi::Boolean::New(env, false);
    }
    //std::cout << "cap opened" << std::endl;
    return Napi::External<cv::VideoCapture>::New(env, cap);
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
#ifdef VIDEO_CLOSER_DEBUG
        std::cout << "cap not closed" << std::endl;
#endif
        return Napi::Boolean::New(env, false);
    }
    std::cout << "cap closed" << std::endl;

    return Napi::Boolean::New(env, true);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "videoOpener"), Napi::Function::New(env, videoOpener));
    exports.Set(Napi::String::New(env, "imgFromVideo"), Napi::Function::New(env, imgFromVideo));
    exports.Set(Napi::String::New(env, "videoCloser"), Napi::Function::New(env, videoCloser));
    exports.Set(Napi::String::New(env, "typedArrayFromCvMat"), Napi::Function::New(env, typedArrayFromCvMat));
    exports.Set(Napi::String::New(env, "yellowDetectorRun"), Napi::Function::New(env, yellowDetectorRun));

    return exports;
}

NODE_API_MODULE(videoUtil, Init)
