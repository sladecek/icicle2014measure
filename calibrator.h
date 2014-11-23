#include "opencv2/core/core.hpp"
using namespace cv;


// Interface for calibrators. Take picture, return geometric transformation
// from pixels to millimeters.
class Calibrator
{
public:
    
    // Compute calibrat
    virtual void ComputeCalibrationFromPicture(const Mat& picture) = 0;

    // Is the result usable?
    virtual bool IsOk() const = 0;

    // Get result of calibration - 1x12 matrix [a_0..a_5, b_0..b_5].
    // x' = a_0 + a_1*x + a_2*y + a_3*x^2 + a_4*y^2 + a_5*x*y
    // y' = b_0 + b_1*x + b_2*y + b_3*x^2 + b_4*y^2 + b_5*x*y    
    virtual Mat GetCalibration() const = 0;
    
};
