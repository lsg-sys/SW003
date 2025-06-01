#ifndef __TEST_H__
#define __TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_EN 1
#if TEST_EN

#define TEST_PRINT  0
#define TEST_PERIODIC_TIMER 0
#define TEST_W25Q128  0
#define TEST_FATFS  0
#define TEST_LSM6DSO 0
#define TEST_RESOURCE_MANAGER 0

#endif /*TEST*/

void test();

#ifdef __cplusplus
}
#endif

#endif /* __TEST_H__ */