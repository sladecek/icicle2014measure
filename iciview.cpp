
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include "dirent.h"

using namespace std;
using namespace cv;

vector<string> files;
vector<Mat> images;
vector<int> times;
vector<bool> outliers;
vector<double> sums;

void saveTimeFunctionForOctave(int y, int x)
{
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
// --------------------------------------------------------------------------------

void mouseClickCallback(int event, int x, int y, int flags, void* userdata)
{
     if(event == EVENT_LBUTTONDOWN )
     {
	 
	 saveTimeFunctionForOctave(y, x);	 
     }
}
// --------------------------------------------------------------------------------

void scanDirectory(const string& dir) 
{
    DIR* dirp = opendir(dir.c_str());
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
}
// --------------------------------------------------------------------------------

void loadFiles(const string& dir) 
{
  int cnt = files.size();
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

      string fn = dir + "/" + files[i];
      images.push_back(imread(fn));
      if (images.back().empty()) 
      {
	  cerr << "unreadable image " <<  fn << endl;
      }
      if (i % 10 == 0) cerr << ".";
  }
}
// --------------------------------------------------------------------------------

void computeOutliers() 
{
  int cnt = files.size();
  for (int i = 0; i < cnt; i++) 
  {
      double s = 0;
      if (!images[i].empty()) 
      {
	  uchar* b = images[i].ptr<uchar>(0);
	  const int cnt = images[i].rows * images[i].cols * images[i].elemSize();
	  for (int j = 0; j < cnt; j++)
	  {
	      s += *b++;
	  }
      }
      sums.push_back(s);
      cout << s << endl;
  }
}
// --------------------------------------------------------------------------------

Mat* findLastGood()
{
    for (unsigned i = images.size(); i-- > 0; )
    {
	if (!images[i].empty()) {
	    return &images[i];
	}
    }
    return 0;
}
// --------------------------------------------------------------------------------

int main(int argc, char** argv)
{
  if (argc < 2) 
  {
      cerr << "missing argument" << endl;
      return 2;
  }
  scanDirectory(argv[1]);
  loadFiles(argv[1]);
  computeOutliers();
  Mat* lastGood = findLastGood();
  if (lastGood != 0) 
  {
      const char* windowTitle = "iciview";
      namedWindow(windowTitle, 1);
      setMouseCallback(windowTitle, mouseClickCallback, NULL);
      imshow(windowTitle, *lastGood);
      waitKey(0);
  }
  
  return 0;
}
// --------------------------------------------------------------------------------
