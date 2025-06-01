#include "format.h"

/* 
"5 seconds\n"
"10 seconds\n"
"30 seconds\n"
"1 minute\n"
"2 minutes\n"
"5 minutes\n"
"10 minutes\n"
"Never" */
uint32_t const sleep_index_to_seconds[] = {5, 10, 30, 60, 120, 300, 600, 0xFFFFFFFF};

