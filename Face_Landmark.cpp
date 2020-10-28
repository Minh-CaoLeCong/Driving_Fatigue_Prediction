#include "Face_Landmark.h"

using namespace cv;
using namespace std;
using namespace cv::face;

Ptr<Facemark> FaceMark_OpenCV = FacemarkLBF::create();
bool Face_Landmark_OpenCV_Detection_Check = false;

void Face_Landmark_OpenCV_Ini()
{
	string Face_Landmark_OpenCV_FileName_Path = "./data/facemark_opencv/lbfmodel.yaml";

	// Load landmark detector OpenCV
	printf("[INFOR]: Initializing face landmark.\n");
	printf("[INFOR]: Loading face landmark OpenCV .yaml file.\n");
	FaceMark_OpenCV->loadModel(Face_Landmark_OpenCV_FileName_Path);
	if (FaceMark_OpenCV.empty())
	{
		//printf("[ERROR]: Could NOT load face landmark OpenCV .yaml file.\n");
		printf("[" ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET "]: Could NOT load face landmark OpenCV .yaml file.\n");
	}
	else
		printf("[" ANSI_COLOR_GREEN "DONE" ANSI_COLOR_RESET "]: Loaded face landmark OpenCV .yaml file successfully.\n");

	return;
}

void Face_Landmark_OpenCV_Detection_Process(Mat imgin, vector<Rect> face_detected, vector<vector<Point2f>> &landmarks_opencv)
{
	// Run landmark detector opencv
	Face_Landmark_OpenCV_Detection_Check = FaceMark_OpenCV->fit(imgin, face_detected, landmarks_opencv);
	if (!Face_Landmark_OpenCV_Detection_Check)
	{
		//printf("[ERROR]: Could NOT detect landmarks.\n");
		printf("[" ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET "]: Could NOT detect landmarks.\n");
	}
	return;
}

void Face_Landmark_OpenCV_Draw(Mat &imgin, vector<vector<Point2f>> landmarks_opencv)
{
	if (Face_Landmark_OpenCV_Detection_Check)
	{
		for (int i = 36; i < 60; i++)
		{
			circle(imgin, landmarks_opencv[0][i], 1, Scalar(71, 99, 255), 1, 8, 0);
		}
	}

	return;
}