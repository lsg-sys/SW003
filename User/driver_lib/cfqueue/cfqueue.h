#ifndef __CFQUEUE_H__
#define __CFQUEUE_H__

/* cfqueue := Circular Fixed-Size Queue  */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 队列结构体定义
 */
typedef struct {
    uint8_t* buffer;       // 队列缓冲区指针
    size_t element_size;   // 每个元素的大小（单位：字节）
    size_t capacity;       // 队列容量（最大元素数量）
    size_t head;           // 队列头部索引
    size_t tail;           // 队列尾部索引
    size_t count;          // 当前队列中的元素数量
} CFQueue;

/**
 * @brief 判断队列是否为空
 * @param queue 队列指针
 * @return 如果队列为空返回 true，否则返回 false
 */
bool CFQueue_IsEmpty(const CFQueue* queue);

/**
 * @brief 判断队列是否已满
 * @param queue 队列指针
 * @return 如果队列已满返回 true，否则返回 false
 */
bool CFQueue_IsFull(const CFQueue* queue);

/**
 * @brief 获取队列中的元素数量
 * @param queue 队列指针
 * @return 队列中的元素数量
 */
size_t CFQueue_GetCount(const CFQueue* queue);

/**
 * @brief 获取第一个元素的指针
 * @param queue 队列指针
 * @return 第一个元素的指针
 */
const void* CFQueue_GetFirst(const CFQueue* queue);

/**
 * @brief 获取最后一个元素的指针
 * @param queue 队列指针
 * @return 最后一个元素的指针
 */
const void* CFQueue_GetLast(const CFQueue* queue);

/**
 * @brief 入队操作
 * @param queue 队列指针
 * @param element 要入队的元素指针
 * @return 成功返回 true，失败返回 false
 */
bool CFQueue_Enqueue(CFQueue* queue, const void* element);

/**
 * @brief 出队操作
 * @param queue 队列指针
 * @param element 用于存储出队元素的指针
 * @return 成功返回 true，失败返回 false
 */
bool CFQueue_Dequeue(CFQueue* queue, void* element);

/**
 * @brief 静态创建队列宏
 * @param _name 队列变量名
 * @param _element_size 每个元素的大小（单位：字节）
 * @param _capacity 队列容量（最大元素数量）
 */
#define CFQUEUE_STATIC_CREATE(_name, _element_size, _capacity) \
    uint8_t _name##_buffer[(_element_size) * (_capacity)];     \
    CFQueue _name = {                                        \
        .buffer       = _name##_buffer,                      \
        .element_size = (_element_size),                     \
        .capacity     = (_capacity),                         \
        .head         = 0,                                  \
        .tail         = 0,                                  \
        .count        = 0}

#ifdef __cplusplus
}
#endif

#endif /* __CFQUEUE_H__ */
