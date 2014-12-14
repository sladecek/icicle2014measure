#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#pragma once

using namespace std;

// Exports series of pictures into octave/matlab.2
class MatlabExport
{
public:

MatlabExport(const string& fileNamePrefix_) 
    : fileNamePrefix(fileNamePrefix_), headerDone(false)
    {
	string f = fileNamePrefix + ".dat";
	fs.open(f.c_str(), ofstream::binary);
    }

    virtual ~MatlabExport()
    {}

    void Export(const Mat& picture, int index, int time)
    {
	Size size = picture.size();
	if (headerDone) 
	{
	    if (firstPictureSize != size) 
	    {
		cerr << "incompatible size " << firstPictureSize << " != " << size << endl;
		return;
	    }
	}
	else
	{
	    firstPictureSize = size;
	    writeHeader(size);
	    headerDone = true;
	}
	writePicture(picture);
    }


    void Close()
    {
	fs.close();
    }

protected:
    string fileNamePrefix;
    bool headerDone;
    Size firstPictureSize;
    ofstream fs;

    void writeHeader(Size size)
    {
	ofstream headerFile;
	string f = fileNamePrefix+"_size.txt";
	headerFile.open (f.c_str(), iostream::out);
	headerFile << size.height << " " << size.width << endl;
	headerFile.close();

    }

    void writePicture(const Mat& picture)
    {
	Size s = picture.size();
	for (int x = 0; x < s.width; x++) 
	{
	    for (int y = 0; y < s.height; y++) 
	    {
		uchar b = picture.at<Vec3b>(y, x)[0];
		fs.write((const char*)&b, 1);
	    }
	}
    }
};
