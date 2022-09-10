#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int imreadmultiTest1()
{
    vector<Mat> rmats;
    TickMeter tm;
    tm.start();
    size_t imcnt = imcount("test100.tif", 1);
    tm.stop();
    cout << "imcount:" << tm << endl;
    tm.reset();
    tm.start();
    bool ret = imreadmulti("test100.tif", rmats, 0, 99);
    tm.stop();
    cout << "read 0-99:" << tm << endl;
    cout << "result of imreadmulti : " << ret << endl;
    return 0;    
}

int imreadmultiTest2()
{
    vector<Mat> rmats;
    TickMeter tm;
    tm.start();
    size_t imcnt = imcount("test100.tif", 1);
    tm.stop();
    cout << "imcount:" << tm << endl;
    tm.reset();
    tm.start();
    bool ret = imreadmulti("test100.tif", rmats, 100, 99);
    tm.stop();
    cout << "read 100-199:" << tm << endl;
    cout << "result of imreadmulti : " << ret << endl;
    return 0;    
}

int imreadmultiTest3()
{
    vector<Mat> rmats;
    TickMeter tm;
    tm.start();
    size_t imcnt = imcount("test100.tif", 1);
    tm.stop();
    cout << "imcount:" << tm << endl;
    tm.reset();
    tm.start();
    bool ret = imreadmulti("test100.tif", rmats);
    tm.stop();
    cout << "read all:" << tm << endl;
    cout << "result of imreadmulti : " << ret << endl;
    return 0;    
}

int imreadNEWTest1(const char* fmt)
{
    Mat frame;
    size_t nFrames = 0;
    int64 t0 = cv::getTickCount();
    for (int i = 1; i < 101; i++)
    {
        String filename = cv::format(fmt, i);
        imread(filename, frame, IMREAD_UNCHANGED);

        if (frame.empty())
        {
            break;
        }
        nFrames++;
    }

    int64 t1 = cv::getTickCount();
    cout << "Frames captured (new imread 1) : " << cv::format("%5lld", (long long int)nFrames)
        << "    Average FPS: " << cv::format("%9.1f", (double)getTickFrequency() * nFrames / (t1 - t0))
        << "    Average time per frame: " << cv::format("%9.2f ms", (double)(t1 - t0) * 1000.0f / (nFrames * getTickFrequency()))
        << endl;
    return nFrames > 0 ? 0 : 1;
}

int imreadNEWTest2(const char* fmt)
{
    UMat frame;
    size_t nFrames = 0;
    int64 t0 = cv::getTickCount();
    for (int i = 1; i < 101; i++)
    {
        String filename = cv::format(fmt, i);
        imquery iminfo(filename);
        if (iminfo.page_count() > 0)
            imread(filename, frame, IMREAD_UNCHANGED);

        if (frame.empty())
        {
            break;
        }
        nFrames++;
    }

    int64 t1 = cv::getTickCount();
    cout << "Frames captured (new imread 2) : " << cv::format("%5lld", (long long int)nFrames)
        << "    Average FPS: " << cv::format("%9.1f", (double)getTickFrequency() * nFrames / (t1 - t0))
        << "    Average time per frame: " << cv::format("%9.2f ms", (double)(t1 - t0) * 1000.0f / (nFrames * getTickFrequency()))
        << endl;
    return nFrames > 0 ? 0 : 1;
}

int imreadTest(const char* fmt)
{
    Mat frame;
    size_t nFrames = 0;
    int64 t0 = cv::getTickCount();
    for (int i = 1; i < 101; i++)
    {
        String filename = cv::format(fmt, i);
        frame = imread(filename, IMREAD_UNCHANGED);
        if (frame.empty())
        {
            break;
        }
        nFrames++;
    }

    int64 t1 = cv::getTickCount();
    cout << "Frames captured (imread)       : " << cv::format("%5lld", (long long int)nFrames)
        << "    Average FPS: " << cv::format("%9.1f", (double)getTickFrequency() * nFrames / (t1 - t0))
        << "    Average time per frame: " << cv::format("%9.2f ms", (double)(t1 - t0) * 1000.0f / (nFrames * getTickFrequency()))
        << endl;
    return nFrames > 0 ? 0 : 1;
}

int VideoCaptureTest(const char* filename)
{
    Mat frame;
    VideoCapture capture(filename); // open the first camera
    if (!capture.isOpened())
    {
        cerr << "ERROR: Can't initialize camera capture : " << filename << endl;
        return 1;
    }

    size_t nFrames = 0;
    int64 t0 = cv::getTickCount();
    for (;;)
    {
        capture >> frame; // read the next frame from camera
        if (frame.empty())
        {
            break;
        }
        nFrames++;
    }

    int64 t1 = cv::getTickCount();
    cout << "Frames captured (VideoCapture) : " << cv::format("%5lld", (long long int)nFrames)
        << "    Average FPS: " << cv::format("%9.1f", (double)getTickFrequency() * nFrames / (t1 - t0))
        << "    Average time per frame: " << cv::format("%9.2f ms", (double)(t1 - t0) * 1000.0f / (nFrames * getTickFrequency()))
        << endl;
    return nFrames > 0 ? 0 : 1;
}

int main()
{
    const char* filename = "C:/projects/opencv-image-reading/sequence/%08d.jpg";
    for (int i = 0; i < 5; i++)
    {
        imreadmultiTest1();
        imreadmultiTest2();
        imreadmultiTest3()
        VideoCaptureTest(filename);
        imreadTest(filename);
        imreadNEWTest1(filename);
        imreadNEWTest2(filename);
        cout << "---------------------------------------------------------" << endl;
    }

    return 0;
}
