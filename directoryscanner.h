#pragma once

#include "picture.h"
#include "dirent.h"

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
	    result.push_back(Picture(fn));
	}
	(void)closedir(dirp);

//	sort(result.begin(), result.end());

	return result;
    }
};
