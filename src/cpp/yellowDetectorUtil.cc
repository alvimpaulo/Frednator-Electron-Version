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

Napi::Value getParameters(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    Napi::Object attrObj = Napi::Object::New(env);

    if (info.Length() == 1 && info[0].IsExternal())
    {
        YellowDetector *detector = info[0].As<Napi::External<YellowDetector>>().Data();

        attrObj.Set("minThreshold", detector->minThreshold);
        attrObj.Set("maxThreshold", detector->maxThreshold);
        attrObj.Set("filterByArea", detector->filterByArea);
        attrObj.Set("minArea", detector->minArea);
        attrObj.Set("filterByCircularity", detector->filterByCircularity);
        attrObj.Set("minCircularity", detector->minCircularity);
        attrObj.Set("filterByConvexity", detector->filterByConvexity);
        attrObj.Set("minConvexity", detector->minConvexity);
        attrObj.Set("filterByInertia", detector->filterByInertia);
        attrObj.Set("minInertiaRatio", detector->minInertiaRatio);
        attrObj.Set("iLowH", detector->iLowH);
        attrObj.Set("iHighH", detector->iHighH);
        attrObj.Set("iLowS", detector->iLowS);
        attrObj.Set("iHighS", detector->iHighS);
        attrObj.Set("iLowV", detector->iLowV);
        attrObj.Set("iHighV", detector->iHighV);

        return attrObj;
    }
}

Napi::Value setParameters(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() == 2 && info[0].IsExternal() && info[1].IsObject())
    {
        YellowDetector *detector = info[0].As<Napi::External<YellowDetector>>().Data();
        Napi::Object attrObj = info[1].As<Napi::Object>();

        Napi::Array attrObjProperties = attrObj.GetPropertyNames();

        for (int i = 0; i < attrObjProperties.Length(); ++i)
        {
            std::string propertyName = attrObjProperties[i].operator Napi::Value().ToString().Utf8Value();

            std::cout << attrObjProperties[i].operator Napi::Value().ToString().Utf8Value() << std::endl;
            if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("minThreshold") == 0)
                detector->minThreshold = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("maxThreshold") == 0)
            {
                detector->maxThreshold = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
            }
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("filterByArea") == 0)
            {
                detector->filterByArea = attrObj[propertyName].operator Napi::Value().ToBoolean();
            }
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("minArea") == 0)
            {
                detector->minArea = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
            }
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("filterByCircularity") == 0)
            {
                detector->filterByCircularity = attrObj[propertyName].operator Napi::Value().ToBoolean();
            }
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("minCircularity") == 0)
            {
                detector->minCircularity = attrObj[propertyName].operator Napi::Value().ToNumber().DoubleValue();
            }
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("filterByConvexity") == 0)
            {
                detector->filterByConvexity = attrObj[propertyName].operator Napi::Value().ToBoolean();
            }
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("minConvexity") == 0)
            {
                detector->minConvexity = attrObj[propertyName].operator Napi::Value().ToNumber().DoubleValue();
            }
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("filterByInertia") == 0)
            {
                detector->filterByInertia = attrObj[propertyName].operator Napi::Value().ToBoolean();
            }
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("minInertiaRatio") == 0)
            {
                detector->minInertiaRatio = attrObj[propertyName].operator Napi::Value().ToNumber().DoubleValue();
            }
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("iLowH") == 0)
            {
                detector->iLowH = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
            }
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("iHighH") == 0)
            {
                detector->iHighH = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
            }
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("iLowS") == 0)
            {
                detector->iLowS = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
            }
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("iHighS") == 0)
            {
                detector->iHighS = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
            }
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("iLowV") == 0)
            {
                detector->iLowV = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
            }
            else if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("iHighV") == 0)
            {
                detector->iHighV = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
            }
        }

        return attrObj;
    }
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{

    //functions to be called within JS
    exports.Set(Napi::String::New(env, "run"), Napi::Function::New(env, run));
    exports.Set(Napi::String::New(env, "getDebugImageSize"), Napi::Function::New(env, getDebugImageSize));
    exports.Set(Napi::String::New(env, "getParameters"), Napi::Function::New(env, getParameters));
    exports.Set(Napi::String::New(env, "setParameters"), Napi::Function::New(env, setParameters));

    //object creation
    YellowDetector *yellowDetector = new YellowDetector();
    exports.Set(Napi::String::New(env, "detector"), Napi::External<YellowDetector>::New(env, yellowDetector, genericFinalizer<YellowDetector>, "Yellow Detector"));

    PerceptionData *perceptionData = new PerceptionData();
    exports.Set(Napi::String::New(env, "perceptionData"), Napi::External<PerceptionData>::New(env, perceptionData, genericFinalizer<PerceptionData>, "Perception Data"));

    return exports;
}

NODE_API_MODULE(yellowDetectorUtil, Init)