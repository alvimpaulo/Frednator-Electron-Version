#include <napi.h>

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

Napi::Value videoOpener(const Napi::CallbackInfo &info)
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
    exports.Set(Napi::String::New(env, "videoOpener"), Napi::Function::New(env, videoOpener));

    return exports;
}

NODE_API_MODULE(videoUtil, Init)
