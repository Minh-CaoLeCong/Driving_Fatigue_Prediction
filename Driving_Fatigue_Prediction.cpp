#include "Driving_Fatigue_Prediction.h"

using namespace cv;
using namespace std;

Mat Frame_Original;
Mat Frame_ImageProcessing_Face_Detection_HaarCascade;

vector<Rect> face_detected;

Rect2d face_roi;

int main()
{
	Driving_Fatigue_Prediction();

	return 0;
}

void Driving_Fatigue_Prediction_Ini()
{
	Camera_Device_Ini();
	Face_Detection_HaarCascade_Ini();


	return;
}

void Driving_Fatigue_Prediction()
{
	Driving_Fatigue_Prediction_Ini();


	while (1)
	{
		// estimate FPS
		if (FPS_count_frame == 0)
			time(&FPS_Start_time);

		//cap >> frame;
		cap.read(Frame_Original);

		Frame_ImageProcessing_Face_Detection_HaarCascade = ImageProcessing_Face_Detection_HaarCascade(Frame_Original);

		Face_Detection_HaarCascade_Process(Frame_ImageProcessing_Face_Detection_HaarCascade, face_detected);

		rectangle(Frame_Original, face_detected[0], Scalar(255, 255, 0), 1, 8, 0);

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