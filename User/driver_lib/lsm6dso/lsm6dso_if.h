#ifndef __LSM6DSO_IF_H__
#define __LSM6DSO_IF_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void LSM6DSO_get_sensor_bus_handle(void **handle);

/*
 *   WARNING:
 *   Functions declare in this section are defined at the end of this file
 *   and are strictly related to the hardware platform used.
 *
 */
int32_t LSM6DSO_platform_write(void *handle, uint8_t reg, uint8_t *bufp,
                       uint16_t len);
int32_t LSM6DSO_platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                      uint16_t len);
// void tx_com( uint8_t *tx_buffer, uint16_t len );
void LSM6DSO_platform_delay(uint32_t ms);
void LSM6DSO_platform_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __LSM6DSO_IF_H__ */