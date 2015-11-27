#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <iostream>
#include <iterator>
#include <string>
#include <fstream>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <cctype>
#include <vector>

// Input: $./detect [threshold] [img_path] [dscriptor_path] [store_img] [res_dir] [win_parameters]
// Output: 
//          * RESULT: empty
//          * RESULT: [left_pos]/[right_pos]/[img_cols]/[thres]

#define MARGIN 0

#define RESULT_HEADER "RESULT: "
#define SPLITTER "/"

#define UPPER 0.0 // upper 15% will be ignored // modified for large images
#define LOWER 0.33 // lower 1/3 will be ignored

#define ALPHA 1.2 // factor that the contrast is magnified
#define BETA 0 // factor that the brightness is increased

using namespace std;
using namespace cv;

static string res_dir = "res/";

string getFileName(string fname) {
    int startPos = fname.find_last_of("/");
    return fname.substr(startPos + 1, fname.length());
}

Rect getCroppingRec(Mat& img, int x, int y, int width, int height) {
    int x_res = (x - MARGIN < 0) ? 0: (x - MARGIN); // 
    int y_res = (y - MARGIN < 0) ? 0: (y - MARGIN); // 
    int width_res = (x + width + MARGIN > img.cols) ? (img.cols - x_res - 1) : (x + width + MARGIN - x_res - 1);    // 
    int height_res = (y + height + MARGIN > img.rows) ? (img.rows - y_res - 1) : (y + height + MARGIN - y_res - 1); // 
    return Rect(x_res, y_res, width_res, height_res);
}

static void readDescriptorFromFile(vector<float>& descriptorVector, const string fileName) {
    ifstream ifs(fileName.c_str());
    string str;
    int count = 0;
    while (ifs >> str) {
        descriptorVector.push_back(atof(str.c_str()));
        count++;
    }
    cout << endl;
    printf("reading vector success\n");
    ifs.close();
}

static void storeDetections(const vector<Rect>& found, Mat& imageData, string res_name) {
    vector<Rect> found_filtered;
    size_t i, j;
    for (i = 0; i < found.size(); ++i) {
        Rect r = found[i];
        for (j = 0; j < found.size(); ++j)
            if (j != i && (r & found[j]) == r)
                break;
        if (j == found.size())
            found_filtered.push_back(r);
    }
    for (i = 0; i < found_filtered.size(); i++) {
        Rect r = found_filtered[i];
        int offset_y = imageData.rows * UPPER; // offset added
        rectangle(imageData, Point(r.tl().x, r.tl().y + offset_y), 
            Point(r.br().x, r.br().y + offset_y), Scalar(64, 255, 64), 3);
    }
    // imshow("HOG custom detection", imageData);
    imwrite(res_dir + res_name, imageData);
}

static void detectImages(const HOGDescriptor& hog, const double threshold, string img_path, int store_img, Size winStride) {
    printf("detecting... %s\n", img_path.c_str());
    Mat imageData_origin = imread(img_path, 1);
    Mat imageData = Mat::zeros(imageData_origin.size(), imageData_origin.type());
    for(int y = 0; y < imageData.rows; y++) { 
        for( int x = 0; x < imageData.cols; x++ ) { 
            for( int c = 0; c < 3; c++ ) {
                imageData.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(ALPHA * (imageData_origin.at<Vec3b>(y,x)[c]) + BETA);
             }
        }
    }
    vector<Rect> found;
    Size padding(Size(0, 0));
    double hitThreshold = threshold;
    // ignore the upper and lower part
    Rect temp_croppingRec = getCroppingRec(imageData, 0, (int)((float)imageData.rows * UPPER), 
                            imageData.cols, (int)((float)imageData.rows * (1.0 - UPPER - LOWER))); // Setup a rectangle (x, y, width, height)
    Mat imageData_cropped = imageData(temp_croppingRec);  // get the cropped image
    hog.detectMultiScale(imageData_cropped, found, hitThreshold, winStride, padding);
    while (found.size() > 1) { // increase the thres until only one obj left
        hitThreshold += 0.1;
        printf("try thres %f\n", hitThreshold);
        hog.detectMultiScale(imageData_cropped, found, hitThreshold, winStride, padding);
    }
    if (found.size() != 1)
        cout << RESULT_HEADER << " empty" << endl;
    else {
        cout << RESULT_HEADER << (found[0].x)
            << SPLITTER << (found[0].x + found[0].width - 1)
            << SPLITTER << imageData.cols 
            << SPLITTER << hitThreshold << endl; 
        if (store_img) {
            storeDetections(found, imageData, getFileName(img_path)); // display the res image
        }
    }
}

int main(int argc, char** argv) {
    vector<std::string> args(argc);     
    for (int i = 0; i < argc; ++i){
        args[i] = argv[i];
    }

    double threshold = atof(args[1].c_str());
    cout << args[1] << " threshold: " << threshold << endl;
    string img_path = args[2];
    string descriptorVectorFile = args[3];
    int store_img = stoi(args[4].c_str());
    res_dir = args[5];

    int WSIZE_WIDTH = stoi(args[6].c_str()); // window size
    int WSIZE_HEIGHT = stoi(args[7].c_str());

    int BSIZE_WIDTH = stoi(args[8].c_str()); // block size
    int BSIZE_HEIGHT = stoi(args[9].c_str());

    int BSTRIDE_WIDTH = stoi(args[10].c_str()); // block stride
    int BSTRIDE_HEIGHT = stoi(args[11].c_str());

    int CSIZE_WIDTH = stoi(args[12].c_str()); // cell size
    int CSIZE_HEIGHT = stoi(args[13].c_str());

    HOGDescriptor hog; // Use standard parameters here
    hog.winSize = Size(WSIZE_WIDTH, WSIZE_HEIGHT); // Default training images size as used in paper
    hog.cellSize = Size(CSIZE_WIDTH, CSIZE_HEIGHT); // cell size 
    hog.blockSize = Size(BSIZE_WIDTH, BSIZE_HEIGHT); // blocksize 
    hog.blockStride = Size(BSTRIDE_WIDTH, BSTRIDE_HEIGHT); // blockstride
    vector<float> descriptorVector;
    readDescriptorFromFile(descriptorVector, descriptorVectorFile);
    hog.setSVMDetector(descriptorVector);

    // hog, thres, img_path, store_img, win_stride_size (same as cell size)
    detectImages(hog, threshold, img_path, store_img, Size(CSIZE_WIDTH, CSIZE_HEIGHT)); 

    printf("Finished\n");
    return EXIT_SUCCESS;
}
