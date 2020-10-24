#include "Face_Detection.h"

using namespace cv;
using namespace std;
using namespace cv::dnn;

CascadeClassifier Face_Cascade; // haar cascade face detection

void Face_Detection_HaarCascade_Ini()
{
	// Load the cascade file
	printf("[INFOR]: Initializing Haar Cascade face detection.\n");
	printf("[INFOR]: Loading Haar Cascade .xml file.\n");
	const string HaarCascade_FaceDetection_FileName_Path = "./data/haarcascades/haarcascade_frontalface_default.xml";
	if (!Face_Cascade.load(HaarCascade_FaceDetection_FileName_Path))
	{
		printf("[ERROR]: Could NOT load Haar Cascade xml file.\n");
	}

	return;
}

void Face_Detection_HaarCascade_Process(Mat img, vector<Rect> &face_detected_haar)
{
	Face_Cascade.detectMultiScale(img, face_detected_haar, 1.1, 3, 0, Size(30, 30), Size(150, 150));


	return;
}

#ifdef FACE_DETECTION_DNN_CAFFE_OPENCV
Net net; // dnn face detection OpenCV

void Face_Detection_DNN_OpenCV_Caffe_Ini()
{
	// Load the dnn caffe models
	printf("[INFOR]: Initializing DNN-CAFFE face detection.\n");
	printf("[INFOR]: Loading DNN-CAFFE models.\n");

	const string caffeConfigFile = "./models/deploy.prototxt";
	const string caffeWeightFile = "./models/res10_300x300_ssd_iter_140000_fp16.caffemodel";

	net = readNetFromCaffe(caffeConfigFile, caffeWeightFile);

	return;
}

void Face_Detection_DNN_OpenCV_Caffe_Process(Mat img, vector<Rect> &face_detected_dnn_opencv_caffe)
{
	const size_t inWidth = 300;
	const size_t inHeight = 300;
	const double inScaleFactor = 1.0;
	const float confidenceThreshold = 0.7f;
	const Scalar meanVal(104.0, 177.0, 123.0);

	int imgHeight = img.rows;
	int imgWidth = img.cols;

	Mat inputBlob = blobFromImage(img, inScaleFactor, Size(inWidth, inHeight), meanVal, false, false);

	net.setInput(inputBlob, "data");
	Mat detection = net.forward("detection_out");
	Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

	float confidence = detectionMat.at<float>(0, 2);
	if (confidence > confidenceThreshold)
	{
		int x1 = static_cast<int>(detectionMat.at<float>(0, 3) * imgWidth);
		int y1 = static_cast<int>(detectionMat.at<float>(0, 4) * imgHeight);
		int x2 = static_cast<int>(detectionMat.at<float>(0, 5) * imgWidth);
		int y2 = static_cast<int>(detectionMat.at<float>(0, 6) * imgHeight);
		face_detected_dnn_opencv_caffe[0].x = x1;
		face_detected_dnn_opencv_caffe[0].y = y1;
		face_detected_dnn_opencv_caffe[0].width = x2 - x1;
		face_detected_dnn_opencv_caffe[0].height = y2 - y1;
		/*rectangle(img, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 2, 4);*/
	}

	return;
}
#endif // !FACE_DETECTION_DNN_CAFFE_OPENCV

#ifdef FACE_DETECTION_DNN_TENSORFLOW_OPENCV
Net net; // dnn face detection OpenCV

void Face_Detection_DNN_OpenCV_TensorFlow_Ini()
{
	// Load the dnn tensorflow models
	printf("[INFOR]: Initializing DNN-TENSORFLOW face detection.\n");
	printf("[INFOR]: Loading DNN-TENSORFLOW models.\n");

	const string tensorflowConfigFile = "./models/opencv_face_detector.pbtxt";
	const string tensorflowWeightFile = "./models/opencv_face_detector_uint8.pb";

	net = readNetFromTensorflow(tensorflowWeightFile, tensorflowConfigFile);

	return;
}

void Face_Detection_DNN_OpenCV_TensorFlow_Process(Mat img, vector<Rect> &face_detected_dnn_opencv_tensorflow)
{
	const size_t inWidth = 300;
	const size_t inHeight = 300;
	const double inScaleFactor = 1.0;
	const float confidenceThreshold = 0.7f;
	const Scalar meanVal(104.0, 177.0, 123.0);

	int imgHeight = img.rows;
	int imgWidth = img.cols;

	Mat inputBlob = blobFromImage(img, inScaleFactor, Size(inWidth, inHeight), meanVal, true, false);

	net.setInput(inputBlob, "data");
	Mat detection = net.forward("detection_out");
	Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

	float confidence = detectionMat.at<float>(0, 2);
	if (confidence > confidenceThreshold)
	{
		int x1 = static_cast<int>(detectionMat.at<float>(0, 3) * imgWidth);
		int y1 = static_cast<int>(detectionMat.at<float>(0, 4) * imgHeight);
		int x2 = static_cast<int>(detectionMat.at<float>(0, 5) * imgWidth);
		int y2 = static_cast<int>(detectionMat.at<float>(0, 6) * imgHeight);
		face_detected_dnn_opencv_tensorflow[0].x = x1;
		face_detected_dnn_opencv_tensorflow[0].y = y1;
		face_detected_dnn_opencv_tensorflow[0].width = x2 - x1;
		face_detected_dnn_opencv_tensorflow[0].height = y2 - y1;
		/*rectangle(img, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 2, 4);*/
	}

	return;
}
#endif // !FACE_DETECTION_DNN_TENSORFLOW_OPENCV

//string cascadeName, nestedCascadeName;
//
//void Test_HaarCascade()
//{
//	// VideoCapture class for playing video for which faces to be detected 
//	VideoCapture capture;
//	Mat frame, image;
//
//	// PreDefined trained XML classifiers with facial features 
//	CascadeClassifier cascade, nestedCascade;
//	double scale = 1;
//
//	// Load classifiers from "opencv/data/haarcascades" directory  
//	nestedCascade.load("./data/haarcascades/haarcascade_eye_tree_eyeglasses.xml");
//
//	// Change path before execution  
//	cascade.load("./data/haarcascades/haarcascade_frontalcatface.xml");
//
//	// Start Video..1) 0 for WebCam 2) "Path to Video" for a Local Video 
//	capture.open(0);
//	if (capture.isOpened())
//	{
//		// Capture frames from video and detect faces 
//		cout << "Face Detection Started...." << endl;
//		while (1)
//		{
//			capture >> frame;
//			if (frame.empty())
//				break;
//			Mat frame1 = frame.clone();
//			detectAndDraw(frame1, cascade, nestedCascade, scale);
//			char c = (char)waitKey(10);
//
//			// Press q to exit from window 
//			if (c == 27 || c == 'q' || c == 'Q')
//				break;
//		}
//	}
//	else
//		cout << "Could not Open Camera";
//	return;
//}
//
//void detectAndDraw(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale)
//{
//	vector<Rect> faces, faces2;
//	Mat gray, smallImg;
//
//	cvtColor(img, gray, COLOR_BGR2GRAY); // Convert to Gray Scale 
//	double fx = 1 / scale;
//
//	// Resize the Grayscale Image  
//	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
//	equalizeHist(smallImg, smallImg);
//
//	// Detect faces of different sizes using cascade classifier  
//	cascade.detectMultiScale(smallImg, faces, 1.1,
//		2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
//
//	// Draw circles around the faces 
//	for (size_t i = 0; i < faces.size(); i++)
//	{
//		Rect r = faces[i];
//		Mat smallImgROI;
//		vector<Rect> nestedObjects;
//		Point center;
//		Scalar color = Scalar(255, 0, 0); // Color for Drawing tool 
//		int radius;
//
//		double aspect_ratio = (double)r.width / r.height;
//		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
//		{
//			center.x = cvRound((r.x + r.width*0.5)*scale);
//			center.y = cvRound((r.y + r.height*0.5)*scale);
//			radius = cvRound((r.width + r.height)*0.25*scale);
//			circle(img, center, radius, color, 3, 8, 0);
//		}
//		else
//			rectangle(img, Point(cvRound(r.x*scale), cvRound(r.y*scale)),
//				Point(cvRound((r.x + r.width - 1)*scale),
//					cvRound((r.y + r.height - 1)*scale)), color, 3, 8, 0);
//		if (nestedCascade.empty())
//			continue;
//		smallImgROI = smallImg(r);
//
//		// Detection of eyes int the input image 
//		nestedCascade.detectMultiScale(smallImgROI, nestedObjects, 1.1, 2,
//			0 | CASCADE_SCALE_IMAGE, Size(30, 30));
//
//		// Draw circles around eyes 
//		for (size_t j = 0; j < nestedObjects.size(); j++)
//		{
//			Rect nr = nestedObjects[j];
//			center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
//			center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
//			radius = cvRound((nr.width + nr.height)*0.25*scale);
//			circle(img, center, radius, color, 3, 8, 0);
//		}
//	}
//
//	// Show Processed Image with detected faces 
//	imshow("Face Detection", img);
//}