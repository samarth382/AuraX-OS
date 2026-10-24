/**
 * ============================================================================
 * FILE 3: src/ui_engine.cpp
 * MODULE: 4-Way Gesture Engine, Watch Face, Clock Studio, Control Center & Wheel
 * ============================================================================
 */

#include "board_pins.h"

static void ui_nav_cb(lv_event_t* e) {
    AuraScreenID dest = (AuraScreenID)(uintptr_t)lv_event_get_user_data(e);
    aurax_switch_screen(dest);
}

// Universal 4-Way Swipe Navigation Handler
static void screen_gesture_event_cb(lv_event_t* e) {
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
    if (dir == LV_DIR_BOTTOM) {
        aurax_switch_screen(SCR_SHADE_CONTROL_CENTER, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 180);
    } else if (dir == LV_DIR_TOP) {
        aurax_switch_screen(SCR_SHADE_NOTIFICATIONS_LIST, LV_SCR_LOAD_ANIM_OVER_TOP, 180);
    } else if (dir == LV_DIR_LEFT) {
        aurax_switch_screen(SCR_LAUNCHER_CYBER_WHEEL, LV_SCR_LOAD_ANIM_OVER_LEFT, 180);
    } else if (dir == LV_DIR_RIGHT) {
        aurax_switch_screen(SCR_HOME_AURA_DIGITAL, LV_SCR_LOAD_ANIM_OVER_RIGHT, 180);
    }
}

void aurax_attach_gesture_nav(lv_obj_t* scr) {
    lv_obj_add_event_cb(scr, screen_gesture_event_cb, LV_EVENT_GESTURE, NULL);
}

// 1. Home Watch Face (Long-press opens Clock Studio Editor)
lv_obj_t* build_home_watchface_screen() {
    lv_obj_t* scr = aurax_create_base_screen();

    char timeBuf[16];
    snprintf(timeBuf, sizeof(timeBuf), "%02u:%02u", g_auraState.hour, g_auraState.minute);

    lv_obj_t* timeLbl = lv_label_create(scr);
    lv_label_set_text(timeLbl, timeBuf);
    lv_obj_set_style_text_font(timeLbl, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(timeLbl, lv_color_hex(AURAX_PALETTE_HEX[g_auraState.accent]), 0);
    lv_obj_align(timeLbl, LV_ALIGN_TOP_MID, 0, 100);

    lv_obj_t* dateLbl = lv_label_create(scr);
    lv_label_set_text(dateLbl, "Thu 22/10/2026");
    lv_obj_set_style_text_font(dateLbl, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(dateLbl, lv_color_hex(0xCCCCCC), 0);
    lv_obj_align(dateLbl, LV_ALIGN_TOP_MID, 0, 160);

    lv_obj_t* compBtn = aurax_create_pill_button(scr, "5,812 Steps  |  78%  |  06:30", 0x151922, 350, 48);
    lv_obj_align(compBtn, LV_ALIGN_BOTTOM_MID, 0, -95);
    lv_obj_add_event_cb(compBtn, ui_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_ACTIVITY_RINGS_TODAY);

    lv_obj_t* menuBtn = aurax_create_pill_button(scr, "Apps Menu", 0x00E5FF, 220, 46);
    lv_obj_align(menuBtn, LV_ALIGN_BOTTOM_MID, 0, -32);
    lv_obj_add_event_cb(menuBtn, ui_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_LAUNCHER_CYBER_WHEEL);

    lv_obj_add_event_cb(scr, ui_nav_cb, LV_EVENT_LONG_PRESSED, (void*)(uintptr_t)SCR_STUDIO_LIVE_EDITOR);
    return scr;
}

// 2. Live Clock Studio Editor (Presets | Font | Colour | Done)
static void cycle_accent_cb(lv_event_t* e) {
    g_auraState.accent = (AuraAccentColor)((g_auraState.accent + 1) % COLOR_COUNT);
    aurax_switch_screen(SCR_STUDIO_LIVE_EDITOR, LV_SCR_LOAD_ANIM_NONE, 0);
}

lv_obj_t* build_studio_editor_screen() {
    lv_obj_t* scr = aurax_create_base_screen();

    lv_obj_t* hdr = lv_label_create(scr);
    lv_label_set_text(hdr, "CLOCK STUDIO LIVE EDITOR");
    lv_obj_set_style_text_color(hdr, lv_color_hex(0xAAAAAA), 0);
    lv_obj_align(hdr, LV_ALIGN_TOP_MID, 0, 24);

    lv_obj_t* preview = lv_label_create(scr);
    lv_label_set_text(preview, "10:42\nThu 22/10/2026");
    lv_obj_set_style_text_font(preview, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_align(preview, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(preview, lv_color_hex(AURAX_PALETTE_HEX[g_auraState.accent]), 0);
    lv_obj_center(preview);

    lv_obj_t* colBtn = aurax_create_pill_button(scr, "Change Neon Colour", AURAX_PALETTE_HEX[g_auraState.accent], 310, 50);
    lv_obj_align(colBtn, LV_ALIGN_BOTTOM_MID, 0, -95);
    lv_obj_add_event_cb(colBtn, cycle_accent_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* doneBtn = aurax_create_pill_button(scr, "Done", 0x00E5FF, 310, 50);
    lv_obj_align(doneBtn, LV_ALIGN_BOTTOM_MID, 0, -32);
    lv_obj_add_event_cb(doneBtn, ui_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_HOME_AURA_DIGITAL);

    return scr;
}

// 3. Control Center Shade
lv_obj_t* build_control_center_screen() {
    lv_obj_t* scr = aurax_create_base_screen();

    lv_obj_t* setBtn = aurax_create_pill_button(scr, "78% Battery   |   Open Settings", 0x161922, 354, 44);
    lv_obj_align(setBtn, LV_ALIGN_TOP_MID, 0, 18);
    lv_obj_add_event_cb(setBtn, ui_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_SETTINGS_MAIN_LIST);

    lv_obj_t* brSlider = lv_slider_create(scr);
    lv_obj_set_size(brSlider, 354, 42);
    lv_slider_set_range(brSlider, 10, 100);
    lv_slider_set_value(brSlider, g_auraState.brightnessPct, LV_ANIM_OFF);
    lv_obj_align(brSlider, LV_ALIGN_TOP_MID, 0, 80);
    lv_obj_set_style_bg_color(brSlider, lv_color_hex(0x00E5FF), LV_PART_INDICATOR);

    const char* tileNames[4] = {"Do Not\nDisturb", "Torch", "Raise to\nWake", "Home"};
    AuraScreenID targets[4] = {
        SCR_SHADE_CONTROL_CENTER,
        SCR_TORCH_ACTIVE_MODE,
        SCR_SHADE_CONTROL_CENTER,
        SCR_HOME_AURA_DIGITAL
    };

    for (uint8_t i = 0; i < 4; i++) {
        lv_obj_t* tile = lv_btn_create(scr);
        lv_obj_set_size(tile, 170, 145);
        lv_obj_set_pos(tile, 28 + (i % 2) * 184, 145 + (i / 2) * 160);
        lv_obj_set_style_radius(tile, 24, 0);
        lv_obj_set_style_bg_color(tile, lv_color_hex((i == 2) ? 0x00E5FF : 0x15181E), 0);

        lv_obj_t* tLbl = lv_label_create(tile);
        lv_label_set_text(tLbl, tileNames[i]);
        lv_obj_set_style_text_align(tLbl, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(tLbl, lv_color_hex((i == 2) ? 0x000000 : 0xFFFFFF), 0);
        lv_obj_center(tLbl);

        lv_obj_add_event_cb(tile, ui_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)targets[i]);
    }
    return scr;
}

// 4. Notifications Shade
lv_obj_t* build_notifications_screen() {
    lv_obj_t* scr = aurax_create_base_screen();

    lv_obj_t* hdr = lv_label_create(scr);
    lv_label_set_text(hdr, "Notifications");
    lv_obj_set_style_text_font(hdr, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(hdr, lv_color_hex(0x00E5FF), 0);
    lv_obj_align(hdr, LV_ALIGN_TOP_MID, 0, 20);

    const char* items[4] = {
        "Battery 20%               09:12",
        "Alarm missed              06:30",
        "Pomodoro done             17:45",
        "Sleep note ready          07:02"
    };

    for (uint8_t i = 0; i < 4; i++) {
        lv_obj_t* card = lv_obj_create(scr);
        lv_obj_set_size(card, 360, 68);
        lv_obj_set_pos(card, 25, 68 + i * 80);
        lv_obj_set_style_radius(card, 18, 0);
        lv_obj_set_style_bg_color(card, lv_color_hex(0x161920), 0);

        lv_obj_t* lbl = lv_label_create(card);
        lv_label_set_text(lbl, items[i]);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), 0);
        lv_obj_center(lbl);
    }

    lv_obj_t* backBtn = aurax_create_pill_button(scr, "Clear All / Back", 0x00E5FF, 280, 48);
    lv_obj_align(backBtn, LV_ALIGN_BOTTOM_MID, 0, -28);
    lv_obj_add_event_cb(backBtn, ui_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_HOME_AURA_DIGITAL);
    return scr;
}

// 5. Torch Screen
lv_obj_t* build_torch_screen() {
    lv_obj_t* scr = aurax_create_base_screen();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xFFFFFF), 0);

    lv_obj_t* timerLbl = lv_label_create(scr);
    lv_label_set_text(timerLbl, "0:47 left\nWhite 600-nit Torch");
    lv_obj_set_style_text_align(timerLbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(timerLbl, lv_color_hex(0x000000), 0);
    lv_obj_center(timerLbl);

    lv_obj_t* offBtn = aurax_create_pill_button(scr, "Turn Off Torch", 0x1A1A1E, 280, 52);
    lv_obj_align(offBtn, LV_ALIGN_BOTTOM_MID, 0, -35);
    lv_obj_add_event_cb(offBtn, ui_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_HOME_AURA_DIGITAL);
    return scr;
}

// 6. Cyber-Wheel & Card Grid App Launchers
lv_obj_t* build_cyber_wheel_launcher() {
    lv_obj_t* scr = aurax_create_base_screen();

    lv_obj_t* hdr = lv_label_create(scr);
    lv_label_set_text(hdr, "AURAX CYBER-WHEEL");
    lv_obj_set_style_text_color(hdr, lv_color_hex(0x00E5FF), 0);
    lv_obj_align(hdr, LV_ALIGN_TOP_MID, 0, 18);

    const char* labels[6] = {
        "Stopwatch",
        "Activity Rings (5,812)",
        "Study Pomodoro (25m)",
        "World Clock",
        "Periodic Table (Fe)",
        "Settings & About"
    };
    AuraScreenID targets[6] = {
        SCR_APP_STOPWATCH_IDLE,
        SCR_ACTIVITY_RINGS_TODAY,
        SCR_POMODORO_ACTIVE_RING,
        SCR_WORLD_CLOCK_LIST,
        SCR_CHEAT_PERIODIC_TABLE,
        SCR_SETTINGS_MAIN_LIST
    };

    for (uint8_t i = 0; i < 6; i++) {
        lv_obj_t* b = aurax_create_pill_button(scr, labels[i], (i == 2) ? 0x00E5FF : 0x151922, 340, 52);
        lv_obj_set_pos(b, 35, 58 + i * 64);
        lv_obj_add_event_cb(b, ui_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)targets[i]);
    }

    lv_obj_t* gridSwitch = aurax_create_pill_button(scr, "2x2 Grid View", 0x7C4DFF, 220, 42);
    lv_obj_align(gridSwitch, LV_ALIGN_BOTTOM_MID, 0, -16);
    lv_obj_add_event_cb(gridSwitch, ui_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_LAUNCHER_CARD_GRID);
    return scr;
}

lv_obj_t* build_card_grid_launcher() {
    lv_obj_t* scr = aurax_create_base_screen();
    const char* apps[4] = {"Stopwatch", "Pomodoro", "World Clock", "Settings"};
    AuraScreenID targets[4] = {
        SCR_APP_STOPWATCH_IDLE,
        SCR_POMODORO_ACTIVE_RING,
        SCR_WORLD_CLOCK_LIST,
        SCR_SETTINGS_MAIN_LIST
    };

    for (uint8_t i = 0; i < 4; i++) {
        lv_obj_t* card = lv_btn_create(scr);
        lv_obj_set_size(card, 170, 175);
        lv_obj_set_pos(card, 28 + (i % 2) * 184, 60 + (i / 2) * 190);
        lv_obj_set_style_radius(card, 26, 0);
        lv_obj_set_style_bg_color(card, lv_color_hex(0x14181F), 0);
        lv_obj_set_style_border_color(card, lv_color_hex(0x00E5FF), 0);
        lv_obj_set_style_border_width(card, 2, 0);

        lv_obj_t* lbl = lv_label_create(card);
        lv_label_set_text(lbl, apps[i]);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), 0);
        lv_obj_center(lbl);

        lv_obj_add_event_cb(card, ui_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)targets[i]);
    }
    return scr;
}

void aurax_ui_init() {}
