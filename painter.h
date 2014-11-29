#include "opencv2/imgproc/imgproc.hpp"
#include "pointsetcalibrator.h"
#include "experiment.h"
#include <iostream>
#include <iomanip>

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

    Mat CreateAnnotation(const Mat& icicle, const Picture& picture, const Experiment* ex, int t0)
    {
	const int annotationSize = 270;
	Size icsz = icicle.size();
	Mat result(icsz.height, icsz.width + annotationSize, CV_8UC3, Scalar(230, 230, 230));

	Rect roi(Point(0, 0), icsz);
/*	cerr << "icicle: " << icicle.size() << endl;
	cerr << "picture: " << picture.GetMat().size() << endl;
	cerr << "result: " << result.size() << endl;
	cerr << "roi: " << roi.size() << endl;
*/	icicle.copyTo(result(roi));


	if (ex != NULL)
	{
	    const State* s = ex->FindNearestState(picture.GetAbsoluteTime_s());
	    if (s != NULL)
	    {
		std::ostringstream temp;
		temp << setw(4) << (s->GetTime() - t0) << ends;
		int x = icsz.width + 20;
		WriteText(result, temp.str(), x, 40);
		WriteText(result, s->GetId(), x, 70);
	    }
	}
	return result;
    }
protected:
    Mat* image;

    void WriteText(Mat& mat, const string& s, int x, int y)
    {
	putText(mat, s, Point(x, y), FONT_HERSHEY_SIMPLEX, 1, Scalar(250,0,0), 3);
    }


};
