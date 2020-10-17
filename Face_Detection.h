#ifndef FACE_DETECTION_H
#define FACE_DETECTION_H

#include "OpenCV440.h"
#include "opencv2/objdetect.hpp"
#include "opencv2/dnn.hpp"

using namespace cv;
using namespace std;
using namespace cv::dnn;

extern CascadeClassifier Face_Cascade;
extern const string HaarCascade_FaceDetection_FileName_Path;

extern void Face_Detection_HaarCascade_Ini();
extern void Face_Detection_HaarCascade_Process(Mat img, vector<Rect> &face_detected_haar);


extern void Test_HaarCascade();
extern void detectAndDraw(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale);

#endif // !FACE_DETECTION_H