#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <cmath> 
#include "dirent.h"

using namespace std;
using namespace cv;

vector<Point> points;
vector<Point> vertical;
Mat picture;
const char* windowTitle = "calibrate";

void mouseClickCallback(int event, int x, int y, int flags, void* userdata)
{
     if(event == EVENT_LBUTTONDOWN )
     {	 
	 circle(picture, Point(x,y), 3.0, Scalar( 0, 0, 255 ), 1, 8 );
	 imshow(windowTitle, picture);
	 points.push_back(Point(y, x));
     }

     if(event == EVENT_RBUTTONDOWN )
     {	 
	 circle(picture, Point(x,y), 3.0, Scalar( 255, 0, 0 ), 1, 8 );
	 imshow(windowTitle, picture);
	 vertical.push_back(Point(y, x));
     }


}
// --------------------------------------------------------------------------------



int main(int argc, char** argv)
{
  if (argc < 2) 
  {
      cerr << "missing argument" << endl;
      return 2;
  }
  picture = imread(argv[1]);


  namedWindow(windowTitle, 1);
  setMouseCallback(windowTitle, mouseClickCallback, NULL);
  imshow(windowTitle, picture);
  waitKey(0);

  for (int i = 0; i < points.size(); i++ ) 
  {
      cout << "p" << points[i].y << "," << points[i].x << endl;
  }
  for (int i = 0; i < vertical.size(); i++ ) 
  {
      cout << "v" << vertical[i].y << "," << vertical[i].x << endl;
  }
  
  return 0;
}
// --------------------------------------------------------------------------------
