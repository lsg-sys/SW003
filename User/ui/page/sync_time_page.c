#include "sync_time_page.h"

#include "ui/ui_framework.h"

#include "settings_page.h"


lv_group_t * group_roller = NULL;
lv_obj_t * rollers[6] = {NULL};
const char * roller_opts[6] = {
    "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59", /* year */
    "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12", /* month */
    "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31", /* day */
    "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23", /* hour */
    "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59", /* minute */
    "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59", /* second */
};
int current_roller_value[6] = {0};
int const roller_sel_max_val[6] = {59, 11, 30, 23, 59, 59};
int const roller_sel_min_val[6] = {0, 0, 0, 0, 0, 0};

void sync_time_page_create(lv_obj_t* screen)
{
    lv_obj_t * cont = lv_obj_create(screen);
    lv_obj_set_size(cont, lv_pct(100), lv_pct(100));
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /*A style to make the selected option larger*/
    static lv_style_t style_sel;
    lv_style_init(&style_sel);
    lv_style_set_text_font(&style_sel, &lv_font_montserrat_24);
    lv_style_set_bg_color(&style_sel, lv_color_hex3(0xf88));
    lv_style_set_border_width(&style_sel, 2);
    lv_style_set_border_color(&style_sel, lv_color_hex3(0xf00));


    group_roller = lv_group_create();

    date_and_time_t data;
    hardware_api_handler->get_date_and_time(&data);
    data.s.year = data.s.year % 100; /* 20xx -> xx */
    data.s.month = data.s.month - 1; /* 1..12 -> 0..11 */
    data.s.day = data.s.day - 1; /* 1..31 -> 0..30 */
    int visible_rows[6] = {2, 4, 6, 6, 4, 2};
    for (int i = 0; i < 6; i++) {
        rollers[i] = lv_roller_create(cont);
        lv_roller_set_options(rollers[i], roller_opts[i], LV_ROLLER_MODE_NORMAL);
        lv_roller_set_visible_row_count(rollers[i], visible_rows[i]);
        lv_obj_set_width(rollers[i], lv_pct(15));
        lv_obj_add_style(rollers[i], &style_sel, LV_PART_SELECTED);
        // lv_obj_set_style_text_align(rollers[i], LV_TEXT_ALIGN_CENTER, 0);
        current_roller_value[i] = data.ints[5 - i];
        lv_roller_set_selected(rollers[i], current_roller_value[i], LV_ANIM_OFF);

        lv_group_add_obj(group_roller, rollers[i]);
    }
}

void sync_time_page_destroy(lv_obj_t* screen) {

    date_and_time_t data;
    for (int i = 0; i < sizeof(rollers)/sizeof(rollers[0]); i++) {
        data.ints[5 - i] = current_roller_value[i];
    }
    data.s.year += 2000;
    data.s.month += 1;
    data.s.day += 1;
    hardware_api_handler->set_date_and_time(&data);

    lv_group_remove_all_objs(group_roller);
    lv_group_delete(group_roller);
}

void sync_time_page_key_callback(lv_event_t *e) {
    // lv_event_code_t code = lv_event_get_code(e);

    uint32_t key = lv_event_get_key(e);
    switch (key) {
    case LV_KEY_UP:
        {

        lv_obj_t * focused = lv_group_get_focused(group_roller);

        int current_roller_index;
        for (current_roller_index = 0; current_roller_index < 6; current_roller_index++) {
            if (rollers[current_roller_index] == focused) {
                break;
            }
        }

        uint32_t selected = lv_roller_get_selected(focused);
        if (selected < roller_sel_max_val[current_roller_index]) {
            lv_roller_set_selected(focused, selected + 1, LV_ANIM_ON);
            current_roller_value[current_roller_index] = lv_roller_get_selected(focused);
        }

        }
        break;
    case LV_KEY_DOWN:
        {

        lv_obj_t * focused = lv_group_get_focused(group_roller);

        int current_roller_index;
        for (current_roller_index = 0; current_roller_index < 6; current_roller_index++) {
            if (rollers[current_roller_index] == focused) {
                break;
            }
        }

        uint32_t selected = lv_roller_get_selected(focused);
        if (roller_sel_min_val[current_roller_index] < selected) {
            lv_roller_set_selected(focused, selected - 1, LV_ANIM_ON);
            current_roller_value[current_roller_index] = lv_roller_get_selected(focused);
        }

        }
        break;
    case LV_KEY_LEFT:
        lv_group_focus_prev(group_roller);
        break;
    case LV_KEY_RIGHT:
        lv_group_focus_next(group_roller);
        break;
    case LV_KEY_ENTER:
        break;
    case LV_KEY_ESC:
        SmartWatchesConfig_t config;
        hardware_api_handler->get_config(&config);
        hardware_api_handler->get_date_and_time(&config.date_time);
        hardware_api_handler->set_config(&config);
        UIFW_async_switch_to_page(&settings_page);
        break;
    default:
        break;
    }
}

app_page_controler_t const sync_time_page_ctrl = {
    .create_page_cb = sync_time_page_create,
    .delete_page_cb = sync_time_page_destroy,
    .key_event_cb   = sync_time_page_key_callback
};


void sync_time_page_created_callback(void *args) {
    lv_group_focus_obj(rollers[0]);
}

void sync_time_page_destroy_new(void *args) {
    sync_time_page_destroy(NULL);
}

lv_obj_t* sync_time_page_create_new(void *args) {
    lv_obj_t* new_screen = lv_obj_create(NULL);
    sync_time_page_create(new_screen);
    return new_screen;
}

struct PageControler sync_time_page = {
    .create_page             = sync_time_page_create_new,
    .delete_page             = sync_time_page_destroy_new,
    .key_event_callback = sync_time_page_key_callback,
    .page_created_callback   = sync_time_page_created_callback,
    .anim               = LV_SCR_LOAD_ANIM_FADE_IN};