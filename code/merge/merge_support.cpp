// merge_support.cpp

#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

#define DST_WID 1920
#define DST_HEI 960

bool exists_test (const string& fname) {
  struct stat buffer;   
  return (stat (fname.c_str(), &buffer) == 0); 
}

int main(int argc, char** argv) {
	// input format: ./merge_support [src_path] [src_pos_x] [src_pos_y] [dst_path]
	if (argc != 5) {
		cout << "WRONG ARGC! should be:\n" << 
		"./merge_support [src_path] [src_pos_x] [src_pos_y] [dst_path]" << endl;
		return -1;
	}
	vector<string> args(argc);     
    for (int i = 0; i < argc; ++i) {
        args[i] = argv[i];
    }
    cout << argv[1] << endl;
	Mat src = imread(args[1], 1);// read in src
	int src_pos_x = stoi(args[2].c_str());
	int src_pos_y = stoi(args[3].c_str());
	Mat dst;
	if (exists_test(args[4])) // if dst alread there
		dst = imread(args[4], 1);
	else // else create an empty one
		dst = Mat(DST_HEI, DST_WID, CV_8UC3, Scalar(0,0,0));

	// paste src to dst and save
	src.copyTo(dst(Rect(src_pos_x, src_pos_y, src.cols, src.rows)));
	imwrite(args[4], dst);

	return 0;
}