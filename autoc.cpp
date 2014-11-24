#include "opencv2/highgui/highgui.hpp"
#include "redstripcalibrator.h"
#include "mapper.h"
#include <iostream>

using namespace std;
using namespace cv;


Mat picture;

int main(int argc, char** argv)
{
  if (argc < 2) 
  {
      cerr << "missing argument" << endl;
      return 2;
  }
  
  const double horizontalDistanceBetweenStrips_mm = 86;
  const double verticalDistanceBetweenMarks_mm = 94;
  RedStripCalibrator c(
      horizontalDistanceBetweenStrips_mm, verticalDistanceBetweenMarks_mm);
  picture = imread(argv[1]);
  c.ComputeCalibrationFromPicture(picture);
  if (c.IsOk()) 
  {
      cout << c.GetCalibration() << endl;
      Mapper m;
      Mat mappedPicture = m.ConvertToMm(picture, c, 7, 10, 70, 0, 150);
      namedWindow("ou", 1);
      imshow("ou", mappedPicture);
      waitKey(0);

  }
  else
  {
      cout << "invalid" << endl;
  }
  
  
  return 0;
}
// --------------------------------------------------------------------------------
