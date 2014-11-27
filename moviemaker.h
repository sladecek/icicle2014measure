#include "opencv2/core/core.hpp"
#pragma once


using namespace std;


// Makes movie from a serie of pictures.
class MovieMaker
{
public:

    MovieMaker(const string& movieName) 
    {
    }

    virtual ~MovieMaker()
    {}

    void AddPicture(const Mat& picture)
    {
    }


    void CloseMovie()
    {
    }

};
