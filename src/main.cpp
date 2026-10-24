/**
 * ============================================================================
 * FILE 6: src/main.cpp
 * MODULE: Display & Touch Driver Registration, Screen Router & Main Loop
 * ============================================================================
 */

#include "board_pins.h"
#include <Wire.h>

static lv_disp_draw_buf_t s_drawBuf;
static lv_color_t s_buf1[AURAX_SCREEN_WIDTH * 20];
static lv_disp_drv_t s_dispDrv;
static lv_indev_drv_t s_indevDrv;

static uint32_t s_bootTimestamp = 0;
static bool s_bootCompleted = false;

// Display Flush Callback (Bridges LVGL Buffer to CO5300 QSPI AMOLED)
static void aurax_disp_flush_cb(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p) {
    lv_disp_flush_ready(disp);
}

// Touch Read Callback (Bridges FT3168 I2C Capacitive Touch to LVGL)
static void aurax_touch_read_cb(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
    data->state = LV_INDEV_STATE_REL;
}

void aurax_switch_screen(AuraScreenID targetScreen, lv_scr_load_anim_t animType, uint32_t timeMs) {
    lv_obj_t* newScr = NULL;

    switch (targetScreen) {
        case SCR_BOOT_AURAX_LOGO:          newScr = build_boot_logo_screen(); break;
        case SCR_CRT_SLEEP_LINE:           newScr = build_crt_sleep_screen(); break;
        case SCR_SETUP_1_WELCOME:          newScr = build_setup_wizard_screen(1); break;
        case SCR_SETUP_2_TIME_DATE:        newScr = build_setup_wizard_screen(2); break;
        case SCR_SETUP_3_TIMEZONE:         newScr = build_setup_wizard_screen(3); break;
        case SCR_SETUP_4_BODY_INFO:        newScr = build_setup_wizard_screen(4); break;
        case SCR_SETUP_5_STEP_GOAL:        newScr = build_setup_wizard_screen(5); break;
        case SCR_SETUP_6_ACCENT_COLOR:     newScr = build_setup_wizard_screen(6); break;
        case SCR_SETUP_7_ALL_SET:          newScr = build_setup_wizard_screen(7); break;
        case SCR_HOME_AURA_DIGITAL:        newScr = build_home_watchface_screen(); break;
        case SCR_STUDIO_LIVE_EDITOR:       newScr = build_studio_editor_screen(); break;
        case SCR_SHADE_CONTROL_CENTER:     newScr = build_control_center_screen(); break;
        case SCR_SHADE_NOTIFICATIONS_LIST: newScr = build_notifications_screen(); break;
        case SCR_TORCH_ACTIVE_MODE:        newScr = build_torch_screen(); break;
        case SCR_LAUNCHER_CYBER_WHEEL:     newScr = build_cyber_wheel_launcher(); break;
        case SCR_LAUNCHER_CARD_GRID:       newScr = build_card_grid_launcher(); break;
        case SCR_APP_STOPWATCH_IDLE:       newScr = build_stopwatch_screen(false); break;
        case SCR_APP_STOPWATCH_RUNNING:    newScr = build_stopwatch_screen(true); break;
        case SCR_ACTIVITY_RINGS_TODAY:     newScr = build_activity_rings_screen(); break;
        case SCR_WORLD_CLOCK_LIST:         newScr = build_world_clock_screen(); break;
        case SCR_POMODORO_ACTIVE_RING:     newScr = build_pomodoro_study_screen(); break;
        case SCR_CHEAT_PERIODIC_TABLE:     newScr = build_periodic_table_screen(); break;
        case SCR_CHEAT_QUADRATIC_MATH:     newScr = build_quadratic_cheat_screen(); break;
        case SCR_DECOY_HOMEWORK_LIST:      newScr = build_decoy_homework_screen(); break;
        case SCR_SETTINGS_MAIN_LIST:       newScr = build_settings_main_screen(); break;
        case SCR_ABOUT_WATCH_INFO:         newScr = build_about_watch_screen(); break;
        case SCR_VAULT_SCRAMBLE_PIN_PAD:   newScr = build_vault_scramble_pin_screen(); break;
        case SCR_VAULT_HOME_GRID_P1:       newScr = build_vault_home_grid(); break;
        case SCR_VAULT_INTRUDER_ALERT:     newScr = build_vault_intruder_screen(); break;
        case SCR_SOUNDHUB_SECTIONS_GRID:   newScr = build_soundhub_sections_screen(); break;
        case SCR_SOUNDHUB_NOW_PLAYING_3D:  newScr = build_soundhub_3d_vortex_player(); break;
        case SCR_SOUNDHUB_TRACK_MENU_MODAL:newScr = build_delete_track_modal(); break;
        case SCR_AURAX_LINK_QR_ACTIVE:     newScr = build_aurax_link_qr_screen(); break;
        case SCR_POPUP_BATTERY_20_WARN:    newScr = build_battery_popup_screen(false); break;
        case SCR_POPUP_BATTERY_5_CRITICAL: newScr = build_battery_popup_screen(true); break;
        case SCR_CHARGING_RING_78:         newScr = build_charging_ring_screen(); break;
        case SCR_POWER_SLIDE_OFF_MODAL:    newScr = build_power_slide_modal(); break;
        default:                           newScr = build_home_watchface_screen(); break;
    }

    if (newScr != NULL) {
        lv_scr_load_anim(newScr, animType, timeMs, 0, true);
    }
}

void setup() {
    Serial.begin(115200);
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

    lv_init();

    // 1. Register Display Buffer & Driver (Prevents NULL Display Crash)
    lv_disp_draw_buf_init(&s_drawBuf, s_buf1, NULL, AURAX_SCREEN_WIDTH * 20);
    lv_disp_drv_init(&s_dispDrv);
    s_dispDrv.hor_res = AURAX_SCREEN_WIDTH;
    s_dispDrv.ver_res = AURAX_SCREEN_HEIGHT;
    s_dispDrv.flush_cb = aurax_disp_flush_cb;
    s_dispDrv.draw_buf = &s_drawBuf;
    lv_disp_drv_register(&s_dispDrv);

    // 2. Register Capacitive Touch Driver
    lv_indev_drv_init(&s_indevDrv);
    s_indevDrv.type = LV_INDEV_TYPE_POINTER;
    s_indevDrv.read_cb = aurax_touch_read_cb;
    lv_indev_drv_register(&s_indevDrv);

    // 3. Initialize Sub-Modules
    aurax_power_init();
    aurax_ui_init();
    aurax_public_apps_init();
    aurax_vault_init();

    // 4. Show Boot Logo for 2 Seconds
    aurax_switch_screen(SCR_BOOT_AURAX_LOGO, LV_SCR_LOAD_ANIM_NONE, 0);
    s_bootTimestamp = millis();
}

void loop() {
    lv_timer_handler();
    aurax_vault_loop();

    // Auto-advance from AURAX Boot Logo to Home Watch Face after 2000ms
    if (!s_bootCompleted && (millis() - s_bootTimestamp > 2000)) {
        s_bootCompleted = true;
        aurax_switch_screen(SCR_HOME_AURA_DIGITAL, LV_SCR_LOAD_ANIM_FADE_ON, 250);
    }

    // Instant 60ms Hardware Panic Button (Seals Secret Vault -> Opens Pomodoro Study)
    if (digitalRead(PIN_BTN_SIDE_LOWER) == LOW) {
        aurax_trigger_panic_cover();
        delay(200);
    }
    delay(5);
}
