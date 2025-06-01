#ifndef __LCD_dh11d_H__
#define __LCD_dh11d_H__

#include <stdint.h>
#include "lcd_if.h"

#ifdef __cplusplus
extern "C" {
#endif

int lcd_init(void);
void lcd_deinit(void);

void lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcd_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
void lcd_fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
void lcd_draw_picture(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, const uint8_t picture[]);
void lcd_draw_u16_buf(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, const uint16_t buf[], uint16_t len);

void lcd_enter_sleep(void);
void lcd_exit_sleep(void);

void lcd_set_lightness(uint32_t lightness);
uint32_t lcd_get_lightness(void);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_dh11d_H__ */
