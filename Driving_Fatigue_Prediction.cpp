#include "Driving_Fatigue_Prediction.h"

using namespace cv;
using namespace std;
using namespace cv::face;

Mat Frame_Original;
Mat Frame_ImageProcessing_Face_Detection_HaarCascade; // image processing for haar cascade face detection

vector<Rect> face_detected;	// face detection

Rect2d face_roi; // face detected

vector<vector<Point2f>> landmarks;

int main()
{
	Driving_Fatigue_Prediction();
	
	


	return 0;
}

void Driving_Fatigue_Prediction_Ini()
{
	// init camera device
	Camera_Device_Ini();
	// init haar cascade facedetection
	Face_Detection_HaarCascade_Ini();
	// init face landmark opencv
	Face_Landmark_OpenCV_Ini();

	return;
}

void Driving_Fatigue_Prediction()
{
	Driving_Fatigue_Prediction_Ini();

	//cap >> frame;
	cap.read(Frame_Original);

	// image processing for haar cascade face detection
	Frame_ImageProcessing_Face_Detection_HaarCascade = ImageProcessing_Face_Detection_HaarCascade(Frame_Original);

	// detecting face using haar cascade
	Face_Detection_HaarCascade_Process(Frame_ImageProcessing_Face_Detection_HaarCascade, face_detected);

	// draw face to original frame
	rectangle(Frame_Original, face_detected[0], Scalar(255, 255, 0), 1, 8, 0);

	// init medianflow face tracking
	face_roi = face_detected[0];
	Face_Tracking_MedianFlow->init(Frame_ImageProcessing_Face_Detection_HaarCascade, face_roi);

	// detecting face landmarks opencv
	Face_Landmark_OpenCV_Detection_Process(Frame_ImageProcessing_Face_Detection_HaarCascade, face_detected, landmarks);

	// draw face landmarks opencv
	Face_Landmark_OpenCV_Draw(Frame_Original, landmarks);

	putText(Frame_Original, to_string(int(FPS)), Point(3, 13), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2, 8, 0);

	imshow("Driving Fatigue Prediction", Frame_Original);

	while (1)
	{
		// estimate FPS
		if (FPS_count_frame == 0)
			time(&FPS_Start_time);

		//cap >> frame;
		cap.read(Frame_Original);

		// image processing for haar cascade face detection
		Frame_ImageProcessing_Face_Detection_HaarCascade = ImageProcessing_Face_Detection_HaarCascade(Frame_Original);

		//// detecting face using haar cascade
		//Face_Detection_HaarCascade_Process(Frame_ImageProcessing_Face_Detection_HaarCascade, face_detected);

		// medianflow face tracking
		Face_Tracking_MedianFlow->update(Frame_ImageProcessing_Face_Detection_HaarCascade, face_roi);
		face_detected[0] = face_roi;

		// draw face to original frame
		rectangle(Frame_Original, face_detected[0], Scalar(255, 255, 0), 1, 8, 0);

		// detecting face landmarks opencv
		Face_Landmark_OpenCV_Detection_Process(Frame_ImageProcessing_Face_Detection_HaarCascade, face_detected, landmarks);

		// draw face landmarks opencv
		Face_Landmark_OpenCV_Draw(Frame_Original, landmarks);

		// estimate FPS
		FPS_count_frame++;
		if (FPS_count_frame == FPS_NUM_FRAMES)
		{
			FPS_count_frame = 0;
			time(&FPS_End_time);
			FPS_time_elapsed = difftime(FPS_End_time, FPS_Start_time);
			FPS = FPS_NUM_FRAMES / FPS_time_elapsed;
		}
		putText(Frame_Original, to_string(int(FPS)), Point(3, 13), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2, 8, 0);

		imshow("Driving Fatigue Prediction", Frame_Original);

		waitKey(1);
		//if (waitKey(30) >= 0) break;
	}

	return;
}