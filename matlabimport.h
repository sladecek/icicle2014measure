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

    vector<double> Load()
    {
	vector<double> result;
	double d = 0;
	while(file >> d)
	{
	    result.push_back(d);
	}
	cout << result.size() << endl;
	return result;
    }

protected:
    ifstream file;
};
