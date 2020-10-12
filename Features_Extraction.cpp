#include "Features_Extraction.h"

using namespace cv;
using namespace std;

double Eye_Aspect_Ratio(vector<vector<Point2f>> landmarks)
{
	/// left eye:

	//Distance between P37 and P41
	double P37_41_x = landmarks[0][37].x - landmarks[0][41].x;
	double P37_41_y = landmarks[0][37].y - landmarks[0][41].y;
	//Distance Two point in 2-Dimension: P1(x1, y1), P2(x2, y2) => Equa: d(P1, P2) = sqrt( (x1 - x2)^2 + (y1 - y2)^2 )
	double p37_41_sqrt = sqrt((P37_41_x * P37_41_x) + (P37_41_y * P37_41_y));

	//Distance between P38 and P40
	double P38_40_x = landmarks[0][38].x - landmarks[0][40].x;
	double P38_40_y = landmarks[0][38].y - landmarks[0][40].y;
	//Distance Two point in 2-Dimension: P1(x1, y1), P2(x2, y2) => Equa: d(P1, P2) = sqrt( (x1 - x2)^2 + (y1 - y2)^2 )
	double p38_40_sqrt = sqrt((P38_40_x * P38_40_x) + (P38_40_y * P38_40_y));

	//Distance between P36 and P39
	double P36_39_x = landmarks[0][36].x - landmarks[0][39].x;
	double P36_39_y = landmarks[0][36].y - landmarks[0][39].y;
	//Distance Two point in 2-Dimension: P1(x1, y1), P2(x2, y2) => Equa: d(P1, P2) = sqrt( (x1 - x2)^2 + (y1 - y2)^2 )
	double p36_39_sqrt = sqrt((P36_39_x * P36_39_x) + (P36_39_y * P36_39_y));

	//Eye aspect ratio (EAR) was calculated: EAR = ( |P37 - P41| + |P38 - P40| ) / 2 * |P36 - P39|
	double left_eye_aspect_ratio_value = (p37_41_sqrt + p38_40_sqrt) / (2 * p36_39_sqrt);

	///right eye:

	//Distance between P43 and P47
	double P43_47_x = landmarks[0][43].x - landmarks[0][47].x;
	double P43_47_y = landmarks[0][43].y - landmarks[0][47].y;
	//Distance Two point in 2-Dimension: P1(x1, y1), P2(x2, y2) => Equa: d(P1, P2) = sqrt( (x1 - x2)^2 + (y1 - y2)^2 )
	double p43_47_sqrt = sqrt((P43_47_x*P43_47_x) + (P43_47_y*P43_47_y));

	//Distance between P44 and P46
	double P44_46_x = landmarks[0][44].x - landmarks[0][46].x;
	double P44_46_y = landmarks[0][44].y - landmarks[0][46].y;
	//Distance Two point in 2-Dimension: P1(x1, y1), P2(x2, y2) => Equa: d(P1, P2) = sqrt( (x1 - x2)^2 + (y1 - y2)^2 )
	double p44_46_sqrt = sqrt((P44_46_x*P44_46_x) + (P44_46_y*P44_46_y));

	//Distance between P42 and P45
	double P42_45_x = landmarks[0][42].x - landmarks[0][45].x;
	double P42_45_y = landmarks[0][42].y - landmarks[0][45].y;
	//Distance Two point in 2-Dimension: P1(x1, y1), P2(x2, y2) => Equa: d(P1, P2) = sqrt( (x1 - x2)^2 + (y1 - y2)^2 )
	double p42_45_sqrt = sqrt((P42_45_x*P42_45_x) + (P42_45_y*P42_45_y));

	//Eye aspect ratio (EAR) was calculated: EAR = ( |P37 - P41| + |P38 - P40| ) / 2 * |P36 - P39|
	double right_eye_aspect_ratio_value = (p43_47_sqrt + p44_46_sqrt) / (2 * p42_45_sqrt);

	//Calculate EAR's average from left eye and right eye  
	double Eye_Aspect_Ratio_Value = (left_eye_aspect_ratio_value + right_eye_aspect_ratio_value) / 2;

	return Eye_Aspect_Ratio_Value;
}

double Mouth_Aspect_Ratio(vector<vector<Point2f>> landmarks)
{
	//Distance between P48 and P54
	double P48_54_x = landmarks[0][48].x - landmarks[0][54].x;
	double P48_54_y = landmarks[0][48].y - landmarks[0][54].y;
	//Distance Two point in 2-Dimension: P1(x1, y1), P2(x2, y2) => Equa: d(P1, P2) = sqrt( (x1 - x2)^2 + (y1 - y2)^2 )
	double p48_54_sqrt = sqrt((P48_54_x*P48_54_x) + (P48_54_y*P48_54_y));

	//dis_mouth = p48_54_sqrt * 2;

	//Distance between P50 and P58
	double P50_58_x = landmarks[0][50].x - landmarks[0][58].x;
	double P50_58_y = landmarks[0][50].y - landmarks[0][58].y;
	//Distance Two point in 2-Dimension: P1(x1, y1), P2(x2, y2) => Equa: d(P1, P2) = sqrt( (x1 - x2)^2 + (y1 - y2)^2 )
	double p50_58_sqrt = sqrt((P50_58_x*P50_58_x) + (P50_58_y*P50_58_y));

	//Distance between P52 and P56
	double P52_56_x = landmarks[0][52].x - landmarks[0][56].x;
	double P52_56_y = landmarks[0][52].y - landmarks[0][56].y;
	//Distance Two point in 2-Dimension: P1(x1, y1), P2(x2, y2) => Equa: d(P1, P2) = sqrt( (x1 - x2)^2 + (y1 - y2)^2 )
	double p52_56_sqrt = sqrt((P52_56_x*P52_56_x) + (P52_56_y*P52_56_y));

	//Mouth aspect ratio (MAR) was calculated: EAR = ( |P50 - P58| + |P52 - P56| ) / ( 2 * |P48 - P54| )
	double Mouth_Aspect_Ratio_Value = (p50_58_sqrt + p52_56_sqrt) / (2 * p48_54_sqrt);

	return Mouth_Aspect_Ratio_Value;
}

bool Eye_Blink(double EAR_Feature_Value, double threshold)
{
	static bool Eye_Blink_State = false;

	if ((EAR_Feature_Value < threshold) && (!Eye_Blink_State))
	{
		Eye_Blink_State = true;
		return true;
	}
	else if ((EAR_Feature_Value < threshold) && (Eye_Blink_State))
	{
		return false;
	}
	else
	{
		Eye_Blink_State = false;
		return false;
	}
}