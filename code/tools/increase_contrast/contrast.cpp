#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace cv;

double alpha; /**< Simple contrast control */
int beta;  /**< Simple brightness control */

int main( int argc, char** argv )
{
 /// Read image given by user
 Mat original_image = imread( argv[1] );
 Mat new_original_image = Mat::zeros( original_image.size(), original_image.type() );
 Mat image;
 cvtColor(original_image, image, CV_BGR2GRAY);
 Mat new_image = Mat::zeros( image.size(), image.type() );

 /// Initialize values
 std::cout<<" Basic Linear Transforms "<<std::endl;
 std::cout<<"-------------------------"<<std::endl;
 std::cout<<"* Enter the alpha value [1.0-3.0]: ";std::cin>>alpha;
 std::cout<<"* Enter the beta value [0-100]: "; std::cin>>beta;

 /// Do the operation new_image(i,j) = alpha*image(i,j) + beta
 for( int y = 0; y < image.rows; y++ )
    { for( int x = 0; x < image.cols; x++ )
         { for( int c = 0; c < 3; c++ )
              {
      new_image.at<Vec3b>(y,x)[c] =
         saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );

      new_original_image.at<Vec3b>(y,x)[c] =
         saturate_cast<uchar>( alpha*( original_image.at<Vec3b>(y,x)[c] ) + beta );
             }
    }
    }

 Mat nog_image;
 cvtColor(new_original_image, nog_image, CV_BGR2GRAY);

 /// Create Windows
 namedWindow("Original Image", 1);
 namedWindow("New Image", 1);

 /// Show stuff
 imshow("Original Image", nog_image);
 imshow("New Image", new_image);

 /// Wait until user press some key
 waitKey();
 return 0;
}
