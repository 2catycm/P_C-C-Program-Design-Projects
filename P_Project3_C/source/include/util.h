//
// Created by 叶璨铭 on 2021/10/19.
//

#ifndef P_PROJECT3_C_UTIL_H
#define P_PROJECT3_C_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//功能：提供

#if defined __GNUC__
#define __Matmul_Deprecated
#define Matmul_Deprecated__ __attribute__ ((deprecated))
#elif defined(__MSVC__) 
#define __Matmul_Deprecated __declspec(deprecated)
#define Matmul_Deprecated__
#else
#define __Matmul_Deprecated
#define Matmul_Deprecated__
#endif

//功能：时间处理：1.多线程下保证准确。2.保证在毫秒级别准确。3.跨平台。
#ifdef WIN32
// windows
#include <Windows.h>
#include <stdint.h>
#pragma comment(lib, "winmm.lib ")
DWORD timer_start_time;  // private
DWORD timer_duration;    // public
#define TimerStart (timer_start_time = timeGetTime())
#define TimerEnd (timer_duration = timeGetTime() - timer_start_time)
#define TimerPrintln(msg) printf("%s :%lums\n", msg, timer_duration)
#define TimerEndPrintln(msg) TimerEnd; TimerPrintln(msg)
#define TimerEndPrintlnStart(msg) TimerEndPrintln(msg); TimerStart
#elif __linux__
#include <sys/time.h>
struct timeval timer_start_time;  // private
struct timeval timer_end_time;    // private
double timer_duration;              // public
#define TimerStart gettimeofday(&timer_start_time, NULL);
#define TimerEnd                        \
    gettimeofday(&timer_end_time, NULL); \
    timer_duration = 1000 * (timer_end_time.tv_sec - timer_start_time.tv_sec) + 0.001* (timer_end_time.tv_usec - timer_start_time.tv_usec);
#endif
void abort_with_message(const char* message);
size_t file_len(FILE* file);
//float* file_read_floatPtr(const char* filename);
struct StringBuilder{
    char**stringArray;
    size_t arrayLength;
    //private
    size_t k;
    size_t strSize;
};
struct StringBuilder* _StringBuilder(char** stringArray, size_t arrayLength);
struct StringBuilder* StringBuilder(size_t arrayLength);
struct StringBuilder* append_string(struct StringBuilder* stringBuilder, const char* string);
void fprintf_string_builder(FILE* stream, struct StringBuilder* stringBuilder);
char* stringBuilderToString(struct StringBuilder* stringBuilder);//slow because of strcat is slow. Analyze it in report later.
#endif //P_PROJECT3_C_UTIL_H
