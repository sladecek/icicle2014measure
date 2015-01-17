#include "opencv2/core/core.hpp"
#include "directoryscanner.h"
#include "outlierremover.h"
#include "redstripcalibrator.h"
#include "moviemaker.h"
#include "matlabexport.h"
#include "matlabimport.h"
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

  cerr << "(5) export to octave " << endl;
  Database db("../icicle-data/db02/var/i.db");
  Experiment* experiment = db.FindExperiment(85);
  if (experiment == NULL)
  {
      cerr << "no experiment" << endl;
      return 2;

  }

  const int pxPerMmm = 2; // 7;
  Mapper mapper(calibrator, pxPerMmm, 10, 70, 0, 150);

  MatlabExport matlab("icicle");

  int t0 = experiment->GetStartTime();

  for(int i = 0; i < acceptedPictures.size(); i++) 
  {
      Picture raw = acceptedPictures[i];
      raw.OpenImage();

      Mat pic = csconv.transformMatrix(raw.GetMat());
      Mat roi = mapper.CreateRoi(pic);
      matlab.Export(roi, i, acceptedPictures[i].GetTime_s());
      if (i % 30 == 0) cerr << "+";
      raw.CloseImage();
  }
  cerr << endl;

  matlab.Close();

  cerr << "(6) running octave script " << endl;
  system("octave icicle_measure.m");

  cerr << "(7) importing octave results " << endl;

  vector<double> ile = MatlabImport("icicle_length.txt").Load();
  vector<double> isp = MatlabImport("icicle_speed.txt").Load();


  const int pxPerMmmMovie = 7;
  Mapper mapperMovie(calibrator, pxPerMmmMovie, 10, 70, 0, 150);
  double maxTime_h = acceptedPictures[acceptedPictures.size()-1].GetTime_s() / 3600.0;

  cerr << "(8) movie " << endl;
  MovieMaker movieMaker("movie", mapperMovie.GetSize());
  for(int i = 0; i < acceptedPictures.size(); i++) 
  {
      Picture raw = acceptedPictures[i];
      raw.OpenImage();

      Mat roi = mapperMovie.CreateRoi(raw.GetMat());

      int icicle_length_px = floor(ile[i]*mapperMovie.GetSize().height * mapperMovie.GetPxPerMmm());
      double icicle_length_mm = ile[i]*mapperMovie.GetSize().height ;
      double icicle_speed_mm_h = isp[i]*mapperMovie.GetSize().height /  maxTime_h;

      Painter painter(&roi);
      painter.DrawGrid(5);
      painter.DrawLine(icicle_length_mm);
      Mat annotation = painter.CreateAnnotation(roi, raw, experiment, t0, 
						icicle_length_mm, icicle_speed_mm_h);
      movieMaker.AddPicture(annotation, i);
      if (i % 30 == 0) cerr << "+";
      raw.CloseImage();
  }
  cerr << endl;
  movieMaker.CloseMovie();




  return 0;
}
// --------------------------------------------------------------------------------
