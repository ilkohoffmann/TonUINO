#include "SettingsController.h"

SettingsController::SettingsController() : adminSettings(new AdminSettings()) {
    storeAdminSettingsAddress = sizeof(uint8_t) * 100;
    if (!loadSettingsFromFlash()) {
        resetSettings();
    };
}

bool SettingsController::loadSettingsFromFlash() {
    Serial.println(F("=== loadSettingsFromFlash()"));
    Serial.print(F("storeAdminSettingsAddress: "));
    Serial.println(storeAdminSettingsAddress);
    EEPROM.get(storeAdminSettingsAddress, adminSettings);
    Serial.print(F("cookie: "));
    Serial.println(adminSettings->cookie);
    Serial.print(F("CARD_COOKIE: "));
    Serial.println(CARD_COOKIE);
    if (adminSettings->cookie != CARD_COOKIE) {
        Serial.println(F("Settings not found."));
        
        return false;
    } else {
        Serial.println(F("Settings loaded."));
        printAdminSettings();
        return true;
    }

    // migrateSettings(adminSettings->version);
}

void SettingsController::writeSettingsToFlash() {
    Serial.println(F("=== writeSettingsToFlash()"));
    Serial.print(F("storeAdminSettingsAddress: "));
    Serial.println(storeAdminSettingsAddress);

    EEPROM.put(storeAdminSettingsAddress, adminSettings);
    printAdminSettings();
}

void SettingsController::resetSettings() {
    
    Serial.println(F("=== initializeAdminSettings()"));
    
    adminSettings = new AdminSettings();
    adminSettings->cookie = CARD_COOKIE;
    adminSettings->version = CARD_VERSION_TEST;
    adminSettings->maxVolume = DEFAULT_MAX_VOLUME;
    adminSettings->minVolume = DEFAULT_MIN_VOLUME;
    adminSettings->initVolume = DEFAULT_INIT_VOLUME;
    adminSettings->eq = DEFAULT_EQ;
    adminSettings->standbyTimer = DEFAULT_STANDBY_TIMER;
    adminSettings->invertVolumeButtons = DEFAULT_INVERT_VOLUME_BUTTONS;

    // Reset all shortcuts to folder 0
    for (int i = 0; i < CONFIG_NUM_SHORTCUTS; i++) {
        adminSettings->shortCuts[i].number = 0;
    }
    // Set adminPin to 1111
    memset(adminSettings->adminMenuPin, 1, sizeof(adminSettings->adminMenuPin));

    writeSettingsToFlash();
}

void SettingsController::setShortCut(uint8_t shortcutIndex, Folder& folder) {
    Serial.println(F("=== setShortCut()"));

    if (shortcutIndex > 0 && shortcutIndex <= CONFIG_NUM_SHORTCUTS) {
        adminSettings->shortCuts[shortcutIndex] = folder;
    } else {
        Serial.println("Shortcut index is out of bounds.");
    }
    adminSettings->shortCuts[shortcutIndex] = folder;
}

// Private functions

// TODO: Check if function is needed
// void SettingsController::migrateSettings(int oldVersion) {
//     Serial.println(F("=== migrateSettings()"));

//     if (oldVersion == 1) {
//         Serial.println(F("1 -> 2"));

//         // Update adminSettings values
//         adminSettings->version = 2;
//         adminSettings->adminMenuLocked = 0;

//         // Set adminPin to 1111
//         memset(adminSettings->adminMenuPin, 1,
//                sizeof(adminSettings->adminMenuPin));

//         writeSettingsToFlash();
//     }
// }

void SettingsController::printAdminSettings() {
    Serial.println(F("=== printAdminSettings()"));

    Serial.print(F("Version: "));
    Serial.println(adminSettings->version);

    Serial.print(F("Maximal Volume: "));
    Serial.println(adminSettings->maxVolume);

    Serial.print(F("Minimal Volume: "));
    Serial.println(adminSettings->minVolume);

    Serial.print(F("Initial Volume: "));
    Serial.println(adminSettings->initVolume);

    Serial.print(F("EQ: "));
    Serial.println(adminSettings->eq);

    Serial.print(F("Locked: "));
    Serial.println(adminSettings->locked);

    Serial.print(F("Sleep Timer: "));
    Serial.println(adminSettings->standbyTimer);

    Serial.print(F("Inverted Volume Buttons: "));
    Serial.println(adminSettings->invertVolumeButtons);

    Serial.print(F("Admin Menu locked: "));
    Serial.println(adminSettings->adminMenuLocked);

    Serial.print(F("Admin Menu Pin: "));
    Serial.print(adminSettings->adminMenuPin[0]);
    Serial.print(adminSettings->adminMenuPin[1]);
    Serial.print(adminSettings->adminMenuPin[2]);
    Serial.println(adminSettings->adminMenuPin[3]);
}