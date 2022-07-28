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
    // Create mat with alpha channel
    Mat mat(480, 640, CV_8UC4);
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
            imwrite(format("PNG_STRATEGY_%d_PNG_COMPRESSION_%d.png", j, i), mat, compression_params);
        }
    imwrite("PNG_SAVED_DEFAULT.png", mat);
    return 0;
}

int main()
{
    pngWritingParamsTest();

    vector<String> filenames;
    String folder = "C:/projects/opencv-image-reading/*.png";
    glob(folder, filenames);

    for (size_t i = 0; i < filenames.size(); i++)
    {
        Mat src = imread(filenames[i], IMREAD_UNCHANGED);

        if (src.empty())
            std::cout << filenames[i] << "--------- Could not read the image" << std::endl;
        else
        {
            std::cout << filenames[i] << std::endl;
        }

    }
    return 0;
}