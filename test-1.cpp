#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat img0 = imread(samples::findFile("C:\\projects\\opencv-image-reading\\source1.png"));
    TickMeter tm;
    Mat inImage, SplicingImage;
    cv::resize(img0, inImage, Size(936, 936)); //Set the size of the input image
    tm.reset();
    tm.start();
    hconcat(inImage, inImage, SplicingImage);
    vconcat(SplicingImage, SplicingImage, SplicingImage);
    Mat mask = 255 * Mat::ones(inImage.rows, inImage.cols, inImage.depth());
    Point centerPt = Point(SplicingImage.cols / 2, SplicingImage.rows / 2);
    seamlessClone(inImage, SplicingImage, mask, centerPt, SplicingImage, NORMAL_CLONE);
    tm.stop();
    imwrite("cloned1.png", SplicingImage);
    cout << tm << endl;

    cv::resize(img0, inImage, Size(937, 937)); //Set the size of the input image
    tm.reset();
    tm.start();
    hconcat(inImage, inImage, SplicingImage);
    vconcat(SplicingImage, SplicingImage, SplicingImage);
    Mat mask2 = 255 * Mat::ones(inImage.rows, inImage.cols, inImage.depth());
    seamlessClone(inImage, SplicingImage, mask2, centerPt, SplicingImage, NORMAL_CLONE);
    tm.stop();
    cout << tm << endl;
    imwrite("cloned2.png", SplicingImage);
    return 0;
}
