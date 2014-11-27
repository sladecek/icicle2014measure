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
	const int annotationSize = 300;
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
		int x = icsz.width + 20;
		int y = 60;
		WriteInt(result, "t=", (s->GetTime() - t0), x, annotationSize-40, &y, 2);
		WriteText(result, "id=", ex->GetId(), x, annotationSize-40, &y, 1);
		WriteInt(result, "ix=", ex->GetIx(), x, annotationSize-40, &y, 1);
		WriteText(result, "prof=", s->GetProfile(), x, annotationSize-40, &y, 1);
		y+=10;
		WriteText(result, "", s->GetStringPhase(), x, annotationSize-40, &y, 1);
		y+=20;
		WriteDouble(result, "Tc=", s->GetTc(), x, annotationSize-40, &y, 1);
		WriteDouble(result, "Td=", s->GetTd(), x, annotationSize-40, &y, 1);
		WriteDouble(result, "Tf=", s->GetTf(), x, annotationSize-40, &y, 1);
		WriteDouble(result, "Tb=", s->GetTb(), x, annotationSize-40, &y, 1);
		WriteDouble(result, "Tw=", s->GetTw(), x, annotationSize-40, &y, 1);
		WriteDouble(result, "To=", s->GetTo(), x, annotationSize-40, &y, 1);
		WriteDouble(result, "Tp=", s->GetTp(), x, annotationSize-40, &y, 1);
		y+=20;
		WriteText(result, "fr", s->GetFr(), x, annotationSize-40, &y, 1);
		WriteDouble(result, "", s->GetFt(), x, annotationSize-40, &y, 1);
		WriteText(result, "hr", s->GetHr(), x, annotationSize-40, &y, 1);
		WriteDouble(result, "", s->GetHt(), x, annotationSize-40, &y, 1);
	    }
	}
	return result;
    }
protected:
    Mat* image;

    void WriteText(Mat& mat, const string& label, const string& text, int x, int width, int* y, double fontScale)
    {

	int fontFace = CV_FONT_HERSHEY_SIMPLEX;
	int thickness = 3;

	int baseline=0;

	Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);
	baseline += thickness;

	Point labelOrg(x, *y);
	Point textOrg(x - textSize.width + width, *y);

	const double labelFontScale = 1;
	putText(mat, label, labelOrg, fontFace, labelFontScale,
		Scalar::all(0), thickness, 8);

	putText(mat, text, textOrg, fontFace, fontScale,
		Scalar::all(0), thickness, 8);

	*y += textSize.height + 5;



    }

    void WriteInt(Mat& mat, const string& label, int v, int x, int width, int* y, double fontScale)
    {
	std::ostringstream temp;
	temp << v << ends;
	WriteText(mat, label, temp.str(), x, width, y, fontScale);
    }

    void WriteDouble(Mat& mat, const string& label, double v, int x, int width, int* y, double fontScale)
    {
	string txt;
	
	if (!isnan(v)) 
	{
	    std::ostringstream temp;
	    temp << setw(7) << fixed << setprecision( 2 ) << v << ends;
	    txt = temp.str();
	}
	WriteText(mat, label, txt, x, width, y, fontScale);
    }

};
