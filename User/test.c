#include "test.h"
#include "FreeRTOS.h"

void test() {
#if TEST_EN

#if TEST_PRINT
    extern void test_print(void);
    test_print();
#endif /*TEST_PRINT*/

#if TEST_PERIODIC_TIMER
    extern int test_periodic_timer(void);
    configASSERT(test_periodic_timer() == 0);
#endif /*TEST_PERIODIC_TIMER*/

#if TEST_LSM6DSO
    extern int test_lsm6dso(void);
    configASSERT(test_lsm6dso() == 0);
#endif /*TEST_LSM6DSO*/

#if TEST_W25Q128
    extern int test_w25q128(void);
    configASSERT(test_w25q128() == 0);
#endif /*TEST_W25Q128*/

#if TEST_FATFS
    extern int test_fatfs(void);
    configASSERT(test_fatfs() == 0);
#endif /*TEST_FATFS*/

#if TEST_RESOURCE_MANAGER
    extern int test_resource_manager();
    configASSERT(test_resource_manager() == 0);
#endif /*TEST_RESOURCE_MANAGER*/

#endif /*TEST_EN*/
}