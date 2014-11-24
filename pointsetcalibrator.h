#include "calibrator.h"
#include <iostream>

#pragma once

using namespace cv;

/// Calibrates scene by solving overdetermined set of point equations.
class PointSetCalibrator: public CalibratorBase
{
public:
    
    // Constructor
    PointSetCalibrator()
	:
    isOk(false),
	row(0)
    {
	// minimal size, should be larger
	configurationMatrix = Mat::zeros(12, 12, CV_64F); 
	rightHandSideMatrix = Mat::zeros(12, 1, CV_64F); 
    }

    // Add one equation for x coordinate.
    void AddXEquation(double y_px, double x_px, double x_mm)
    {
	AddEquation(0, y_px, x_px, x_mm);
    }

    // Add one equation for y coordinate.
    void AddYEquation(double y_px, double x_px, double y_mm)
    {
	AddEquation(6, y_px, x_px, y_mm);
    }


    // Compute calibration 
    virtual void ComputeCalibration()
    {
	isOk = false;
	if (row >= 12)
	{
	    Mat& A = configurationMatrix;
//	    std::cout << A;
	    result = (A.t()*A).inv()*A.t()*rightHandSideMatrix;		
	    isOk = true;
	}
    };

    // Implement Calibrator interface.
    virtual bool IsOk() const
    { return isOk; }

    // Implement Calibrator interface.
    virtual Mat GetCalibration() const 
    { return result; }

protected:
    bool isOk;
    int row;
    Mat configurationMatrix;
    Mat rightHandSideMatrix;
    Mat result;


    void EnsureRow()
    {
	if (row >= configurationMatrix.size().height)
	{	    
	    Mat lineCm = Mat::zeros(1, 12, CV_64F); 
	    configurationMatrix.push_back(lineCm);   

	    Mat lineRhs = Mat::zeros(1, 1, CV_64F); 
	    rightHandSideMatrix.push_back(lineRhs);   

	}
    }


    void AddEquation(int offset, double y_px, double x_px, double rhs)
    {
	EnsureRow();

	int i = offset;
	configurationMatrix.at<double>(row, i++) = 1;
	configurationMatrix.at<double>(row, i++) = x_px;
	configurationMatrix.at<double>(row, i++) = y_px;
	configurationMatrix.at<double>(row, i++) = x_px*x_px;
	configurationMatrix.at<double>(row, i++) = y_px*y_px;
	configurationMatrix.at<double>(row, i++) = x_px*y_px;;
	rightHandSideMatrix.at<double>(row, 0) = rhs;
	row++;

    }

};
