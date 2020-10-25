#include "Driving_Fatigue_Prediction.h"

using namespace cv;
using namespace std;
using namespace cv::face;
using namespace cv::dnn;

Mat Frame_Original;
Mat Frame_ImageProcessing_Face_Detection_HaarCascade; // image processing for haar cascade face detection
Mat Take_Sample_Frames[TAKE_SAMPLE_NUM_FRAMES];
Mat Frame_Show;

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

// Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file. 
VideoWriter video("./video/outcpp.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(CAMERA_DEVICE_WIDTH, CAMERA_DEVICE_HEIGHT));

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

	Face_Detection_HaarCascade_Ini(); // init haar cascade facedetection

#ifdef FACE_DETECTION_DNN_CAFFE_OPENCV
	Face_Detection_DNN_OpenCV_Caffe_Ini();
#endif

#ifdef FACE_DETECTION_DNN_TENSORFLOW_OPENCV
	Face_Detection_DNN_OpenCV_TensorFlow_Ini();
#endif

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

			FPS_Start_time = time(NULL);
			cout << ctime(&FPS_Start_time) << endl;

			// estimate FPS
			/*if (FPS_count_frame == 0)
			{
				time(&FPS_Start_time);
			}*/
			Time_Execution_Start1 = clock();

			//cap >> Frame_Original;
			cap.read(Frame_Original);

			// Write the frame into the file 'outcpp.avi'
			video.write(Frame_Original);

#ifdef FACE_DETECTION_HAAR_CASCADE
			// image processing for haar cascade face detection
			Frame_ImageProcessing_Face_Detection_HaarCascade = ImageProcessing_Face_Detection_HaarCascade(Frame_Original);
			// detecting face using haar cascade
			Face_Detection_HaarCascade_Process(Frame_ImageProcessing_Face_Detection_HaarCascade, face_detected);
#ifdef FACE_TRACKING
			// init medianflow face tracking
			face_roi = face_detected[0];
			Face_Tracking_MedianFlow->init(Frame_ImageProcessing_Face_Detection_HaarCascade, face_roi);
#endif
			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Frame_ImageProcessing_Face_Detection_HaarCascade, face_detected, landmarks);
#endif

#ifdef FACE_DETECTION_DNN_CAFFE_OPENCV
			// dnn face detection OpenCV
			Face_Detection_DNN_OpenCV_Caffe_Process(Frame_Original, face_detected);
#ifdef FACE_TRACKING
			// init medianflow face tracking
			face_roi = face_detected[0];
			Face_Tracking_MedianFlow->init(Frame_Original, face_roi);
#endif
			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Frame_Original, face_detected, landmarks);
#endif

#ifdef FACE_DETECTION_DNN_TENSORFLOW_OPENCV
			// dnn face detection OpenCV
			Face_Detection_DNN_OpenCV_TensorFlow_Process(Frame_Original, face_detected);
#ifdef FACE_TRACKING
			// init medianflow face tracking
			face_roi = face_detected[0];
			Face_Tracking_MedianFlow->init(Frame_Original, face_roi);
#endif
			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Frame_Original, face_detected, landmarks);
#endif

			// estimating eye aspect ratio
			EAR_Feature = Eye_Aspect_Ratio(landmarks);

			// count eye blink
			Eye_Blink_Checked = Eye_Blink(EAR_Feature, EAR_Feature_Threshold);
			if (Eye_Blink_Checked)
				Eye_Blink_Count++;

			// estimating mouth aspect ratio
			MAR_Feature = Mouth_Aspect_Ratio(landmarks);

#ifdef FACE_DETECTION_HAAR_CASCADE
			if (FRAME_SHOW_IMAGE_PROCESSING)
				Frame_Show = Frame_ImageProcessing_Face_Detection_HaarCascade;
			else
				Frame_Show = Frame_Original;
#endif

#ifdef FACE_DETECTION_DNN_CAFFE_OPENCV
			Frame_Show = Frame_Original;
#endif

#ifdef FACE_DETECTION_DNN_TENSORFLOW_OPENCV
			Frame_Show = Frame_Original;
#endif
			// display information on frame
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
			//putText(Frame_Show, Text_on_Frame, Point(2, 52), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 2, 8, 0);

			//sprintf(Text_on_Frame, "E: %.1f", double(Time_Execution_End1));
			//putText(Frame_Show, Text_on_Frame, Point(2, 72), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 2, 8, 0);

			// display time processing per frame
			sprintf(Text_on_Frame, "T: %.3f", double(Time_Processing_per_Frame));
			putText(Frame_Show, Text_on_Frame, Point(2, 32), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 2, 8, 0);

			// display FPS on frame
			sprintf(Text_on_Frame, "FPS: %.2lf", FPS);
			putText(Frame_Show, Text_on_Frame, Point(2, 12), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 2, 8, 0);

			// display total time period
			sprintf(Text_on_Frame, "TT: %.3f", Time_Period_Total);
			putText(Frame_Show, Text_on_Frame, Point(2, 52), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 2, 8, 0);

			imshow("Driving Fatigue Prediction", Frame_Show);

			// Press  ESC on keyboard to  exit
			char c = (char)waitKey(1);
			if (c == 27)
				break;
		}
		else
		{
			// estimate FPS
			/*if (FPS_count_frame == 0)
			{
				time(&FPS_Start_time);
			}*/
			Time_Execution_Start1 = clock();
			
			//cap >> Frame_Original;
			cap.read(Frame_Original);

			// Write the frame into the file 'outcpp.avi'
			video.write(Frame_Original);

#ifdef FACE_TRACKING

#ifdef FACE_DETECTION_HAAR_CASCADE
			// image processing for haar cascade face detection
			Frame_ImageProcessing_Face_Detection_HaarCascade = ImageProcessing_Face_Detection_HaarCascade(Frame_Original);
			// medianflow face tracking
			Face_Tracking_MedianFlow->update(Frame_ImageProcessing_Face_Detection_HaarCascade, face_roi);
			face_detected[0] = face_roi;
			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Frame_ImageProcessing_Face_Detection_HaarCascade, face_detected, landmarks);
#endif
#ifdef FACE_DETECTION_DNN_CAFFE_OPENCV
			// medianflow face tracking
			Face_Tracking_MedianFlow->update(Frame_Original, face_roi);
			face_detected[0] = face_roi;
			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Frame_Original, face_detected, landmarks);
#endif
#ifdef FACE_DETECTION_DNN_TENSORFLOW_OPENCV
			// medianflow face tracking
			Face_Tracking_MedianFlow->update(Frame_Original, face_roi);
			face_detected[0] = face_roi;
			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Frame_Original, face_detected, landmarks);
#endif

#else

#ifdef FACE_DETECTION_HAAR_CASCADE
			// image processing for haar cascade face detection
			Frame_ImageProcessing_Face_Detection_HaarCascade = ImageProcessing_Face_Detection_HaarCascade(Frame_Original);
			// detecting face using haar cascade
			Face_Detection_HaarCascade_Process(Frame_ImageProcessing_Face_Detection_HaarCascade, face_detected);
			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Frame_ImageProcessing_Face_Detection_HaarCascade, face_detected, landmarks);
#endif
#ifdef FACE_DETECTION_DNN_CAFFE_OPENCV
			// dnn face detection OpenCV
			Face_Detection_DNN_OpenCV_Caffe_Process(Frame_Original, face_detected);
			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Frame_Original, face_detected, landmarks);
#endif
#ifdef FACE_DETECTION_DNN_TENSORFLOW_OPENCV
			// dnn face detection OpenCV
			Face_Detection_DNN_OpenCV_TensorFlow_Process(Frame_Original, face_detected);
			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Frame_Original, face_detected, landmarks);
#endif

#endif // !FACE_TRACKING

			// estimating eye aspect ratio
			EAR_Feature = Eye_Aspect_Ratio(landmarks);

			// count eye blink
			Eye_Blink_Checked = Eye_Blink(EAR_Feature, EAR_Feature_Threshold);
			if (Eye_Blink_Checked)
				Eye_Blink_Count++;

			// estimating mouth aspect ratio
			MAR_Feature = Mouth_Aspect_Ratio(landmarks);

#ifdef FACE_DETECTION_HAAR_CASCADE
			if (FRAME_SHOW_IMAGE_PROCESSING)
				Frame_Show = Frame_ImageProcessing_Face_Detection_HaarCascade;
			else
				Frame_Show = Frame_Original;
#endif

#ifdef FACE_DETECTION_DNN_CAFFE_OPENCV
			Frame_Show = Frame_Original;
#endif

#ifdef FACE_DETECTION_DNN_TENSORFLOW_OPENCV
			Frame_Show = Frame_Original;
#endif

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
			//putText(Frame_Show, Text_on_Frame, Point(2, 52), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 2, 8, 0);

			//sprintf(Text_on_Frame, "E: %.1f", double(Time_Execution_End1));
			//putText(Frame_Show, Text_on_Frame, Point(2, 72), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 255), 2, 8, 0);

			// display time processing per frame
			sprintf(Text_on_Frame, "T: %.3f", double(Time_Processing_per_Frame));
			putText(Frame_Show, Text_on_Frame, Point(2, 32), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 2, 8, 0);

			// display FPS on frame
			sprintf(Text_on_Frame, "FPS: %.2lf", FPS);
			putText(Frame_Show, Text_on_Frame, Point(2, 12), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 2, 8, 0);

			// display total time period
			sprintf(Text_on_Frame, "TT: %.3f", Time_Period_Total);
			putText(Frame_Show, Text_on_Frame, Point(2, 52), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 2, 8, 0);


			imshow("Driving Fatigue Prediction", Frame_Show);

			// Press  ESC on keyboard to  exit
			char c = (char)waitKey(1);
			if (c == 27)
				break;
		}
	}

	// When everything done, release the video capture and write object
	cap.release();
	video.release();

	// Closes all the windows
	destroyAllWindows();

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
		if (i == 0)
		{
			// preprocessing image
			Mat Sample_Frame = ImageProcessing_Face_Detection_HaarCascade(Take_Sample_Frames[i]);

			// detecting face using haar cascade
			Face_Detection_HaarCascade_Process(Sample_Frame, face_detected);

			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Sample_Frame, face_detected, landmarks);
		}
		else
		{
#ifdef FACE_DETECTION_HAAR_CASCADE
			// preprocessing image
			Mat Sample_Frame = ImageProcessing_Face_Detection_HaarCascade(Take_Sample_Frames[i]);
			// detecting face using haar cascade
			Face_Detection_HaarCascade_Process(Sample_Frame, face_detected);
			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Sample_Frame, face_detected, landmarks);
#endif

#ifdef FACE_DETECTION_DNN_CAFFE_OPENCV
			// dnn face detection OpenCV
			Face_Detection_DNN_OpenCV_Caffe_Process(Take_Sample_Frames[i], face_detected);
			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Take_Sample_Frames[i], face_detected, landmarks);
#endif
			
#ifdef FACE_DETECTION_DNN_TENSORFLOW_OPENCV
			// dnn face detection OpenCV
			Face_Detection_DNN_OpenCV_TensorFlow_Process(Take_Sample_Frames[i], face_detected);
			// detecting face landmarks opencv
			Face_Landmark_OpenCV_Detection_Process(Take_Sample_Frames[i], face_detected, landmarks);
#endif

			// estimating eye aspect ratio
			EAR_Feature_Sample[i] = Eye_Aspect_Ratio(landmarks);

			// estimating mouth aspect ratio
			MAR_Feature_Sample[i] = Mouth_Aspect_Ratio(landmarks);

			printf("[INFOR][%d] EAR: %lf\tMAR: %lf\n", i, EAR_Feature_Sample[i], MAR_Feature_Sample[i]);

			EAR_Feature_Sample_Sum += EAR_Feature_Sample[i];
			MAR_Feature_Sample_Sum += MAR_Feature_Sample[i];
		}
	}

	// estimate eye aspect ratio threshold
	EAR_Feature_Threshold = (EAR_Feature_Sample_Sum / (TAKE_SAMPLE_NUM_FRAMES - 1)) - 0.08;
	MAR_Feature_Threshold = (MAR_Feature_Sample_Sum / (TAKE_SAMPLE_NUM_FRAMES - 1)) + 0.1;

	printf("[INFOR] EAR_Threshold: %lf\tMAR_Threshold: %lf\n", EAR_Feature_Threshold, MAR_Feature_Threshold);
	printf("[INFOR]: END TAKE SAMPLE PROCESSING.\n");
}

void Display(void)
{
	// draw face to original frame
	rectangle(Frame_Show, face_detected[0], Scalar(255, 255, 0), 1, 8, 0);

	// draw face landmarks opencv
	Face_Landmark_OpenCV_Draw(Frame_Show, landmarks);

	// display EAR feature on frame
	sprintf(Text_on_Frame, "EAR: %.2lf", EAR_Feature);
	putText(Frame_Show, Text_on_Frame, Point(252, 12), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 2, 8, 0);

	// display MAR feature on frame
	sprintf(Text_on_Frame, "MAR: %.2lf", MAR_Feature);
	putText(Frame_Show, Text_on_Frame, Point(252, 32), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 2, 8, 0);

	// display num of eye blink
	sprintf(Text_on_Frame, "BLINK: %d", Eye_Blink_Count);
	putText(Frame_Show, Text_on_Frame, Point(252, 52), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 2, 8, 0);

	return;
}

//#define CAFFE
//
//const size_t inWidth = 300;
//const size_t inHeight = 300;
//const double inScaleFactor = 1.0;
//const float confidenceThreshold = 0.7f;
//const Scalar meanVal(104.0, 177.0, 123.0);
//
//const string caffeConfigFile = "./models/deploy.prototxt";
//const string caffeWeightFile = "./models/res10_300x300_ssd_iter_140000_fp16.caffemodel";
//
//const string tensorflowConfigFile = "./models/opencv_face_detector.pbtxt";
//const string tensorflowWeightFile = "./models/opencv_face_detector_uint8.pb";
//
//Mat frameOpenCVDNN;
//
//int main()
//{
//	// init camera device
//	Camera_Device_Ini();
//	// init haar cascade facedetection
//	Face_Detection_HaarCascade_Ini();
//	// init face landmark opencv
//	Face_Landmark_OpenCV_Ini();
//
//#ifdef CAFFE
//	Net net = readNetFromCaffe(caffeConfigFile, caffeWeightFile);
//#else
//	Net net = readNetFromTensorflow(tensorflowWeightFile, tensorflowConfigFile);
//#endif
//
//	while (1)
//	{
//		Time_Execution_Start1 = clock();
//
//		cap.read(frameOpenCVDNN);
//
//		//frameOpenCVDNN = ImageProcessing_Face_Detection_HaarCascade(frameOpenCVDNN);
//
//		int frameHeight = frameOpenCVDNN.rows;
//		int frameWidth = frameOpenCVDNN.cols;
//
//		//frameOpenCVDNN = ImageProcessing_Face_Detection_HaarCascade(frameOpenCVDNN);
//
//#ifdef CAFFE
//		Mat inputBlob = blobFromImage(frameOpenCVDNN, inScaleFactor, Size(inWidth, inHeight), meanVal, false, false);
//#else
//		Mat inputBlob = blobFromImage(frameOpenCVDNN, inScaleFactor, Size(inWidth, inHeight), meanVal, true, false);
//#endif
//
//		net.setInput(inputBlob, "data");
//		Mat detection = net.forward("detection_out");
//		Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
//		for (int i = 0; i < detectionMat.rows; i++)
//		{
//			float confidence = detectionMat.at<float>(i, 2);
//			if (confidence > confidenceThreshold)
//			{
//				int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameWidth);
//				int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameHeight);
//				int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameWidth);
//				int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameHeight);
//				rectangle(frameOpenCVDNN, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 2, 4);
//			}
//		}
//
//		Time_Execution_End1 = clock();
//		Time_Processing_per_Frame = double(Time_Execution_End1 - Time_Execution_Start1) / double(CLOCKS_PER_SEC);
//		FPS = 1.0 / Time_Processing_per_Frame;
//
//		// display time processing per frame
//		sprintf(Text_on_Frame, "T: %.3f", double(Time_Processing_per_Frame));
//		putText(frameOpenCVDNN, Text_on_Frame, Point(2, 32), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 2, 8, 0);
//
//		// display FPS on frame
//		sprintf(Text_on_Frame, "FPS: %.2lf", FPS);
//		putText(frameOpenCVDNN, Text_on_Frame, Point(2, 12), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(255, 0, 0), 2, 8, 0);
//
//
//		imshow("test", frameOpenCVDNN);
//		waitKey(1);
//	}
//
//	return 0;
//}