#include "WS2812FX.h"
#include <neopixel.h>
#include <DFRobotDFPlayerMini.h>

SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN SPI1
#define PIXEL_COUNT 75
#define PIXEL_TYPE WS2812B
#define PIR_PIN A2
#define MIC_PIN A1

WS2812FX ws2812fx = WS2812FX(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

DFRobotDFPlayerMini myDFPlayer;

#define TIMER_MS 10000

unsigned long last_change = 0;
unsigned  last_weather_change = 0;
unsigned long now = 0;
bool demo_mode = true;

bool weatherResponsive = false;
bool musicResponsive = false;
bool presetSelect = false;
bool motionResponsive = false;

// Variables for sound reactive mode
#define THRESHOLD 5 
#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 255
uint16_t quietLevel = 0;
uint16_t maxSample = 0;
uint16_t maxSampleEver = 0;
unsigned long timer = 0;

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

// Morse Code Dictionary
const String morseCode[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",
                            ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", "/"}; // Indexes match ASCII (A-Z, 0-9, and space)

void displayMorse(String code) {
    for (unsigned int i = 0; i < code.length(); i++) {
        if (code.charAt(i) == '.') {
            //ws2812fx.setColor(ws2812fx.getColor());
            ws2812fx.setBrightness(255);
            ws2812fx.service();
            delay(200); // Short blink duration for dot
            //ws2812fx.setColor(BLACK);
            ws2812fx.setBrightness(0);
            ws2812fx.service();
            delay(200); // Pause between dots and dashes
        } else if (code.charAt(i) == '-') {
            Serial.print(ws2812fx.getColor());
            //ws2812fx.setColor(ws2812fx.getColor());
            ws2812fx.setBrightness(255);
            ws2812fx.service();
            delay(600); // Longer blink duration for dash
            //ws2812fx.setColor(BLACK);
            ws2812fx.setBrightness(0);
            ws2812fx.service();
            delay(200); // Pause between dots and dashes
        }
    }
    delay(400); // Pause between letters
}

int playMorseCode(String message) {
    for (unsigned int i = 0; i < message.length(); i++) {
        char ch = toupper(message.charAt(i));
        
        // Ignore non-alphabetical characters and spaces
        if (ch >= 'A' && ch <= 'Z') {
            String code = morseCode[ch - 'A']; // Get Morse code for the letter
            displayMorse(code); // Display the Morse code
        } else if (ch >= '0' && ch <= '9') {
            String code = morseCode[ch - '0' + 26]; // Get Morse code for numbers
            displayMorse(code); // Display the Morse code
        } else if (ch == ' ') {
            delay(1000); // Space between words
        }
    }
    return 0;
}

int weatherReactive(String msg){
    demo_mode = false;
    musicResponsive = false;
    motionResponsive = false;
    weatherResponsive = true;
    return 0;
}

void weatherResponseHandler(const char *event, const char *data) {
    if (data) {
        Serial.println("Received weather data:");
        Serial.println(data);
        if (String(data) == "Clouds"){
            ws2812fx.setColor(CYAN);
            ws2812fx.service();
        }
    } else {
        Serial.println("No data received from the webhook.");
        ws2812fx.setColor(WHITE);
        ws2812fx.service();
    }
}

int musicReactive(String msg){
    demo_mode = false;
    weatherResponsive = false;
    motionResponsive = false;
    musicResponsive = true;
    return 0;
} 

void handleSoundReactiveMode() {
    // Take an audio sample
    uint16_t audioSample = abs(analogRead(MIC_PIN) - quietLevel);

    if (audioSample > maxSample) {
    maxSample = audioSample; // Track the highest audio sample
    }

    // If the timer has expired, recalculate the LED brightness based on audio
    if (millis() > timer) {
    if (maxSample > THRESHOLD) { // Ensure the audio is above the threshold to reduce LED flickering
        if (maxSample > maxSampleEver) maxSampleEver = maxSample;

        // Calculate a new brightness, properly scaled to the sampled audio
        uint8_t newBrightness = map(maxSample, THRESHOLD, maxSampleEver, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        Serial.print("maxSample:"); Serial.print(maxSample); // Debugging
        Serial.print(", maxSampleEver:"); Serial.print(maxSampleEver);
        Serial.print(", newBrightness:"); Serial.println(newBrightness);

        ws2812fx.setBrightness(newBrightness); // Update the LED brightness
    } else {
        ws2812fx.setBrightness(MIN_BRIGHTNESS); // Set the LED brightness to minimum if below threshold
    }

    // Reset for the next audio sample period
    maxSample = 0;
    timer = millis() + 10; // Recalculate every 10ms (100Hz)
    }

    // Update the LED effects
    ws2812fx.service();
}

int motionReactive(String msg){
    demo_mode = false;
    weatherResponsive = false;
    musicResponsive = false;
    motionResponsive = true;
    return 0;
} 

void setup() {
    Serial.begin(115200);
    // Initialize DFPlayer
    Serial2.begin(9600);
    if (!myDFPlayer.begin(Serial2)) {
        Serial.println("DFPlayer initialization failed!");
        //while (true);
    }
    myDFPlayer.volume(25);
    ws2812fx.init();
    ws2812fx.setBrightness(255);
    ws2812fx.setSpeed(1000);
    ws2812fx.setColor(0x007BFF);
    ws2812fx.setMode(FX_MODE_STATIC);
    ws2812fx.start();

     for(int i=0; i<20; i++) {
        quietLevel += analogRead(MIC_PIN); // 0-1023
        delay(25);
    }
    quietLevel /= 20;
    Serial.print("\nquietLevel is "); Serial.println(quietLevel);

    Particle.function("setSpeed", setSpeed);
    Particle.function("incrSpeed", increaseSpeed);
    Particle.function("decrSpeed", decreaseSpeed);
    Particle.function("setBright", setBrightness);
    Particle.function("incrBright", increaseBrightness);
    Particle.function("decrBright", decreaseBrightness);
    Particle.function("setColor", setColor);
    Particle.function("setMode", setMode);
    Particle.function("playMorse", playMorseCode);
    Particle.function("weatherReactive", weatherReactive);
    Particle.function("musicReactive", musicReactive);
    Particle.function("motionReactive", motionReactive);

    Particle.subscribe("hook-response/getWeather", weatherResponseHandler, MY_DEVICES);

}

void loop() {
    if (demo_mode) {
        now = millis();
        ws2812fx.service();

        if (now - last_change > TIMER_MS) {
            ws2812fx.setMode((ws2812fx.getMode() + 1) % ws2812fx.getModeCount());
            last_change = now;
        }
    } 
    else if (weatherResponsive){
        now = millis();
        ws2812fx.service();
        if (now - last_weather_change > 60000) {
            String data = String("{ \"lat\": \"11.845632\", \"lon\": \"75.5236864\"}");
            Particle.publish("getWeather", data, PRIVATE);
            last_weather_change = now;
        }
    }
    else if (musicResponsive){
        //myDFPlayer.pause();
        ws2812fx.setMode(FX_MODE_RAINBOW_CYCLE);
        handleSoundReactiveMode();
       
    }
    else if(motionResponsive){
        int pirState = analogRead(PIR_PIN);
        Serial.println(pirState);
        ws2812fx.setMode(FX_MODE_RANDOM_COLOR);
        if (pirState > 1000){
            for (int i=0; i<=255; i+=5){
                ws2812fx.setBrightness(i);
                ws2812fx.service();
                delay(100);
            }
        }
        else{
            ws2812fx.setBrightness(0);
            ws2812fx.service();
        }
    }
    else{
        ws2812fx.service();
    }
}