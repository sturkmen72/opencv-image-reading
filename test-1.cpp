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
    imInfo iminfo = imquery(filename0);

    cout << "file name : " << filename0 << endl;
    cout << "page_count : " << iminfo.page_count() << endl;
    cout << "imquery result code : " << names[iminfo.result_code] << endl << endl;

    for (int i = 0; i < iminfo.page_count(); i++)
    {
        pageInfo pageinfo = iminfo.getPageInfo(i);
        cout << "page index : " << i << endl;
        cout << "type : " << pageinfo.type << endl;
        cout << "width : " << pageinfo.width << endl;
        cout << "height : " << pageinfo.height << endl;
        cout << "scalable : " << pageinfo.scalable << endl << endl;
    }

    Mat img;
    cout << "imread result code: " << names[imread(filename0, img, IMREAD_REDUCED_COLOR_2, 2)] << endl << endl;

    if (argc < 3)
        return 0;

    String filename1 = argv[2];
    iminfo = imquery(filename1);

    cout << "file name : " << filename1 << endl;
    cout << "page_count : " << iminfo.page_count() << endl;
    cout << "imquery result code : " << names[iminfo.result_code] << endl << endl;

    for (int i = 0; i < iminfo.page_count(); i++)
    {
        pageInfo pageinfo = iminfo.getPageInfo(i);
        cout << "page index : " << i << endl;
        cout << "type : " << pageinfo.type << endl;
        cout << "width : " << pageinfo.width << endl;
        cout << "height : " << pageinfo.height << endl;
        cout << "scalable : " << pageinfo.scalable << endl << endl;
    }

    pageInfo pageinfo = iminfo.getPageInfo();
    Mat roi = img(Rect(30, 100, pageInfo.width / 2, pageInfo.height / 2));
    cout << "imread result code: " << imread(filename1, roi, IMREAD_REDUCED_COLOR_2) << endl;

    imwrite("multipage-2.jpg", img);

    vector<Mat> images;
    images.push_back(img);
    images.push_back(roi);
    Mat gray;
    cvtColor(roi, gray, COLOR_BGR2GRAY);
    images.push_back(gray);
    imwritemulti("multipage-test.tif", images);

    iminfo = imquery("multipage-test.tif");

    cout << "\n+++++++ here we are reading the file we wrote earlier +++++++++++\n\n";
    cout << "file name : multipage-test.tif" << endl;
    cout << "page_count : " << iminfo.page_count() << endl;
    cout << "imquery result code : " << names[iminfo.result_code] << endl << endl;

    for (int i = 0; i < iminfo.page_count; i++)
    {
        pageInfo pageinfo = iminfo.getPageInfo(i);
        cout << "page index : " << i << endl;
        cout << "type : " << pageinfo.type << endl;
        cout << "width : " << pageinfo.width << endl;
        cout << "height : " << pageinfo.height << endl;
        cout << "scalable : " << pageinfo.scalable << endl << endl;
    }

    imshow("multipage-2.jpg", img);
    waitKey(1000);
    return 0;
}
