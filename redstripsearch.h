#pragma once
#include "opencv2/core/core.hpp"
#include <algorithm>
#include <iostream>

using namespace cv;


/// Searches for red/green boundary in a picture.
class RedStripSearch
{
public:
    
    // Constructor
    RedStripSearch(const Mat& picture)
	:picture(picture)
    {
    }

    // Search for a strip
    virtual void FindStrip(
	bool vertical,   
	bool reverse,     
	int a,            // fixed coordinate, x when vertical
	int b1,           // moving coordinate lower bound, y when vertical
	int b2,	          // moving coordinate upper bound, y when vertical
	int halfWindow,       // half of window width
	double minContrast,  // minimal contrast
	int* b,           // out: maximal contrast point
        bool* found       // out: true when strip has been found
	)
    {
	*found = false;
//	std::cout << "\n\n-------------\n";
	double maxContrast = minContrast*(2*halfWindow);
	for (int ib = b1 + halfWindow; ib <= b2 - halfWindow; ib++) {
	    double contrast = 0;

	    for (int i = -halfWindow; i <= halfWindow; i++) {
		int x = a;
		int y = ib + i;
		if (!vertical) {
		    std::swap(x,y);
		}
		Vec3b intensity = picture.at<Vec3b>(y, x);
		double green = intensity[1];
		double value = green;
		if (value < 40) value = 0;
		if (value > 100) value = 100;
		if (reverse) value = -value;

		if (i < 0)  contrast -= value;		    
		if (i > 0)  contrast += value;		    

	    }
//	    std::cout << "    " << ib << " " << contrast << "\n";
	    if (contrast > maxContrast)
	    {
		maxContrast = contrast;
		*b = ib;
		*found = true;
	    }
	}

/*	std::cout << "RedStripSearch(" 
	     << vertical << "," 
	     << reverse << "," 
	     << a << "," 
	     << b1 << "," 
	     << b2 << "," 
	     << *b << "," 	
	     << *found 
		  << ")  " << maxContrast << "\n";
*/
  };


protected:

    Mat picture;

};
