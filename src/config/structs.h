#ifndef STRUCTS_H
#define STRUCTS_H

#include <Arduino.h>

#include "../enums/FolderModeEnum.h"
#include "../modifier/Modifier.h"
#include "config.h"

struct Folder {
    uint8_t number;
    FolderMode mode;
    uint8_t special;
    uint8_t special2;
};

// This object stores nfc tag data
struct NfcCard {
    String uid;
    uint32_t cookie;
    uint8_t version;
    Folder folder;
    Modifier modifier;
};

// Admin settings stored in eeprom
struct AdminSettings {
    uint32_t cookie;
    byte version;
    uint8_t maxVolume;
    uint8_t minVolume;
    uint8_t initVolume;
    uint8_t eq;
    bool locked;
    long standbyTimer;
    unsigned long sleepAtMillis;  // new
    bool invertVolumeButtons;
    bool ignorePauseButton;  // new
    Folder shortCuts[CONFIG_NUM_SHORTCUTS];
    uint8_t adminMenuLocked;
    uint8_t adminMenuPin[4];
};

// Current state of the Tonuino
struct TonuinoState {
    // unsigned long sleepAtMillis;
    NfcCard *activeNfcCard;
    Modifier *activeModifier;
    bool cardPresent;
    TonuinoState() : activeNfcCard(new NfcCard()), activeModifier(nullptr) {}
};

#endif