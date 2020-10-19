#ifndef FACE_TRACKING_H
#define FACE_TRACKING_H

#include "Preprocessor_Configuration.h"
#include "OpenCV440.h"
#include "opencv2/tracking.hpp"

using namespace cv;
using namespace std;

extern Ptr<TrackerMedianFlow> Face_Tracking_MedianFlow;


#endif // !FACE_TRACKING_H