#include "ui_framework.h"

#include "page/clock_page.h"


lv_obj_t                   *current_page_obj     = NULL; // Current page
app_page_controler_t const *current_page_ctrl    = NULL;
hardware_api_t             *hardware_api_handler = NULL;


lv_obj_t * create_app_page(lv_obj_t * screen, app_page_controler_t const *ctrl) {

    lv_obj_t * page = lv_obj_create(screen);
    lv_obj_set_size(page, lv_pct(100), lv_pct(100));
    // lv_obj_set_align(page, LV_ALIGN_TOP_LEFT);
    // lv_obj_set_pos(page, 0, 0);
    lv_obj_set_style_pad_all(page, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(page, 0, LV_PART_MAIN);

    ctrl->create_page_cb(page);

    lv_obj_add_event_cb(page, ctrl->key_event_cb, LV_EVENT_KEY, NULL);

    return page;
}

void delete_app_page(lv_obj_t * page_obj) {

    if (current_page_ctrl->delete_page_cb) {
        current_page_ctrl->delete_page_cb(page_obj);
    }

    lv_obj_delete(page_obj);
}


#include "driver_lib/print/print.h"
void switch_to_page(app_page_controler_t const * new_page_ctrl) {

    lv_obj_t *screen_active = lv_screen_active();
    lv_group_t *p_default_group = lv_group_get_default();
    print("p_default_group : %p", p_default_group);

    if (current_page_obj != NULL) {
        print("obj count of p_default_group before remove current page is %d \n", lv_group_get_obj_count(p_default_group));
        lv_group_remove_all_objs(p_default_group);
        delete_app_page(current_page_obj);
        print("obj count of p_default_group after remove current page is %d \n", lv_group_get_obj_count(p_default_group));
    }

    current_page_obj = create_app_page(screen_active, new_page_ctrl);
    current_page_ctrl = new_page_ctrl;

    print("lvgl number of groups %d \n", lv_group_get_count());

    print("before lv_group_add_obj, group count %d \n", lv_group_get_obj_count(p_default_group));
    lv_group_add_obj(p_default_group, current_page_obj);
    print("after lv_group_add_obj, group count %d \n", lv_group_get_obj_count(p_default_group));

}

struct PageControler const *curr_page_ctrl = NULL;
struct PageControler const *next_page_ctrl = NULL;
int uifw_switch_status = 0; /*  0: no switch, 
                                1: request switch to new page, 
                                2: wait for switch to new page */

void UIFW_async_switch_to_page(struct PageControler const * page_ctrl) {
    if (0 == uifw_switch_status) {
        next_page_ctrl = page_ctrl;
        uifw_switch_status = 1;
    }
}

void UIFW_tick(void) {
    
    /**
     * 页面切换服务，页面内若有切换请求，则会设置标志，检查这些标志，结合当前页面的状态，决定是否切换页面
     */
    static lv_obj_t *old_screen = NULL;
    static lv_obj_t *new_screen = NULL;
    if (1 == uifw_switch_status) {

        if (NULL != curr_page_ctrl) {
            /* 移除输入信号 */
            if (old_screen) {
                lv_obj_remove_event_cb(old_screen, curr_page_ctrl->key_event_callback);
            }
            lv_group_remove_all_objs(lv_group_get_default());

            /* 释放Widget以外的其他资源，断言这些资源可以立即释放，比如定时器/group */
            curr_page_ctrl->delete_page(NULL);
        }
        
        /* 创建下一个页面，获取sreen对象，只创建screen和widget，不创建任何其他资源 */
        new_screen = next_page_ctrl->create_page(NULL);

        /* 切换到新的页面，切换完成后，自动销毁上一个页面 */
        lv_screen_load_anim(new_screen, next_page_ctrl->anim,
                            300, 0, true);
        
        /* 切换状态 */
        uifw_switch_status = 2;

    } else if (2 == uifw_switch_status) {
        
        /* 等待页面切换结束 */
        if (new_screen == lv_screen_active() && false == lv_obj_is_valid(old_screen)) {
            /* 切换完成，执行创建完成回调，申请新的页面资源 */
            next_page_ctrl->page_created_callback(NULL);
            /* 绑定页面的输入设备 */
            lv_obj_add_event_cb(new_screen, next_page_ctrl->key_event_callback, LV_EVENT_KEY, NULL);
            lv_group_add_obj(lv_group_get_default(), new_screen);

            /* 切换状态，等待下一次切换 */
            curr_page_ctrl     = next_page_ctrl;
            old_screen         = new_screen;
            new_screen         = NULL;
            uifw_switch_status = 0;
        }
    }
}

#include "eez/ui/ui.h"
void app_ui_entry(void* indev_keypad, hardware_api_t * p_hw_api) {

    hardware_api_handler = p_hw_api;

    /**
     * 创建一个 group 并立即设置为default，然后绑定输入设备。
     * New Widgets will be added to this group if it's enabled in their class 
     * with add_to_def_group = true.
     */
    lv_group_t *new_group = lv_group_create();
    lv_group_set_default(new_group);
    lv_indev_set_group((lv_indev_t*)indev_keypad, new_group);
    print("new_group as default : %p", new_group);

    SmartWatchesConfig_t config;
    hardware_api_handler->get_config(&config);
    bool is_dark_mode = (0 == config.theme_dark) ? false : true;

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_color_hex(config.theme_color), lv_color_hex(config.theme_color), is_dark_mode, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);

    if (0 == config.clock_style) {
        UIFW_async_switch_to_page(&clock_page);
    } else if (1 == config.clock_style) {
        UIFW_async_switch_to_page(&eez_page);
    } else {
        UIFW_async_switch_to_page(&clock_2_page);
    }
    // ui_init();
}

