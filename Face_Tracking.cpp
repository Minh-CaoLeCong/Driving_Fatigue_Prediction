#include "Face_Tracking.h"

using namespace cv;
using namespace std;

Ptr<TrackerMedianFlow> Face_Tracking_MedianFlow = TrackerMedianFlow::create();
