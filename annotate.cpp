
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <cmath> 
#include "dirent.h"

using namespace std;
using namespace cv;

const char* colorWindow = "original";
const char* bwWindow = "threshold";

int c0=12;
int c1=12;
int xl=90;
int xr=320;

vector<string> files;
vector<Mat> images;
vector<int> times;
vector<bool> outliers;


vector<bool> accepted;
Mat backgroundMean;
Mat backgroundSd;
Mat transformed;

int trackbarValue = 0;

double transformPixel(int y, int x, uchar r, uchar g, uchar b)
{
    const double coef[] = { 1.0, 0.5, 1.0 };
    const double p[] = { r, g, b };

    double result = 0;
    for (int i = 0; i < 3; i++)
    {
	double p0 = backgroundMean.at<Vec3f>(y, x)[i];
	result += (p[i] - p0) * coef[i];
    }
    return result;
}
// --------------------------------------------------------------------------------

void transformMatrix(const Mat& input, Mat* output)
{
    Size s = input.size();
    *output = Mat(s, CV_8UC1);
    for (int y = 0; y < s.height; y++) {
	for (int x = 0; x < s.width; x++) {
	    Vec3b intensity = input.at<Vec3b>(y, x);
	    uchar blue = intensity.val[0];
	    uchar green = intensity.val[1];
	    uchar red = intensity.val[2];
	    double d = transformPixel(y, x, red, green, blue);
	    d = std::max(0.0, d);
	    d = std::min(255.0, d);
	    output->at<uchar>(y, x) = (uchar)floor(d); 		
	}
    }
}
// --------------------------------------------------------------------------------

Mat* findLastGood()
{
    for (unsigned i = images.size(); i-- > 0; )
    {
	if (accepted[i]) 
	{
	    return &images[i];
	}
    }
    return 0;
}
// --------------------------------------------------------------------------------

int findTimeOfLastGood()
{
    for (unsigned i = images.size(); i-- > 0; )
    {
	if (accepted[i]) 
	{
	    return times[i];
	}
    }
    return 300000;
}
// --------------------------------------------------------------------------------

void saveTimeFunctionForOctave(int y, int x)
{
    ofstream octaveFile;
    octaveFile.open ("oiv.txt");
    
    for (int i = 0; i < images.size(); i++) 
    {
//	cout << files[i] << " " << accepted[i] << endl;
	if (accepted[i]) 
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
    }
    octaveFile.close();
}
// --------------------------------------------------------------------------------

void saveTransformedTimeFunctionForOctave(int y, int x)
{
    ofstream octaveFile;
    octaveFile.open ("oit.txt");
    
    double prevPx = 0;

    for (int i = 0; i < images.size(); i++) 
    {
//	cout << files[i] << " " << accepted[i] << endl;
	if (accepted[i]) 
	{
	    Vec3b intensity = images[i].at<Vec3b>(y, x);
	    uchar blue = intensity.val[0];
	    uchar green = intensity.val[1];
	    uchar red = intensity.val[2];
	    double px = transformPixel(y, x, red, green, blue);	
	    double diff = 0;
	    
	    if (i > 0) 
	    {
		diff = (px-prevPx)/(times[i]-times[i-1]);		
	    }

	    octaveFile << times[i] 
		       << "," << px 
		       << "," << diff * diff
		       << "," << 100* diff * diff + px
		       << endl;

	    prevPx = px;

	}
    }
    octaveFile.close();
}
// --------------------------------------------------------------------------------


void mouseClickCallback(int event, int x, int y, int flags, void* userdata)
{
     if(event == EVENT_LBUTTONDOWN )
     {	 
	 saveTransformedTimeFunctionForOctave(y, x);	 
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
//    cout << files.size() << endl;
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
      Mat wholePicture(imread(fn));
      Mat partial;
      Mat(wholePicture, Range::all(), Range(90,320)).copyTo(partial);
      images.push_back(partial);
      if (images.back().empty()) 
      {
	  cerr << "unreadable image " <<  fn << endl;
      }
      if (i % 10 == 0) cerr << ".";
  }
}
// --------------------------------------------------------------------------------

vector<double> computeImageSums(int height)
{
  vector<double> sums;
  int cnt = files.size();
  for (int i = 0; i < cnt; i++) 
  {
      double s = 0;
      if (!images[i].empty()) 
      {
	  uchar* b = images[i].ptr<uchar>(0);
	  int rows = images[i].rows;
	  if (rows > height)
	  {
	      rows = height;
	  }
	  const int cnt = rows * images[i].cols * images[i].elemSize();
	  for (int j = 0; j < cnt; j++)
	  {
	      s += *b++;
	  }
      }
      sums.push_back(s);
//      cout << files[i] << ": " << s << endl;
  }
  return sums;
}
// --------------------------------------------------------------------------------

void computeImageMeanAndDeviation(const vector<double>& sums, double* mean, double* sd)
{
    *mean = 0;
    *sd = 0;
    
    double sum = 0;
    double sumSq = 0;
    int c = 0;

    int cnt = files.size();
    for (int i = 0; i < cnt; i++) 
    {
	if (!images[i].empty()) 
	{
	    c++;
	    sum += sums[i];
	    sumSq += sums[i]*sums[i];
	}
    }
    
    if (c > 1)
    {
	*mean = sum / c;
	*sd = sqrt((sumSq - c * *mean * *mean)/(c-1));
    }


}
// --------------------------------------------------------------------------------

void computeOutliers(double tolerance, int height) 
{
    double mean = 0, sd = 0;
    int remain = 0;
    vector<double> sums = computeImageSums(height);

    int cnt = files.size();
    while (accepted.size() < cnt) 
    {
	accepted.push_back(true);
    }

    computeImageMeanAndDeviation(sums, &mean, &sd);
    if (sd != 0) {
	for (int i = 0; i < cnt; i++) 
	{
	    bool ok = false;
	    if (!images[i].empty()) 
	    {
		double delta = (sums[i] -  mean)/(sd*tolerance);
		ok = (delta >= -1 && delta <= 1);
//		cout << files[i] << ": " << delta << " ok " << ok << endl;
	    }
	    if (!ok) 
	    {
		accepted[i] = false;
	    }
	    if (ok) remain++;
	}
    }
    cout << "mean " << mean << " sd " << sd 
	 << " remain " << remain << " of " << files.size() << endl;
}
// --------------------------------------------------------------------------------

void computeBackground()
{
    Mat* p = findLastGood();
    if (p == NULL) 
    {
	return;
    }
    int w = p->size().width;
    int h = p->size().height;
   
    Mat sum(h, w, CV_32FC3, Scalar(0.0, 0.0, 0.0));
    int cnt = 0;
    for (int i = 0; i < c0; i++) 
    {
	if (accepted[i]) 
	{
	    ++cnt;
	    for (int y = 0; y < h; y++) 
	    {
		for (int x = 0; x < w; x++) 
		{
		    for (int c = 0; c < 3; c++)
		    {
			sum.at<Vec3f>(y, x)[c] += images[i].at<Vec3b>(y, x)[c];
		    }
		}
	    }
	}
    }

    if (cnt == 0)
    {
	return;
    }

    backgroundMean = Mat(h, w, CV_32FC3);
    for (int y = 0; y < h; y++) 
    {
	for (int x = 0; x < w; x++) 
	{
	    Vec3f v = sum.at<Vec3f>(y, x);
	    for (int c = 0; c < 3; c++)
	    {
		backgroundMean.at<Vec3f>(y, x)[c] = v[c] / cnt;
	    }
	}
    }

    Mat sumX(h, w, CV_32FC1, Scalar(0.0, 0.0, 0.0));
    Mat sumXSq(h, w, CV_32FC1, Scalar(0.0, 0.0, 0.0));

    for (int i = 0; i < c0; i++) 
    {
	if (accepted[i]) 
	{
	    Mat m;
	    transformMatrix(images[i], &m);
	    for (int y = 0; y < h; y++) 
	    {
		for (int x = 0; x < w; x++) 
		{
		    double val = m.at<float>(y, x);
		    sumX.at<float>(y, x) += val;
		    sumXSq.at<float>(y, x) += val * val;		    
		}
	    }
	}
    }

    ofstream octaveFile;
    octaveFile.open ("osd.txt");
	    
    backgroundSd = Mat(h, w, CV_32FC1);
    for (int y = 0; y < h; y++) 
    {
	for (int x = 0; x < w; x++) 
	{
	    backgroundSd.at<float>(y, x) = sqrt(sumXSq.at<float>(y,x)/cnt);
	    octaveFile << y 
		       << "," << x
		       << "," << sumX.at<float>(y,x)
		       << "," << backgroundSd.at<float>(y,x)
		       << endl;
		
	}
    }

}
// --------------------------------------------------------------------------------

void trackbarCallback(int value)
{
    transformMatrix(images[value], &transformed);
    imshow(bwWindow, transformed);
    imshow(colorWindow, images[value]);
}
// --------------------------------------------------------------------------------


void annotate() 
{
  Mat* lastGood = findLastGood();

  if (lastGood != 0) 
  {

      namedWindow(bwWindow, 1);
      namedWindow(colorWindow, 1);
      setMouseCallback(colorWindow, mouseClickCallback, NULL);

      cvCreateTrackbar("time", colorWindow, &trackbarValue, files.size()-1, *trackbarCallback );
      trackbarCallback(files.size()-1);

      waitKey(0);
  }
}
// --------------------------------------------------------------------------------


void plotGrow()
{
    ofstream octaveFile;
    octaveFile.open ("grow.txt");

    Mat* p = findLastGood();
    if (p == NULL) 
    {
	return;
    }
    int maxT = findTimeOfLastGood() + 1;
    int w = p->size().width;
    int h = p->size().height;

    for (int y = 0; y < h; y++)
    {
	if (y % 10 == 0) cerr << ".";
	for (int x = 0; x < w; x++)
	{

	    double prevPx = 0;
	    int t = maxT;
	    
	    for (int i = 0; i < images.size(); i++) 
	    {
		if (accepted[i]) 
		{
		    Vec3b intensity = images[i].at<Vec3b>(y, x);
		    uchar blue = intensity.val[0];
		    uchar green = intensity.val[1];
		    uchar red = intensity.val[2];
		    double px = transformPixel(y, x, red, green, blue);	
		    double diff = 0;
	    
		    if (i > 0) 
		    {
			diff = (px-prevPx)/(times[i]-times[i-1]);		
		    }
		    double v = 100* diff * diff + px;
		    if (v > 150) 
		    {
			t = times[i] - times[0];
			break;
		    }
		    prevPx = px;
		}
	    }
	    if (x > 0) octaveFile << ",";
	    octaveFile << t;
	}
	octaveFile << endl;
    }
    octaveFile.close();

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
  computeOutliers(1.7, 1000);
  computeOutliers(1.7, 100);
  computeBackground();

  annotate();
//  clickableTimeFunction();
//  plotGrow();
  
  return 0;
}
// --------------------------------------------------------------------------------
