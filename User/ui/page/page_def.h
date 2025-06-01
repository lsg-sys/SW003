#ifndef __PAGE_DEF_H__
#define __PAGE_DEF_H__

#include "driver_lib/lvgl_v9_3_0/lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  {
    void (*create_page_cb)(lv_obj_t *);
    void (*delete_page_cb)(lv_obj_t *);
    lv_event_cb_t key_event_cb;
} app_page_controler_t;

struct PageControler {
    lv_obj_t *(*create_page)(void *args);
    void (*page_created_callback)(void *args);
    void (*delete_page)(void *args);
    lv_event_cb_t key_event_callback;
    lv_screen_load_anim_t anim;
};


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* __PAGE_DEF_H__ */
