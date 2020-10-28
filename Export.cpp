#include "Export.h"

using namespace cv;
using namespace std;

#ifdef VIDEO_WRITER
// Define the codec and create VideoWriter object.
VideoWriter video_writer;
bool video_writer_checked = false;
// set FPS
int video_writer_FPS = VIDEO_WRITER_FPS;
string video_writer_path;
string video_writer_filename;
string video_writer_filename_extention = ".avi";
#endif // VIDEO_WRITER

#ifdef FEATURES_EXTRACTION_FILE_TXT
FILE *features_extraction_file_txt;
bool features_extraction_file_txt_checked = false;
string features_extraction_file_txt_path;
string features_extraction_file_txt_filename;
string features_extraction_file_txt_extention = ".txt";
#endif // FEATURES_EXTRACTION_FILE_TXT

string Current_DataTime_Str(void)
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[25];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d_%m_%Y_%H_%M_%S", timeinfo);

	return buffer;
}

#ifdef VIDEO_WRITER
void Ini_Video_Writer(void)
{
	// folder and file name video path
	video_writer_path = "./video/";
	video_writer_filename = Current_DataTime_Str();
	video_writer_path = video_writer_path + video_writer_filename + video_writer_filename_extention;
	// reinit video writer
	video_writer.open(video_writer_path, VideoWriter::fourcc('M', 'J', 'P', 'G'), video_writer_FPS, Size(CAMERA_DEVICE_WIDTH, CAMERA_DEVICE_HEIGHT));
	
	video_writer_checked = true;

	return;
}
#endif

#ifdef FEATURES_EXTRACTION_FILE_TXT
void Ini_Features_Extraction_File_Txt(void)
{
	features_extraction_file_txt_path = "./features_extraction/";
	features_extraction_file_txt_filename = Current_DataTime_Str();
	features_extraction_file_txt_path = features_extraction_file_txt_path + features_extraction_file_txt_filename + features_extraction_file_txt_extention;
	features_extraction_file_txt = fopen(features_extraction_file_txt_path.c_str(), "wt");

	fprintf(features_extraction_file_txt, "frame_count\tEAR_Feature\tMAR_Feature\tEye_Blink_Count\tTime_Processing_per_Frame\tTime_Period_Total\tFPS\n");

	features_extraction_file_txt_checked = true;

	return;
}
#endif