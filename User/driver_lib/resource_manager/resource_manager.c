#include "resource_manager.h"

#include <stddef.h>
#include <stdlib.h>

#define RESOURCE_MANAGER_RELEASE_MODE_MIDDLE 0
#define RESOURCE_MANAGER_RELEASE_MODE_TIMEOUT 1

enum Resource_state {
    RESOURCE_STATE_FREE = 0,
    RESOURCE_STATE_CREATED = 1,
    RESOURCE_STATE_IDLE = 2
};

struct ResourceNode {
    void (*create)(void);
    void (*destroy)(void);
    uint32_t refCount;
    uint32_t timeout;
    uint32_t release_mode; // 0: middle release, 1: timeout release
    uint32_t expireTime;
    enum Resource_state state;
};


static struct ResourceNode resources[RESOURCE_MANAGER_MAX_RESOURCES]; // 资源数组
static int resourceCount; // 当前已注册资源的数量
static int resources_manager_initialized = 0;


#if RESOURCE_MANAGER_timer_mode_always_on
    // ...
#endif /* RESOURCE_MANAGER_timer_mode_always_on */
#if RESOURCE_MANAGER_timer_mode_resource_manager
    ResourceHandle ResourceManager_timer_handle = NULL;

    TimerHandle_t resrcMana_osTimerHandle;

    void resource_manager_TimerCallback(TimerHandle_t xTimer) {
        ResourceManager_CheckTimeouts(NULL);
    }


    void ResourceManager_enable_timer(void) {
        // 断言，定时器一定能打开。
        if (__get_IPSR() == 0U) {
            configASSERT(pdPASS == xTimerStart(resrcMana_osTimerHandle, 0));
        } else {
            BaseType_t yield = pdFALSE;
            configASSERT(pdPASS == xTimerStartFromISR(resrcMana_osTimerHandle, &yield));
            portYIELD_FROM_ISR(yield);
        }
    }

    void ResourceManager_disable_timer(void) {
        // 断言，定时器一定能关闭。
        if (__get_IPSR() == 0U) {
            configASSERT(pdPASS == xTimerStop(resrcMana_osTimerHandle, 0));
        } else {
            BaseType_t yield = pdFALSE;
            configASSERT(pdPASS == xTimerStopFromISR(resrcMana_osTimerHandle, &yield));
            portYIELD_FROM_ISR(yield);
        }
    }
#endif /* RESOURCE_MANAGER_timer_mode_resource_manager */



ResourceHandle ResourceManager_RegisterResource(
    void (*ResourceCreate)(void), void (*ResourceDestroy)(void),
    uint32_t timeout) {
    
    ResourceHandle ret_handle = NULL;
    
    uint32_t x = DISABLE_INTERRUPTS();
    {
        if (RESOURCE_MANAGER_MAX_RESOURCES <= resourceCount) {
        } else {
            resources[resourceCount].create = ResourceCreate;
            resources[resourceCount].destroy = ResourceDestroy;
            resources[resourceCount].refCount = 0;
            resources[resourceCount].timeout = timeout;
            resources[resourceCount].expireTime = 0;
            if (timeout == 0) {
                resources[resourceCount].release_mode = RESOURCE_MANAGER_RELEASE_MODE_MIDDLE;
            } else {
                resources[resourceCount].release_mode = RESOURCE_MANAGER_RELEASE_MODE_TIMEOUT;
            }
            resources[resourceCount].state = RESOURCE_STATE_FREE;
        
            ret_handle = (ResourceHandle)&resources[resourceCount++];
        
            resource_manager_printf("Register resource (%p)\n", ret_handle);
        }
    }
    ENABLE_INTERRUPTS(x);

    return (ret_handle);
}

uint32_t ResourceManager_is_free(ResourceHandle handle) {
    return (RESOURCE_STATE_FREE == handle->state ? 1 : 0);
}

void ResourceManager_AddDependency(ResourceHandle handle) {
    uint32_t x = DISABLE_INTERRUPTS();
    {
        struct ResourceNode* node = handle;
        if (node->refCount == 0) {

            if (node->state == RESOURCE_STATE_FREE) {

                #if RESOURCE_MANAGER_timer_mode_resource_manager
                    // assert(ResourceManager_timer_handle != NULL);
                    if (ResourceManager_timer_handle != handle) {
                        resource_manager_printf("⏩ add timer dependency\n");
                        ResourceManager_AddDependency(ResourceManager_timer_handle);
                    }
                #endif /* RESOURCE_MANAGER_timer_mode_resource_manager */

                node->create();
                node->state = RESOURCE_STATE_CREATED;

            } else if (node->state == RESOURCE_STATE_IDLE) {

                node->state = RESOURCE_STATE_CREATED;

            } else {
                resource_manager_printf("error： 🟥state\n");
                while (1);
            }

        }
        node->refCount++;
        
        resource_manager_printf("(%p) AddDependency, current ref count %d\n", handle, node->refCount);
    }
    ENABLE_INTERRUPTS(x);
}

void ResourceManager_ReleaseDependency(ResourceHandle handle) {
    uint32_t x = DISABLE_INTERRUPTS();
    {
        struct ResourceNode* node = handle;
        resource_manager_printf("(%p) ReleaseDependency, current ref count %d\n", handle, node->refCount);

        if (0 == node->refCount) {
            // 如果运行到这里，可能是：
            // - 传入了未初始化的handle
            // - AddDependency和ReleaseDependency没有成对使用
            resource_manager_printf("error： 🟥 没有资源可以释放\n");
            while (1);
        } else {
            
            // 减少引用
            node->refCount--;
            // 立刻检查是否已经没有引用了
            if (0 == node->refCount) {
                // 没有引用了，检查是否需要立刻销毁
                if (0 == node->timeout) {
                    // 立刻销毁
                    node->destroy();
                    node->state = RESOURCE_STATE_FREE;
                    #if RESOURCE_MANAGER_timer_mode_resource_manager
                        // assert(ResourceManager_timer_handle != NULL);
                        if (ResourceManager_timer_handle != handle) {
                            resource_manager_printf("⏪ release timer dependency\n");
                            ResourceManager_ReleaseDependency(ResourceManager_timer_handle);
                        }
                    #endif /* RESOURCE_MANAGER_timer_mode_resource_manager */
                } else {
                    // 延迟销毁，计算销毁倒计时，如果在超时前再次加引用，则取消销毁
                    node->expireTime = RESOURCE_MANAGER_get_time_stamp_ms();
                    node->state = RESOURCE_STATE_IDLE;
                    resource_manager_printf("(%p) add expire time %d\n", node, node->expireTime);
                }
            } else{
                // 现在还有引用，不需要销毁
            }
        }
    }
    ENABLE_INTERRUPTS(x);
}

void ResourceManager_ClearTimeout(ResourceHandle handle) {
    uint32_t x = DISABLE_INTERRUPTS();
    {
        struct ResourceNode* node = handle;

        if (0 == node->timeout && 
            RESOURCE_STATE_IDLE == node->state) {
            resource_manager_printf("error： 🟥 ClearTimeout\n");
            while (1);
        } else {
            node->expireTime = RESOURCE_MANAGER_get_time_stamp_ms();
        }
    }
    ENABLE_INTERRUPTS(x);
}

void ResourceManager_CheckTimeouts(void *arg) {
    
    uint32_t x = DISABLE_INTERRUPTS();
    {
        uint32_t now = RESOURCE_MANAGER_get_time_stamp_ms();

        for (int i = 0; i < resourceCount; ++i) {
            struct ResourceNode* node = &resources[i];
            if (node->state == RESOURCE_STATE_IDLE && 
                (now - node->expireTime) >= node->timeout) {


                resource_manager_printf("(%p) Destroy resource on time : %d \n",
                    node , now);
                node->destroy();
                node->state = RESOURCE_STATE_FREE;

                #if RESOURCE_MANAGER_timer_mode_resource_manager
                    // assert(ResourceManager_timer_handle != NULL);
                    if (ResourceManager_timer_handle != (ResourceHandle)node) {
                        resource_manager_printf("⏪ release timer dependency on timer callback\n");
                        ResourceManager_ReleaseDependency(ResourceManager_timer_handle);
                    }
                #endif /* RESOURCE_MANAGER_timer_mode_resource_manager */

            }
        }
    }
    ENABLE_INTERRUPTS(x);
}


int ResourceManager_Init(void) {

    if (resources_manager_initialized) {
        return (0);
    }

    /**
     * 初始化平台相关的临界区保护功能
     */
    #if RESOURCE_MANAGER_mcu_stm32f411ce
        // ...
    #endif /* RESOURCE_MANAGER_mcu_stm32f411ce */
    #if RESOURCE_MANAGER_mcu_esp32s3
        // ...
    #endif /* RESOURCE_MANAGER_mcu_esp32s3 */
    #if RESOURCE_MANAGER_mcu_linuxDebug
        if (0 != resource_manager_simulate_global_interrupts_init()) {
            return (-1);
        }
    #endif /* RESOURCE_MANAGER_mcu_linuxDebug */

    resourceCount = 0;


    // /**
    //  * 初始化提供定时回调的定时器功能。
    //  */
    // if (0 != RESOURCE_MANAGER_timer_init()) {
    //     return (-1);
    // }

    #if RESOURCE_MANAGER_timer_mode_always_on
        resource_manager_printf("start timer always on mode\n");
        // RESOURCE_MANAGER_timer_open(&RESOURCE_MANAGER_timer,
        //     RESOURCE_MANAGER_timer_period_ms,
        //     ResourceManager_CheckTimeouts);
    #endif /* RESOURCE_MANAGER_timer_mode_always_on */
    #if RESOURCE_MANAGER_timer_mode_resource_manager
        resource_manager_printf("start timer resource manager mode\n");
        resrcMana_osTimerHandle = xTimerCreate(
            "resrcManaTimer", 
            pdMS_TO_TICKS(RESOURCE_MANAGER_timer_period_ms), pdTRUE,
			(void * const)(NULL), resource_manager_TimerCallback);
        ResourceManager_timer_handle =  ResourceManager_RegisterResource(
            ResourceManager_enable_timer,
            ResourceManager_disable_timer,
            0 // 超时设置为0，当定时器没有依赖时直接释放。
        );
        if (NULL == ResourceManager_timer_handle) {
            return (-1);
        }
    #endif /* RESOURCE_MANAGER_timer_mode_resource_manager */

    
    resources_manager_initialized = 1;
    return (0);
}

void ResourceManager_Deinit(void) {

    if (!resources_manager_initialized) {
        return ;
    }

    /**
     * 关闭定时器
     */
    #if RESOURCE_MANAGER_timer_mode_always_on
        // RESOURCE_MANAGER_timer_close(&RESOURCE_MANAGER_timer);
    #endif /* RESOURCE_MANAGER_timer_mode_always_on */
    #if RESOURCE_MANAGER_timer_mode_resource_manager
        // ...
        xTimerDelete(resrcMana_osTimerHandle, 0);
        resrcMana_osTimerHandle = NULL;
    #endif /* RESOURCE_MANAGER_timer_mode_resource_manager */

    // RESOURCE_MANAGER_timer_deinit();

    /**
     * 关闭临界区保护功能
     */
    #if RESOURCE_MANAGER_mcu_stm32f411ce
        // ...
    #endif /* RESOURCE_MANAGER_mcu_stm32f411ce */
    #if RESOURCE_MANAGER_mcu_esp32s3
        // ...
    #endif /* RESOURCE_MANAGER_mcu_esp32s3 */
    #if RESOURCE_MANAGER_mcu_linuxDebug
        resource_manager_simulate_global_interrupts_deinit();
    #endif /* RESOURCE_MANAGER_mcu_linuxDebug */


    resources_manager_initialized = 0;
}

/* ---------------------------------------------------------------- */


#include <assert.h>


// 测试用资源回调函数
static int create_called = 0;
static int destroy_called = 0;

void test_resource_create() {
    create_called = 1;
    resource_manager_if_printf("Resource created\n");
}

void test_resource_destroy() {
    destroy_called = 1;
    resource_manager_if_printf("Resource destroyed\n");
}

// 为多资源测试单独定义的回调函数
// 注意：C语言无法直接在循环中定义函数，因此需要预先声明
// 这里用全局变量来模拟不同资源的回调状态
static int created[3] = {0};
static int destroyed[3] = {0};

void test_resource_create_0() { created[0] = 1; }
void test_resource_create_1() { created[1] = 1; }
void test_resource_create_2() { created[2] = 1; }

void test_resource_destroy_0() { destroyed[0] = 1; }
void test_resource_destroy_1() { destroyed[1] = 1; }
void test_resource_destroy_2() { destroyed[2] = 1; }


// 测试用例函数
void test_basic_operations() {
    resource_manager_if_printf("基础测试开始\n");
    // 注册资源
    resource_manager_if_printf("注册一个资源，2秒超时释放资源\n");
    ResourceHandle handle = ResourceManager_RegisterResource(
        test_resource_create, 
        test_resource_destroy, 
        2000); // 2秒超时
    assert(handle != NULL && "注册资源失败");

    // 初始状态
    assert(create_called == 0 && "创建回调被意外调用");
    assert(destroy_called == 0 && "销毁回调被意外调用");

    // 增加依赖
    resource_manager_if_printf("增加依赖\n");
    ResourceManager_AddDependency(handle);
    assert(create_called == 1 && "创建回调未被调用");

    // 再次增加依赖
    resource_manager_if_printf("再次增加依赖\n");
    create_called = 0;
    ResourceManager_AddDependency(handle);
    assert(create_called == 0 && "重复创建被调用");

    // 减少依赖到0
    resource_manager_if_printf("减少依赖\n");
    ResourceManager_ReleaseDependency(handle);
    resource_manager_if_printf("再次减少依赖，预期2秒后销毁资源\n");
    ResourceManager_ReleaseDependency(handle);
    assert(create_called == 0 && "销毁时创建被调用");
    assert(destroy_called == 0 && "销毁回调被过早调用");
    
    // 模拟时间流逝到超时
    resource_manager_if_printf("等待超时，休眠3秒\n");
    resource_manager_if_os_sleep(3); // 等待超时
    resource_manager_if_printf("3秒过去了\n");
    assert(destroy_called == 1 && "超时未触发销毁");
    
    resource_manager_if_printf("资源成功销毁\n");

    resource_manager_if_printf("基础测试通过\n");
}

void test_immediate_destroy() {
    resource_manager_if_printf("立即销毁测试开始\n");
    create_called = 0;
    destroy_called = 0;
    resource_manager_if_printf("注册一个立即销毁的资源\n");
    ResourceHandle handle = ResourceManager_RegisterResource(
        test_resource_create, 
        test_resource_destroy, 
        0); // 立即销毁
    
    resource_manager_if_printf("增加依赖\n");
    ResourceManager_AddDependency(handle);
    assert(create_called == 1 && "创建未被调用");

    create_called = 0;
    resource_manager_if_printf("减少依赖, 预期立即销毁\n");
    ResourceManager_ReleaseDependency(handle);
    assert(destroy_called == 1 && "立即销毁未触发");
    assert(create_called == 0 && "销毁时创建被调用");
    resource_manager_if_printf("销毁成功\n");

    resource_manager_if_printf("立即销毁测试通过\n");
}

void test_multiple_resources() {
    resource_manager_if_printf("多资源测试开始\n");
    ResourceHandle handles[3];
    // 预定义三个资源的回调函数
    void (*creates[])(void) = {
        test_resource_create_0,
        test_resource_create_1,
        test_resource_create_2
    };
    void (*destroys[])(void) = {
        test_resource_destroy_0,
        test_resource_destroy_1,
        test_resource_destroy_2
    };

    // 注册三个资源
    for (int i = 0; i < 3; i++) {
        resource_manager_if_printf("注册第%d个资源，超时时间为1秒\n", i);
        handles[i] = ResourceManager_RegisterResource(
            creates[i], 
            destroys[i], 
            1000);
        assert(handles[i] != NULL);
    }

    // 测试每个资源独立
    for (int i = 0; i < 3; i++) {
        created[i] = 0;
        destroyed[i] = 0;
        resource_manager_if_printf("增加第%d个资源的依赖\n", i);
        ResourceManager_AddDependency(handles[i]);
        assert(created[i] == 1);
    }

    for (int i = 0; i < 3; i++) {
        resource_manager_if_printf("减少第%d个资源的依赖，预期1秒后销毁\n", i);
        ResourceManager_ReleaseDependency(handles[i]);
        assert(destroyed[i] == 0); // 未超时
    }

    // 等待超时
    resource_manager_if_printf("等待超时，休眠2秒\n");
    resource_manager_if_os_sleep(2);
    resource_manager_if_printf("2秒过去了\n");

    for (int i = 0; i < 3; i++) {
        assert(destroyed[i] == 1);
        resource_manager_if_printf("销毁第%d个资源成功\n", i);
    }

    resource_manager_if_printf("多资源测试通过\n");
}


int test_resource_manager(void) {
    if (0 != ResourceManager_Init()) {
        resource_manager_if_printf("资源管理器初始化失败\n");
        return (-1);
    }

    // 运行所有测试
    test_basic_operations();
    test_immediate_destroy();
    test_multiple_resources();

    // 测试资源上限
    for (int i = 0; i < RESOURCE_MANAGER_MAX_RESOURCES; i++) {
        ResourceManager_RegisterResource(test_resource_create, test_resource_destroy, 0);
    }
    assert(ResourceManager_RegisterResource(NULL, NULL, 0) == NULL);
    resource_manager_if_printf("资源上限测试通过\n");

    resource_manager_if_printf("所有测试通过！\n");

    ResourceManager_Deinit();

    return (0);
}
