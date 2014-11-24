#include "opencv2/core/core.hpp"
using namespace cv;

#pragma once


// Interface for calibrators. Take picture, return geometric transformation
// from pixels to millimeters.
class CalibratorBase
{
public:
    // Get result of calibration - 1x12 matrix [a_0..a_5, b_0..b_5].
    // x' = a_0 + a_1*x + a_2*y + a_3*x^2 + a_4*y^2 + a_5*x*y
    // y' = b_0 + b_1*x + b_2*y + b_3*x^2 + b_4*y^2 + b_5*x*y    
    virtual Mat GetCalibration() const = 0;

    double UseXMap(double y, double x) const
    { return UseMap(y,x,0); }

    double UseYMap(double y, double x) const
    { return UseMap(y,x,6); }

protected:
    double UseMap(double y, double x, int offset) const
    {
	int i = offset;
	const Mat& c = GetCalibration();
	return 
	    c.at<double>(0,i++)
	    + c.at<double>(0,i++) * x
	    + c.at<double>(0,i++) * x
	    + c.at<double>(0,i++) * x*x
	    + c.at<double>(0,i++) * y*y
	    + c.at<double>(0,i++) * x*y;
    }
    
};

// Interface for calibrators. Take picture, return geometric transformation
// from pixels to millimeters.
class Calibrator: public CalibratorBase
{
public:
    
    // Compute calibrat
    virtual void ComputeCalibrationFromPicture(const Mat& picture) = 0;

    // Is the result usable?
    virtual bool IsOk() const = 0;

    
};
