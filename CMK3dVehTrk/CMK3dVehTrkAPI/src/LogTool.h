#pragma once

#include <stdio.h>
#include <string.h>
#include <stdarg.h>


#define ENABLE_DEBUG_LOG


#ifndef MAX_PATH
#define MAX_PATH       260
#endif


// level of log
enum { INFO, WARNING, FATAL };


extern bool gbEnableLog;
extern FILE* gpLogFile;
extern char gpLogFilePath[MAX_PATH];


void ltEnableLog(const char* log_file_path = "log.txt");

void ltDisableLog();

void ltDumpLogInfo(const char* format, ...);

void ltDumpLogWarning(const char* format, ...);

void ltDumpLogFatal(const char* format, ...);


#define LOG_INFO             ltDumpLogInfo
#define LOG_WARNING          ltDumpLogWarning
#define LOG_FATAL            ltDumpLogFatal

#define LOG_ENABLE           ltEnableLog
#define LOG_DISABLE          ltDisableLog