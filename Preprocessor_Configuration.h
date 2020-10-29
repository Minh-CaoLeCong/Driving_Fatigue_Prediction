#ifndef PREPROCESSOR_CONFIGURATION_H
#define PREPROCESSOR_CONFIGURATION_H

// set color printf
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// face detection method
//#define FACE_DETECTION_HAAR_CASCADE
#define FACE_DETECTION_DNN_CAFFE_OPENCV
//#define FACE_DETECTION_DNN_TENSORFLOW_OPENCV

// face tracking
#define FACE_TRACKING

// save video
#define VIDEO_WRITER
#define VIDEO_WRITER_FPS 10

// save features extraction
//#define FEATURES_EXTRACTION_FILE_TXT

// save sample features
//#define SAMPLE_FEATURES_FILE_TXT

// show image processing
#define FRAME_SHOW_IMAGE_PROCESSING 0	// 1: show image processing

#endif // !PREPROCESSOR_CONFIGURATION_H