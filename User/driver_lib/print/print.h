#ifndef __PRINT_H__
#define __PRINT_H__

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

#include "print_if.h"

#ifdef __cplusplus
extern "C" {
#endif

int snprint(char *buffer, size_t size, const char *format, ...);
void print(const char *format, ...);
int scan(char *buffer, size_t size, uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif /* __PRINT_H__ */
