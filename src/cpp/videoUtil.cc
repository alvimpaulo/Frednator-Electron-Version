#include <napi.h>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <chrono>

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
            img2->grab();
            // auto imgGrabbed = std::chrono::high_resolution_clock::now();
            // std::cout << "imgGrabbed = " << std::chrono::duration_cast<std::chrono::milliseconds>(imgGrabbed - started).count() << std::endl;
            img2->retrieve(*tempImg);
            // auto imgRetrieved = std::chrono::high_resolution_clock::now();
            // std::cout << "imgRetrieved = " << std::chrono::duration_cast<std::chrono::milliseconds>(imgRetrieved - started).count() << std::endl;
            ////std::cout << "c" << std::endl;
            //cv::imshow("a", tempImg);
            //cv::waitKey(0);
            //cv::destroyAllWindows();
            Napi::External<cv::Mat>
                externalData = Napi::External<cv::Mat>::New(env, tempImg, cvMatFinalizer);

            return externalData;
        }
        return info[0];
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
        cap = new cv::VideoCapture(info[0].ToString().Utf8Value(), cv::CAP_V4L2); //may crash in windows
    else if (info.Length() == 1 && info[0].IsNumber())
        cap = new cv::VideoCapture(info[0].ToNumber().Int64Value(), cv::CAP_V4L2); //may crash in windows

    //std::cout << "D" << std::endl;
    //(*cap).set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    //(*cap).set(CV_CAP_PROP_FRAME_HEIGHT, 720);

    if (!cap->isOpened())
    {
        Napi::Boolean videoCaptureSucceeded = Napi::Boolean::New(env, false);
        return videoCaptureSucceeded;
    }

    Napi::External<cv::VideoCapture> externalData = Napi::External<cv::VideoCapture>::New(env, cap);
    return externalData;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "videoOpener"), Napi::Function::New(env, videoOpener));
    exports.Set(Napi::String::New(env, "imgFromVideo"), Napi::Function::New(env, imgFromVideo));

    return exports;
}

NODE_API_MODULE(videoUtil, Init)
