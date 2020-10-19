#ifndef DRIVING_FATIGUE_PREDICTION_H
#define DRIVING_FATIGUE_PREDICTION_H

#include "Preprocessor_Configuration.h"
#include "OpenCV440.h"
#include "Camera_Device.h"
#include "Image_Processing.h"
#include "Face_Detection.h"
#include "Face_Landmark.h"
#include "Face_Tracking.h"
#include "Features_Extraction.h"
#include "Import.h"
#include "Export.h"

using namespace cv;
using namespace std;

// take sample
#define TAKE_SAMPLE_NUM_FRAMES 10 // number of frames are captured to take sample

#define TIME_PERIOD 10

#define FRAME_SHOW_ORIGINAL 0	// 1: show original frame

extern Mat Frame_Original; // capture from camera
extern Mat Frame_ImageProcessing_Face_Detection_HaarCascade;
extern Mat Take_Sample_Frames[TAKE_SAMPLE_NUM_FRAMES];
extern Mat Frame_Show;

extern vector<Rect> face_detected; // face detected by haar cascade

extern Rect2d face_roi; // face detected by tracking

extern vector<vector<Point2f>> landmarks;

extern char Text_on_Frame[12];

// estimate time processing per frame
extern double Time_Processing_per_Frame;

extern double Time_Period_Total;
extern bool Time_Period_Checked;

// features
extern double EAR_Feature;									// eye aspect ratio
extern double EAR_Feature_Threshold;						// eye aspect ratio threshold
extern double EAR_Feature_Sample[TAKE_SAMPLE_NUM_FRAMES];	// eye aspect ratio sample
extern double EAR_Feature_Sample_Sum;						// eye aspect ratio sample sum
extern double MAR_Feature;									// mouth aspect ratio
extern double MAR_Feature_Threshold;						// mouth aspect ratio threshold
extern double MAR_Feature_Sample[TAKE_SAMPLE_NUM_FRAMES];	// mouth aspect ratio sample
extern double MAR_Feature_Sample_Sum;						// mouth aspect ratio sample sum

extern bool Eye_Blink_Checked;
extern int Eye_Blink_Count;

void Driving_Fatigue_Prediction_Ini();
void Driving_Fatigue_Prediction();
void Take_Sample(void);
void Display(void);

#endif // !DRIVING_FATIGUE_PREDICTION_H