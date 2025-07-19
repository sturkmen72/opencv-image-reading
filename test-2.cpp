#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

std::string flag_names(int value)
{
    struct FlagDef
    {
        int flag;
        const char* name;
    };

    const std::vector<FlagDef> all_flags = {
        {cv::IMREAD_UNCHANGED,           "IMREAD_UNCHANGED"},
        {cv::IMREAD_GRAYSCALE,           "IMREAD_GRAYSCALE"},
        {cv::IMREAD_COLOR,               "IMREAD_COLOR"},
        {256,           "IMREAD_COLOR_RGB"},
        {cv::IMREAD_ANYDEPTH,            "IMREAD_ANYDEPTH"},
        {cv::IMREAD_ANYCOLOR,            "IMREAD_ANYCOLOR"},
        {cv::IMREAD_LOAD_GDAL,           "IMREAD_LOAD_GDAL"},
        {cv::IMREAD_REDUCED_GRAYSCALE_2, "IMREAD_REDUCED_GRAYSCALE_2"},
        {cv::IMREAD_REDUCED_COLOR_2,     "IMREAD_REDUCED_COLOR_2"},
        {cv::IMREAD_REDUCED_GRAYSCALE_4, "IMREAD_REDUCED_GRAYSCALE_4"},
        {cv::IMREAD_REDUCED_COLOR_4,     "IMREAD_REDUCED_COLOR_4"},
        {cv::IMREAD_REDUCED_GRAYSCALE_8, "IMREAD_REDUCED_GRAYSCALE_8"},
        {cv::IMREAD_REDUCED_COLOR_8,     "IMREAD_REDUCED_COLOR_8"},
        {cv::IMREAD_IGNORE_ORIENTATION,  "IMREAD_IGNORE_ORIENTATION"},
    };

    std::vector<std::string> names;
    int matched = 0;

    for (const auto& def : all_flags)
    {
        if ((value & def.flag) == def.flag)
        {
            names.push_back(def.name);
            matched |= def.flag;
        }
    }

    if (names.empty())
        return "UNKNOWN_FLAG";

    if (matched != value)
    {
        std::ostringstream oss;
        oss << "UNKNOWN_FLAG_BITS(0x" << std::hex << (value & ~matched) << ")";
        names.push_back(oss.str());
    }

    std::ostringstream out;
    for (size_t i = 0; i < names.size(); ++i)
    {
        if (i > 0)
            out << " | ";
        out << names[i];
    }

    return out.str();
}

int test_flags()
{
    // Step 1: Create a sample CV_16UC4 Mat
    cv::Mat src(100, 100, CV_16UC4, cv::Scalar(1000, 2000, 3000, 4000));

    // Step 2: Encode as PNG in memory
    std::vector<uchar> buf;
    std::vector<int> params = {cv::IMWRITE_PNG_COMPRESSION, 3};
    cv::imencode(".png", src, buf, params);

    std::cout << "Original image: type = " << src.type()
              << ", depth = " << src.depth()
              << ", channels = " << src.channels() << std::endl;

    // Step 3: Try decoding with flags from 1 to 100
    for (int flags = 1; flags <= 300; ++flags)
    {
        try
        {
            cv::Mat decoded = cv::imdecode(buf, flags);
            if (decoded.empty())
            {
                std::cout << "Flag " << flags << " (" << flag_names(flags) << "): Failed to decode.\n";
                continue;
            }

            int depth = decoded.elemSize1() * 8; // bit depth
            int channels = decoded.channels();

            std::cout << "Flag " << flags << "Channels = " << channels
                      << ", Bit Depth = " << depth << "\n";
        }
        catch (const cv::Exception& e)
        {
            std::cout << "Flag " << flags << " (" << flag_names(flags) << "): Exception - "
                      << e.what() << "\n";
        }
    }

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
        String sfilename = cv::format("C:/projects/opencv-image-reading/sequence/%08d.png", i);
        frame = imread(filename, IMREAD_UNCHANGED);
        if (frame.empty())
        {
            break;
        }
        imwrite(sfilename,frame);
		frame = imread(sfilename, IMREAD_UNCHANGED);
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
        test_flags();
        cout << "---------------------------------------------------------" << endl;
    }

    return 0;
}
