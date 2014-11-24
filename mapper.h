#include "opencv2/imgproc/imgproc.hpp"
#include "pointsetcalibrator.h"
#include <iostream>

#pragma once
using namespace cv;

/// Converts original image to millimeters 
class Mapper 
{
public:
    
    
    // Map picture prom pixels to millimeters
    virtual Mat ConvertToMm(
	const Mat& picture,
	const Calibrator& calibration,
	int pxPerMm,
	int x0, int x1,
	int y0, int y1)
	
    {
	
	int dx = x1 - x0;
	int dy = y1 - y0;
	
	Size dstSize(dx*pxPerMm, dy*pxPerMm);

	Mat dst;
	dst.create(dstSize, CV_8UC3);

	Mat map_x, map_y;
	map_x.create(dstSize, CV_32FC1 );
	map_y.create(dstSize, CV_32FC1 );

	PointSetCalibrator inverseMap;
	std::cout << calibration.GetCalibration() << "\n";
	for (int y = 0; y < 640; y += 100) 
	{
	    for (int x = 0; x < 480; x += 100) 
	    {
		double xmm = calibration.UseXMap(y,x);
		double ymm = calibration.UseYMap(y,x);
//		std::cout << "y_px=" << y << " x_px=" << x
//		     << " y_mm=" << ymm << " x_mm=" << xmm << "\n";
		inverseMap.AddXEquation(ymm, xmm, x);
		inverseMap.AddYEquation(ymm, xmm, y);
	    }
	}
	inverseMap.ComputeCalibration();
	std::cout << inverseMap.GetCalibration() << "\n";
	for (int y = 0; y < dstSize.height; y++) 
	{
	    for (int x = 0; x < dstSize.width; x++) 
	    {
		double xmm = (double)x/pxPerMm;
		double ymm = (double)y/pxPerMm;
		double xpx = inverseMap.UseXMap(ymm,xmm);
		map_x.at<double>(y,x) = xpx;
		double ypx = inverseMap.UseYMap(ymm,xmm);
		map_y.at<double>(y,x) = ypx;

		if (x % 30 == 0 && y % 30 == 0)
		std::cout << "y_px=" << ypx << " x_px=" << xpx
		     << " y_mm=" << ymm << " x_mm=" << xmm << "\n";
	    }
	}

	remap(picture, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0, 0) );
	return dst;
    };


};
