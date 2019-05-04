//! Start of code used and generated by an external program, be careful when changing it
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

//$start #lowercaseDetectector#Util.cc:includes
#include "../../includes-competition-code/perception/include/fieldDetector2.hpp"
//$end #lowercaseDetectector#Util.cc:includes

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
        
        //$start #lowercaseDetectector#Util.cc:run:detectorFromParameteters
        FieldDetector2 *detector = info[1].As<Napi::External<FieldDetector2>>().Data();
        //$end #lowercaseDetectector#Util.cc:run:detectorFromParameteters

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
        //$start #lowercaseDetectector#Util.cc:getDebugImageSize:detectorFromParameters
        FieldDetector2 *detector = info[0].As<Napi::External<FieldDetector2>>().Data();
        //$start #lowercaseDetectector#Util.cc:getDebugImageSize:detectorFromParameters
        
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
        //$start #lowercaseDetectector#Util.cc:getParameters:detectorFromParameters
        FieldDetector2 *detector = info[0].As<Napi::External<FieldDetector2>>().Data();
        //$end #lowercaseDetectector#Util.cc:getParameters:detectorFromParameters

        //$start #lowercaseDetectector#Util.cc:getParameters:attrObjSets
        attrObj.Set("iLowH", detector->iLowH);
		attrObj.Set("iHighH", detector->iHighH);
		attrObj.Set("iLowS", detector->iLowS);
		attrObj.Set("iHighS", detector->iHighS);
		attrObj.Set("iLowL", detector->iLowL);
		attrObj.Set("iHighL", detector->iHighL);
		attrObj.Set("factor", detector->factor);
		attrObj.Set("dp", detector->dp);
		attrObj.Set("minDist", detector->minDist);
		attrObj.Set("param1", detector->param1);
		attrObj.Set("param2", detector->param2);
		attrObj.Set("maxRadius", detector->maxRadius);
		attrObj.Set("minRadius", detector->minRadius);
		attrObj.Set("kernel", detector->kernel);
        //$end #lowercaseDetectector#Util.cc:getParameters:attrObjSets

        return attrObj;
    }
}

Napi::Value setParameters(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() == 2 && info[0].IsExternal() && info[1].IsObject())
    {
        //$start #lowercaseDetectector#Util.cc:setParameters:detectorFromParameters
        FieldDetector2 *detector = info[0].As<Napi::External<FieldDetector2>>().Data();
        //$start #lowercaseDetectector#Util.cc:setParameters:detectorFromParameters
        
        Napi::Object attrObj = info[1].As<Napi::Object>();

        Napi::Array attrObjProperties = attrObj.GetPropertyNames();

        for (int i = 0; i < attrObjProperties.Length(); ++i)
        {
            std::string propertyName = attrObjProperties[i].operator Napi::Value().ToString().Utf8Value();
            //$start #lowercaseDetectector#Util.cc:setParameters:setPropertyNames
            
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("iLowH") == 0)
            detector->iLowH = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
        
		
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("iHighH") == 0)
            detector->iHighH = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
        
		
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("iLowS") == 0)
            detector->iLowS = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
        
		
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("iHighS") == 0)
            detector->iHighS = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
        
		
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("iLowL") == 0)
            detector->iLowL = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
        
		
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("iHighL") == 0)
            detector->iHighL = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
        
		
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("factor") == 0)
            detector->factor = attrObj[propertyName].operator Napi::Value().ToNumber().FloatValue();
        
		
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("dp") == 0)
            detector->dp = attrObj[propertyName].operator Napi::Value().ToNumber().DoubleValue();
        
		
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("minDist") == 0)
            detector->minDist = attrObj[propertyName].operator Napi::Value().ToNumber().DoubleValue();
        
		
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("param1") == 0)
            detector->param1 = attrObj[propertyName].operator Napi::Value().ToNumber().DoubleValue();
        
		
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("param2") == 0)
            detector->param2 = attrObj[propertyName].operator Napi::Value().ToNumber().DoubleValue();
        
		
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("maxRadius") == 0)
            detector->maxRadius = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
        
		
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("minRadius") == 0)
            detector->minRadius = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
        
		
        if (attrObjProperties[i].operator Napi::Value().ToString().Utf8Value().compare("kernel") == 0)
            detector->kernel = attrObj[propertyName].operator Napi::Value().ToNumber().Int32Value();
        
            //$start #lowercaseDetectector#Util.cc:setParameters:setPropertyNames

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
    //$start #lowercaseDetectector#Util.cc:Init:exportDetector
    FieldDetector2 *fieldDetector2 = new FieldDetector2();
    exports.Set(Napi::String::New(env, "detector"), Napi::External<FieldDetector2>::New(env, fieldDetector2, genericFinalizer<FieldDetector2>, "Field Detector2"));
    //$start #lowercaseDetectector#Util.cc:Init:exportDetector

    PerceptionData *perceptionData = new PerceptionData();
    exports.Set(Napi::String::New(env, "perceptionData"), Napi::External<PerceptionData>::New(env, perceptionData, genericFinalizer<PerceptionData>, "Perception Data"));

    return exports;
}

NODE_API_MODULE(fieldDetector2Util, Init)

//! End of code used and generated by an external program, be careful when changing it