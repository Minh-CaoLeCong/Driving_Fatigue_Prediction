#include "Image_Processing.h"

using namespace cv;
using namespace std;

Mat ImageProcessing_Face_Detection_HaarCascade(Mat img)
{
	cvtColor(img, img, COLOR_RGB2GRAY); // convert RGB to GRAY

	equalizeHist(img, img);

	return img;
}