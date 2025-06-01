#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <stdint.h>
#include "resource_manager_if.h"

#ifdef __cplusplus
extern "C" {
#endif



/* ---------------------------------------------------- */

#define resource_manager_printf(...) do {\
    resource_manager_if_printf("🔄 resource manager: ");\
    resource_manager_if_printf(__VA_ARGS__);\
} while (0) 

/* ---------------------------------------------------- */


// 资源句柄结构体声明（细节隐藏）
typedef struct ResourceNode* ResourceHandle;


/**
 * 注册资源管理
 * @param ResourceCreate 资源创建回调函数
 * @param ResourceDestroy 资源销毁回调函数
 * @param timeout 超时时间(ms)，0表示不使用定时器直接销毁
 * @return 返回资源句柄，用于后续操作
 */
ResourceHandle ResourceManager_RegisterResource(
    void (*ResourceCreate)(void), void (*ResourceDestroy)(void),
    uint32_t timeout);

/**
 * 增加资源依赖
 * @param handle 资源句柄
 */
void ResourceManager_AddDependency(ResourceHandle handle);

/**
 * 减少资源依赖
 * @param handle 资源句柄
 */
void ResourceManager_ReleaseDependency(ResourceHandle handle);

void ResourceManager_ClearTimeout(ResourceHandle handle);
uint32_t ResourceManager_is_free(ResourceHandle handle);

/**
 * 定期调用此函数检查所有注册资源是否超时
 * 应在主循环中周期性调用
 */
void ResourceManager_CheckTimeouts();


int ResourceManager_Init(void);

void ResourceManager_Deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* __RESOURCE_MANAGER_H__ */
