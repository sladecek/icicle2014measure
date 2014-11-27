#include "opencv2/core/core.hpp"
using namespace cv;

#pragma once


// A picture with context.
class Picture
{
public:
    Picture(const string& fileName)
    {
    }

    virtual ~Picture() {};

    void OpenImage()
    {
    }

    bool IsOpen() const
    { return false; }


    Mat GetMat() 
    { return mat; }

    const Mat GetMat() const
    { return mat; }


    void CloseImage()
    {
    }

    bool IsValidImage()
    {
	return true;
    }


    double ComputeTotalIntensity(int height)
    {
	return 0;
    }

protected:
    std::string file;

    int time;

    Mat mat;


/*
    vector<double> ComputeImageSums(int height)
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
*/
    
};

