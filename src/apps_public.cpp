/**
 * ============================================================================
 * FILE 4: src/apps_public.cpp
 * MODULE: Interactive Public Apps, Study Tools, Settings & 5-Tap Vault Trigger
 * ============================================================================
 */

#include "board_pins.h"

static uint8_t s_aboutTapCounter = 0;

static void app_nav_cb(lv_event_t* e) {
    AuraScreenID target = (AuraScreenID)(uintptr_t)lv_event_get_user_data(e);
    aurax_switch_screen(target);
}

// 1. Stopwatch Screen
lv_obj_t* build_stopwatch_screen(bool running) {
    lv_obj_t* scr = aurax_create_base_screen();

    lv_obj_t* tLbl = lv_label_create(scr);
    lv_label_set_text(tLbl, running ? "01:23.456" : "00:00.000");
    lv_obj_set_style_text_font(tLbl, &lv_font_montserrat_28, 0);
    lv_obj_set_style_text_color(tLbl, lv_color_hex(0x00E5FF), 0);
    lv_obj_align(tLbl, LV_ALIGN_TOP_MID, 0, 55);

    lv_obj_t* laps = lv_label_create(scr);
    lv_label_set_text(laps, running ?
        "Lap 3   00:27.9   01:23.4\nLap 2   00:28.4   00:55.5\nLap 1   00:27.1   00:27.1" :
        "No laps yet");
    lv_obj_set_style_text_color(laps, lv_color_hex(0xCCCCCC), 0);
    lv_obj_center(laps);

    lv_obj_t* btn1 = aurax_create_pill_button(scr, "Home", 0x222630, 160, 52);
    lv_obj_align(btn1, LV_ALIGN_BOTTOM_LEFT, 28, -35);
    lv_obj_add_event_cb(btn1, app_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_HOME_AURA_DIGITAL);

    lv_obj_t* btn2 = aurax_create_pill_button(scr, running ? "Stop" : "Start", running ? 0xFF4B4B : 0x00E5FF, 160, 52);
    lv_obj_align(btn2, LV_ALIGN_BOTTOM_RIGHT, -28, -35);
    lv_obj_add_event_cb(btn2, app_nav_cb, LV_EVENT_CLICKED,
                        (void*)(uintptr_t)(running ? SCR_APP_STOPWATCH_IDLE : SCR_APP_STOPWATCH_RUNNING));
    return scr;
}

// 2. Daily Activity Rings (5,812 Steps | 330 kcal | 4.1 km)
lv_obj_t* build_activity_rings_screen() {
    lv_obj_t* scr = aurax_create_base_screen();

    lv_obj_t* ringOuter = lv_arc_create(scr);
    lv_obj_set_size(ringOuter, 230, 230);
    lv_arc_set_value(ringOuter, 72);
    lv_obj_remove_style(ringOuter, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_color(ringOuter, lv_color_hex(0x00E5FF), LV_PART_INDICATOR);
    lv_obj_align(ringOuter, LV_ALIGN_TOP_MID, 0, 30);

    lv_obj_t* stepsLbl = lv_label_create(scr);
    lv_label_set_text(stepsLbl, "5,812\nsteps\n330 kcal | 4.1 km");
    lv_obj_set_style_text_align(stepsLbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(stepsLbl, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(stepsLbl, LV_ALIGN_TOP_MID, 0, 105);

    lv_obj_t* barTip = lv_label_create(scr);
    lv_label_set_text(barTip, "14:00 - 15:00 : 642 steps\nGoal: 8,000 steps");
    lv_obj_set_style_text_align(barTip, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(barTip, lv_color_hex(0x00E5FF), 0);
    lv_obj_align(barTip, LV_ALIGN_BOTTOM_MID, 0, -90);

    lv_obj_t* backBtn = aurax_create_pill_button(scr, "Back to Watch Face", 0x161922, 280, 46);
    lv_obj_align(backBtn, LV_ALIGN_BOTTOM_MID, 0, -28);
    lv_obj_add_event_cb(backBtn, app_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_HOME_AURA_DIGITAL);
    return scr;
}

// 3. World Clock List
lv_obj_t* build_world_clock_screen() {
    lv_obj_t* scr = aurax_create_base_screen();
    lv_obj_t* hdr = lv_label_create(scr);
    lv_label_set_text(hdr, "World Clock");
    lv_obj_set_style_text_font(hdr, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(hdr, lv_color_hex(0x00E5FF), 0);
    lv_obj_align(hdr, LV_ALIGN_TOP_MID, 0, 18);

    for (uint8_t i = 0; i < 7; i++) {
        lv_obj_t* row = lv_label_create(scr);
        char buf[64];
        snprintf(buf, sizeof(buf), "%-11s (%-7s)   %s",
                 AURAX_WORLD_CITIES[i].city,
                 AURAX_WORLD_CITIES[i].offsetLabel,
                 AURAX_WORLD_CITIES[i].sampleTime);
        lv_label_set_text(row, buf);
        lv_obj_set_style_text_color(row, lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_pos(row, 30, 62 + i * 50);
    }

    lv_obj_t* backBtn = aurax_create_pill_button(scr, "Back", 0x161922, 240, 44);
    lv_obj_align(backBtn, LV_ALIGN_BOTTOM_MID, 0, -22);
    lv_obj_add_event_cb(backBtn, app_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_LAUNCHER_CYBER_WHEEL);
    return scr;
}

// 4. Study Pomodoro Timer (22:41 Study)
lv_obj_t* build_pomodoro_study_screen() {
    lv_obj_t* scr = aurax_create_base_screen();

    lv_obj_t* arc = lv_arc_create(scr);
    lv_obj_set_size(arc, 270, 270);
    lv_arc_set_value(arc, 85);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x00E5FF), LV_PART_INDICATOR);
    lv_obj_align(arc, LV_ALIGN_TOP_MID, 0, 40);

    lv_obj_t* lbl = lv_label_create(scr);
    lv_label_set_text(lbl, "22:41\nStudy Focus");
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(lbl, LV_ALIGN_TOP_MID, 0, 145);

    lv_obj_t* pauseBtn = aurax_create_pill_button(scr, "Notes", 0x00E5FF, 160, 52);
    lv_obj_align(pauseBtn, LV_ALIGN_BOTTOM_LEFT, 28, -35);
    lv_obj_add_event_cb(pauseBtn, app_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_CHEAT_QUADRATIC_MATH);

    lv_obj_t* homeBtn = aurax_create_pill_button(scr, "Home", 0x222630, 160, 52);
    lv_obj_align(homeBtn, LV_ALIGN_BOTTOM_RIGHT, -28, -35);
    lv_obj_add_event_cb(homeBtn, app_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_HOME_AURA_DIGITAL);
    return scr;
}

// 5. Cheat Sheets: Periodic Table (Iron 55.85 Fe) & Quadratic Equations
lv_obj_t* build_periodic_table_screen() {
    lv_obj_t* scr = aurax_create_base_screen();
    lv_obj_t* grid = lv_label_create(scr);
    lv_label_set_text(grid,
        "PERIODIC TABLE\n\n"
        "1 H                            2 He\n"
        "3 Li 4 Be  5 B  6 C  7 N  8 O 10 Ne\n"
        "11 Na 12 Mg 13 Al 14 Si 15 P 17 Ar\n\n"
        "[ 26 Fe : Iron 55.85 ]");
    lv_obj_set_style_text_align(grid, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(grid, lv_color_hex(0x00E5FF), 0);
    lv_obj_align(grid, LV_ALIGN_TOP_MID, 0, 65);

    lv_obj_t* nextBtn = aurax_create_pill_button(scr, "Math Formulas", 0x00E5FF, 280, 48);
    lv_obj_align(nextBtn, LV_ALIGN_BOTTOM_MID, 0, -35);
    lv_obj_add_event_cb(nextBtn, app_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_CHEAT_QUADRATIC_MATH);
    return scr;
}

lv_obj_t* build_quadratic_cheat_screen() {
    lv_obj_t* scr = aurax_create_base_screen();
    lv_obj_t* math = lv_label_create(scr);
    lv_label_set_text(math,
        "Quadratic Equations\n\n"
        "ax^2 + bx + c = 0\n\n"
        "x = (-b +/- sqrt(b^2 - 4ac)) / 2a\n\n"
        "Discriminant D = b^2 - 4ac");
    lv_obj_set_style_text_align(math, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(math, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(math, LV_ALIGN_TOP_MID, 0, 75);

    lv_obj_t* backBtn = aurax_create_pill_button(scr, "Back to Menu", 0x161922, 280, 48);
    lv_obj_align(backBtn, LV_ALIGN_BOTTOM_MID, 0, -35);
    lv_obj_add_event_cb(backBtn, app_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_LAUNCHER_CYBER_WHEEL);
    return scr;
}

// 6. Decoy PIN Homework List (Opens when 0000 is entered)
lv_obj_t* build_decoy_homework_screen() {
    lv_obj_t* scr = aurax_create_base_screen();
    lv_obj_t* list = lv_label_create(scr);
    lv_label_set_text(list,
        "Homework & Revision\n\n"
        "[x] Marathi Grammar Revision\n"
        "[ ] Maths Exercise 4.2\n"
        "[ ] Science Chapter 5 Notes\n"
        "[ ] History Timeline Practice");
    lv_obj_set_style_text_color(list, lv_color_hex(0x00E5FF), 0);
    lv_obj_align(list, LV_ALIGN_TOP_LEFT, 28, 55);

    lv_obj_t* backBtn = aurax_create_pill_button(scr, "Done", 0x161922, 260, 48);
    lv_obj_align(backBtn, LV_ALIGN_BOTTOM_MID, 0, -35);
    lv_obj_add_event_cb(backBtn, app_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_HOME_AURA_DIGITAL);
    return scr;
}

// 7. Settings Main Screen & About Watch (5-Tap Secret Vault Trigger)
lv_obj_t* build_settings_main_screen() {
    lv_obj_t* scr = aurax_create_base_screen();

    lv_obj_t* hdr = lv_label_create(scr);
    lv_label_set_text(hdr, "Settings");
    lv_obj_set_style_text_font(hdr, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(hdr, lv_color_hex(0x00E5FF), 0);
    lv_obj_align(hdr, LV_ALIGN_TOP_MID, 0, 20);

    lv_obj_t* b1 = aurax_create_pill_button(scr, "Clock Studio & Themes", 0x161922, 340, 52);
    lv_obj_set_pos(b1, 35, 75);
    lv_obj_add_event_cb(b1, app_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_STUDIO_LIVE_EDITOR);

    lv_obj_t* b2 = aurax_create_pill_button(scr, "First-Boot Setup Wizard", 0x161922, 340, 52);
    lv_obj_set_pos(b2, 35, 140);
    lv_obj_add_event_cb(b2, app_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_SETUP_1_WELCOME);

    lv_obj_t* b3 = aurax_create_pill_button(scr, "About Watch (Tap 5x inside)", 0x00E5FF, 340, 52);
    lv_obj_set_pos(b3, 35, 205);
    lv_obj_add_event_cb(b3, app_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_ABOUT_WATCH_INFO);

    lv_obj_t* b4 = aurax_create_pill_button(scr, "Power Off / Restart", 0xFF4B4B, 340, 52);
    lv_obj_set_pos(b4, 35, 270);
    lv_obj_add_event_cb(b4, app_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_POWER_SLIDE_OFF_MODAL);

    lv_obj_t* b5 = aurax_create_pill_button(scr, "Back to Home", 0x222630, 260, 46);
    lv_obj_align(b5, LV_ALIGN_BOTTOM_MID, 0, -28);
    lv_obj_add_event_cb(b5, app_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_HOME_AURA_DIGITAL);

    return scr;
}

static void about_version_tap_cb(lv_event_t* e) {
    s_aboutTapCounter++;
    if (s_aboutTapCounter >= 5) {
        s_aboutTapCounter = 0;
        aurax_switch_screen(SCR_VAULT_SCRAMBLE_PIN_PAD);
    }
}

lv_obj_t* build_about_watch_screen() {
    s_aboutTapCounter = 0;
    lv_obj_t* scr = aurax_create_base_screen();
    lv_obj_t* info = lv_label_create(scr);
    lv_label_set_text(info,
        "About Watch\n\n"
        "Chip:     ESP32-S3R8, 240 MHz\n"
        "Memory:   Flash 32 MB, PSRAM 8 MB\n"
        "Card:     32 GB FAT32\n"
        "Version:  AuraX OS v3.5-PRO\n"
        "Battery:  3.94 V, 78%\n\n"
        "(Tap screen 5 times for Vault)");
    lv_obj_set_style_text_color(info, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(info, LV_ALIGN_TOP_LEFT, 28, 40);

    lv_obj_add_flag(scr, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(scr, about_version_tap_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* backBtn = aurax_create_pill_button(scr, "Back", 0x161922, 240, 46);
    lv_obj_align(backBtn, LV_ALIGN_BOTTOM_MID, 0, -28);
    lv_obj_add_event_cb(backBtn, app_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_SETTINGS_MAIN_LIST);
    return scr;
}

void aurax_public_apps_init() {
    s_aboutTapCounter = 0;
}
