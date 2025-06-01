#ifndef __GC9A01_H__
#define __GC9A01_H__

#include <stdint.h>

#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏
#define ORIENTATION 2   // Set the display orientation 0,1,2,3

#ifdef __cplusplus
extern "C" {
#endif

int gc9a01_init(void);
void gc9a01_deinit(void);

void gc9a01_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void gc9a01_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void gc9a01_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void gc9a01_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
void gc9a01_fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
void gc9a01_draw_picture(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, const uint8_t picture[]);
void gc9a01_draw_buf(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint8_t *buf, uint16_t len);

void gc9a01_enter_sleep(void);
void gc9a01_exit_sleep(void);

#ifdef __cplusplus
}
#endif

#endif /* __GC9A01_H__ */