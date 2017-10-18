#pragma once


#ifndef __COMMONDEF_H__
#define __COMMONDEF_H__


#include "cvut.h"
#include "LogTool.h"
#include "Tracker.h"


//---- messages -----------------------------------------------------------//
#define WM_CALLBACK_PRINT_STATUS            (WM_USER + 0x0001)
#define WM_CALLBACK_DRAW_SIMULATION         (WM_USER + 0x0002)
#define WM_CALLBACK_DRAW_VIDEOFRAME         (WM_USER + 0x0003)
#define WM_CALLBACK_VEHICLE_INFO            (WM_USER + 0x0004)


//---- enumeration --------------------------------------------------------//
// camera source
enum { NONE_SOURCE, VIDEO_CLIP, WEB_CAM };

// camera parameter
enum { NONE_PARAM, PROJECTIVE, HOMOGRAPHY };

// type of KernelTracker
enum { KT_2DC, KT_3DC };

// Segmentation Engine
enum { BASIC_BG_SUB };
//enum {BASIC_BG_SUB, SUB_SENSE};

// shape Fitting Engine
enum { DEFORM_MODEL };

// status level
enum { SL_PRINT, SL_INFO, SL_WARNING, SL_FATAL };


//---- global variables ---------------------------------------------------//


//---- global functions ---------------------------------------------------//
static void printStatus(int level, const char* format, ...)
{
	// get string of variables.
	char buffer[MAX_PATH];
	char log_buffer[MAX_PATH];
	va_list args;

	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	// rectify buffer due to '%' character for dumping log
	int c1 = 0, c2 = 0, bc1, bc2,
		c1_len = int(strlen(buffer)), c2_len = c1_len;
	for (; c1 < c1_len; c1++, c2++) {
		if (buffer[c1] != '%')
			log_buffer[c2] = buffer[c1];
		else {
			c2_len++;
			for (bc1 = c1_len-1, bc2 = c2_len-1; bc1 >= c1; bc1--, bc2--)
				log_buffer[bc2] = buffer[bc1];
			log_buffer[c2] = '%'; c2++;
		}
	}
	log_buffer[c2] = '\0';

	// print log & status
	switch (level) {
	case SL_PRINT:
		LOG_INFO(log_buffer);
		break;
	case SL_INFO:
		LOG_INFO(log_buffer);
		break;
	case SL_WARNING:
		LOG_WARNING(log_buffer);
		break;
	case SL_FATAL:
		LOG_FATAL(log_buffer);
		break;
	default:
		break;
	}
}


#endif // __COMMONDEF_H__
