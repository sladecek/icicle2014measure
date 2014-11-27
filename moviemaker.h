#include "opencv2/core/core.hpp"
#pragma once


using namespace std;


// Makes movie from a serie of pictures.
class MovieMaker
{
public:

MovieMaker(const string& movieName, const Size& size) 

    {
	vw = new VideoWriter(movieName,
CV_FOURCC('M','J','P','G') ,

25, size, true);
    }

    virtual ~MovieMaker()
    { delete vw;}

    void AddPicture(const Mat& picture)
    {
	if (vw != 0)
	{
	    vw->write(picture);
	}
    }


    void CloseMovie()
    {
	delete vw;
	vw = 0;
    }

protected:
    VideoWriter* vw;



};
