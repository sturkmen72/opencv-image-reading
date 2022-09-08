#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    if (argc < 2)
        return 0;

    const char* names[] = { "IMREAD_SUCCESS", "IMREAD_FILE_NOT_OPENED",
        "IMREAD_UNKNOWN_FILE_TYPE","IMREAD_READ_HEADER_ERROR", "IMREAD_READ_DATA_ERROR" };

    String filename0 = argv[1];
    imquery iminfo1(filename0);

    cout << "file name : " << filename0 << endl;
    cout << "page_count : " << iminfo1.page_count() << endl;
    cout << "imquery result code : " << names[iminfo1.result_code()] << endl << endl;

    for (int i = 0; i < iminfo1.page_count(); i++)
    {
        cout << "page index : " << i << endl;
        cout << "type : " << iminfo1.type(i) << endl;
        cout << "width : " << iminfo1.width(i) << endl;
        cout << "height : " << iminfo1.height(i) << endl;
        cout << "scalable : " << iminfo1.scalable(i) << endl << endl;
    }

    Mat img;
    cout << "imread result code: " << names[imread(filename0, img, IMREAD_REDUCED_COLOR_2, 2)] << endl << endl;

    if (argc < 3)
        return 0;

    String filename1 = argv[2];
    imquery iminfo2(filename1);

    cout << "file name : " << filename1 << endl;
    cout << "page_count : " << iminfo2.page_count() << endl;
    cout << "imquery result code : " << names[iminfo2.result_code()] << endl << endl;

    for (int i = 0; i < iminfo2.page_count(); i++)
    {
        cout << "page index : " << i << endl;
        cout << "type : " << iminfo2.type(i) << endl;
        cout << "width : " << iminfo2.width(i) << endl;
        cout << "height : " << iminfo2.height(i) << endl;
        cout << "scalable : " << iminfo2.scalable(i) << endl << endl;
    }

    Mat roi = img(Rect(30, 100, iminfo2.width() / 2, iminfo2.height() / 2));
    cout << "imread result code: " << imread(filename1, roi, IMREAD_REDUCED_COLOR_2) << endl;

    imwrite("multipage-2.jpg", img);

    vector<Mat> images;
    images.push_back(img);
    images.push_back(roi);
    Mat gray;
    cvtColor(roi, gray, COLOR_BGR2GRAY);
    images.push_back(gray);
    Mat grayf;
    gray.convertTo(grayf, CV_32F);
    images.push_back(grayf);
    gray.convertTo(grayf, CV_32S);
    images.push_back(grayf);
    imwritemulti("multipage-test.tif", images);

    imquery iminfo3("multipage-test.tif");

    cout << "\n+++++++ here we are reading the file we wrote earlier +++++++++++\n\n";
    cout << "file name : multipage-test.tif" << endl;
    cout << "page_count : " << iminfo3.page_count() << endl;
    cout << "imquery result code : " << names[iminfo3.result_code()] << endl << endl;

    for (int i = 0; i < iminfo3.page_count(); i++)
    {
        cout << "page index : " << i << endl;
        cout << "type : " << iminfo3.type(i) << endl;
        cout << "width : " << iminfo3.width(i) << endl;
        cout << "height : " << iminfo3.height(i) << endl;
        cout << "scalable : " << iminfo3.scalable(i) << endl << endl;
    }

    imshow("multipage-2.jpg", img);
    waitKey(1000);
    
    vector<Mat> mats;

    for (int i = 0; i < 200; i++)
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
    bool ret = imreadmulti("test100.tif", rmats, 199, 1, 0);
    tm.stop();
    cout << "199:" << tm << endl;
    cout << "result of imreadmulti : " << ret << endl;
    imwrite("test199.jpg",rmats[0]); 
    tm.reset();
    tm.start();
    ret = imreadmulti("test100.tif", rmats, 45, 1, 0);
    tm.stop();
    cout << "45:" << tm << endl;
    cout << "result of imreadmulti : " << ret << endl;
    imwrite("test45.jpg",rmats[1]);
    tm.reset();
    tm.start();
    imreadmulti("test100.tif", rmats, 2, 1, 0);
    tm.stop();
    cout << "2:" << tm << endl;
    imwrite("test2.jpg",rmats[2]);
    tm.reset();
    tm.start();
    imreadmulti("test100.tif", rmats, 8, 1, IMREAD_REDUCED_GRAYSCALE_8);
    tm.stop();
    cout << "IMREAD_REDUCED_GRAYSCALE_8 8:" << tm << endl;
    imwrite("test8.jpg",rmats[3]);

    tm.reset();
    tm.start();
    ret = imreadmulti("test100.tiff", rmats, 8, 1, IMREAD_REDUCED_GRAYSCALE_8);
    tm.stop();
    cout << "result of imreadmulti : " << ret << endl;
    return 0;
}
