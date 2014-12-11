#include "opencv2/core/core.hpp"
#include "directoryscanner.h"
#include "outlierremover.h"
#include "redstripcalibrator.h"
#include "moviemaker.h"
#include "matlabexport.h"
#include "mapper.h"
#include "painter.h"
#include "database.h"
#include "colorspaceconvertor.h"
#include <iostream>


using namespace std;
using namespace cv;


Mat picture;

int main(int argc, char** argv)
{
  if (argc < 2) 
  {
      cerr << "missing argument" << endl;
      return 2;
  }
  
  cerr << "(1) reading " << endl;
  vector<Picture>  rawPictures = DirectoryScanner::ScanDirectory(argv[1]);
//  rawPictures.resize(30);
  cerr << "(2) outliers " << endl;
  vector<Picture> acceptedPictures = OutlierRemover::RemoveOutliers(rawPictures, 1.7, 100);
  if (acceptedPictures.size() <= 0)
  {
      cerr << "no pictures" << endl;
      return 2;      
  }



  cerr << "(3) calibration " << endl;
  const double horizontalDistanceBetweenStrips_mm = 86;
  const double verticalDistanceBetweenMarks_mm = 94;
  RedStripCalibrator calibrator(
      horizontalDistanceBetweenStrips_mm, verticalDistanceBetweenMarks_mm);

  Picture& first = acceptedPictures[0];
  first.OpenImage();
  calibrator.ComputeCalibrationFromPicture(first.GetMat());
  first.CloseImage();

  if (!calibrator.IsOk()) 
  {
      cerr << "invalid calibration" << endl;
      return 2;
  }
  // cout << "calibration: " << calibrator.GetCalibration() << endl;

  cerr << "(4) analysis " << endl;
  ColorSpaceConvertor csconv;

  vector<Picture> firstPictures;
  copy ( acceptedPictures.begin(), acceptedPictures.begin() + 12, std::back_inserter(firstPictures) );

  csconv.CalibrateBackground(firstPictures);

  cerr << "(5) movie " << endl;
  Database db("../icicle-data/db02/var/i.db");
  Experiment* experiment = db.FindExperiment(85);
  if (experiment == NULL)
  {
      cerr << "no experiment" << endl;
      return 2;

  }


  Mapper mapper(calibrator, 7, 10, 70, 0, 150);
  MovieMaker movieMaker("movie", mapper.GetSize());
  MatlabExport matlab("icicle");

  int t0 = experiment->GetStartTime();

  for(int i = 0; i < acceptedPictures.size(); i++) 
  {
      Picture raw = acceptedPictures[i];
      raw.OpenImage();

      Mat pic = csconv.transformMatrix(raw.GetMat());
      Mat roi = mapper.CreateRoi(pic);
      Painter painter(&roi);
      painter.DrawGrid(5);
      Mat annotation = painter.CreateAnnotation(roi, raw, experiment, t0);
      movieMaker.AddPicture(annotation, i);
      matlab.Export(roi, i, acceptedPictures[i].GetTime_s());
      if (i % 30 == 0) cerr << "+";
      raw.CloseImage();
  }
  cerr << endl;

  movieMaker.CloseMovie();
  matlab.Close();
  return 0;
}
// --------------------------------------------------------------------------------
