/**
 * ============================================================================
 * FILE 1: src/board_pins.h
 * PROJECT: AuraX OS v3.5-PRO (410x502 AMOLED Smartwatch Firmware)
 * LANGUAGE: 100% English Watch UI
 * ============================================================================
 */

#ifndef BOARD_PINS_H
#define BOARD_PINS_H

#include <Arduino.h>
#include <lvgl.h>

#define AURAX_SCREEN_WIDTH          410
#define AURAX_SCREEN_HEIGHT         502

// CO5300 QSPI AMOLED Hardware Pins
#define PIN_AMOLED_CS               12
#define PIN_AMOLED_CLK              11
#define PIN_AMOLED_D0               4
#define PIN_AMOLED_D1               5
#define PIN_AMOLED_D2               6
#define PIN_AMOLED_D3               7
#define PIN_AMOLED_RST              13
#define PIN_AMOLED_TE               39

// Sensor & Audio I2C/I2S Pins
#define PIN_I2C_SDA                 15
#define PIN_I2C_SCL                 14
#define PIN_TOUCH_INT               38
#define PIN_TOUCH_RST               9
#define PIN_IMU_INT1                21
#define PIN_RTC_INT                 10
#define PIN_PMU_IRQ                 8

#define PIN_I2S_MCLK                42
#define PIN_I2S_BCLK                41
#define PIN_I2S_WS                  40
#define PIN_I2S_DOUT                2
#define PIN_I2S_DIN                 1
#define PIN_PA_ENABLE               46

#define PIN_SDMMC_CLK               17
#define PIN_SDMMC_CMD               16
#define PIN_SDMMC_D0                18

#define PIN_BTN_CROWN               0
#define PIN_BTN_SIDE_LOWER          45

enum AuraAccentColor : uint8_t {
    COLOR_CYAN = 0,
    COLOR_MAGENTA,
    COLOR_LIME,
    COLOR_AMBER,
    COLOR_RED,
    COLOR_VIOLET,
    COLOR_ORANGE,
    COLOR_WHITE,
    COLOR_COUNT = 8
};

static const uint32_t AURAX_PALETTE_HEX[COLOR_COUNT] = {
    0x00E5FF, 0xE040FB, 0x76FF03, 0xFFC400,
    0xFF5252, 0x7C4DFF, 0xFF6D00, 0xFFFFFF
};

enum AuraClockFont : uint8_t {
    FONT_DIGITAL = 0,
    FONT_ROUNDED,
    FONT_THIN,
    FONT_CYBER
};

enum AuraClockPosition : uint8_t {
    POS_TOP = 0,
    POS_BOTTOM,
    POS_LEFT,
    POS_RIGHT,
    POS_CENTER
};

enum AuraThemeMode : uint8_t {
    THEME_GLASS = 0,
    THEME_NEON,
    THEME_MINIMAL
};

enum AuraMenuStyle : uint8_t {
    MENU_CYBER_WHEEL = 0,
    MENU_CARD_GRID
};

enum AuraScreenID : uint16_t {
    SCR_BOOT_AURAX_LOGO = 0,
    SCR_CRT_SLEEP_LINE,
    SCR_SETUP_1_WELCOME,
    SCR_SETUP_2_TIME_DATE,
    SCR_SETUP_3_TIMEZONE,
    SCR_SETUP_4_BODY_INFO,
    SCR_SETUP_5_STEP_GOAL,
    SCR_SETUP_6_ACCENT_COLOR,
    SCR_SETUP_7_ALL_SET,

    SCR_HOME_AURA_DIGITAL,
    SCR_STUDIO_LIVE_EDITOR,
    SCR_SHADE_CONTROL_CENTER,
    SCR_SHADE_NOTIFICATIONS_LIST,
    SCR_TORCH_ACTIVE_MODE,
    SCR_LAUNCHER_CYBER_WHEEL,
    SCR_LAUNCHER_CARD_GRID,

    SCR_APP_STOPWATCH_IDLE,
    SCR_APP_STOPWATCH_RUNNING,
    SCR_ACTIVITY_RINGS_TODAY,
    SCR_WORLD_CLOCK_LIST,
    SCR_POMODORO_ACTIVE_RING,
    SCR_CHEAT_PERIODIC_TABLE,
    SCR_CHEAT_QUADRATIC_MATH,
    SCR_DECOY_HOMEWORK_LIST,
    SCR_SETTINGS_MAIN_LIST,
    SCR_ABOUT_WATCH_INFO,

    SCR_VAULT_SCRAMBLE_PIN_PAD,
    SCR_VAULT_HOME_GRID_P1,
    SCR_VAULT_INTRUDER_ALERT,
    SCR_SOUNDHUB_SECTIONS_GRID,
    SCR_SOUNDHUB_NOW_PLAYING_3D,
    SCR_SOUNDHUB_TRACK_MENU_MODAL,
    SCR_AURAX_LINK_QR_ACTIVE,

    SCR_POPUP_BATTERY_20_WARN,
    SCR_POPUP_BATTERY_5_CRITICAL,
    SCR_CHARGING_RING_78,
    SCR_POWER_SLIDE_OFF_MODAL,

    SCR_TOTAL_COUNT
};

struct MusicSectionInfo {
    const char* name;
    uint16_t songCount;
    uint32_t barColorHex;
};

static const MusicSectionInfo AURAX_MUSIC_SECTIONS[11] = {
    {"Party",        124, 0xFF6D00},
    {"Punjabi",       98, 0xFFC400},
    {"Rap",           61, 0xFF5252},
    {"Sad",          143, 0x00B0FF},
    {"Romantic",     210, 0xE040FB},
    {"Chill",         77, 0x00E676},
    {"Workout",       52, 0x1DE9B6},
    {"Focus",         40, 0x00E5FF},
    {"Devotional",    30, 0xFFAB00},
    {"Old Classics",  36, 0xB388FF},
    {"Unsorted",      12, 0x78909C}
};

struct WorldCityEntry {
    const char* city;
    const char* offsetLabel;
    const char* sampleTime;
};

static const WorldCityEntry AURAX_WORLD_CITIES[7] = {
    {"New Delhi", "Home",     "10:42"},
    {"London",    "-4:30 h",  "06:12"},
    {"Dubai",     "-1:30 h",  "09:12"},
    {"Singapore", "+2:30 h",  "13:12"},
    {"Tokyo",     "+3:30 h",  "14:12"},
    {"Sydney",    "+5:30 h",  "16:12"},
    {"New York",  "-9:30 h",  "01:12"}
};

struct AuraSystemState {
    uint8_t hour          = 10;
    uint8_t minute        = 42;
    uint8_t second        = 37;
    uint8_t day           = 22;
    uint8_t month         = 10;
    uint16_t year         = 2026;
    bool firstBootDone    = true;

    uint8_t brightnessPct = 60;
    uint8_t timeoutSec    = 15;
    bool dndEnabled       = false;
    bool raiseToWake      = true;
    AuraAccentColor accent = COLOR_CYAN;
    AuraMenuStyle menuStyle = MENU_CYBER_WHEEL;

    uint8_t batteryPct    = 78;
    float batteryVoltage  = 3.94f;
    uint16_t heightCm     = 170;
    uint16_t weightKg     = 65;
    uint32_t todaySteps   = 5812;
    uint32_t stepGoal     = 8000;
    uint16_t burnedKcal   = 330;
    float distanceKm      = 4.1f;

    char masterVaultPin[9] = "2411";
    char decoyVaultPin[9]  = "0000";
    char lastWrongPin[9]   = "4821";
    uint8_t failedPinTries = 2;
    bool vaultUnlocked     = false;

    bool wifiPortalActive  = false;
    const char* wifiSSID   = "AuraX_Watch";
    const char* wifiPass   = "K7m2xQ9d";
    const char* wifiIP     = "192.168.4.1";
};

extern AuraSystemState g_auraState;

void aurax_power_init();
void aurax_ui_init();
void aurax_public_apps_init();
void aurax_vault_init();
void aurax_vault_loop();
void aurax_switch_screen(AuraScreenID targetScreen, lv_scr_load_anim_t animType = LV_SCR_LOAD_ANIM_FADE_ON, uint32_t timeMs = 150);
void aurax_trigger_panic_cover();

lv_obj_t* aurax_create_base_screen();
lv_obj_t* aurax_create_pill_button(lv_obj_t* parent, const char* text, uint32_t bgColorHex, lv_coord_t w, lv_coord_t h);
void aurax_attach_gesture_nav(lv_obj_t* scr);

lv_obj_t* build_boot_logo_screen();
lv_obj_t* build_crt_sleep_screen();
lv_obj_t* build_setup_wizard_screen(uint8_t step);
lv_obj_t* build_power_slide_modal();
lv_obj_t* build_battery_popup_screen(bool isCritical5Pct);
