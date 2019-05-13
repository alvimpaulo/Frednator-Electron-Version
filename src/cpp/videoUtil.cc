#include <napi.h>

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <chrono>

// #define IMAGE_FROM_VIDEO_DEBUG
// #define VIDEO_OPENER_DEBUG
// #define VIDEO_CLOSER_DEBUG

/**
 * @brief Function to delete data from a std::vector
 * 
 * Function used to delete the data from a std::vector that holds raw data from a cv::Mat.
 * @param env Napi::Env env.
 * @param vec Pointer to the raw data from a std::vector.
 */

void vectorFinalizer(Napi::Env env, void *vec)
{
    //std::cout << "deleting vec" << std::endl;
    delete (uchar *)vec; // Probably has leak, deleting only the vector raw data, not the object itself
}

/**
 * @brief Template function to delete simple objects.
 * 
 * Function used to delete a simple object that requires a single delete.
 * 
 * @tparam genericClass Class of the object to be deleted.
 * @param env Napi::Env env.
 * @param genericObject Object to be deleted.
 * @param hint String to be printed when debugging the deletion.
 */
template <class genericClass>
void genericFinalizer(Napi::Env env, genericClass *genericObject, std::string hint)
{
    //std::cout << "deleted " << hint << std::endl;
    delete genericObject;
}

/**
 * @brief Function to convert from a cv::mat to a Napi::TypedArrayOf<u_int8_t>.
 * 
 * @param info Arguments, should have, in order: 
 *  [0] - cv::Mat containing the image to be converted.
 * @return Napi::Value Typed array to be returned.
 */

Napi::Value typedArrayFromCvMat(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    cv::Mat *img;

    // getting img from arguments and setting it's size if needed
    img = info[0].As<Napi::External<cv::Mat>>().Data();
    //cv::resize((*img), (*img), cv::Size(320, 240)); //only needed if capture can't be set

    //array that will the 3 channels separated
    std::vector<cv::Mat> *rgbchannels = new std::vector<cv::Mat>(3); 
    //array that will have each of the channels from the original cv::mat + the alpha nedded by the canvas on index.html
    std::vector<u_char> *fourChannelVector = new std::vector<u_char>(img->cols * img->rows * 4); 

    if (img->channels() == 3) // RGB
    {
        cv::cvtColor((*img), (*img), CV_BGR2RGB);
        cv::split((*img), (*rgbchannels));
        for (size_t i = 0; i < img->cols * img->rows; ++i) //put RGB values into fourChannelVector, put alpha = 255
        {
            (*fourChannelVector)[i * 4] = (*rgbchannels)[0].data[i];
            (*fourChannelVector)[i * 4 + 1] = (*rgbchannels)[1].data[i];
            (*fourChannelVector)[i * 4 + 2] = (*rgbchannels)[2].data[i];
            (*fourChannelVector)[i * 4 + 3] = 255;
        }
        //std::cout << " fourchannelvector data: " << (int)fourChannelVector->at(0) << "\t" << (int)fourChannelVector->at(1) << "\t" << (int)fourChannelVector->at(2) << "\t" << (int)fourChannelVector->at(3) << "\t" << std::endl;
    }

    if (img->channels() == 1) //B&W
    {
        for (size_t i = 0; i < (img->cols * img->rows); i++)
        {
            u_char grayPixel = img->data[i];
            (*fourChannelVector)[i * 4] = grayPixel;
            (*fourChannelVector)[i * 4 + 1] = grayPixel;
            (*fourChannelVector)[i * 4 + 2] = grayPixel;
            (*fourChannelVector)[i * 4 + 3] = 255;
        }
    }

    // std::cout << "img data: " << img->cols << "\t" << img->rows << "\t" << img->channels() << "\t" << img->total() << "\t" << std::endl;

    Napi::ArrayBuffer arrayBuffer = Napi::ArrayBuffer::New(env, fourChannelVector->data(), fourChannelVector->size(), vectorFinalizer);

    //Four channel typed array of uchar
    Napi::TypedArrayOf<u_int8_t> typedArray = Napi::TypedArrayOf<uint8_t>::New(env, img->cols * img->rows * 4, arrayBuffer, 0);

    delete rgbchannels;
    return typedArray;
}

/**
 * @brief Function to grab cv::Mat com cv::VideoCapture.
 * 
 * @param info Function arguments, should have, in order: 
 *  [0] - cv::Mat containing the image to be retuened.
 * @return Napi::Value Either next frame from video or false, if it fails/reaches the end of a video
 */

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

        //Get capture from argument [0]
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

            //img to be returned. Will be deleted by the garbage collector.
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

            return Napi::External<cv::Mat>::New(env, tempImg, genericFinalizer<cv::Mat>, "cv::Mat");
        }
    }
    return Napi::Boolean::New(env, false);
}

/**
 * @brief Function to open and return a cv::VideoCapture.
 * 
 * @param info Function arguments, should have, in order: 
 *  [0] - String contaning the capture address.
 * @return Napi::Value Either a cv::VideoCapture or false if it fails to open the capture.
 */

Napi::Value videoOpener(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    //Capture to be returned
    cv::VideoCapture *cap;

    //open video from a file
    if (info.Length() == 1 && info[0].IsString())
    {
        #ifdef VIDEO_OPENER_DEBUG
            std::cout << "capture from path " << info[0].ToString().Utf8Value() << std::endl;
        #endif

        cap = new cv::VideoCapture(info[0].ToString().Utf8Value(), cv::CAP_GSTREAMER); //may crash in windows
    }

    //Open video from an webcam
    else if (info.Length() == 1 && info[0].IsNumber())
    {
        #ifdef VIDEO_OPENER_DEBUG
            std::cout << "capture from number " << info[0].ToNumber().Int64Value() << std::endl;
        #endif
        cap = new cv::VideoCapture(info[0].ToNumber().Int64Value(), cv::CAP_V4L2); //may crash in windows
    }

    //Try to capture with the correct resolution.
    if (info[0].IsNumber())
    {
        (*cap).set(CV_CAP_PROP_FRAME_WIDTH, 640);  //not working
        (*cap).set(CV_CAP_PROP_FRAME_HEIGHT, 480); //not working
    }

    if (!cap->isOpened())
    {
        #ifdef VIDEO_OPENER_DEBUG
            std::cout << "cap not opened" << std::endl;
        #endif
        return Napi::Boolean::New(env, false);
    }
    #ifdef VIDEO_OPENER_DEBUG
        std::cout << "cap opened" << std::endl;
    #endif

    return Napi::External<cv::VideoCapture>::New(env, cap);
}

/**
 * @brief Function to close the cv::VideoCapture.
 * Function to close the video capture, should it be stopped.
 * 
 * @param info Function arguments, should have, in order: 
 *  [0] - cv::VideoCapture to be closed.
 * @return Napi::Value Either true if it succeded or false if it did not.
 */

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
        //Incorrect type of argument or number of arguments.
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

/**
 * @brief Init function.
 * 
 * Function called on module import. Sets everything callable from JS.
 * 
 * @param env Env.
 * @param exports Exports.
 * @return Napi::Object Exports.
 */

Napi::Object Init(Napi::Env env, Napi::Object exports)
{

    //functions to be called within JS
    exports.Set(Napi::String::New(env, "videoOpener"), Napi::Function::New(env, videoOpener));
    exports.Set(Napi::String::New(env, "imgFromVideo"), Napi::Function::New(env, imgFromVideo));
    exports.Set(Napi::String::New(env, "videoCloser"), Napi::Function::New(env, videoCloser));
    exports.Set(Napi::String::New(env, "typedArrayFromCvMat"), Napi::Function::New(env, typedArrayFromCvMat));
    return exports;
}

NODE_API_MODULE(videoUtil, Init)
