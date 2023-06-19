#ifndef SettingsController_h
#define SettingsController_h

#include <Arduino.h>
#include <EEPROM.h>

#include "../config/config.h"
#include "../config/structs.h"

class SettingsController {
   public:
    SettingsController();
    AdminSettings* adminSettings;

    // Functions
    void resetSettings();
    bool loadSettingsFromFlash();
    void writeSettingsToFlash();
    void setShortCut(uint8_t shortcutIndex, Folder& folder);

   private:
    // Members
    int storeAdminSettingsAddress;
    // Functions
    // void migrateSettings(int oldVersion);
    void printAdminSettings();
};

#endif