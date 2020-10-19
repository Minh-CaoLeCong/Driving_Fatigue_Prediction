#ifndef FACE_DETECTION_H
#define FACE_DETECTION_H

#include "Preprocessor_Configuration.h"
#include "OpenCV440.h"
#include "opencv2/objdetect.hpp"
#include "opencv2/dnn.hpp"

using namespace cv;
using namespace std;
using namespace cv::dnn; // dnn face detection OpenCV

// haar cascade face detection
#ifdef FACE_DETECTION_HAAR_CASCADE
extern void Face_Detection_HaarCascade_Ini();
extern void Face_Detection_HaarCascade_Process(Mat img, vector<Rect> &face_detected_haar);
#endif

//extern void Test_HaarCascade();
//extern void detectAndDraw(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale);

// dnn face detection OpenCV
#ifdef FACE_DETECTION_DNN_CAFFE_OPENCV
extern void Face_Detection_DNN_OpenCV_Caffe_Ini();
extern void Face_Detection_DNN_OpenCV_Caffe_Process(Mat img, vector<Rect> &face_detected_dnn_opencv_caffe);
#endif
#ifdef FACE_DETECTION_DNN_TENSORFLOW_OPENCV
extern void Face_Detection_DNN_OpenCV_TensorFlow_Ini();
extern void Face_Detection_DNN_OpenCV_TensorFlow_Process(Mat img, vector<Rect> &face_detected_dnn_opencv_tensorflow);
#endif


#endif // !FACE_DETECTION_H