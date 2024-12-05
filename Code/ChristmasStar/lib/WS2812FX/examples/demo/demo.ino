#include "WS2812FX.h"
#include <neopixel.h>

SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D2
#define PIXEL_COUNT 240
#define PIXEL_TYPE WS2812B

WS2812FX ws2812fx = WS2812FX(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

#define TIMER_MS 10000

unsigned long last_change = 0;
unsigned long now = 0;
bool demo_mode = true;

void setup() {
    ws2812fx.init();
    ws2812fx.setBrightness(255);
    ws2812fx.setSpeed(1000);
    ws2812fx.setColor(0x007BFF);
    ws2812fx.setMode(FX_MODE_STATIC);
    ws2812fx.start();
    
    Particle.function("setSpeed", setSpeed);
    Particle.function("incrSpeed", increaseSpeed);
    Particle.function("decrSpeed", decreaseSpeed);
    Particle.function("setBright", setBrightness);
    Particle.function("incrBright", increaseBrightness);
    Particle.function("decrBright", decreaseBrightness);
    Particle.function("setColor", setColor);
    Particle.function("setMode", setMode);
}

void loop() {
    if (demo_mode) {
        now = millis();
        ws2812fx.service();

        if (now - last_change > TIMER_MS) {
            ws2812fx.setMode((ws2812fx.getMode() + 1) % ws2812fx.getModeCount());
            last_change = now;
        }
    } else {
        ws2812fx.service();
    }
}

int setSpeed(String speed_str) {
    uint16_t speed = atoi(speed_str);
    if (speed < 0 || speed > 65535) return -1;
    ws2812fx.setSpeed(speed);
    return 0;
}

int increaseSpeed(String speed_str) {
    uint8_t speed = atoi(speed_str);
    ws2812fx.increaseSpeed(speed);
    return 0;
}

int decreaseSpeed(String speed_str) {
    uint8_t speed = atoi(speed_str);
    ws2812fx.decreaseSpeed(speed);
    return 0;
}

int setBrightness(String brightness_str) {
    uint16_t brightness = atoi(brightness_str);
    if (brightness < 0 || brightness > 255) return -1;
    ws2812fx.setBrightness(brightness);
    return 0;
}

int increaseBrightness(String brightness_str) {
    uint8_t brightness = atoi(brightness_str);
    ws2812fx.increaseBrightness(brightness);
    return 0;
}

int decreaseBrightness(String brightness_str) {
    uint8_t brightness = atoi(brightness_str);
    ws2812fx.decreaseBrightness(brightness);
    return 0;
}

#define RED        0xFF0000
#define GREEN      0x00FF00
#define BLUE       0x0000FF
#define WHITE      0xFFFFFF
#define BLACK      0x000000
#define YELLOW     0xFFFF00
#define CYAN       0x00FFFF
#define MAGENTA    0xFF00FF
#define PURPLE     0x400080
#define ORANGE     0xFF3000
#define ULTRAWHITE 0xFFFFFFFF

int setColor(String color_str) {
    if (color_str == "red") {
        ws2812fx.setColor(RED);
    } else if (color_str == "green") {
        ws2812fx.setColor(GREEN);
    } else if (color_str == "blue") {
        ws2812fx.setColor(BLUE);
    } else if (color_str == "white") {
        ws2812fx.setColor(WHITE);
    } else if (color_str == "black") {
        ws2812fx.setColor(BLACK);
    } else if (color_str == "yellow") {
        ws2812fx.setColor(YELLOW);
    } else if (color_str == "cyan") {
        ws2812fx.setColor(CYAN);
    } else if (color_str == "magenta") {
        ws2812fx.setColor(MAGENTA);
    } else if (color_str == "purple") {
        ws2812fx.setColor(PURPLE);
    } else if (color_str == "orange") {
        ws2812fx.setColor(ORANGE);
    } else if (color_str == "ultrawhite") {
        ws2812fx.setColor(ULTRAWHITE);
    } else {
        return -1;
    }
    return 0;
}

int setMode(String mode_name) {
    uint16_t mode = FX_MODE_STATIC;
    if (mode_name == "Demo") {
        ws2812fx.setMode(mode);
        demo_mode = true;
        return 0;
    } else if (mode_name == "Static") {
        mode = FX_MODE_STATIC;
    } else if (mode_name == "Blink") {
        mode = FX_MODE_BLINK;
    } else if (mode_name == "Color Wipe") {
        mode = FX_MODE_COLOR_WIPE;
    } else if (mode_name == "Color Wipe Inverse") {
        mode = FX_MODE_COLOR_WIPE_INV;
    } else if (mode_name == "Color Wipe Reverse") {
        mode = FX_MODE_COLOR_WIPE_REV;
    } else if (mode_name == "Color Wipe Reverse Inverse") {
        mode = FX_MODE_COLOR_WIPE_REV_INV;
    } else if (mode_name == "Color Wipe Random") {
        mode = FX_MODE_COLOR_WIPE_RANDOM;
    } else if (mode_name == "Random Color") {
        mode = FX_MODE_RANDOM_COLOR;
    } else if (mode_name == "Single Dynamic") {
        mode = FX_MODE_SINGLE_DYNAMIC;
    } else if (mode_name == "Multi Dynamic") {
        mode = FX_MODE_MULTI_DYNAMIC;
    } else if (mode_name == "Rainbow") {
        mode = FX_MODE_RAINBOW;
    } else if (mode_name == "Rainbow Cycle") {
        mode = FX_MODE_RAINBOW_CYCLE;
    } else if (mode_name == "Scan") {
        mode = FX_MODE_SCAN;
    } else if (mode_name == "Dual Scan") {
        mode = FX_MODE_DUAL_SCAN;
    } else if (mode_name == "Fade") {
        mode = FX_MODE_FADE;
    } else if (mode_name == "Theater Chase") {
        mode = FX_MODE_THEATER_CHASE;
    } else if (mode_name == "Theater Chase Rainbow") {
        mode = FX_MODE_THEATER_CHASE_RAINBOW;
    } else if (mode_name == "Running Lights") {
        mode = FX_MODE_RUNNING_LIGHTS;
    } else if (mode_name == "Twinkle") {
        mode = FX_MODE_TWINKLE;
    } else if (mode_name == "Twinkle Random") {
        mode = FX_MODE_TWINKLE_RANDOM;
    } else if (mode_name == "Twinkle Fade") {
        mode = FX_MODE_TWINKLE_FADE;
    } else if (mode_name == "Twinkle Fade Random") {
        mode = FX_MODE_TWINKLE_FADE_RANDOM;
    } else if (mode_name == "Sparkle") {
        mode = FX_MODE_SPARKLE;
    } else if (mode_name == "Flash Sparkle") {
        mode = FX_MODE_FLASH_SPARKLE;
    } else if (mode_name == "Hyper Sparkle") {
        mode = FX_MODE_HYPER_SPARKLE;
    } else if (mode_name == "Strobe") {
        mode = FX_MODE_STROBE;
    } else if (mode_name == "Strobe Rainbow") {
        mode = FX_MODE_STROBE_RAINBOW;
    } else if (mode_name == "Multi Strobe") {
        mode = FX_MODE_MULTI_STROBE;
    } else if (mode_name == "Blink Rainbow") {
        mode = FX_MODE_BLINK_RAINBOW;
    } else if (mode_name == "Chase White") {
        mode = FX_MODE_CHASE_WHITE;
    } else if (mode_name == "Chase Color") {
        mode = FX_MODE_CHASE_COLOR;
    } else if (mode_name == "Chase Random") {
        mode = FX_MODE_CHASE_RANDOM;
    } else if (mode_name == "Chase Rainbow") {
        mode = FX_MODE_CHASE_RAINBOW;
    } else if (mode_name == "Chase Flash") {
        mode = FX_MODE_CHASE_FLASH;
    } else if (mode_name == "Chase Flash Random") {
        mode = FX_MODE_CHASE_FLASH_RANDOM;
    } else if (mode_name == "Chase Rainbow White") {
        mode = FX_MODE_CHASE_RAINBOW_WHITE;
    } else if (mode_name == "Chase Blackout") {
        mode = FX_MODE_CHASE_BLACKOUT;
    } else if (mode_name == "Chase Blackout Rainbow") {
        mode = FX_MODE_CHASE_BLACKOUT_RAINBOW;
    } else if (mode_name == "Color Sweep Random") {
        mode = FX_MODE_COLOR_SWEEP_RANDOM;
    } else if (mode_name == "Running Color") {
        mode = FX_MODE_RUNNING_COLOR;
    } else if (mode_name == "Running Red Blue") {
        mode = FX_MODE_RUNNING_RED_BLUE;
    } else if (mode_name == "Running Random") {
        mode = FX_MODE_RUNNING_RANDOM;
    } else if (mode_name == "Larson Scanner") {
        mode = FX_MODE_LARSON_SCANNER;
    } else if (mode_name == "Comet") {
        mode = FX_MODE_COMET;
    } else if (mode_name == "Fireworks") {
        mode = FX_MODE_FIREWORKS;
    } else if (mode_name == "Fireworks Random") {
        mode = FX_MODE_FIREWORKS_RANDOM;
    } else if (mode_name == "Merry Christmas") {
        mode = FX_MODE_MERRY_CHRISTMAS;
    } else if (mode_name == "Halloween") {
        mode = FX_MODE_HALLOWEEN;
    } else if (mode_name == "Fire Flicker") {
        mode = FX_MODE_FIRE_FLICKER;
    } else if (mode_name == "Fire Flicker Soft") {
        mode = FX_MODE_FIRE_FLICKER_SOFT;
    } else if (mode_name == "Fire Flicker Intense") {
        mode = FX_MODE_FIRE_FLICKER_INTENSE;
    } else if (mode_name == "Circus Combustus") {
        mode = FX_MODE_CIRCUS_COMBUSTUS;
    } else if (mode_name == "Bicolor Chase") {
        mode = FX_MODE_BICOLOR_CHASE;
    } else if (mode_name == "Tricolor Chase") {
        mode = FX_MODE_TRICOLOR_CHASE;
    } else if (mode_name == "ICU") {
        mode = FX_MODE_ICU;
    } else if (mode_name == "Custom") {
        mode = FX_MODE_CUSTOM;
    } else {
        return -1;
    }
    ws2812fx.setMode(mode);
    demo_mode = false;
    return 0;
}