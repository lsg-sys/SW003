#include "lighting.h"
#include "lighting_if.h"

inline void lighting_init(void) {
    lighting_if_init();
}

inline void lighting_deinit(void) {
    lighting_if_Deinit();
}

inline void lighting_set_lightness(uint32_t level) {
    lighting_if_set_lightness(level);
}

inline uint32_t lighting_get_lightness(void) {
    return lighting_if_get_lightness();
}

