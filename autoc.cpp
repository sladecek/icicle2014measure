#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <cmath> 
#include "dirent.h"

using namespace std;
using namespace cv;


Mat picture;

void saveForOctave()
{
    ofstream octaveFile;
    octaveFile.open ("cal2.txt");

    int w = picture.size().width;
    int h = picture.size().height;

    for (int y = 0; y < h; y++)
    {
	for (int x = 0; x < w; x++)
	{
	    Vec3b intensity = picture.at<Vec3b>(y, x);
	    uchar blue = intensity.val[0];
	    uchar green = intensity.val[1];
	    uchar red = intensity.val[2];
	    if (x > 0) octaveFile << ",";
	    octaveFile << (double)red-(double)green;
	}
	octaveFile << endl;
    }
    octaveFile.close();

}
// -----------------------------------------------------------------------------

int main(int argc, char** argv)
{
  if (argc < 2) 
  {
      cerr << "missing argument" << endl;
      return 2;
  }
  picture = imread(argv[1]);
  saveForOctave();
  
  return 0;
}
// --------------------------------------------------------------------------------
