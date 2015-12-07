/* 
 * Author: Xiaochen Liu
 * Time: 2015/12/05
 * Function: find the eyesight in an given Google map image
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>

#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

#define NUM_DST 360
#define IMG_EDGE 638 // 2px is dismissed to give margin for boundary
#define IMG_EDGE_HALF (IMG_EDGE / 2)

/* for math */
#define PI 3.14159265
#define ANGLE_UNIT (PI / 180.0) 

/* for color matching */
#define B1 226
#define G1 240
#define R1 244

#define B2 236
#define G2 243
#define R2 244

/* for output format */
#define RESULT_TRUE "True" // output this before every edge point
 
int temp_xy[2];

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
void eyesightDetect (Mat & src, vector<Point> & edge_pts, int bearing, int aov) {
	Mat dst = src;

	// get the range of fov (up is 0") 
	// the range goes clockwise
	double range_start = (bearing < aov / 2) ? (360 + bearing - aov / 2) : (bearing - aov / 2);
	double range_end = (bearing + aov / 2) % 360;
	
	for (int i = range_start; i < range_end; i = (i + 1) % 360) { // go from range_start to end
		int angle_phase = 45 - (i + 45) % 90;
		double angle_tan = tan((double)angle_phase * ANGLE_UNIT);
		bool pos; // positve = true when (up, right) 
		bool hor; // horizonal = true when (left, right)
		switch((i + 45) % 360 / 90) { // check which phase does the i in
			case 0: // up
				pos = true; hor = false; break;
			case 1: // right
				pos = true; hor = true; break;
			case 2: // down
				pos = false; hor = false; break;
			case 3: // left
				pos = false; hor = true; break;
			default:
				cout << "Error angle! not in 360: " << i << endl;
				break;
		}

		findBlockPoint(pos, hor, src, angle_tan); // up right
		cout << i << ": " << temp_xy[0] << "," << temp_xy[1] << endl;
		edge_pts.push_back(Point(temp_xy[0], temp_xy[1]));  // add the result 
	}
}

/* main func */
int main(int argc, char** argv) {

	// input format: ./detect_fov [img_path] [bearing_angle] [angle_of_view] [dst_x] [dst_y]
	// note: dst x y should be pixel number instead of meters
	if (argc != 6) {
		cout << "WRONG ARGC! should be:\n" << 
		"./detect_fov [img_path] [bearing_angle] [angle_of_view] [dst_x] [dst_y]" << endl;
		return -1;
	}
	vector<string> args(argc);     
    for (int i = 0; i < argc; ++i) {
        args[i] = argv[i];
    }
	Mat img = imread(args[1], 1);// read in src map
	int bearing = stoi(args[2].c_str()); // bearing
	int aov = stoi(args[3].c_str()); // angle of view
	int dst_x = stoi(args[4].c_str()); // distance to the triangulation result - x value
	int dst_y = stoi(args[5].c_str()); // y value

	// show the input parameters
    cout << "input to detect_fov: " << args[1] << " " << bearing << " " << aov << " " << dst_x << " " << dst_y << endl;

    vector<Point> edge_pts;
    eyesightDetect(img, edge_pts, bearing, aov); // get the eyesight result

    /* draw FoV */
    // start from original point to first edge pt
	line(img, Point(IMG_EDGE_HALF, IMG_EDGE_HALF), edge_pts[0], Scalar(255,0,0), 2, 8, 0);
	// linke each edge pts
	for (int i = 0; i < edge_pts.size() - 1; i++)
		line(img, edge_pts[i], edge_pts[i + 1], Scalar(255,0,0), 2, 8, 0);
	// finally link to the original point
	line(img, Point(IMG_EDGE_HALF, IMG_EDGE_HALF), edge_pts[edge_pts.size() - 1], Scalar(255,0,0), 2, 8, 0);
	/* draw t_p */
	circle(img, Point(dst_x, dst_y), 4, Scalar(0,0,255), 4, 8, 0);
	/* draw vp */
	circle(img, Point(IMG_EDGE_HALF, IMG_EDGE_HALF), 4, Scalar(0,0,0), 4, 8, 0);

	imwrite(args[1], img);
	/*
    if (testInside(edge_pts, dst_x, dst_y)) // test if the t_p is within FoV
    	cout << RESULT_TRUE << endl;
	*/
	return 0;
}
