#include "opencv2/imgproc/imgproc.hpp"
#include "pointsetcalibrator.h"
#include <iostream>

#pragma once
using namespace cv;

/// Converts color space image from RGB to one dimension for analysis.
class ColorSpaceConvertor
{
public:
    
    ColorSpaceConvertor()
    {};

    void CalibrateBackground(vector<Picture>& images)
    {
	int h = 0;
	int w = 0;
	bool allocated = false;
	Mat sum;
	int cnt = images.size();
	for (int i = 0; i < cnt; i++) 
	{
	    Picture& pic = images[i];
	    pic.OpenImage();
	    Mat image = pic.GetMat();
	    
	    if (!allocated) 
	    {
		w = image.size().width;
		h = image.size().height;
   
		sum = Mat(h, w, CV_32FC3, Scalar(0.0, 0.0, 0.0));
		allocated = true;
	    }
	    for (int y = 0; y < h; y++) 
	    {
		for (int x = 0; x < w; x++) 
		{
		    for (int c = 0; c < 3; c++)
		    {
			sum.at<Vec3f>(y, x)[c] += images[i].GetMat().at<Vec3b>(y, x)[c];
		    }
		}
	    }
	}
    

	if (!allocated)
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

	for (int i = 0; i < cnt; i++) 
	{
	    Picture& pic = images[i];
	    Mat image = pic.GetMat();
	    Mat m = transformMatrix(image);
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
	
	for (int i = 0; i < cnt; i++) 
	{
	    images[i].CloseImage();
	}
	
    }
// --------------------------------------------------------------------------------


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

    Mat transformMatrix(const Mat& input)
    {
	Size s = input.size();

	Mat output(s, CV_8UC3);
	for (int y = 0; y < s.height; y++) {
	    for (int x = 0; x < s.width; x++) {
		Vec3b intensity = input.at<Vec3b>(y, x);
		uchar blue = intensity.val[0];
		uchar green = intensity.val[1];
		uchar red = intensity.val[2];
		double d = transformPixel(y, x, red, green, blue);
		d = std::max(0.0, d);
		d = std::min(255.0, d);
		uchar v = (uchar)floor(d); 		
		for (int c = 0; c < 3; c++) 
		{
		    output.at<Vec3b>(y, x)[c] = v;
		}
	    }
	}
	return output;
    }
// --------------------------------------------------------------------------------


protected:

Mat backgroundMean;
Mat backgroundSd;


};
