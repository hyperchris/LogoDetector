/*
 * Function: Store the position of ground truth object as well as the object image (in fixed ratio)
 * What will be written into pos file: [img-name],[img_wid],[res_x_left],[res_x_y]\n
 */

// what's new: 
 // - exclude the area that is smaller than MIN_SIZE
 // - add resize factor to the input parameter

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <cctype>

using namespace std;
using namespace cv;

#define POS_FILE "positions.txt" // the position information will be stored here
#define RES_DIR "pos/" // this dir must exist together w/ the compiled program

#define MIN_SIZE 10 // if the box is smaller than this size, ignore.

Mat org; // image read in  
Mat dst; // cropped image
Mat img; // image to show
Mat tmp; // temporary image for displaying the box

int res_left = 0; // result
int res_right = 0; // result

string getFileName(string fname) {
    int startPos = fname.find_last_of("/");
    return fname.substr(startPos + 1, fname.length());
}

static string img_path;
static int win_wid, win_hei;
static float y_ratio;
static float resize_ratio; 

void on_mouse(int event,int x,int y,int flags,void *ustc) { 
  static Point pre_pt = Point(-1,-1); // initial position
  static Point cur_pt = Point(-1,-1); // mouse position
  static Point box_pt = Point(-1,-1); // bottom-right corner of the box

  char temp[16];
  if (event == CV_EVENT_LBUTTONDOWN) {// Press down the left key
    org.copyTo(img);
    sprintf(temp,"(%d,%d)",x,y);
    pre_pt = Point(x,y);
    putText(img,temp,pre_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255),1,8);// show the coordinate
    circle(img,pre_pt,2,Scalar(255,0,0,0),CV_FILLED,CV_AA,0);// draw the initial point that the mouse pressed
    imshow("img",img);
  }
  else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON)) {// Mouse move w/o key pressed
    img.copyTo(tmp);
    sprintf(temp,"(%d,%d)",x,y);
    cur_pt = Point(x,y);
    putText(tmp,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));
    imshow("img",tmp);
  }
  else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON)) { // Draw the box while moving the mouse
    img.copyTo(tmp);
    sprintf(temp,"(%d,%d)",x,y);
    cur_pt = Point(x,y);
    putText(tmp,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));
    box_pt = Point(x,pre_pt.y+(cur_pt.x-pre_pt.x)*y_ratio);
    rectangle(tmp,pre_pt,box_pt,Scalar(0,255,0,0),1,8,0);
    imshow("img",tmp);
  }
  else if (event == CV_EVENT_LBUTTONUP) { // show the box after depressing left key
    org.copyTo(img);
    sprintf(temp,"(%d,%d)",x,y);
    cur_pt = Point(x,y);
    putText(img,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));
    circle(img,pre_pt,2,Scalar(255,0,0,0),CV_FILLED,CV_AA,0);
    box_pt = Point(x,pre_pt.y+(cur_pt.x-pre_pt.x)*y_ratio);
    rectangle(img,pre_pt,box_pt,Scalar(0,255,0,0),1,8,0); 
    imshow("img",img);
    img.copyTo(tmp);

    int width = abs(pre_pt.x - box_pt.x); // get the wid/hei of cropped area
    int height = abs(pre_pt.y - box_pt.y);
    if (width == 0 || height == 0) {
      printf("Image not stored: width == 0 || height == 0");
      return;
    }
    dst = org(Rect(min(box_pt.x,pre_pt.x), min(box_pt.y,pre_pt.y), width, height));
    Size size(win_wid,win_hei);
    resize(dst,dst,size);
    imwrite(RES_DIR + string("pos_") + getFileName(img_path),dst); // store the cropped image
    res_left = pre_pt.x;
    res_right = cur_pt.x;
  }
}

// append a line to a text file with / endl
void appendFile(string msg, string path) {
  ofstream outfile; // output file handler
  outfile.open(path, ofstream::app);
  outfile << msg << endl;
  outfile.close();
}

int main(int argc, char** argv) {
  // input format: ./marker [image_path] [win_width] [win_height]
  if (argc <= 5) {
    cout << "ERROR: wrong argc! Should be: ./marker [win_width] [win_height] [image_n] .. [image_n]" << endl;
    return -1;
  }
  vector<string> args(argc);     
  for (int i = 0; i < argc; ++i) {
      args[i] = argv[i];
  }
  
  win_wid = stoi(args[1].c_str());
  win_hei = stoi(args[2].c_str());
  y_ratio = (float)win_hei / (float)win_wid;
  resize_ratio = (float)stod(args[3].c_str());

  string position_file = string(RES_DIR) + string(POS_FILE);
  
  for (int i = 4; i < argc; i++) {
    img_path = args[i];
    org = imread(img_path);
    resize(org, org, Size(org.cols / resize_ratio, org.rows / resize_ratio)); // resize the image 
    org.copyTo(img);
    org.copyTo(tmp);
    namedWindow("img"); // show the original image
    setMouseCallback("img",on_mouse,0); // main function
    imshow("img",img);
    waitKey(0);
    if (res_left > 0 && res_right > 0) { // add a line to result 
      string line = getFileName(img_path) + ' ' + to_string(int(org.cols * resize_ratio)) + ",";
      line += to_string(int(res_left * resize_ratio)) + "," + to_string(int(res_right * resize_ratio));
      if (abs(res_left - res_right) < 10) // dismiss the box that is too small.
        appendFile(line, position_file);
    }
    cout << "write into file: position: " << res_left << " " << res_right << endl;
    // reset the detection result to zero
    res_left = 0;
    res_right = 0;
  }
  return 0;
}