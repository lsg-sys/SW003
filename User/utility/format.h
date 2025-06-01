#ifndef __TIME_FORMAT_H__
#define __TIME_FORMAT_H__

#include <stdint.h>

typedef union {
    char bytes[6 * sizeof(int)];
    int ints[6]; /* seconds, minutes, hours, day, month, year */
    struct {
        int sec;
        int min;
        int hour;
        int day;
        int month;
        int year;
    } s;
} date_and_time_t;

typedef struct {
    date_and_time_t date_time;
    uint32_t        sleep_index;
    uint32_t        brightness;
    uint32_t        theme_color;
    unsigned char   language; /* 0: Simplified Chinese, 1: English */
    unsigned char   theme_dark; /* 0: Light, 1: Dark */
    unsigned char   clock_style; /* 0: s1, 1: s2 */
} SmartWatchesConfig_t;

/* 
"5 seconds\n"
"10 seconds\n"
"30 seconds\n"
"1 minute\n"
"2 minutes\n"
"5 minutes\n"
"10 minutes\n"
"Never" */
extern uint32_t const sleep_index_to_seconds[];

#endif /* __TIME_FORMAT_H__ */
