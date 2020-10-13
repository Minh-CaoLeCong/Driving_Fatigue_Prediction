#include "Driving_Fatigue_Prediction.h"

using namespace cv;
using namespace std;
using namespace cv::face;

Mat Frame_Original;
Mat Frame_ImageProcessing_Face_Detection_HaarCascade; // image processing for haar cascade face detection
Mat Take_Sample_Frames[TAKE_SAMPLE_NUM_FRAMES];

vector<Rect> face_detected;	// face detection

Rect2d face_roi; // face detected

vector<vector<Point2f>> landmarks;

char Text_on_Frame[12];

double EAR_Feature;										// eye aspect ratio
double EAR_Feature_Threshold;							// eye aspect ratio threshold
double EAR_Feature_Sample[TAKE_SAMPLE_NUM_FRAMES] = {}; // eye aspect ratio sample
double EAR_Feature_Sample_Sum;							// eye aspect ratio sample sum
double MAR_Feature;										// mouth aspect ratio
double MAR_Feature_Threshold;							// mouth aspect ratio threshold
double MAR_Feature_Sample[TAKE_SAMPLE_NUM_FRAMES] = {}; // mouth aspect ratio sample
double MAR_Feature_Sample_Sum;							// mouth aspect ratio sample sum

double Time_Processing_per_Frame = 0.0;

bool Eye_Blink_Checked = false;
int Eye_Blink_Count = 0;

double Time_Period_Total = 0.0;
bool Time_Period_Checked = true;

int main()
{
	// init
	Driving_Fatigue_Prediction_Ini();

	// take sample
	Take_Sample();
	
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
	while (1)
	{
		if (Time_Period_Checked)
		{
			Time_Period_Checked = false;
			Time_Period_Total = 0.0;
			Eye_Blink_Count = 0;

			// estimate FPS
			/*if (FPS_count_frame == 0)
			{
				time(&FPS_Start_time);
			}*/
			Time_Execution_Start1 = clock();

			//cap >> frame;
			cap.read(Frame_Original);

			// image processing for haar cascade face detection
			Frame_ImageProcessing_Face_Detection_HaarCascade = ImageProcessing_Face_Detection_HaarCascade(Frame_Original);

			// detecting face using haar cascade
			Face_Detection_HaarCascade_Process(Frame_ImageProcessing_Face_Detection_HaarCascade, face_detected);

			// init medianflow face tracking
			face_roi = face_detected[0];
			Face_Tracking_MedianFlow->init(Frame_ImageProcessing_Face_Detection_HaarCascade, face_roi);

			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Frame_ImageProcessing_Face_Detection_HaarCascade, face_detected, landmarks);

			// estimating eye aspect ratio
			EAR_Feature = Eye_Aspect_Ratio(landmarks);

			// count eye blink
			Eye_Blink_Checked = Eye_Blink(EAR_Feature, EAR_Feature_Threshold);
			if (Eye_Blink_Checked)
				Eye_Blink_Count++;

			// estimating mouth aspect ratio
			MAR_Feature = Mouth_Aspect_Ratio(landmarks);

			Display();

			// estimate FPS
			/*FPS_count_frame++;
			if (FPS_count_frame == FPS_NUM_FRAMES)
			{
				FPS_count_frame = 0;
				time(&FPS_End_time);
				FPS_time_elapsed = difftime(FPS_End_time, FPS_Start_time);
				FPS = FPS_NUM_FRAMES / FPS_time_elapsed;
			}*/
			Time_Execution_End1 = clock();
			Time_Processing_per_Frame = double(Time_Execution_End1 - Time_Execution_Start1) / double(CLOCKS_PER_SEC);
			FPS = 1.0 / Time_Processing_per_Frame;

			Time_Period_Total += Time_Processing_per_Frame;

			//sprintf(Text_on_Frame, "S: %.1f", double(Time_Execution_Start1));
			//putText(Frame_Original, Text_on_Frame, Point(2, 52), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 2, 8, 0);

			//sprintf(Text_on_Frame, "E: %.1f", double(Time_Execution_End1));
			//putText(Frame_Original, Text_on_Frame, Point(2, 72), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 2, 8, 0);

			// display time processing per frame
			sprintf(Text_on_Frame, "T: %.3f", double(Time_Processing_per_Frame));
			putText(Frame_Original, Text_on_Frame, Point(2, 32), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 2, 8, 0);

			// display FPS on frame
			sprintf(Text_on_Frame, "FPS: %.2lf", FPS);
			putText(Frame_Original, Text_on_Frame, Point(2, 12), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 2, 8, 0);

			// display total time period
			sprintf(Text_on_Frame, "TT: %.3f", Time_Period_Total);
			putText(Frame_Original, Text_on_Frame, Point(2, 52), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 2, 8, 0);

			imshow("Driving Fatigue Prediction", Frame_Original);

			waitKey(1);
			//if (waitKey(30) >= 0) break;
		}
		else
		{
			// estimate FPS
			/*if (FPS_count_frame == 0)
			{
				time(&FPS_Start_time);
			}*/
			Time_Execution_Start1 = clock();

			//cap >> frame;
			cap.read(Frame_Original);

			// image processing for haar cascade face detection
			Frame_ImageProcessing_Face_Detection_HaarCascade = ImageProcessing_Face_Detection_HaarCascade(Frame_Original);

			//// detecting face using haar cascade
			//Face_Detection_HaarCascade_Process(Frame_ImageProcessing_Face_Detection_HaarCascade, face_detected);

			// medianflow face tracking
			Face_Tracking_MedianFlow->update(Frame_ImageProcessing_Face_Detection_HaarCascade, face_roi);
			face_detected[0] = face_roi;

			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Frame_ImageProcessing_Face_Detection_HaarCascade, face_detected, landmarks);

			// estimating eye aspect ratio
			EAR_Feature = Eye_Aspect_Ratio(landmarks);

			// count eye blink
			Eye_Blink_Checked = Eye_Blink(EAR_Feature, EAR_Feature_Threshold);
			if (Eye_Blink_Checked)
				Eye_Blink_Count++;

			// estimating mouth aspect ratio
			MAR_Feature = Mouth_Aspect_Ratio(landmarks);

			Display();

			// estimate FPS
			/*FPS_count_frame++;
			if (FPS_count_frame == FPS_NUM_FRAMES)
			{
				FPS_count_frame = 0;
				time(&FPS_End_time);
				FPS_time_elapsed = difftime(FPS_End_time, FPS_Start_time);
				FPS = FPS_NUM_FRAMES / FPS_time_elapsed;
			}*/
			Time_Execution_End1 = clock();
			Time_Processing_per_Frame = double(Time_Execution_End1 - Time_Execution_Start1) / double(CLOCKS_PER_SEC);
			FPS = 1.0 / Time_Processing_per_Frame;

			Time_Period_Total += Time_Processing_per_Frame;

			if (Time_Period_Total >= double(TIME_PERIOD))
				Time_Period_Checked = true;

			//sprintf(Text_on_Frame, "S: %.1f", double(Time_Execution_Start1));
			//putText(Frame_Original, Text_on_Frame, Point(2, 52), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 2, 8, 0);

			//sprintf(Text_on_Frame, "E: %.1f", double(Time_Execution_End1));
			//putText(Frame_Original, Text_on_Frame, Point(2, 72), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 2, 8, 0);

			// display time processing per frame
			sprintf(Text_on_Frame, "T: %.3f", double(Time_Processing_per_Frame));
			putText(Frame_Original, Text_on_Frame, Point(2, 32), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 2, 8, 0);

			// display FPS on frame
			sprintf(Text_on_Frame, "FPS: %.2lf", FPS);
			putText(Frame_Original, Text_on_Frame, Point(2, 12), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 2, 8, 0);

			// display total time period
			sprintf(Text_on_Frame, "TT: %.3f", Time_Period_Total);
			putText(Frame_Original, Text_on_Frame, Point(2, 52), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 2, 8, 0);


			imshow("Driving Fatigue Prediction", Frame_Original);

			waitKey(1);
			//if (waitKey(30) >= 0) break;
		}
	}

	return;
}

void Take_Sample(void)
{
	// interact with user
	{
		printf("[INFOR]: STARTING TAKE SAMPLE PROCESSING: \n");
		printf("[INFOR]: Now, please look at the camera and open your eyes in normal size \n");
		printf("[NOTE]: There have to only one person in front of the camera \n");
		printf("[INFOR]: If you're ready, press ENTER to continue. \n");
		cin.get();
	}

	// capture images to take sample
	printf("[INFOR]: Capturing images... \n");
	for (int i = 0; i < TAKE_SAMPLE_NUM_FRAMES; i++)
	{
		cap.read(Take_Sample_Frames[i]);
		// check capture frame
		if (Take_Sample_Frames[i].empty()) 
		{ 
			printf("[INFOR]: Could NOT capture frame \n");
			break; 
		}
	}

	// taking sample
	printf("[INFOR]: Taking sample... \n");
	for (int i = 0; i < TAKE_SAMPLE_NUM_FRAMES; i++)
	{
		// preprocessing image
		Mat Sample_Frame = ImageProcessing_Face_Detection_HaarCascade(Take_Sample_Frames[i]);

		// detecting face using haar cascade
		Face_Detection_HaarCascade_Process(Sample_Frame, face_detected);

		// detecting face landmarks opencv
		Face_Landmark_OpenCV_Detection_Process(Sample_Frame, face_detected, landmarks);

		// estimating eye aspect ratio
		EAR_Feature_Sample[i] = Eye_Aspect_Ratio(landmarks);
		
		// estimating mouth aspect ratio
		MAR_Feature_Sample[i] = Mouth_Aspect_Ratio(landmarks);

		printf("[INFOR][%d] EAR: %lf\tMAR: %lf\n", i, EAR_Feature_Sample[i], MAR_Feature_Sample[i]);

		EAR_Feature_Sample_Sum += EAR_Feature_Sample[i];
		MAR_Feature_Sample_Sum += MAR_Feature_Sample[i];
	}

	// estimate eye aspect ratio threshold
	EAR_Feature_Threshold = (EAR_Feature_Sample_Sum / TAKE_SAMPLE_NUM_FRAMES) - 0.08;
	MAR_Feature_Threshold = (MAR_Feature_Sample_Sum / TAKE_SAMPLE_NUM_FRAMES) + 0.1;

	printf("[INFOR] EAR_Threshold: %lf\tMAR_Threshold: %lf\n", EAR_Feature_Threshold, MAR_Feature_Threshold);
	printf("[INFOR]: END TAKE SAMPLE PROCESSING.\n");
}

void Display(void)
{
	// draw face to original frame
	rectangle(Frame_Original, face_detected[0], Scalar(255, 255, 0), 1, 8, 0);

	// draw face landmarks opencv
	Face_Landmark_OpenCV_Draw(Frame_Original, landmarks);

	// display EAR feature on frame
	sprintf(Text_on_Frame, "EAR: %.2lf", EAR_Feature);
	putText(Frame_Original, Text_on_Frame, Point(252, 12), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 2, 8, 0);

	// display MAR feature on frame
	sprintf(Text_on_Frame, "MAR: %.2lf", MAR_Feature);
	putText(Frame_Original, Text_on_Frame, Point(252, 32), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 2, 8, 0);

	// display num of eye blink
	sprintf(Text_on_Frame, "BLINK: %d", Eye_Blink_Count);
	putText(Frame_Original, Text_on_Frame, Point(252, 52), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 2, 8, 0);

	return;
}