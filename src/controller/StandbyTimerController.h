#ifndef STANDBYTIMERCONTROLLER_H
#define STANDBYTIMERCONTROLLER_H

#include <Arduino.h>
#include <avr/sleep.h>

#include "../config/config.h"
#include "../config/structs.h"
#include "SettingsController.h"

class MP3Module;
class RFIDModule;

class StandbyTimerController {
   public:
    StandbyTimerController(SettingsController* settingsController);
    void setTimer();
    void disableTimer();
    void checkTimerAtMillis(unsigned long sleepAtMillis, MP3Module* mp3Module,
                            RFIDModule* rfidModule);

   private:
    SettingsController* settingsController;
};

#endif