#include "StandbyTimerController.h"

#include "../modules/MP3Module.h"
#include "../modules/RFIDModule.h"

StandbyTimerController::StandbyTimerController(
    SettingsController* settingsController)
    : settingsController(settingsController) {}

void StandbyTimerController::setTimer() {
    Serial.println(F("=== setStandbyTimer()"));

    AdminSettings* adminSettings = settingsController->adminSettings;

    if (adminSettings->standbyTimer != 0) {
        adminSettings->sleepAtMillis =
            millis() + (adminSettings->standbyTimer * 60 * 1000);
    } else {
        adminSettings->sleepAtMillis = 0;
    }
    Serial.println(adminSettings->sleepAtMillis);
}

void StandbyTimerController::disableTimer() {
    Serial.println(F("=== disableTimer()"));

    AdminSettings* adminSettings = settingsController->adminSettings;
    adminSettings->sleepAtMillis = 0;
}

void StandbyTimerController::checkTimerAtMillis(unsigned long sleepAtMillis,
                                                MP3Module* mp3Module,
                                                RFIDModule* rfidModule) {
    Serial.println(F("=== checkTimerAtMillis()"));

    if (sleepAtMillis != 0 && millis() > sleepAtMillis) {
        Serial.println(F("=== power off!"));
        
        // enter sleep state
        digitalWrite(CONFIG_SHUTDOWN_PIN, LOW);

        rfidModule->sleep();
        mp3Module->sleep();

        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        cli();  // Disable interrupts
        sleep_mode();
    }
}
