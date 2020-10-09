#ifndef DRIVING_FATIGUE_PREDICTION_H
#define DRIVING_FATIGUE_PREDICTION_H

#include "OpenCV440.h"
#include "Camera_Device.h"
#include "Image_Processing.h"
#include "Face_Detection.h"
#include "Face_Landmark.h"
#include "Face_Tracking.h"

using namespace cv;
using namespace std;

extern Mat Frame_Original;
extern Mat Frame_ImageProcessing_Face_Detection_HaarCascade;

extern vector<Rect> face_detected;

extern Rect2d face_roi; // face detected

extern vector<vector<Point2f>> landmarks;

void Driving_Fatigue_Prediction_Ini();
void Driving_Fatigue_Prediction();

#endif // !DRIVING_FATIGUE_PREDICTION_H