#include "lcd.h"
#include "lcd_if.h"

static uint32_t lcd_current_brightness = 50;

int lcd_init(void) {
    #if LCD_mcu_stm32f411ce
        /**
        * @param display_ic object
        */
        #define LCD_display_ic_st7735s 0
        #define LCD_display_ic_gc9a01 1
        #if LCD_display_ic_st7735s
            // ...
        #endif /* LCD_display_ic_st7735s */
        #if LCD_display_ic_gc9a01
            gc9a01_init();
        #endif /* LCD_display_ic_gc9a01 */
    #endif /* LCD_mcu_stm32f411ce */
    #if LCD_mcu_esp32s3
        // ...
    #endif /* LCD_mcu_esp32s3 */
    
    LCD_if_backlight_init();

    return (0);
}

void lcd_deinit(void) {
    #if LCD_mcu_stm32f411ce
        /**
        * @param display_ic object
        */
        #define LCD_display_ic_st7735s 0
        #define LCD_display_ic_gc9a01 1
        #if LCD_display_ic_st7735s
            // ...
        #endif /* LCD_display_ic_st7735s */
        #if LCD_display_ic_gc9a01
            // ...
        #endif /* LCD_display_ic_gc9a01 */
    #endif /* LCD_mcu_stm32f411ce */
    #if LCD_mcu_esp32s3
        // ...
    #endif /* LCD_mcu_esp32s3 */

    LCD_if_backlight_deinit();
}

void lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    #if LCD_mcu_stm32f411ce
    /**
    * @param display_ic object
    */
    #define LCD_display_ic_st7735s 0
    #define LCD_display_ic_gc9a01 1
    #if LCD_display_ic_st7735s
        // ...
    #endif /* LCD_display_ic_st7735s */
    #if LCD_display_ic_gc9a01
        gc9a01_draw_pixel(x, y, color);
    #endif /* LCD_display_ic_gc9a01 */
    #endif /* LCD_mcu_stm32f411ce */
    #if LCD_mcu_esp32s3
        // ...
    #endif /* LCD_mcu_esp32s3 */
}

void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    
    #if LCD_mcu_stm32f411ce
    /**
    * @param display_ic object
    */
    #define LCD_display_ic_st7735s 0
    #define LCD_display_ic_gc9a01 1
    #if LCD_display_ic_st7735s
        // ...
    #endif /* LCD_display_ic_st7735s */
    #if LCD_display_ic_gc9a01
        gc9a01_draw_line(x1, y1, x2, y2, color);
    #endif /* LCD_display_ic_gc9a01 */
    #endif /* LCD_mcu_stm32f411ce */
    #if LCD_mcu_esp32s3
        // ...
    #endif /* LCD_mcu_esp32s3 */
}

void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    #if LCD_mcu_stm32f411ce
    /**
    * @param display_ic object
    */
    #define LCD_display_ic_st7735s 0
    #define LCD_display_ic_gc9a01 1
    #if LCD_display_ic_st7735s
        // ...
    #endif /* LCD_display_ic_st7735s */
    #if LCD_display_ic_gc9a01
        gc9a01_draw_rectangle(x1, y1, x2, y2, color);
    #endif /* LCD_display_ic_gc9a01 */
    #endif /* LCD_mcu_stm32f411ce */
    #if LCD_mcu_esp32s3
        // ...
    #endif /* LCD_mcu_esp32s3 */
}

void lcd_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color) {
    #if LCD_mcu_stm32f411ce
    /**
    * @param display_ic object
    */
    #define LCD_display_ic_st7735s 0
    #define LCD_display_ic_gc9a01 1
    #if LCD_display_ic_st7735s
        // ...
    #endif /* LCD_display_ic_st7735s */
    #if LCD_display_ic_gc9a01
        gc9a01_draw_circle(x, y, r, color);
    #endif /* LCD_display_ic_gc9a01 */
    #endif /* LCD_mcu_stm32f411ce */
    #if LCD_mcu_esp32s3
        // ...
    #endif /* LCD_mcu_esp32s3 */
}

void lcd_fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color) {
    #if LCD_mcu_stm32f411ce
    /**
    * @param display_ic object
    */
    #define LCD_display_ic_st7735s 0
    #define LCD_display_ic_gc9a01 1
    #if LCD_display_ic_st7735s
        // ...
    #endif /* LCD_display_ic_st7735s */
    #if LCD_display_ic_gc9a01
        gc9a01_fill(xsta, ysta, xend, yend, color);
    #endif /* LCD_display_ic_gc9a01 */
    #endif /* LCD_mcu_stm32f411ce */
    #if LCD_mcu_esp32s3
        // ...
    #endif /* LCD_mcu_esp32s3 */
}

void lcd_draw_picture(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, const uint8_t picture[]) {
    #if LCD_mcu_stm32f411ce
    /**
    * @param display_ic object
    */
    #define LCD_display_ic_st7735s 0
    #define LCD_display_ic_gc9a01 1
    #if LCD_display_ic_st7735s
        // ...
    #endif /* LCD_display_ic_st7735s */
    #if LCD_display_ic_gc9a01
        gc9a01_draw_picture(x, y, width, hight, picture);
    #endif /* LCD_display_ic_gc9a01 */
    #endif /* LCD_mcu_stm32f411ce */
    #if LCD_mcu_esp32s3
        // ...
    #endif /* LCD_mcu_esp32s3 */
}

void lcd_draw_u16_buf(uint16_t x, uint16_t y, uint16_t width, uint16_t hight, const uint16_t buf[], uint16_t len) {
    
    #if LCD_mcu_stm32f411ce
    /**
    * @param display_ic object
    */
    #define LCD_display_ic_st7735s 0
    #define LCD_display_ic_gc9a01 1
    #if LCD_display_ic_st7735s
        // ...
    #endif /* LCD_display_ic_st7735s */
    #if LCD_display_ic_gc9a01
        gc9a01_draw_buf(x, y, width, hight, (uint8_t *)buf, len * sizeof(uint16_t));
    #endif /* LCD_display_ic_gc9a01 */
    #endif /* LCD_mcu_stm32f411ce */
    #if LCD_mcu_esp32s3
        // ...
    #endif /* LCD_mcu_esp32s3 */
}


void lcd_enter_sleep(void) {
    #if LCD_mcu_stm32f411ce

        lcd_current_brightness = LCD_if_backlight_get_lightness();
        LCD_if_backlight_set_lightness(0);
        LCD_if_backlight_deinit();

        /**
        * @param display_ic object
        */
        #define LCD_display_ic_st7735s 0
        #define LCD_display_ic_gc9a01 1
        #if LCD_display_ic_st7735s
            // ...
        #endif /* LCD_display_ic_st7735s */
        #if LCD_display_ic_gc9a01
            gc9a01_enter_sleep();
        #endif /* LCD_display_ic_gc9a01 */
    #endif /* LCD_mcu_stm32f411ce */
    #if LCD_mcu_esp32s3
        // ...
    #endif /* LCD_mcu_esp32s3 */
}

void lcd_exit_sleep(void) {
    #if LCD_mcu_stm32f411ce
        /**
        * @param display_ic object
        */
        #define LCD_display_ic_st7735s 0
        #define LCD_display_ic_gc9a01 1
        #if LCD_display_ic_st7735s
            // ...
        #endif /* LCD_display_ic_st7735s */
        #if LCD_display_ic_gc9a01
            gc9a01_exit_sleep();
        #endif /* LCD_display_ic_gc9a01 */
    #endif /* LCD_mcu_stm32f411ce */
    #if LCD_mcu_esp32s3
        // ...
    #endif /* LCD_mcu_esp32s3 */
    
    LCD_if_backlight_init();
    LCD_if_backlight_set_lightness(lcd_current_brightness);
}


void lcd_set_lightness(uint32_t lightness) {
    LCD_if_backlight_set_lightness(lightness);
}

uint32_t lcd_get_lightness(void) {
    return (LCD_if_backlight_get_lightness());
}