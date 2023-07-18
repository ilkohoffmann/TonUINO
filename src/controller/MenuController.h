#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include <Arduino.h>

#include "../config/structs.h"
#include "../controller/StandbyTimerController.h"
#include "../enums/AdminLockEnum.h"
#include "../enums/AdminMenuEnum.h"
#include "../enums/ButtonPressEnum.h"
#include "../enums/FolderModeEnum.h"
#include "../enums/MP3TrackEnum.h"
#include "../modules/MP3Module.h"
#include "../modules/RfidModule.h"
#include "ButtonController.h"
#include "SettingsController.h"

class MenuController {
   public:
    MenuController(MP3Module* mp3Module, RFIDModule* rfidModule,
                   ButtonController* buttonController,
                   SettingsController* settingsController,
                   StandbyTimerController* standbyTimeController);

    // Functions
    void adminMenu(bool fromCard = false);
    uint8_t voiceMenu(uint8_t numOptions, MP3Track startMessageTrack,
                      int8_t offsetMessageNum = -1, bool shouldPreview = 0,
                      uint8_t previewFolderNum = 0);
    Folder setupFolder();

   private:
    // Members
    MP3Module* mp3Module;
    RFIDModule* rfidModule;
    ButtonController* buttonController;
    SettingsController* settingsController;
    StandbyTimerController* standbyTimeController;

    // Functions
    bool isAdminMenuLocked();
    uint8_t* getPinlock();
    bool isPinlockValid(uint8_t pin[4]);
    void playFolderPreview(uint8_t selection, int previewFolderNum);
};

#endif