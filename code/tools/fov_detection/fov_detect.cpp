
/* 
 * Author: Xiaochen Liu
 * Time: 2014-03-05
 * Function: find the eyesight in an given Google map image
 */

#define NUM_DST 360
#define ANGLE_RANGE 45
#define IMG_EDGE 620
#define IMG_EDGE_HALF IMG_EDGE / 2

/* for canny */
#define CANNY_LOW_THRES 25
#define CANNY_RATIO 3
#define CANNY_KERN_SIZE 3

/* for math */
#define PI 3.14159265
#define ANGLE_UNIT PI / 180.0 

/* for color matching */
#define B1 226
#define G1 240
#define R1 244

#define B2 236
#define G2 243
#define R2 244

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;
 
int dst_point[4][NUM_DST/4][2];	// array storing dest points
int temp_xy[2];

/* help func */
void help() {
	cout << "Usage:\n" << 
		"input image must be square-shaped!\n" << 
	 	"./houghlines <image_name>, Default is pic1.jpg\n" << endl;
}

/* do edge detection */
Mat edgeDetect (Mat src) {
	Mat edge_res, final_res;
	Canny(src, edge_res, CANNY_LOW_THRES, CANNY_LOW_THRES * CANNY_RATIO, CANNY_KERN_SIZE); // canny detection
 
	vector<Vec4i> lines;
	HoughLinesP(edge_res, lines, 1, CV_PI/180, 50, 50, 10 ); // hough detection (must based on the canny result)
	
	cvtColor(edge_res, final_res, CV_GRAY2BGR);
	for( size_t i = 0; i < lines.size(); i++ ) { // draw lines of both canny and hough
		Vec4i l = lines[i];
		line(final_res, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, CV_AA);	
	}
	
	return final_res;
}

void findBlockPoint (bool positive, bool horizonal, Mat src, double angle_tan) {
	int pos_x = 0;
	int pos_y = 0;
	int pos_x_next = 0;
	int pos_y_next = 0;
	
	int change_unit;
	if (positive)
		change_unit = 1;
	else
		change_unit = -1;
		
	double len_a = 0;
	double len_a_next = 0;
	double len_b = 0;
	double len_b_next = 0;
	
	while ((int)len_a_next < IMG_EDGE_HALF && (int)len_a_next > -IMG_EDGE_HALF) {
		len_a += change_unit;
		len_a_next = len_a + change_unit;
		len_b = len_a * angle_tan;
		len_b_next = (len_a + change_unit) * angle_tan;
		
		if ((int)len_b_next > IMG_EDGE_HALF || (int)len_b_next < -IMG_EDGE_HALF)
			break;
			
		if (horizonal) {
			pos_x = len_a + IMG_EDGE_HALF;
			pos_y = len_b + IMG_EDGE_HALF;
			pos_x_next = pos_x + change_unit;
			pos_y_next = len_b_next + IMG_EDGE_HALF;
		} else {
			pos_x = len_b + IMG_EDGE_HALF;
			pos_y = len_a + IMG_EDGE_HALF;
			pos_x_next = len_b_next + IMG_EDGE_HALF;
			pos_y_next = pos_y + change_unit;
		}
		
		Vec3b intensity = src.at<Vec3b>(pos_y, pos_x);
		Vec3b intensity_next = src.at<Vec3b>(pos_y_next, pos_x_next);
		bool match1 = 	(int)intensity.val[0] == B1 && (int)intensity_next.val[0] == B1 &&
				(int)intensity.val[1] == G1 && (int)intensity_next.val[1] == G1 &&
				(int)intensity.val[2] == R1 && (int)intensity_next.val[2] == R1;
				
		bool match2 = 	(int)intensity.val[0] == B2 && (int)intensity_next.val[0] == B2 &&
				(int)intensity.val[1] == G2 && (int)intensity_next.val[1] == G2 &&
				(int)intensity.val[2] == R2 && (int)intensity_next.val[2] == R2;
		if (match1 || match2)
			break;
	}
	
	temp_xy[0] = pos_x;
	temp_xy[1] = pos_y;
}

/* find the lines of eyesight */
Mat eyesightDetect (Mat src) {
	Mat dst = src;
	
	for (double i = -ANGLE_RANGE; i < ANGLE_RANGE; i += 1.0) {
		double angle_tan = tan(i * ANGLE_UNIT);
		
		findBlockPoint(true, true, src, angle_tan);
		dst_point[0][(int)i + ANGLE_RANGE][0] = temp_xy[0];
		dst_point[0][(int)i + ANGLE_RANGE][1] = temp_xy[1];
		
		findBlockPoint(false, true, src, angle_tan);
		dst_point[1][(int)i + ANGLE_RANGE][0] = temp_xy[0];
		dst_point[1][(int)i + ANGLE_RANGE][1] = temp_xy[1];
		
		findBlockPoint(true, false, src, angle_tan);
		dst_point[2][(int)i + ANGLE_RANGE][0] = temp_xy[0];
		dst_point[2][(int)i + ANGLE_RANGE][1] = temp_xy[1];
		
		findBlockPoint(false, false, src, angle_tan);
		dst_point[3][(int)i + ANGLE_RANGE][0] = temp_xy[0];
		dst_point[3][(int)i + ANGLE_RANGE][1] = temp_xy[1];
	}
	
	for (int i = 0; i < 4; i++) // show the eyesight lines
		for (int j = 0; j < NUM_DST / 4; j++)
			line(dst, Point(IMG_EDGE_HALF, IMG_EDGE_HALF), Point(dst_point[i][j][0], dst_point[i][j][1]), Scalar(255,150,150), 1, CV_AA);
	
	circle(dst, Point(IMG_EDGE_HALF, IMG_EDGE_HALF), 4, Scalar(0,0,255), 3, 8, 0); // show the view point
	
	return dst;
}

/* main func */
int main(int argc, char** argv) {

	/* read in image */
	const char* filename = argc >= 2 ? argv[1] : "map.png";		
	Mat src = imread(filename);
	if(src.empty()) {
		help();
		cout << "can not open " << filename << endl;
		return -1;
	}

	// imshow("detected lines", edgeDetect(src)); // result of canny and hough
	imshow("eyesight result", eyesightDetect(src)); // print the eyesight result

	waitKey();

	return 0;
}
