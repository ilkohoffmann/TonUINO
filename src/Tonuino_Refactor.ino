
#include <Arduino.h>

#include "TonuinoComponents.h"
#include "config/config.h"
#include <JC_Button.h>

/*
   _____         _____ _____ _____ _____
  |_   _|___ ___|  |  |     |   | |     |
    | | | . |   |  |  |-   -| | | |  |  |
    |_| |___|_|_|_____|_____|_|___|_____|
    TonUINO Version 2.1

    created by Thorsten Voß and licensed under GNU/GPL.
    Information and contribution at https://tonuino.de.
*/

// Uncomment the below line
// to enable five button support
// #define FIVEBUTTONS

// #define USBCON

TonuinoComponents* tonuinoComponents;
void initializeRandomSeed();
void initializePins();
void resetIfAllButtonsPressed();
void playStartupShortcut();

void setup() {

    // Initialize serial communication at 115200 baud rate
    Serial.begin(115200);

    // Dieser Hinweis darf nicht entfernt werden
    Serial.println(F("\n _____         _____ _____ _____ _____"));
    Serial.println(F("|_   _|___ ___|  |  |     |   | |     |"));
    Serial.println(F("  | | | . |   |  |  |-   -| | | |  |  |"));
    Serial.println(F("  |_| |___|_|_|_____|_____|_|___|_____|\n"));
    Serial.println(F("TonUINO Version 2.1"));
    Serial.println(F("created by Thorsten Voß and licensed under GNU/GPL."));
    Serial.println(F("Information and contribution at https://tonuino.de.\n"));

    initializeRandomSeed();
    initializePins();
    tonuinoComponents = new TonuinoComponents();
    resetIfAllButtonsPressed();
    playStartupShortcut();
}

void loop() { tonuinoComponents->loopTemp(); }

void initializeRandomSeed() {
    Serial.println(F("=== initializeRandomSeed()"));
    uint32_t ADC_LSB;  // Least Significant Bit of ADC value
    uint32_t ADCSeed;  // Seed to be generated from LSB values

    // Generate random seed by reading and combining LSB values from 128
    // analogRead() calls
    for (uint8_t i = 0; i < 128; i++) {
        // Read LSB of analog value
        ADC_LSB = analogRead(CONFIG_OPEN_ANALOG_PIN) & 0x1;
        // Combine LSB values into seed using XOR
        ADCSeed ^= ADC_LSB << (i % 32);
    }

    // Set random seed for pseudo-random number
    // generation using analogRead() values
    randomSeed(ADCSeed);
}

void initializePins() {
    Serial.println(F("=== setPins()"));

    // spannung einschalten
    pinMode(CONFIG_SHUTDOWN_PIN, OUTPUT);
    digitalWrite(CONFIG_SHUTDOWN_PIN, HIGH);

    // sd karten zugang aus
    pinMode(20, OUTPUT);
    digitalWrite(20, LOW);

    // verstärker an
    pinMode(19, OUTPUT);
    digitalWrite(19, LOW);

    // Set the busy pin as input
    pinMode(CONFIG_BUSY_PIN, INPUT);

    // Set pins for pause, up, and down buttons as inputs with pull-up resistors
    pinMode(CONFIG_BUTTON_PAUSE_PIN, INPUT_PULLUP);
    pinMode(CONFIG_BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(CONFIG_BUTTON_DOWN_PIN, INPUT_PULLUP);

#ifdef FIVEBUTTONS
    // Set pins for button 4 and button 5 as inputs with pull-up resistors, if
    // defined
    pinMode(CONFIG_BUTTON_4_PIN, INPUT_PULLUP);
    pinMode(CONFIG_BUTTON_5_PIN, INPUT_PULLUP);
#endif

    // Set the shutdown pin as an output and initially set it to LOW
    pinMode(CONFIG_SHUTDOWN_PIN, OUTPUT);
    digitalWrite(CONFIG_SHUTDOWN_PIN, LOW);
}

void resetIfAllButtonsPressed() {
    Serial.println(F("=== resetIfAllButtonsPressed()"));
    // Hold all buttons down on startup
    if (digitalRead(CONFIG_BUTTON_PAUSE_PIN) == LOW &&
        digitalRead(CONFIG_BUTTON_UP_PIN) == LOW &&
        digitalRead(CONFIG_BUTTON_DOWN_PIN) == LOW) {
        Serial.println(F("Reset -> EEPROM will be erased"));
        // Loop through every address in EEPROM
        for (uint8_t i = 0; i < EEPROM.length(); i++) {
            // Set each address to 0
            EEPROM.update(i, 0);
        }
        // Call reset handler
        tonuinoComponents->reset();
    }
}

void playStartupShortcut() {
    // Start Shortcut "at Startup" - e.g. Welcome Sound
    // playShortCut(3);
}
