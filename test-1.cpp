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
            std::cout << " \t read time " << tm.getTimeMilli() << " ms." << std::endl;
        }
    tm.reset();
    tm.start();

    //imwrite("PNG_SAVED_DEFAULT.png", mat, { IMWRITE_PNG_COMPRESSION, 9 });
    imwrite("PNG_SAVED_DEFAULT.png", mat);
    tm.stop();
    std::cout << "PNG_SAVED_DEFAULT.png" << "  saved in " << tm.getTimeMilli() << " ms.";

    tm.reset();
    tm.start();
    Mat img = imread("PNG_SAVED_DEFAULT.png");
    tm.stop();
    std::cout << " \t read time " << tm.getTimeMilli() << " ms." << std::endl;
    return 0;
}

int main()
{
    vector<Mat> mats;

    for (int i = 0; i < 100; i++)
    {
        Mat img(640, 640, CV_8UC3);
        randu(img, Scalar(0, 0, 0), Scalar(255, 255, 255));
        putText(img, format("%d", i),
                Point(img.cols / 4, img.rows / 2),
                FONT_HERSHEY_COMPLEX, 5, Scalar(0, 0, 255),4);
        mats.push_back(img.clone());
        //imshow("random colors image", img);
        //waitKey(1);
    }
    imwrite("test100.tif", mats);

    vector<Mat> rmats;
    TickMeter tm;
    tm.start();
    size_t imcnt = imcount("test100.tif", 1);
    tm.stop();
    cout << "imcount:" << tm << endl;
    tm.reset();
    tm.start();
    imreadmulti("test100.tif", rmats, 99, 1, 0);
    tm.stop();
    cout << "99:" << tm << endl;
    imwrite("test99.jpg",rmats[0]); 
    tm.reset();
    tm.start();
    imreadmulti("test100.tif", rmats, 45, 1, 0);
    tm.stop();
    cout << "45:" << tm << endl;
    imwrite("test45.jpg",rmats[1]);
    tm.reset();
    tm.start();
    imreadmulti("test100.tif", rmats, 2, 1, 0);
    tm.stop();
    cout << "2:" << tm << endl;
    imwrite("test2.jpg",rmats[1]);
    tm.reset();
    tm.start();
    imreadmulti("test100.tif", rmats, 8, 1, IMREAD_REDUCED_GRAYSCALE_8);
    tm.stop();
    cout << "IMREAD_REDUCED_GRAYSCALE_8 8:" << tm << endl;
    imwrite("test8.jpg",rmats[1]); 
    return 0;
}
