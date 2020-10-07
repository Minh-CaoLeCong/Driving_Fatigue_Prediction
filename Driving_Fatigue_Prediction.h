#ifndef DRIVING_FATIGUE_PREDICTION_H
#define DRIVING_FATIGUE_PREDICTION_H

#include "OpenCV440.h"
#include "Face_Detection.h"
#include "Camera_Device.h"
#include "Image_Processing.h"

using namespace cv;
using namespace std;

extern Mat Frame_Original;
extern Mat Frame_ImageProcessing_Face_Detection_HaarCascade;

extern vector<Rect> face_detected;

void Driving_Fatigue_Prediction_Ini();
void Driving_Fatigue_Prediction();

#endif // !DRIVING_FATIGUE_PREDICTION_H