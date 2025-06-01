#ifndef __LCD_IF_H__
#define __LCD_IF_H__


#ifdef __cplusplus
extern "C" {
#endif

/**
* @param mcu object
*/
#define LCD_mcu_stm32f411ce      1
#define LCD_mcu_esp32s3      0
#if LCD_mcu_stm32f411ce
    /**
    * @param lighting reference
    */
    #include "../lighting/lighting.h"

    #define LCD_if_backlight_init lighting_init
    #define LCD_if_backlight_deinit lighting_deinit
    #define LCD_if_backlight_set_lightness lighting_set_lightness
    #define LCD_if_backlight_get_lightness lighting_get_lightness

    /**
    * @param display_ic object
    */
    #define LCD_display_ic_st7735s      0
    #define LCD_display_ic_gc9a01      1
    #if LCD_display_ic_st7735s
        // ...
    #endif /* LCD_display_ic_st7735s */
    #if LCD_display_ic_gc9a01
        /**
        * @param gc9a01 reference
        */
        #include "../gc9a01/gc9a01.h"

    #endif /* LCD_display_ic_gc9a01 */
#endif /* LCD_mcu_stm32f411ce */
#if LCD_mcu_esp32s3
    // ...
#endif /* LCD_mcu_esp32s3 */

#ifdef __cplusplus
}
#endif

#endif /* __LCD_IF_H__ */
