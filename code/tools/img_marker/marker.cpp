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

#define POS_FILE "positions.txt" // the position information will be stored here
#define RES_DIR "pos/" // this dir must exist together w/ the compiled program

Mat org; // image read in  
Mat dst; // cropped image
Mat img; // image to show
Mat tmp; // temporary image for displaying the box

string getFileName(string fname) {
    int startPos = fname.find_last_of("/");
    return fname.substr(startPos + 1, fname.length());
}

static string img_path;
static int win_wid, win_hei;
static float y_ratio;
static ofstream outfile; // output file handler

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
    
    cout << "before write to file" << endl;
    outfile << getFileName(img_path) << ',' << org.cols << ',' << pre_pt.x << ',' << cur_pt.x << endl;
    cout << "after write to file" << endl;
    
  }
}

int main(int argc, char** argv) {
  // input format: ./marker [image_path] [win_width] [win_height]
  if (argc != 4) {
    cout << "ERROR: wrong argc! Should be: ./marker [image_path] [win_width] [win_height]" << endl;
    return -1;
  }
  vector<std::string> args(argc);     
  for (int i = 0; i < argc; ++i) {
      args[i] = argv[i];
  }
  
  img_path = args[1];
  win_wid = stoi(args[2].c_str());
  win_hei = stoi(args[3].c_str());
  y_ratio = (float)win_hei / (float)win_wid;

  string position_file = string(RES_DIR) + string(POS_FILE);
  outfile.open(position_file);
  org = imread(img_path);
  org.copyTo(img);
  org.copyTo(tmp);
  namedWindow("img");// show the original image
  setMouseCallback("img",on_mouse,0); // main function
  imshow("img",img);
  waitKey(0);
  outfile.close();
  return 0;
}