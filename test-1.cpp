#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

void createAlphaMat(Mat& mat)
{
    CV_Assert(mat.channels() == 4);
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            Vec4b& bgra = mat.at<Vec4b>(i, j);
            bgra[0] = UCHAR_MAX; // Blue
            bgra[1] = saturate_cast<uchar>((float(mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX); // Green
            bgra[2] = saturate_cast<uchar>((float(mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX); // Red
            bgra[3] = saturate_cast<uchar>(0.5 * (bgra[1] + bgra[2])); // Alpha
        }
    }
}

int pngWritingParamsTest()
{
    TickMeter tm;
    // Create mat with alpha channel
    Mat mat(1440, 1920, CV_8UC4);
    createAlphaMat(mat);

    vector<int> compression_params;
    compression_params.push_back(IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(0);
    compression_params.push_back(IMWRITE_PNG_STRATEGY);
    compression_params.push_back(IMWRITE_PNG_STRATEGY_DEFAULT);
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 5; j++)
        {
            compression_params[1] = i;
            compression_params[3] = j;
            tm.start();
            imwrite(format("PNG_STRATEGY_%d_PNG_COMPRESSION_%d.png", j, i), mat, compression_params);
            tm.stop();
            std::cout << format("PNG_STRATEGY_%d_PNG_COMPRESSION_%d.png", j, i) << "  saved in " << tm.getTimeMilli() << " ms.";
			
            tm.reset();
            tm.start();
            Mat img = imread(format("PNG_STRATEGY_%d_PNG_COMPRESSION_%d.png", j, i));
            tm.stop();
            std::cout << "\t read time " << tm.getTimeMilli() << " ms." << std::endl;
        }
    tm.reset();
    tm.start();

    //imwrite("PNG_SAVED_DEFAULT.png", mat, { IMWRITE_PNG_COMPRESSION, 9 });
    imwrite("PNG_SAVED_DEFAULT.png", mat);
    tm.stop();
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "PNG_SAVED_DEFAULT.png" << "  saved in " << tm.getTimeMilli() << " ms." << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    tm.reset();
    tm.start();
    Mat img = imread("PNG_SAVED_DEFAULT.png");
    tm.stop();
    std::cout << "\t read time " << tm.getTimeMilli() << " ms." << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    tm.reset();
    tm.start();
    img = imread("C:/projects/opencv-image-reading/opencv_extra/testdata/highgui/readwrite/read.png");
    tm.stop();
    std::cout << "\t read.png read time " << tm.getTimeMilli() << " ms." << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    tm.reset();
    tm.start();
    img = imread("C:/projects/opencv-image-reading/opencv_extra/testdata/highgui/readwrite/read.png", IMREAD_UNCHANGED);
    tm.stop();
    std::cout << "\t read.png read time " << tm.getTimeMilli() << " ms." << " IMREAD_UNCHANGED" << std::endl;

    tm.reset();
    tm.start();
    imwrite("C:/projects/opencv-image-reading/opencv_extra/testdata/highgui/readwrite/read1.png", img);
    tm.stop();
    std::cout << "\t read.png write time " << tm.getTimeMilli() << " ms." << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    return 0;
}

int main()
{
    std::cout << "reading the file : chunk_data_is_too_large.png" << std::endl;
    Mat src0 = imread("C:/projects/opencv-image-reading/chunk_data_is_too_large.png");
    std::cout << "file : chunk_data_is_too_large.png, its dimensions : " << src0.cols << "x" << src0.rows << std::endl;
    
    pngWritingParamsTest();

    vector<String> filenames;
    String folder = "C:/projects/opencv-image-reading/pngsuite/*.png";
    glob(folder, filenames);

    for (size_t i = 0; i < filenames.size(); i++)
    {
        TickMeter tm;
        tm.start();
        Mat src = imread(filenames[i], IMREAD_UNCHANGED);
        tm.stop();
        if (src.empty())
            std::cout << filenames[i] << "--------- Could not read the image" << std::endl;
        else
        {
            std::cout << filenames[i] << " \t read time " << tm.getTimeMilli() << " ms. \t" << src.channels() << " channels" <<std::endl;
        }

    }
    return 0;
}
