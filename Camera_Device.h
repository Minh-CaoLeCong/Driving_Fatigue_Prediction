#ifndef CAMERA_DEVICE_H
#define CAMERA_DEVICE_H

#include "OpenCV440.h"
#include <time.h>

// Init
#define CAMERA_DEVICE 0
#define CAMERA_DEVICE_WIDTH 320
#define CAMERA_DEVICE_HEIGHT 240

// estimate FPS
#define FPS_NUM_FRAMES 60


using namespace cv;

extern VideoCapture cap;
extern Mat frame;

// estimate FPS
extern double FPS;
extern double FPS_time_elapsed;
extern int FPS_count_frame;
extern time_t FPS_Start_time;
extern time_t FPS_End_time;
extern clock_t Time_Execution_Start1;
extern clock_t Time_Execution_End1;

extern void Camera_Device_Ini();

extern void Camera_Device_Test();









#endif // !CAMERA_DEVICE_H