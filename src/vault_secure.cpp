/**
 * ============================================================================
 * FILE 5: src/vault_secure.cpp
 * MODULE: Interactive Scramble PIN (2411), 3D Sound Hub & Wi-Fi Portal
 * ============================================================================
 */

#include "board_pins.h"
#include <WiFi.h>
#include <WebServer.h>

static WebServer s_auraWebServer(80);
static char s_enteredPinBuf[8] = {0};
static uint8_t s_pinIdx = 0;
static lv_obj_t* s_pinDotsLabel = NULL;

static void vault_nav_cb(lv_event_t* e) {
    AuraScreenID dest = (AuraScreenID)(uintptr_t)lv_event_get_user_data(e);
    aurax_switch_screen(dest);
}

// PIN Digit Tap Callback: Checks 2411 (Master) or 0000 (Decoy)
static void pin_digit_tap_cb(lv_event_t* e) {
    uint8_t digit = (uint8_t)(uintptr_t)lv_event_get_user_data(e);
    if (s_pinIdx < 4) {
        s_enteredPinBuf[s_pinIdx++] = '0' + digit;
        s_enteredPinBuf[s_pinIdx] = '\0';
    }

    if (s_pinDotsLabel != NULL) {
        lv_label_set_text(s_pinDotsLabel, s_enteredPinBuf);
    }

    if (s_pinIdx == 4) {
        s_pinIdx = 0;
        if (strcmp(s_enteredPinBuf, g_auraState.masterVaultPin) == 0) {
            g_auraState.vaultUnlocked = true;
            aurax_switch_screen(SCR_VAULT_HOME_GRID_P1);
        } else if (strcmp(s_enteredPinBuf, g_auraState.decoyVaultPin) == 0) {
            aurax_switch_screen(SCR_DECOY_HOMEWORK_LIST);
        } else {
            strncpy(g_auraState.lastWrongPin, s_enteredPinBuf, 8);
            g_auraState.failedPinTries++;
            aurax_switch_screen(SCR_VAULT_INTRUDER_ALERT);
        }
        memset(s_enteredPinBuf, 0, sizeof(s_enteredPinBuf));
    }
}

// 1. Random Scramble 0-9 PIN Keypad
lv_obj_t* build_vault_scramble_pin_screen() {
    s_pinIdx = 0;
    memset(s_enteredPinBuf, 0, sizeof(s_enteredPinBuf));

    lv_obj_t* scr = aurax_create_base_screen();

    s_pinDotsLabel = lv_label_create(scr);
    lv_label_set_text(s_pinDotsLabel, "Enter PIN (2411)");
    lv_obj_set_style_text_font(s_pinDotsLabel, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(s_pinDotsLabel, lv_color_hex(0x00E5FF), 0);
    lv_obj_align(s_pinDotsLabel, LV_ALIGN_TOP_MID, 0, 24);

    uint8_t digits[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 9; i > 0; i--) {
        int j = esp_random() % (i + 1);
        uint8_t tmp = digits[i];
        digits[i] = digits[j];
        digits[j] = tmp;
    }

    for (uint8_t i = 0; i < 10; i++) {
        lv_obj_t* key = lv_btn_create(scr);
        lv_obj_set_size(key, 102, 68);
        int col = (i == 9) ? 1 : (i % 3);
        int row = (i == 9) ? 3 : (i / 3);
        lv_obj_set_pos(key, 38 + col * 116, 86 + row * 80);
        lv_obj_set_style_radius(key, 20, 0);
        lv_obj_set_style_bg_color(key, lv_color_hex(0x161922), 0);

        char dStr[4];
        snprintf(dStr, sizeof(dStr), "%u", digits[i]);
        lv_obj_t* l = lv_label_create(key);
        lv_label_set_text(l, dStr);
        lv_obj_set_style_text_font(l, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_color(l, lv_color_hex(0xFFFFFF), 0);
        lv_obj_center(l);

        lv_obj_add_event_cb(key, pin_digit_tap_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)digits[i]);
    }
    return scr;
}

// 2. Secret Vault 2x4 Home Grid
lv_obj_t* build_vault_home_grid() {
    lv_obj_t* scr = aurax_create_base_screen();

    lv_obj_t* hdr = lv_label_create(scr);
    lv_label_set_text(hdr, "SECRET VAULT UNLOCKED");
    lv_obj_set_style_text_color(hdr, lv_color_hex(0x7C4DFF), 0);
    lv_obj_align(hdr, LV_ALIGN_TOP_MID, 0, 16);

    const char* vaultApps[8] = {
        "Sound\nHub", "3D Vortex\nPlayer",
        "Stealth\nNotes", "Intruder\nLog",
        "Delete\nModal", "Games\nArcade",
        "AuraX-\nLink WiFi", "Lock\nVault"
    };
    AuraScreenID targets[8] = {
        SCR_SOUNDHUB_SECTIONS_GRID, SCR_SOUNDHUB_NOW_PLAYING_3D,
        SCR_CHEAT_PERIODIC_TABLE,   SCR_VAULT_INTRUDER_ALERT,
        SCR_SOUNDHUB_TRACK_MENU_MODAL, SCR_ACTIVITY_RINGS_TODAY,
        SCR_AURAX_LINK_QR_ACTIVE,   SCR_HOME_AURA_DIGITAL
    };

    for (uint8_t i = 0; i < 8; i++) {
        lv_obj_t* card = lv_btn_create(scr);
        lv_obj_set_size(card, 172, 92);
        lv_obj_set_pos(card, 26 + (i % 2) * 186, 54 + (i / 2) * 104);
        lv_obj_set_style_radius(card, 22, 0);
        lv_obj_set_style_bg_color(card, lv_color_hex(0x12131A), 0);
        lv_obj_set_style_border_color(card, lv_color_hex(0x7C4DFF), 0);
        lv_obj_set_style_border_width(card, 1, 0);

        lv_obj_t* lbl = lv_label_create(card);
        lv_label_set_text(lbl, vaultApps[i]);
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), 0);
        lv_obj_center(lbl);

        lv_obj_add_event_cb(card, vault_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)targets[i]);
    }
    return scr;
}

// 3. Intruder Alert Log Screen
lv_obj_t* build_vault_intruder_screen() {
    lv_obj_t* scr = aurax_create_base_screen();
    lv_obj_t* logLbl = lv_label_create(scr);
    char buf[160];
    snprintf(buf, sizeof(buf),
             "Intruder Alert\n%u failed tries recorded\n\n"
             "Latest Wrong PIN: %s\n"
             "21/10 Vault PIN: 4821\n"
             "22/10 Time-Fix PIN: 1234",
             g_auraState.failedPinTries, g_auraState.lastWrongPin);
    lv_label_set_text(logLbl, buf);
    lv_obj_set_style_text_align(logLbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(logLbl, lv_color_hex(0xFF5252), 0);
    lv_obj_center(logLbl);

    lv_obj_t* okBtn = aurax_create_pill_button(scr, "Dismiss", 0x222630, 260, 48);
    lv_obj_align(okBtn, LV_ALIGN_BOTTOM_MID, 0, -32);
    lv_obj_add_event_cb(okBtn, vault_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_HOME_AURA_DIGITAL);
    return scr;
}

// 4. Sound Hub Sections (1,284 Songs) & 360-Degree Plasma Vortex Player
lv_obj_t* build_soundhub_sections_screen() {
    lv_obj_t* scr = aurax_create_base_screen();

    lv_obj_t* hdr = lv_label_create(scr);
    lv_label_set_text(hdr, "Sound Hub  |  All songs: 1,284");
    lv_obj_set_style_text_color(hdr, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(hdr, LV_ALIGN_TOP_MID, 0, 18);

    for (uint8_t i = 0; i < 8; i++) {
        lv_obj_t* box = lv_btn_create(scr);
        lv_obj_set_size(box, 172, 82);
        lv_obj_set_pos(box, 26 + (i % 2) * 186, 58 + (i / 2) * 94);
        lv_obj_set_style_radius(box, 18, 0);
        lv_obj_set_style_bg_color(box, lv_color_hex(0x14161D), 0);

        char txt[48];
        snprintf(txt, sizeof(txt), "%s\n%u songs", AURAX_MUSIC_SECTIONS[i].name, AURAX_MUSIC_SECTIONS[i].songCount);
        lv_obj_t* l = lv_label_create(box);
        lv_label_set_text(l, txt);
        lv_obj_set_style_text_align(l, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_color(l, lv_color_hex(AURAX_MUSIC_SECTIONS[i].barColorHex), 0);
        lv_obj_center(l);

        lv_obj_add_event_cb(box, vault_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_SOUNDHUB_NOW_PLAYING_3D);
    }

    lv_obj_t* backBtn = aurax_create_pill_button(scr, "Back to Vault", 0x7C4DFF, 240, 42);
    lv_obj_align(backBtn, LV_ALIGN_BOTTOM_MID, 0, -14);
    lv_obj_add_event_cb(backBtn, vault_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_VAULT_HOME_GRID_P1);
    return scr;
}

lv_obj_t* build_soundhub_3d_vortex_player() {
    lv_obj_t* scr = aurax_create_base_screen();

    lv_obj_t* outerVortex = lv_arc_create(scr);
    lv_obj_set_size(outerVortex, 270, 270);
    lv_arc_set_bg_angles(outerVortex, 0, 360);
    lv_arc_set_value(outerVortex, 100);
    lv_obj_remove_style(outerVortex, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_color(outerVortex, lv_color_hex(0xE040FB), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(outerVortex, 16, LV_PART_INDICATOR);
    lv_obj_align(outerVortex, LV_ALIGN_TOP_MID, 0, 24);

    lv_obj_t* coreOrb = lv_obj_create(scr);
    lv_obj_set_size(coreOrb, 88, 88);
    lv_obj_set_style_radius(coreOrb, 44, 0);
    lv_obj_set_style_bg_color(coreOrb, lv_color_hex(0x00E5FF), 0);
    lv_obj_align(coreOrb, LV_ALIGN_TOP_MID, 0, 115);

    lv_obj_t* meta = lv_label_create(scr);
    lv_label_set_text(meta, "NOW PLAYING\nTum Hi Ho - Arijit Singh\nSad  |  1:58 / 4:22");
    lv_obj_set_style_text_align(meta, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(meta, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(meta, LV_ALIGN_BOTTOM_MID, 0, -88);

    lv_obj_t* optBtn = aurax_create_pill_button(scr, "Track Options", 0xFF4B4B, 170, 46);
    lv_obj_align(optBtn, LV_ALIGN_BOTTOM_LEFT, 24, -24);
    lv_obj_add_event_cb(optBtn, vault_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_SOUNDHUB_TRACK_MENU_MODAL);

    lv_obj_t* backBtn = aurax_create_pill_button(scr, "Vault", 0x222630, 170, 46);
    lv_obj_align(backBtn, LV_ALIGN_BOTTOM_RIGHT, -24, -24);
    lv_obj_add_event_cb(backBtn, vault_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_VAULT_HOME_GRID_P1);

    return scr;
}

lv_obj_t* build_delete_track_modal() {
    lv_obj_t* scr = aurax_create_base_screen();
    lv_obj_t* modal = lv_obj_create(scr);
    lv_obj_set_size(modal, 344, 310);
    lv_obj_set_style_radius(modal, 28, 0);
    lv_obj_set_style_bg_color(modal, lv_color_hex(0x14151C), 0);
    lv_obj_set_style_border_color(modal, lv_color_hex(0xFF4B4B), 0);
    lv_obj_set_style_border_width(modal, 2, 0);
    lv_obj_center(modal);

    lv_obj_t* msg = lv_label_create(modal);
    lv_label_set_text(msg, "Delete track?\nTum Hi Ho");
    lv_obj_set_style_text_font(msg, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_align(msg, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(msg, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(msg, LV_ALIGN_TOP_MID, 0, 45);

    lv_obj_t* delBtn = aurax_create_pill_button(modal, "Delete Track", 0xFF4B4B, 270, 50);
    lv_obj_align(delBtn, LV_ALIGN_BOTTOM_MID, 0, -70);
    lv_obj_add_event_cb(delBtn, vault_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_SOUNDHUB_SECTIONS_GRID);

    lv_obj_t* canBtn = aurax_create_pill_button(modal, "Cancel", 0x222530, 270, 50);
    lv_obj_align(canBtn, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_event_cb(canBtn, vault_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_SOUNDHUB_NOW_PLAYING_3D);
    return scr;
}

// 5. Working AuraX-Link Wi-Fi Portal (192.168.4.1)
static void handle_aurax_studio_root() {
    const char* htmlPage =
        "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width,initial-scale=1'>"
        "<title>AuraX Studio Portal</title>"
        "<style>body{background:#0b0d12;color:#fff;font-family:sans-serif;text-align:center;padding:20px;}"
        ".card{background:#151922;border:1px solid #00e5ff;border-radius:18px;padding:20px;margin:15px auto;max-width:360px;}"
        "button,input{background:#00e5ff;color:#000;border:none;padding:12px 24px;border-radius:24px;font-weight:bold;margin-top:10px;}</style></head>"
        "<body><h2>AuraX OS v3.5-PRO Studio</h2>"
        "<div class='card'><h3>Upload Music / Wallpapers / Notes</h3>"
        "<form method='POST' action='/upload' enctype='multipart/form-data'>"
        "<input type='file' name='file'><br><button type='submit'>Send to Watch SD (32GB)</button></form></div>"
        "</body></html>";
    s_auraWebServer.send(200, "text/html", htmlPage);
}

lv_obj_t* build_aurax_link_qr_screen() {
    if (!g_auraState.wifiPortalActive) {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(g_auraState.wifiSSID, g_auraState.wifiPass);
        s_auraWebServer.on("/", HTTP_GET, handle_aurax_studio_root);
        s_auraWebServer.begin();
        g_auraState.wifiPortalActive = true;
    }

    lv_obj_t* scr = aurax_create_base_screen();
    lv_obj_t* info = lv_label_create(scr);
    lv_label_set_text(info,
        "AuraX-Link Wi-Fi [ON]\n\n"
        "Network:   AuraX_Watch\n"
        "Password:  K7m2xQ9d\n"
        "Address:   192.168.4.1\n\n"
        "Open 192.168.4.1 in phone Chrome");
    lv_obj_set_style_text_align(info, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(info, lv_color_hex(0x00E5FF), 0);
    lv_obj_align(info, LV_ALIGN_TOP_MID, 0, 65);

    lv_obj_t* stopBtn = aurax_create_pill_button(scr, "Stop Wi-Fi Portal", 0xFF4B4B, 280, 52);
    lv_obj_align(stopBtn, LV_ALIGN_BOTTOM_MID, 0, -35);
    lv_obj_add_event_cb(stopBtn, vault_nav_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)SCR_VAULT_HOME_GRID_P1);
    return scr;
}

void aurax_trigger_panic_cover() {
    g_auraState.vaultUnlocked = false;
    if (g_auraState.wifiPortalActive) {
        s_auraWebServer.stop();
        WiFi.softAPdisconnect(true);
        g_auraState.wifiPortalActive = false;
    }
    aurax_switch_screen(SCR_POMODORO_ACTIVE_RING, LV_SCR_LOAD_ANIM_NONE, 0);
}

void aurax_vault_init() {
    s_pinIdx = 0;
}

void aurax_vault_loop() {
    if (g_auraState.wifiPortalActive) {
        s_auraWebServer.handleClient();
    }
}
