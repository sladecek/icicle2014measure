#pragma once

#include "picture.h"
#include "dirent.h"
#include <iostream>

using namespace std;


// Directory scanner scans a directory to get set of pictures.
class DirectoryScanner
{
public:
    // Static function - create ordered list of pictures.
    static vector<Picture>  ScanDirectory(const string& directory)
    {
	vector<Picture> result;
	
	DIR* dirp = opendir(directory.c_str());
	struct dirent * dp = NULL;

	while ((dp = readdir(dirp)) != NULL)
	{
	    string fn = dp->d_name;
	    if (fn == "." || fn == "..") 
	    {
		continue;
	    }	    
	    result.push_back(Picture(directory, fn));
	    if (result.size() % 30 == 0) cerr << '.';
	}
	cerr << endl;
	(void)closedir(dirp);

	sort(result.begin(), result.end());

	if (!result.empty())
	{
	    int time0_s = result[0].GetAbsouluteTime_s();
	    for (int i = 1; i < result.size(); i++)
	    {
		result[i].SetTime_s(result[i].GetAbsouluteTime_s() - time0_s);
	    }
	}



	return result;
    }



};
