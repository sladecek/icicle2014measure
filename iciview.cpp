
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include "dirent.h"

using namespace std;
using namespace cv;

vector<string> files;
vector<Mat> images;
vector<int> times;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

	  ofstream octaveFile;
	  octaveFile.open ("oiv.txt");

	  for (int i = 0; i < images.size(); i++) 
	  {
	      Vec3b intensity = images[i].at<Vec3b>(y, x);
	      uchar blue = intensity.val[0];
	      uchar green = intensity.val[1];
	      uchar red = intensity.val[2];

	      octaveFile << times[i] 
		   << "," << (int)blue
		   << "," << (int)green
		   << "," << (int)red
		   << endl;
	  }
	  octaveFile.close();
     }
}

int main(int argc, char** argv)
{
  if (argc < 2) 
  {
      cerr << "missing argument" << endl;
      return 2;
  }


  DIR* dirp = opendir(argv[1]);
  struct dirent * dp = NULL;

  while ((dp = readdir(dirp)) != NULL)
  {
      string fn = dp->d_name;
      if (fn == "." || fn == "..") 
      {
	  continue;
      }
      files.push_back(fn);
  }
  (void)closedir(dirp);
  cout << files.size() << endl;
  sort(files.begin(), files.end());

  namedWindow("My Window", 1);
  setMouseCallback("My Window", CallBackFunc, NULL);

  int cnt = files.size();
  if (cnt > 20000) 
  {
      cnt = 20;
  }

  Mat* lastGood = 0;
  int startTime = -1;
  for (int i = 0; i < cnt; i++) 
  {
      // extract base name - contains time
      int pos = files[i].find('.');
      int n = atoi(files[i].substr(0, pos).c_str());
      if (startTime < 0)
      {
	  startTime = n;
      }
      times.push_back(n - startTime);

      string fn = string(argv[1]) + "/" + files[i];
      images.push_back(imread(fn));
      if (images.back().empty()) 
      {
	  cerr << "unreadable image " <<  fn << endl;
      }
      else
      {
	  lastGood = &images.back();
      }
  }
  if (lastGood != 0) 
  {
      imshow("My Window", *lastGood);
  }
  waitKey(0);
  
  return 0;
}

