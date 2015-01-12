#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#pragma once

using namespace std;

// Import numerical vector from matlab/octave.
class MatlabImport
{
public:

MatlabImport(const string& fileName) 
    {
	file.open(fileName.c_str());
    }

    virtual ~MatlabImport()
    {}

    Mat Load()
    {
	return Mat();
    }


protected:
    ifstream file;
};
