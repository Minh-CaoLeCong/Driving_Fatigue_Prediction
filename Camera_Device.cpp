#include "Camera_Device.h"

using namespace cv;
using namespace std;

VideoCapture cap;
Mat frame;

// estimate FPS
double FPS = 0.0;
double FPS_time_elapsed = 0.0;
int FPS_count_frame = 0;
time_t FPS_Start_time;
time_t FPS_End_time;
clock_t Time_Execution_Start1;
clock_t Time_Execution_End1;

void Camera_Device_Ini()
{
	printf("[INFOR]: Initializing camera device.\n");

	// Open camera
	cap.open(CAMERA_DEVICE);

	// Resolution
	cap.set(CAP_PROP_FRAME_WIDTH, CAMERA_DEVICE_WIDTH);
	cap.set(CAP_PROP_FRAME_HEIGHT, CAMERA_DEVICE_HEIGHT);

	if (!cap.isOpened())
		printf("[ERROR]: Could NOT open camera.\n");

	return;
}

void Camera_Device_Test()
{
	Camera_Device_Ini();

	while (1)
	{
		// estimate FPS
		if (FPS_count_frame == 0)
			time(&FPS_Start_time);

		//cap >> frame;
		cap.read(frame);
		
		// estimate FPS
		FPS_count_frame++;
		if (FPS_count_frame == FPS_NUM_FRAMES)
		{
			FPS_count_frame = 0;
			time(&FPS_End_time);
			FPS_time_elapsed = difftime(FPS_End_time, FPS_Start_time);
			FPS = FPS_NUM_FRAMES / FPS_time_elapsed;
		}
		putText(frame, to_string(int(FPS)), Point(3, 13), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2, 8, 0);

		imshow("Test_Camera_Device", frame);

		waitKey(1);
		//if (waitKey(30) >= 0) break;
	}
	return;
}