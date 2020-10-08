#ifndef FACE_LANDMARK_H
#define FACE_LANDMARK_H

#include "OpenCV440.h"
#include "opencv2/face.hpp"

using namespace cv;
using namespace std;
using namespace cv::face;

extern Ptr<Facemark> FaceMark_OpenCV;
extern string Face_Landmark_OpenCV_FileName_Path;
extern bool Face_Landmark_OpenCV_Detection_Check;


extern void Face_Landmark_OpenCV_Ini();
extern void Face_Landmark_OpenCV_Detection_Process(Mat imgin, vector<Rect> face_detected, vector<vector<Point2f>> &landmarks_opencv);
extern void Face_Landmark_OpenCV_Draw(Mat &imgin, vector<vector<Point2f>> landmarks_opencv);


#endif // !FACE_LANDMARK_H