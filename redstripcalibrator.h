#pragma once
#include "opencv2/highgui/highgui.hpp"

#include "pointsetcalibrator.h"
#include "redstripsearch.h"
#include <iostream>

using namespace cv;

/// Calibrates scene using two vertical red strips.
class RedStripCalibrator: public Calibrator
{
public:
    
    // Constructor
    RedStripCalibrator(double horizontalDistanceBetweenStrips_mm, 
		       double verticalDistanceBetweenMarks_mm)
	:
    isOk(false),
    horizontalDistanceBetweenStrips_mm(horizontalDistanceBetweenStrips_mm),
    verticalDistanceBetweenMarks_mm(verticalDistanceBetweenMarks_mm)
    {
    }
    
    // Implement Calibrator interface.
    virtual void ComputeCalibrationFromPicture(const Mat& picture)
    {
	Mat logPicture = picture.clone();

	Size s = picture.size();
	RedStripSearch rss(picture);
	const int halfWin = 7;
	const double contrast = 30;

/*	for (int x = 0; x < 300; x++) {
	    Vec3b intensity = picture.at<Vec3b>(100, x);
	    double red = intensity[0];
	    double green = intensity[1];
	    double blue = intensity[2];
	    std::cout << red << "," << green << "," << blue << "\n";
	}
*/



	for (int y = 0; y < s.height; y++)
	{
	    {
		// left strip
		int x = 0;
		bool found = false;
		rss.FindStrip(false, false, y, 0, s.width/2, halfWin, contrast, &x, &found);
		if (found) 
		{
		    psCalibrator.AddXEquation(y, x, 0);
		    circle( logPicture,Point( x, y  ), 1, Scalar( 255, 0, 0 ), -1);
		}
	    }
	    {
		// right strip
		int x = 0;
		bool found = false;
		rss.FindStrip(false, true, y, s.width/2, s.width-1,  halfWin, contrast, &x, &found);
		if (found) 
		{
		    psCalibrator.AddXEquation(y, x, horizontalDistanceBetweenStrips_mm);
		    circle( logPicture,Point( x, y  ), 1, Scalar( 255, 0, 0 ), -1);
		}
	    }
	}
	for (int x = 0; x < s.width; x++)
	{
	    {
		// upper mark
		int y = 0;
		bool found = false;
		rss.FindStrip(true, false, x, 0, 200, halfWin, contrast, &y, &found);
		if (found) 
		{
		    psCalibrator.AddYEquation(y, x, 0);
		    circle( logPicture,Point( x, y  ), 1, Scalar( 255, 0, 0 ), -1);
		}
	    }
	    {
		// lower mark
		int y = 0;
		bool found = false;
		rss.FindStrip(true, true, x, 200, 400,  halfWin, contrast, &y, &found);
		if (found) 
		{
		    psCalibrator.AddYEquation(y, x, verticalDistanceBetweenMarks_mm);
		    circle( logPicture,Point( x, y  ), 1, Scalar( 255, 0, 0 ), -1);
		}
	    }
	}
	isOk = true;
	psCalibrator.ComputeCalibration();

      namedWindow("test", 1);
      imshow("test", logPicture);
//     waitKey(0);

    };

    // Implement Calibrator interface.
    virtual bool IsOk() const
    { return isOk && psCalibrator.IsOk(); }

    // Implement Calibrator interface.
    virtual Mat GetCalibration() const 
    { return psCalibrator.GetCalibration(); }


protected:
    bool isOk;
    double horizontalDistanceBetweenStrips_mm; 
    double verticalDistanceBetweenMarks_mm;
    
    PointSetCalibrator psCalibrator;

};
