#ifndef __LIGHTING_H__
#define __LIGHTING_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void lighting_init(void);
void lighting_deinit(void);
void lighting_set_lightness(uint32_t level);
uint32_t lighting_get_lightness(void);

#ifdef __cplusplus
}
#endif

#endif /* __LIGHTING_H__ */