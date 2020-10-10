#ifndef FEATURES_EXTRACTION_H
#define FEATURES_EXTRACTION_H

#include "OpenCV440.h"

using namespace cv;
using namespace std;

extern double Eye_Aspect_Ratio(vector<vector<Point2f>> landmarks);
extern double Mouth_Aspect_Ratio(vector<vector<Point2f>> landmarks);


#endif // !FEATURES_EXTRACTION_H