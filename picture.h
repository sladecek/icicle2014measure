#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

#pragma once


// A picture with context.
class Picture
{
public:

Picture()
    :time_s(-1), absoluteTime_s(-1)
    {
	int pos = -1;
    }

// constructror creates but does not read the image
Picture(const string& directory_,  const string& file_)
    :directory(directory_), file(file_), time_s(-1), absoluteTime_s(-1)
    {
      int pos = file.find('.');
      absoluteTime_s = atoi(file.substr(0, pos).c_str());      
    }

    virtual ~Picture() 
    {};

    // Read the image to memory.
    void OpenImage()
    {
      // extract base name - contains time
      string fn = directory + "/" + file;
//      cerr << "reading " << file << endl;
      mat = imread(fn);
    }
    
    bool IsValidImage() const
    { return !mat.empty(); }

    Mat GetMat() 
    { return mat; }

    const Mat GetMat() const
    { return mat; }

    void CloseImage()
    { mat.release(); }

    // Time obtained from file name
    int GetAbsoluteTime_s() const
    { return absoluteTime_s; }

    // Time relative to the first image.
    int GetTime_s() const
    { return time_s; }


    void SetTime_s(int value) 
    { time_s = value; }

    double ComputeTotalIntensity(int height)
    {
	double result = 0;
	if (IsValidImage())
	{
	    uchar* b = mat.ptr<uchar>(0);
	    int rows = mat.rows;
	    if (rows > height)
	    {
		rows = height;
	    }
	    const int cnt = rows * mat.cols * mat.elemSize();
	    for (int j = 0; j < cnt; j++)
	    {
		result += *b++;
	    }
	}
	return result;
    }

    bool operator < (const Picture& other) const
    { return absoluteTime_s < other.absoluteTime_s; }


protected:
    std::string directory;
    std::string file;

    int absoluteTime_s;
    int time_s;

    Mat mat;
    
};

