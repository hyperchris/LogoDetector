/*
 * Function: Store the position of ground truth object as well as the object image (in fixed ratio)
 * What will be written into pos file: [img-name],[img_wid],[res_x_left],[res_x_y]\n
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <cctype>

using namespace std;
using namespace cv;

string getFileName(string fname) {
    int startPos = fname.find_last_of("/");
    return fname.substr(startPos + 1, fname.length());
}


int main(int argc, char** argv) {
  // input format: ./marker [image_path] [win_width] [win_height]
  if (argc != 3) {
    cout << "ERROR: wrong argc! Should be: ./gen [image_path] [mask_path]" << endl;
    return -1;
  }

  Mat img = imread(argv[1],1);
  Mat mask_image = imread(argv[2],0);

  Mat newImg(img.rows, img.cols, CV_8UC3, Scalar(0,0,0));

  for (int i = 0; i < img.rows; i++)
    for (int j = 0; j < img.cols; j++) {
      if (mask_image.at<uchar>(j,i) > 0)
        newImg.at<Vec3b>(j,i) = img.at<Vec3b>(j,i);
    }

  imshow("res", newImg);
  waitKey(0);

  return 0;
}