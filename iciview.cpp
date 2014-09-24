
#include <stdio.h>
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)

#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O

using namespace cv;

/// Global Variables
const int alpha_slider_max = 100;
int alpha_slider;
double alpha;
double beta;

/// Matrices to store images
Mat image;


// Implement mouse callback
void my_mouse_callback( int event, int x, int y, int flags, void* param ){


	switch( event ){

		case CV_EVENT_LBUTTONDOWN:
		  printf("%d %d\n", y, x);
			break;

	}
}


int main( int argc, char** argv )
{
 /// Read image ( same size, same type )
 image = imread("test.jpg");
 if( !image.data ) { printf("Error loading image \n"); return -1; }

const char* name = "Box Example";


	cvNamedWindow( name , WINDOW_AUTOSIZE );
    imshow("Display Image", image);
	// Set up the callback
    cvSetMouseCallback( name, my_mouse_callback, NULL);

    waitKey(0);


 /// Wait until user press some key
 waitKey(0);
 return 0;
}
