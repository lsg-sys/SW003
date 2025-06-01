#include "resource_manager.h"


#if RESOURCE_MANAGER_mcu_stm32f411ce
    // ...
    
#endif /* RESOURCE_MANAGER_mcu_stm32f411ce */
#if RESOURCE_MANAGER_mcu_esp32s3
    // ...
#endif /* RESOURCE_MANAGER_mcu_esp32s3 */
#if RESOURCE_MANAGER_mcu_linuxDebug
    #include <semaphore.h>
    #include <assert.h>
    sem_t resource_manager_sem;
    int sem_is_init = 0;
    int interrupts_count = 0;

    int resource_manager_simulate_global_interrupts_init(void) {
        int ret = 0;
        if (!sem_is_init) {
            ret = sem_init(&resource_manager_sem, 0, 1); // pshared=no, value=1
            if (ret == 0) {
                sem_is_init = 1;
            }
        }
        return 0;
    }

    void resource_manager_simulate_global_interrupts_deinit(void) {
        if (sem_is_init) {
            sem_destroy(&resource_manager_sem);
            sem_is_init = 0;
        }
    }

    void resource_manager_simulate_closing_global_interrupts(void) {
        if (0 == interrupts_count) {
            sem_wait(&resource_manager_sem);
        } else {
        }
        interrupts_count++;
    }

    void resource_manager_simulate_opening_global_interrupts(void) {
        if (0 == interrupts_count) {
            assert(sem_is_init && "error: open global interrupts before init semaphore");
            assert(0 && "error");
        } else {
            interrupts_count--;
            if (0 == interrupts_count) {
                sem_post(&resource_manager_sem);
            }
        }
    }


#endif /* RESOURCE_MANAGER_mcu_linuxDebug */
