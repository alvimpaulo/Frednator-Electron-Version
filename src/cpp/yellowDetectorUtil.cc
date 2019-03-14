#include <napi.h>

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <chrono>

#include "../../includes-competition-code/perception/include/yellowDetector.hpp"

// #define YELLOW_DETECTOR_DEBUG

/**
 * @brief Template function to be used when there is a need to simply delete the class.
 * 
 * @tparam genericClass Class of the parameter to be deleted.
 * @param env Enviroment which the class exists (not used).
 * @param genericObject Pointer to the object to be deleted.
 * @param hint String with information about the object to be deleted.
 */
template <class genericClass>
void genericFinalizer(Napi::Env env, genericClass *genericObject, std::string hint)
{
    //std::cout << "deleted " << hint << std::endl;
    delete genericObject;
}

Napi::Value run(const Napi::CallbackInfo &info)
{

#ifdef YELLOW_DETECTOR_DEBUG
    std::cout << "yd run" << std::endl;
#endif

    Napi::Env env = info.Env();

#ifdef YELLOW_DETECTOR_DEBUG
    std::cout << info.Length() << "\t" << info[0].IsExternal() << "\t" << info[1].IsExternal() << "\t" << info[2].IsExternal() << "\t" << info[3].IsNumber() << "\t" << std::endl;
#endif

    if (info.Length() == 4 && info[0].IsExternal() && info[1].IsExternal() && info[2].IsExternal() && info[3].IsNumber())
    { //recieved cv mat
#ifdef YELLOW_DETECTOR_DEBUG
        std::cout << "yd run received cvmat" << std::endl;
#endif
        cv::Mat *img = info[0].As<Napi::External<cv::Mat>>().Data();
        YellowDetector *detector = info[1].As<Napi::External<YellowDetector>>().Data();
        PerceptionData *perceptionData = info[2].As<Napi::External<PerceptionData>>().Data();
        uint32_t debugImagesIndex = info[3].ToNumber().Uint32Value();
#ifdef YELLOW_DETECTOR_DEBUG
        std::cout << "yd run converted cvmat" << std::endl;
#endif
        cv::Mat *imgFromDetector = new cv::Mat();
        detector->run(*img, *img, perceptionData);
        *imgFromDetector = detector->debugImgVector[debugImagesIndex];
#ifdef YELLOW_DETECTOR_DEBUG
        std::cout << "yd run runned" << std::endl;
#endif

        return Napi::External<cv::Mat>::New(env, imgFromDetector, genericFinalizer<cv::Mat>, "cv::Mat");
    }
    else
    {
        throw Napi::Error::New(env, "Unexpected parameters, it should have been cv::Mat*, YellowDetector*, PerceptionData*, Uint");
    }
}

Napi::Value getDebugImageSize(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() == 1 && info[0].IsExternal())
    {
        YellowDetector *detector = info[0].As<Napi::External<YellowDetector>>().Data();
        return Napi::Number::New(env, detector->debugImgVector.size());
    }
    else
    {
        throw Napi::Error::New(env, "Parameter passed isn't an Yellow Detector object");
    }
}

Napi::Value getYellowDetectorParameters(const Napi::CallbackInfo &info)
{
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{

    //functions to be called within JS
    exports.Set(Napi::String::New(env, "run"), Napi::Function::New(env, run));
    exports.Set(Napi::String::New(env, "getDebugImageSize"), Napi::Function::New(env, getDebugImageSize));

    //object creation
    YellowDetector *yellowDetector = new YellowDetector();
    exports.Set(Napi::String::New(env, "detector"), Napi::External<YellowDetector>::New(env, yellowDetector, genericFinalizer<YellowDetector>, "Yellow Detector"));

    PerceptionData *perceptionData = new PerceptionData();
    exports.Set(Napi::String::New(env, "perceptionData"), Napi::External<PerceptionData>::New(env, perceptionData, genericFinalizer<PerceptionData>, "Perception Data"));

    Napi::Object attrObj = Napi::Object::New(env);
    exports.Set(Napi::String::New(env, "attrObj"), attrObj);

    return exports;
}

NODE_API_MODULE(yellowDetectorUtil, Init)