#ifndef EXPORT_H
#define EXPORT_H

#include "Preprocessor_Configuration.h"
#include "OpenCV440.h"
#include <time.h>
#include "Camera_Device.h"

using namespace cv;
using namespace std;

#ifdef VIDEO_WRITER
// Define the codec and create VideoWriter object.
extern VideoWriter video_writer;
extern bool video_writer_checked;
extern int video_writer_FPS;
extern string video_writer_path;
extern string video_writer_filename;
extern string video_writer_filename_extention;
#endif // VIDEO_WRITER

#ifdef FEATURES_EXTRACTION_FILE_TXT
extern FILE *features_extraction_file_txt;
extern bool features_extraction_file_txt_checked;
extern string features_extraction_file_txt_path;
extern string features_extraction_file_txt_filename;
extern string features_extraction_file_txt_extention;
#endif // FEATURES_EXTRACTION_FILE_TXT

#ifdef SAMPLE_FEATURES_FILE_TXT
FILE *sample_features_file_txt;
string sample_features_file_txt_path;
string sample_features_file_txt_filename;
string sample_features_file_txt_extention;
#endif // SAMPLE_FEATURES_FILE_TXT

extern string Current_DataTime_Str(void);

#ifdef VIDEO_WRITER
extern void Ini_Video_Writer(void);
#endif

#ifdef FEATURES_EXTRACTION_FILE_TXT
extern void Ini_Features_Extraction_File_Txt(void);
#endif

#endif // !EXPORT_H