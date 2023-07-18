#include <Arduino.h>

#include "config/structs.h"
#include "controller/ButtonController.h"
#include "controller/MenuController.h"
#include "controller/SettingsController.h"
#include "controller/StandbyTimerController.h"
#include "enums/FolderModeEnum.h"
#include "enums/CardEventEnum.h"
#include "modifier/Modifier.h"
#include "modules/MP3Module.h"
#include "modules/RFIDModule.h"
#include "utils/MemoryUtils.h"

class TonuinoComponents {
   public:
    // Constructor
    TonuinoComponents();

    // Functions
    void loopTemp();
    void reset();

   private:
    // Members
    TonuinoState *tonuinoState;
    SettingsController *settingsController;
    StandbyTimerController *standbyTimerController;
    MP3Module *mp3Module;
    ButtonController *buttonController;
    RFIDModule *rfidModule;
    MenuController *menuController;

    // Functions
    void handleButtons();
    void handleAllButtonsPress();
    void handlePauseButtonPress();
    void handlePauseButtonLongPress();
    void handleUpButtonPress();
    void handleUpButtonLongPress();
    void handleDownButtonPress();
    void handleDownButtonLongPress();
    void handleRFIDCard(CardEvent cardEvent);
    void handleNewRFIDCard();
    void handleRemovedRFIDCard();
    void playShortcut(uint8_t shortcutIndex);
};