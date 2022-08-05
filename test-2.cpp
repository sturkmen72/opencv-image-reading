#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int imreadTest(const char* fmt)
{
    Mat frame;
    size_t nFrames = 0;
    int64 t0 = cv::getTickCount();
    for (int i = 1; i < 101; i++)
    {
        String filename = cv::format(fmt, i);
        String sfilename = cv::format("C:/projects/opencv-image-reading/sequence/%08d.jpg", i);
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
        VideoCaptureTest(filename);
        imreadTest(filename);
        cout << "---------------------------------------------------------" << endl;
    }

    return 0;
}
