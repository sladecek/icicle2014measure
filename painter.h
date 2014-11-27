#include "opencv2/imgproc/imgproc.hpp"
#include "pointsetcalibrator.h"
#include <iostream>

#pragma once
using namespace cv;

/// Draws overlayed information over the picture.
class Painter 
{
public:
    
Painter(Mat* image_) :
	image(image_)
	{
	}
    
    virtual ~Painter() 
    {}

    void DrawGrid(int pxPerMm) 
    {
	int h = image->size().height;
	int w = image->size().width;
	for (int y = 0; y < h; y+=10*pxPerMm) 
	{
	    line(*image, Point(0, y), Point(w, y), Scalar(30, 30, 30));
	}
	for (int x = 0; x < w; x+=10*pxPerMm) 
	{
	    line(*image, Point(x, 0), Point(x, h), Scalar(30, 30, 30));
	}
    }

    void DrawOverlayText(const Picture& picture)
    {
    }
protected:
    Mat* image;

};
