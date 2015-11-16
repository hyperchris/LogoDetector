#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ios>
#include <iostream> 
#include <iterator>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <cctype>
#include <vector>

// Usage: $./detect [threshold] [img_dir] [img_num] [res_dir]

#define WSIZE_WIDTH 120 // window size
#define WSIZE_HEIGHT 36

#define BSIZE_WIDTH 12 // 
#define BSIZE_HEIGHT 12

#define BSTRIDE_WIDTH 6
#define BSTRIDE_HEIGHT 6

#define CSIZE_WIDTH 6
#define CSIZE_HEIGHT 6

#define MARGIN 0

using namespace std;
using namespace cv;

// Directory containing positive sample images
static string posSamplesDir = "pos/";
// Directory containing negative sample images
static string negSamplesDir = "neg/";
// Set the file to write the resulting detecting descriptor vector to
static string descriptorVectorFile = "genfiles/descriptorvector.dat";

static const Size trainingPadding = Size(0, 0);
static const Size winStride = Size(8, 8);

static void readDescriptorFromFile(vector<float>& descriptorVector, string fileName) {
    printf("Reading descriptor vector from file '%s'\n", fileName.c_str());
    ifstream ifs(fileName);
    string str;
    int count = 0;
    while (ifs >> str) {
        descriptorVector.push_back(atof(str.c_str()));
        // cout << descriptorVector[descriptorVector.size() - 1] << " ";
        count++;
    }
    cout << endl;
    ifs.close();
}

static void showDetections(const vector<Rect>& found, Mat& imageData) {
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
        rectangle(imageData, r.tl(), r.br(), Scalar(64, 255, 64), 3);
    }
    imshow("HOG custom detection", imageData);
    waitKey(0);
}

Rect getCroppingRec (Mat& img, int x, int y, int width, int height) {
    int x_res = (x - MARGIN < 0) ? 0: (x - MARGIN); // 
    int y_res = (y - MARGIN < 0) ? 0: (y - MARGIN); // 
    int width_res = (x + width + MARGIN > img.cols) ? (img.cols - x_res - 1) : (x + width + MARGIN - x_res - 1);    // 
    int height_res = (y + height + MARGIN > img.rows) ? (img.rows - y_res - 1) : (y + height + MARGIN - y_res - 1); // 
    return Rect(x_res, y_res, width_res, height_res);
}

static void storeResult(const vector<Rect>& found, Mat& imageData, string res_dir, string res_name) {
    if (found.size() < 1)
        return;
    vector<Rect>::const_iterator r = found.begin();
    Rect temp_croppingRec = getCroppingRec(imageData, (int)r->x, (int)r->y, (int)r->width, (int)r->height); // Setup a rectangle (x, y, width, height)
    Mat res = imageData(temp_croppingRec);  // get the cropped image
    imwrite( res_dir + res_name, res);
}

static void detectTest(const HOGDescriptor& hog, double hitThreshold, Mat& imageData, string res_dir, string res_name) {
    vector<Rect> found;
    Size padding(Size(32, 32));
    Size winStride(Size(8, 8));
    hog.detectMultiScale(imageData, found, hitThreshold, winStride, padding);
    while (found.size() > 1) { // increase the thres until only one obj left
        hitThreshold += 0.1;
        printf("try thres %f\n", hitThreshold);
        hog.detectMultiScale(imageData, found, hitThreshold, winStride, padding);
    }
    storeResult(found,imageData, res_dir, res_name); // store the cropped res image
    // showDetections(found, imageData); // display the res image
}

static void detectImages(const HOGDescriptor& hog, const double hitThreshold, string root_dir, int img_num, string res_dir) {
    for (int i = 0; i < img_num; i++) {
        string img_name = "img-" + to_string(i) + ".jpg";
        Mat testImage = imread(root_dir + img_name, 1);
        printf("detecting... %s\n", img_name.c_str());
        detectTest(hog, hitThreshold, testImage, res_dir, "img-" + to_string(i) + "-res.jpg");
    }
    return;
}

int main(int argc, char** argv) {
    std::vector<std::string> args(argc);     
     for (int i = 0; i < argc; ++i){
         args[i] = argv[i];
     }

    HOGDescriptor hog; // Use standard parameters here
    hog.winSize = Size(WSIZE_WIDTH, WSIZE_HEIGHT); // Default training images size as used in paper
    hog.cellSize = Size(CSIZE_WIDTH, CSIZE_HEIGHT); // cell size 
    hog.blockSize = Size(BSIZE_WIDTH, BSIZE_HEIGHT); // blocksize 
    hog.blockStride = Size(BSTRIDE_WIDTH, BSTRIDE_HEIGHT); // blockstride
    vector<float> descriptorVector;
    readDescriptorFromFile(descriptorVector, descriptorVectorFile);
    hog.setSVMDetector(descriptorVector);

    double threshold = stod(args[1]);
    string img_dir = args[2];
    int img_num = stoi(args[3]);
    string res_dir = args[4];

    cout << "INPUT: thres: " << threshold << " img_dir: " << img_dir << " img_num: " << img_num << " res_dir: " << res_dir << endl;

    detectImages(hog, threshold, img_dir, img_num, res_dir); // thres, img_dir, img_num

    printf("Finished\n");
    return EXIT_SUCCESS;
}