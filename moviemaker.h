#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <stdlib.h>
#pragma once


using namespace std;


// Makes movie from a serie of pictures.
class MovieMaker
{
public:

MovieMaker(const string& movieName_, const Size& size) 
    : movieName(movieName_)
    {
	system("rm -rf _tmp");
	system("mkdir _tmp");
	char s[1000];
	sprintf(s, "rm -f %s.avi", movieName.c_str());
	system(s);

    }

    virtual ~MovieMaker()
    {}

    void AddPicture(const Mat& picture, int d)
    {
	char s[100];
	sprintf(s, "_tmp/%06d.png", (int)d);
	imwrite(s, picture);
    }


    void CloseMovie()
    {
	char s[1000];

	sprintf(s, "ffmpeg -framerate 5 -r 25 -f image2 -i _tmp/%%06d.png -vcodec mpeg4 -b 2400k %s.avi", movieName.c_str());
	system(s);
    }

protected:
    string movieName;



};
