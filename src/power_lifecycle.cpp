/**
 * ============================================================================
 * FILE 2: src/power_lifecycle.cpp
 * MODULE: Interactive Boot Sequence, 7-Step Setup Wizard & Power Modals
 * ============================================================================
 */

#include "board_pins.h"

AuraSystemState g_auraState;

static void nav_btn_event_cb(lv_event_t* e) {
    AuraScreenID target = (AuraScreenID)(uintptr_t)lv_event_get_user_data(e);
    aurax_switch_screen(target);
}

lv_obj_t* aurax_create_base_screen() {
    lv_obj_t* scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, AURAX_SCREEN_WIDTH, AURAX_SCREEN_HEIGHT);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
    lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
    aurax_attach_gesture_nav(scr);
    return scr;
}

lv_obj_t* aurax_create_pill_button(lv_obj_t* parent, const char* text, uint32_t bgColorHex, lv_coord_t w, lv_coord_t h) {
    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_set_size(btn, w, h);
    lv_obj_set_style_radius(btn, h / 2, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(bgColorHex), 0);
    lv_obj_set_style_shadow_color(btn, lv_color_hex(bgColorHex), 0);
    lv_obj_set_style_shadow_width(btn, 16, 0);
    lv_obj_set_style_shadow_opa(btn, LV_OPA_40, 0);

    lv_obj_t* lbl = lv_label_create(btn);
    lv_label_set_text(lbl, text);
    lv_obj_set_style_text_color(lbl, (bgColorHex == 0x00E5FF) ? lv_color_hex(0x000000) : lv_color_hex(0xFFFFFF), 0);
    lv_obj_center(lbl);
    return btn;
}

lv_obj_t* build_boot_logo_screen() {
    lv_obj_t* scr = aurax_create_base_screen();
    lv_obj_t* logo = lv_label_create(scr);
    lv_label_set_text(logo, "AURAX");
    lv_obj_set_style_text_font(logo, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(logo, lv_color_hex(0x00E5FF), 0);
    lv_obj_center(logo);

    lv_obj_t* sub = lv_label_create(scr);
    lv_label_set_text(sub, "OS v3.5-PRO");
    lv_obj_set_style_text_color(sub, lv_color_hex(0x888888), 0);
    lv_obj_align(sub, LV_ALIGN_CENTER, 0, 36);
    return scr;
}

lv_obj_t* build_crt_sleep_screen() {
    lv_obj_t* scr = aurax_create_base_screen();
    lv_obj_t* line = lv_obj_create(scr);
    lv_obj_set_size(line, 260, 4);
    lv_obj_set_style_bg_color(line, lv_color_hex(0x00E5FF), 0);
    lv_obj_set_style_border_width(line, 0, 0);
    lv_obj_center(line);
    return scr;
}

lv_obj_t* build_setup_wizard_screen(uint8_t step) {
    lv_obj_t* scr = aurax_create_base_screen();
    AuraScreenID nextScr = SCR_HOME_AURA_DIGITAL;

    if (step == 1) {
        nextScr = SCR_SETUP_2_TIME_DATE;
        lv_obj_t* brand = lv_label_create(scr);
        lv_label_set_text(brand, "AURAX");
        lv_obj_set_style_text_font(brand, &lv_font_montserrat_28, 0);
        lv_obj_set_style_text_color(brand, lv_color_hex(0x00E5FF), 0);
        lv_obj_align(brand, LV_ALIGN_TOP_MID, 0, 95);

        lv_obj_t* title = lv_label_create(scr);
        lv_label_set_text(title, "Welcome\nLet's set up your watch");
        lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
        lv_obj_center(title);

        lv_obj_t* btn = aurax_create_pill_button(scr, "Start", 0x00E5FF, 310, 54);
        lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -38);
        lv_obj_add_event_cb(btn, nav_btn_event_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)nextScr);
    } else if (step >= 2 && step <= 6) {
        nextScr = (AuraScreenID)(SCR_SETUP_1_WELCOME + step);
        const char* headers[5] = {
            "SETUP 2 OF 7\nSet time and date",
            "STEP 3 OF 7\nTime zone",
            "STEP 4 OF 7\nAbout you",
            "SETUP 5 OF 7\nDaily step goal",
            "STEP 6 OF 7\nAccent colour"
        };
        const char* bodies[5] = {
            "10 : 42\n22 / 10 / 2026",
            "+05:30\nIndia Standard Time",
            "Height: 170 cm  |  Weight: 65 kg\nUsed for calorie estimates",
            "-   8,000   +\nsteps per day (500 per tap)",
            "Cyan | Magenta | Lime | Amber\nRed | Violet | Orange | White"
        };

        lv_obj_t* hdr = lv_label_create(scr);
        lv_label_set_text(hdr, headers[step - 2]);
        lv_obj_set_style_text_align(hdr, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(hdr, lv_color_hex(0xAAAAAA), 0);
        lv_obj_align(hdr, LV_ALIGN_TOP_MID, 0, 32);

        lv_obj_t* body = lv_label_create(scr);
        lv_label_set_text(body, bodies[step - 2]);
        lv_obj_set_style_text_align(body, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(body, lv_color_hex(0x00E5FF), 0);
        lv_obj_center(body);

        lv_obj_t* btn = aurax_create_pill_button(scr, "Next", 0x00E5FF, 310, 54);
        lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -38);
        lv_obj_add_event_cb(btn, nav_btn_event_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)nextScr);
    } else {
        lv_obj_t* msg = lv_label_create(scr);
        lv_label_set_text(msg, "All set\nYour watch is ready");
        lv_obj_set_style_text_font(msg, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_align(msg, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(msg, lv_color_hex(0xFFFFFF), 0);
        lv_obj_center(msg);

        lv_obj_t* btn = aurax_create_pill_button(scr, "Done", 0x00E5FF, 310, 54);
        lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -38);
        lv_obj_add_event_cb(btn, nav_btn_event_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_HOME_AURA_DIGITAL);
    }
    return scr;
}

lv_obj_t* build_power_slide_modal() {
    lv_obj_t* scr = aurax_create_base_screen();

    lv_obj_t* slider = lv_slider_create(scr);
    lv_obj_set_size(slider, 350, 64);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, -45);
    lv_obj_set_style_bg_color(slider, lv_color_hex(0x281212), LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider, lv_color_hex(0xFF4B4B), LV_PART_INDICATOR);

    lv_obj_t* slLbl = lv_label_create(slider);
    lv_label_set_text(slLbl, "Slide to Power Off");
    lv_obj_set_style_text_color(slLbl, lv_color_hex(0xFFFFFF), 0);
    lv_obj_center(slLbl);

    lv_obj_t* btnRestart = aurax_create_pill_button(scr, "Restart", 0x1E222A, 310, 52);
    lv_obj_align(btnRestart, LV_ALIGN_CENTER, 0, 45);
    lv_obj_add_event_cb(btnRestart, nav_btn_event_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_BOOT_AURAX_LOGO);

    lv_obj_t* btnCancel = aurax_create_pill_button(scr, "Cancel", 0x12141A, 220, 46);
    lv_obj_align(btnCancel, LV_ALIGN_BOTTOM_MID, 0, -35);
    lv_obj_add_event_cb(btnCancel, nav_btn_event_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_HOME_AURA_DIGITAL);

    return scr;
}

lv_obj_t* build_battery_popup_screen(bool isCritical5Pct) {
    lv_obj_t* scr = aurax_create_base_screen();
    lv_obj_t* card = lv_obj_create(scr);
    lv_obj_set_size(card, 340, 280);
    lv_obj_set_style_radius(card, 28, 0);
    lv_obj_set_style_bg_color(card, lv_color_hex(0x12141C), 0);
    lv_obj_set_style_border_color(card, lv_color_hex(isCritical5Pct ? 0xFF4B4B : 0xFFB300), 0);
    lv_obj_set_style_border_width(card, 2, 0);
    lv_obj_center(card);

    lv_obj_t* txt = lv_label_create(card);
    lv_label_set_text(txt, isCritical5Pct ? "Battery 5%\nCharge now" : "Battery 20%\nCharge soon");
    lv_obj_set_style_text_font(txt, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_align(txt, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(txt, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(txt, LV_ALIGN_CENTER, 0, -25);

    lv_obj_t* okBtn = aurax_create_pill_button(card, "OK", 0x00E5FF, 240, 48);
    lv_obj_align(okBtn, LV_ALIGN_BOTTOM_MID, 0, -12);
    lv_obj_add_event_cb(okBtn, nav_btn_event_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_HOME_AURA_DIGITAL);
    return scr;
}

lv_obj_t* build_charging_ring_screen() {
    lv_obj_t* scr = aurax_create_base_screen();
    lv_obj_t* arc = lv_arc_create(scr);
    lv_obj_set_size(arc, 290, 290);
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_arc_set_value(arc, g_auraState.batteryPct);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x00E5FF), LV_PART_INDICATOR);
    lv_obj_center(arc);

    lv_obj_t* lbl = lv_label_create(scr);
    lv_label_set_text(lbl, "78%\nCharging");
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), 0);
    lv_obj_center(lbl);
    return scr;
}

void aurax_power_init() {
    pinMode(PIN_BTN_CROWN, INPUT_PULLUP);
    pinMode(PIN_BTN_SIDE_LOWER, INPUT_PULLUP);
}
