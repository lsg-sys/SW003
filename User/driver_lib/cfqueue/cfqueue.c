#include "cfqueue.h"
#include <string.h>

bool CFQueue_IsEmpty(const CFQueue* queue) {
    return queue->count == 0;
}

bool CFQueue_IsFull(const CFQueue* queue) {
    return queue->count == queue->capacity;
}

size_t CFQueue_GetCount(const CFQueue* queue) {
    return queue->count;
}

const void* CFQueue_GetFirst(const CFQueue* queue) {
    if (CFQueue_IsEmpty(queue)) {
        return NULL; // 队列为空时返回 NULL
    }
    return queue->buffer + queue->head * queue->element_size;
}

const void* CFQueue_GetLast(const CFQueue* queue) {
    if (CFQueue_IsEmpty(queue)) {
        return NULL; // 队列为空时返回 NULL
    }
    size_t last_index = (queue->tail == 0) ? queue->capacity - 1 : queue->tail - 1;
    return queue->buffer + last_index * queue->element_size;
}

bool CFQueue_Enqueue(CFQueue* queue, const void* element) {
    if (CFQueue_IsFull(queue)) {
        return false; // 队列已满，无法入队
    }

    memcpy(queue->buffer + queue->tail * queue->element_size, element, queue->element_size);
    queue->tail = (queue->tail + 1) % queue->capacity; // 循环队列
    queue->count++;

    return true;
}

bool CFQueue_Dequeue(CFQueue* queue, void* element) {
    if (CFQueue_IsEmpty(queue)) {
        return false; // 队列为空，无法出队
    }

    memcpy(element, queue->buffer + queue->head * queue->element_size, queue->element_size);
    queue->head = (queue->head + 1) % queue->capacity; // 循环队列
    queue->count--;

    return true;
}
