#pragma once

#include "picture.h"
#include <iostream>

using namespace std;


// Removes outliers from picture list.
class OutlierRemover
{
public:

    // Static function - filter out outliers.
    static vector<Picture>  RemoveOutliers(
	vector<Picture>& src, 
	double tolerance,
	int height)

    {
	// try open images and sum their intensity 
	double mean = 0, sd = 0;

	double sum = 0;
	double sumSq = 0;
	int c = 0;

	vector<Picture> result;
	vector<double> totals;

	int cnt = src.size();
	for (int i = 0; i < cnt; i++) 
	{
	    src[i].OpenImage();
	    if (i % 30 == 0)  cerr << "+";
	    totals.push_back(0);
	    if (src[i].IsValidImage())
	    {
		double ti = src[i].ComputeTotalIntensity(height);
		totals[i] = ti;
		c++;
		sum += ti;
		sumSq += ti*ti;
	    }
	    src[i].CloseImage();
	}
    
	// compute mean and deviation
	if (c > 1)
	{
	    mean = sum / c;
	    sd = sqrt((sumSq - c * mean * mean)/(c-1));
	}

	// filter images within tollerance around mean
	if (sd != 0) {
	    for (int i = 0; i < cnt; i++) 
	    {
		bool ok = false;
		if (totals[i] > 0) 
		{
		    double delta = (totals[i] -  mean)/(sd*tolerance);
		    ok = (delta >= -1 && delta <= 1);
		}
		
		if (ok) 
		{
		    result.push_back(src[i]);
		}
	    }
	}

cout  << endl << "   mean " << mean << " sd " << sd 
      << " remain " << result.size() << " of " << src.size() << endl;
	return result;
    }

};
