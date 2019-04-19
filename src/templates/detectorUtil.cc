//! Used and generated by an external program, be careful when changing it
/* Patterns info:
lowercaseDetector  = detectorDetector
uppercaseDetector = DetectorDetector
spacedDetector = Detector Detector
attrObjSets = for each detector public varaible, 
    attrObj.Set("-varName-", detector->-varName-);
setPropertyNames = for each detector public variable, 
    if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("-varName-") == 0)
                detector->-varName- = attrObj[propertyName].operator Napi::Value().-varType-;
 */

#include <napi.h>

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <chrono>

#include "../../includes-competition-code/perception/include/#lowecaseDetector#.hpp"

template <class genericClass>
void genericFinalizer(Napi::Env env, genericClass *genericObject, std::string hint)
{
    delete genericObject;
}

Napi::Value run(const Napi::CallbackInfo &info)
{

    Napi::Env env = info.Env();

    if (info.Length() == 4 && info[0].IsExternal() && info[1].IsExternal() && info[2].IsExternal() && info[3].IsNumber())
    { //recieved cv mat
        cv::Mat *img = info[0].As<Napi::External<cv::Mat>>().Data();
        #uppercaseDetector# *detector = info[1].As<Napi::External<#uppercaseDetector#>>().Data();
        PerceptionData *perceptionData = info[2].As<Napi::External<PerceptionData>>().Data();
        uint32_t debugImagesIndex = info[3].ToNumber().Uint32Value();

        cv::Mat *imgFromDetector = new cv::Mat();
        detector->run(*img, *img, perceptionData);
        *imgFromDetector = detector->debugImgVector[debugImagesIndex];

        return Napi::External<cv::Mat>::New(env, imgFromDetector, genericFinalizer<cv::Mat>, "cv::Mat");
    }
    else
    {
        throw Napi::Error::New(env, "Unexpected parameters, it should have been cv::Mat*, Detector*, PerceptionData*, Uint");
    }
}

Napi::Value getDebugImageSize(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() == 1 && info[0].IsExternal())
    {
        #uppercaseDetector# *detector = info[0].As<Napi::External<#uppercaseDetector#>>().Data();
        return Napi::Number::New(env, detector->debugImgVector.size());
    }
    else
    {
        throw Napi::Error::New(env, "Parameter passed isn't a Detector object");
    }
}

Napi::Value getParameters(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    Napi::Object attrObj = Napi::Object::New(env);

    if (info.Length() == 1 && info[0].IsExternal())
    {
        #uppercaseDetector# *detector = info[0].As<Napi::External<#uppercaseDetector#>>().Data();

        #attrObjSets#

        return attrObj;
    }
}

Napi::Value setParameters(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() == 2 && info[0].IsExternal() && info[1].IsObject())
    {
        #uppercaseDetector# *detector = info[0].As<Napi::External<#uppercaseDetector#>>().Data();
        Napi::Object attrObj = info[1].As<Napi::Object>();

        Napi::Array attrObjProperties = attrObj.GetPropertyNames();

        for (int i = 0; i < attrObjProperties.Length(); ++i)
        {
            std::string propertyName = attrObjProperties[i].operator Napi::Value().ToString().Utf8Value();

            #setPropertyNames#
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
    #uppercaseDetector# *#lowecaseDetector# = new #uppercaseDetector#();
    exports.Set(Napi::String::New(env, "detector"), Napi::External<#uppercaseDetector#>::New(env, #lowecaseDetector#, genericFinalizer<#uppercaseDetector#>, "#spacedDetector#"));

    PerceptionData *perceptionData = new PerceptionData();
    exports.Set(Napi::String::New(env, "perceptionData"), Napi::External<PerceptionData>::New(env, perceptionData, genericFinalizer<PerceptionData>, "Perception Data"));

    return exports;
}

NODE_API_MODULE(#lowecaseDetector#Util, Init)