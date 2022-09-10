#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

static bool
imreadmulti_(const String& filename, int flags, std::vector<Mat>& mats, int start, int count)
{
    ImageCollection collection;

    try {
        collection.init(filename, flags);
    } catch (const cv::Exception& e)
    {
        std::cerr << "imreadmulti_('" << filename << "'): can't read header: " << e.what() << std::endl << std::flush;
        return 0;
    }
    catch (...)
    {
        std::cerr << "imreadmulti_('" << filename << "'): can't read header: unknown exception" << std::endl << std::flush;
        return 0;
    }

    ImageCollection::iterator iter = collection.begin();

    if(collection.size() < (size_t)start)
        return false;
    if(count != -1) {
        for(int i = 0; i < start; ++i) {
            ++iter;
        }
    }

    try{
        for(size_t i = 0; i < (size_t)count && i < collection.size(); ++i) {
            mats.push_back(*iter);
            iter++;
        }
    } catch (const cv::Exception& e)
    {
        std::cerr << "imreadmulti_('" << filename << "'): can't read data: " << e.what() << std::endl << std::flush;
    }
    catch (...)
    {
        std::cerr << "imreadmulti_('" << filename << "'): can't read data: unknown exception" << std::endl << std::flush;
    }

    return !mats.empty();
}

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

int A_imreadmultiTest1()
{
    vector<Mat> rmats;
    TickMeter tm;
    tm.start();
    size_t imcnt = imcount("test100.tif", 1);
    tm.stop();
    cout << "imcount:" << tm << endl;
    tm.reset();
    tm.start();
    bool ret = imreadmulti_("test100.tif", IMREAD_ANYCOLOR, rmats, 0, 99);
    tm.stop();
    cout << "read 0-99:" << tm << endl;
    cout << "result of A_imreadmulti : " << ret << endl;
    return 0;    
}

int A_imreadmultiTest2()
{
    vector<Mat> rmats;
    TickMeter tm;
    tm.start();
    size_t imcnt = imcount("test100.tif", 1);
    tm.stop();
    cout << "imcount:" << tm << endl;
    tm.reset();
    tm.start();
    bool ret = imreadmulti_("test100.tif", IMREAD_ANYCOLOR, rmats, 100, 99);
    tm.stop();
    cout << "read 100-199:" << tm << endl;
    cout << "result of A_imreadmulti : " << ret << endl;
    return 0;    
}

int A_imreadmultiTest3()
{
    vector<Mat> rmats;
    TickMeter tm;
    tm.start();
    size_t imcnt = imcount("test100.tif", 1);
    tm.stop();
    cout << "imcount:" << tm << endl;
    tm.reset();
    tm.start();
    bool ret = imreadmulti_("test100.tif", IMREAD_ANYCOLOR, rmats, 0, 199);
    tm.stop();
    cout << "read all:" << tm << endl;
    cout << "result of A_imreadmulti : " << ret << endl;
    return 0;    
}

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
        imreadmultiTest1();
        A_imreadmultiTest1();
        imreadmultiTest2();
        A_imreadmultiTest2();
        imreadmultiTest3();
        A_imreadmultiTest3();
        VideoCaptureTest(filename);
        imreadTest(filename);
        cout << "---------------------------------------------------------" << endl;
    }

    return 0;
}
