#include "MenuController.h"

MenuController::MenuController(MP3Module* mp3Module, RFIDModule* rfidModule,
                               ButtonController* buttonController,
                               SettingsController* settingsController,
                               StandbyTimerController* standbyTimeController)
    : mp3Module(mp3Module),
      rfidModule(rfidModule),
      buttonController(buttonController),
      settingsController(settingsController),
      standbyTimeController(standbyTimeController) {}

void MenuController::adminMenu(bool fromCard) {
    Serial.println(F("=== adminMenu()"));

    AdminSettings* adminSettings = settingsController->adminSettings;

    mp3Module->pause();
    standbyTimeController->disableTimer();

    // Check if card has been used to access admin menu
    if (!fromCard && isAdminMenuLocked()) {
        return;
    }

    const uint8_t subMenu = voiceMenu(12, MP3Track::ADMIN_MENU);
    uint8_t numOptions = 0;

    switch (subMenu) {
        case AdminMenu::CARD_RESET: {
            Folder folder = setupFolder();
            rfidModule->setupCard(folder);
        } break;
        case AdminMenu::MAX_VOLUME: {
            numOptions = 30 - adminSettings->minVolume;
            adminSettings->maxVolume = voiceMenu(
                numOptions, MP3Track::SELECT_MAX_VOLUME, adminSettings->minVolume);
        } break;
        case AdminMenu::MIN_VOLUME: {
            numOptions = adminSettings->maxVolume;
            adminSettings->minVolume =
                voiceMenu(numOptions, MP3Track::SELECT_MIN_VOLUME, 0);
        } break;
        case AdminMenu::INITIAL_VOLUME: {
            numOptions = adminSettings->maxVolume - adminSettings->minVolume + 1;
            adminSettings->initVolume =
                voiceMenu(numOptions, MP3Track::SELECT_INITIAL_VOLUME,
                          adminSettings->minVolume - 1);
        } break;
        case AdminMenu::EQUALIZER: {
            adminSettings->eq = voiceMenu(6, MP3Track::SELECT_EQ);
            mp3Module->updateEqualizer();
        } break;
        case AdminMenu::MODIFIER_CARD: {
            // TODO: Review whole thing
            // create modifier card
            NfcCard modifierCard;
            modifierCard.cookie = CARD_COOKIE;
            modifierCard.version = 1;
            modifierCard.folder.number = 0;
            modifierCard.folder.special = 0;
            modifierCard.folder.special2 = 0;
            modifierCard.folder.mode =
                (FolderMode)voiceMenu(6, MP3Track::SELECT_MODIFIER);

            if (modifierCard.folder.mode != 0) {
                if (modifierCard.folder.mode == FolderMode::RANDOM_MODE) {
                    switch (voiceMenu(4, MP3Track::SELECT_TIMER)) {
                        case 1:
                            modifierCard.folder.special = 5;
                            break;
                        case 2:
                            modifierCard.folder.special = 15;
                            break;
                        case 3:
                            modifierCard.folder.special = 30;
                            break;
                        case 4:
                            modifierCard.folder.special = 60;
                            break;
                    }
                }

                mp3Module->playMp3FolderTrack(MP3Track::WATING_FOR_CARD);

                // Check if current operation is aborted
                do {
                    if (buttonController->upOrDownButtonReleased()) {
                        // Serial.println(F("Abgebrochen!"));
                        mp3Module->playMp3FolderTrack(MP3Track::ABORT);
                        return;
                    }
                } while (!rfidModule->mfrc522.PICC_IsNewCardPresent());

                // RFID Karte wurde aufgelegt
                if (rfidModule->mfrc522.PICC_ReadCardSerial()) {
                    // Serial.println(F("schreibe Karte..."));
                    rfidModule->writeCard(modifierCard);
                }
            }
        } break;
        case AdminMenu::SHORTCUTS: {
            uint8_t shortcut = voiceMenu(4, MP3Track::SELECT_SHORTCUT);
            Folder newFolder = setupFolder();
            settingsController->setShortCut(shortcut, newFolder);
            mp3Module->playMp3FolderTrack(MP3Track::CONFIRM_CARD_CONFIGURED);
        } break;
        case AdminMenu::STANDBY_TIMER: {
            uint8_t standbyTimes[6] = {0, 5, 15, 30, 60};
            uint8_t timerSelection = voiceMenu(5, MP3Track::SELECT_TIMER);
            adminSettings->standbyTimer = standbyTimes[timerSelection];
        } break;
        case AdminMenu::BATCH_CARDS: {
            // Create Cards for Folder
            // Ordner abfragen
            NfcCard newCard;
            newCard.cookie = CARD_COOKIE;
            newCard.version = 1;
            newCard.folder.mode = FolderMode::ALBUM_MODE;
            newCard.folder.number = voiceMenu(99, MP3Track::SELECT_FOLDER, 0);
            uint8_t folderTrackCount =
                mp3Module->getFolderTrackCount(newCard.folder);
            uint8_t special =
                voiceMenu(folderTrackCount, MP3Track::SELECT_FIRST_FILE, 0,
                          true, newCard.folder.number);
            uint8_t special2 =
                voiceMenu(folderTrackCount, MP3Track::SELECT_LAST_FILE, 0, true,
                          newCard.folder.number);

            mp3Module->playMp3FolderTrack(MP3Track::BATCH_CARD_INTRO);

            for (uint8_t i = special; i <= special2; i++) {
                mp3Module->playMp3FolderTrack(i);
                newCard.folder.special = i;
                // Serial.print(i);
                // Serial.println(F(" Karte auflegen"));
                do {
                    if (buttonController->upOrDownButtonReleased()) {
                        // Serial.println(F("Aborted"));
                        mp3Module->playMp3FolderTrack(MP3Track::ABORT);
                        return;
                    }
                } while (!rfidModule->mfrc522.PICC_IsNewCardPresent());

                // RFID Karte wurde aufgelegt
                if (rfidModule->mfrc522.PICC_ReadCardSerial()) {
                    // Serial.println(F("Writing card..."));
                    rfidModule->writeCard(newCard);
                }
            }
        } break;
        case AdminMenu::INVERT_BUTTONS: {
            // Invert Functions for Up/Down Buttons
            uint8_t temp = voiceMenu(2, MP3Track::INVERT_BUTTONS_MSG);
            if (temp == 2) {
                adminSettings->invertVolumeButtons = true;
            } else {
                adminSettings->invertVolumeButtons = false;
            }
        } break;
        case AdminMenu::RESET: {
            // Serial.println(F("Reset -> EEPROM wird gelöscht"));
            for (uint8_t i = 0; i < EEPROM.length(); i++) {
                EEPROM.update(i, 0);
            }
            settingsController->resetSettings();
            mp3Module->playMp3FolderTrack(MP3Track::RESET_DONE);
        } break;
        case AdminMenu::ADMIN_LOCK: {
            uint8_t optionSelected = voiceMenu(4, MP3Track::ADMIN_MENU_LOCK);
            if (optionSelected == 1) {
                adminSettings->adminMenuLocked = AdminLock::NO_LOCK;
            } else if (optionSelected == 2) {
                adminSettings->adminMenuLocked = AdminLock::CARD_LOCK;
            } else if (optionSelected == 3) {
                uint8_t* pin = getPinlock();
                mp3Module->playMp3FolderTrack(991);
                memcpy(adminSettings->adminMenuPin, pin, 4);
                adminSettings->adminMenuLocked = AdminLock::PIN_LOCK;
            } else if (optionSelected == 4) {
                adminSettings->adminMenuLocked = AdminLock::MATH_PROBLEM_LOCK;
            }
            settingsController->writeSettingsToFlash();
            standbyTimeController->setTimer();
        }

        break;
    }
}

uint8_t MenuController::voiceMenu(uint8_t numOptions,
                                  MP3Track startMessageTrack,
                                  int8_t offsetMessageNum, bool shouldPreview,
                                  uint8_t previewFolderNum) {
    // Print debug message
    // Serial.print(F("=== voiceMenu() ("));
    // Serial.print(numOptions);
    // Serial.println(F(" Options)"));

    uint8_t selection = 0;
    uint8_t startOptionNum =
        offsetMessageNum != -1 ? offsetMessageNum : startMessageTrack;

    // Play starting message (if any)
    if (startMessageTrack != 0) {
        mp3Module->playMp3FolderTrack(startMessageTrack);
    }

    while (true) {
        // Check for serial input
        // if (// Serial.available() > 0) {
        //     int optionFormSerial = // Serial.parseInt();
        //     if (optionFormSerial != 0 && optionFormSerial <= numOptions) {
        //         return optionFormSerial;
        //     }
        // }

        // Check for new commands received from the
        // DFPlayer moduleto execute any queued commands
        mp3Module->loop();

        switch (buttonController->waitForButtonPress()) {
            case UP:
                // Increment selection by 1 in range [1, numOptions]
                selection = min(selection + 1, numOptions);
                mp3Module->playMp3FolderTrack(startOptionNum + selection);
                if (shouldPreview) {
                    playFolderPreview(selection, previewFolderNum);
                }
                break;
            case LONG_UP:
                // Increment selection by 10 in range [1, numOptions]
                selection = min(selection + 10, numOptions);
                mp3Module->playMp3FolderTrack(startOptionNum + selection);
                break;
            case DOWN:
                // Decrement selection by 1 in range [1, numOptions]
                selection = max(selection - 1, 1);
                mp3Module->playMp3FolderTrack(startOptionNum + selection);
                if (shouldPreview) {
                    playFolderPreview(selection, previewFolderNum);
                }
                break;
            case LONG_DOWN:
                // Decrement selection by 10 in range [1, numOptions]
                selection = max(selection - 10, 1);
                mp3Module->playMp3FolderTrack(startOptionNum + selection);
                break;
            case PAUSE:
                if (selection != 0) {
                    // Serial.print(F("=== "));
                    // Serial.print(selection);
                    // Serial.println(F(" ==="));
                    return selection;
                }
                break;
            case LONG_PAUSE:
                mp3Module->playMp3FolderTrack(MP3Track::ABORT);
                standbyTimeController->checkTimerAtMillis(
                    settingsController->adminSettings->sleepAtMillis, mp3Module,
                    rfidModule);
                // TODO: Check if return value appropriate
                return 0;
        }
    }
}

Folder MenuController::setupFolder() {
    Folder newFolder;
    uint16_t folderTrackCount;

    newFolder.number = voiceMenu(99, MP3Track::SELECT_FOLDER, 0);

    // TODO: Clean up
    // if (newFolder.number == 0) {
    //     return false;
    // }

    // Wiedergabemodus abfragen
    newFolder.mode = (FolderMode)voiceMenu(9, MP3Track::SELECT_FOLDER_MODE);

    // TODO: Clean up
    // if (newFolder.mode == 0) {
    //     return false;
    // }

    // Handle different modes of operation
    switch (newFolder.mode) {
        case FolderMode::SINGLE_MODE: {
            folderTrackCount = mp3Module->getFolderTrackCount(newFolder);
            newFolder.special =
                voiceMenu(folderTrackCount, MP3Track::SELECT_FILE, (MP3Track)0,
                          true, newFolder.number);
            break;
        }
        case FolderMode::SPECIAL_MODE_1:
        case FolderMode::SPECIAL_MODE_2:
        case FolderMode::SPECIAL_MODE_3: {
            folderTrackCount = mp3Module->getFolderTrackCount(newFolder);
            newFolder.special =
                voiceMenu(folderTrackCount, MP3Track::SELECT_FIRST_FILE,
                          (MP3Track)0, true, newFolder.number);
            newFolder.special2 =
                voiceMenu(folderTrackCount, MP3Track::SELECT_LAST_FILE,
                          (MP3Track)0, true, newFolder.number);
            break;
        }
        case FolderMode::ADMIN_FUNCTIONS: {
            newFolder.number = 0;
            newFolder.mode = FolderMode::ADMIN_FUNCTIONS;
            break;
        }
        default:
            // Unsupported mode
            return Folder();
    }

    return newFolder;
}

// Private functions
bool MenuController::isAdminMenuLocked() {
    bool isLocked = false;
    AdminSettings* adminSettings = settingsController->adminSettings;;

    // Admin menu has been locked.
    // Can still be trigged via admin card.
    if (adminSettings->adminMenuLocked == AdminLock::CARD_LOCK) {
        isLocked = true;
    }
    // Unlock with PIN
    else if (adminSettings->adminMenuLocked == AdminLock::PIN_LOCK) {
        mp3Module->playMp3FolderTrack(MP3Track::ENTER_ADMIN_PIN);

        // Get and check PIN
        uint8_t* pin = getPinlock();
        if (!isPinlockValid(pin)) {
            isLocked = true;
        }
    }
    // Unlock with mathematical problem
    else if (adminSettings->adminMenuLocked == AdminLock::MATH_PROBLEM_LOCK) {
        // Random number between 10 and 19
        uint8_t firstNumber = random(10, 20);
        // Random number between 1 and 9
        uint8_t secondNumber = random(1, 10);
        uint8_t result;
        mp3Module->playMp3FolderTrack(992);
        mp3Module->waitForTrackToFinish();
        mp3Module->playMp3FolderTrack(firstNumber);

        if (random(1, 3) == 2) {
            // a + b
            result = firstNumber + secondNumber;
            mp3Module->waitForTrackToFinish();
            mp3Module->playMp3FolderTrack(993);
        } else {
            // a - b
            secondNumber = random(1, firstNumber);
            result = firstNumber - secondNumber;
            mp3Module->waitForTrackToFinish();
            mp3Module->playMp3FolderTrack(994);
        }
        mp3Module->waitForTrackToFinish();
        mp3Module->playMp3FolderTrack(secondNumber);
        // Serial.println(result);
        uint8_t temp = voiceMenu(255, (MP3Track)0);
        if (temp != result) isLocked = true;
    }
    return isLocked;
}

uint8_t* MenuController::getPinlock() {
    uint8_t* pin = new uint8_t[4];
    uint8_t counter = 0;

    while (counter < 4) {
        buttonController->readButtons();
        // Interrupt pin recording
        if (buttonController->pauseButtonLongPressed()) {
            break;
        } else if (buttonController->pauseButtonReleased()) {
            pin[counter++] = 1;
        } else if (buttonController->upButtonReleased()) {
            pin[counter++] = 2;
        } else if (buttonController->downButtonReleased()) {
            pin[counter++] = 3;
        }
    }

    return pin;
}

bool MenuController::isPinlockValid(uint8_t pin[4]) {
    bool isValid = true;
    uint8_t counter = 0;
    AdminSettings* adminSettings = settingsController->adminSettings;

    while (counter < 4) {
        if (pin[counter] != adminSettings->adminMenuPin[counter]) {
            isValid = false;
            break;
        }
        counter++;
    }
    return isValid;
}

void MenuController::playFolderPreview(uint8_t selection,
                                       int previewFolderNum) {
    if (previewFolderNum == 0) {
        mp3Module->playFolderTrack(selection, 1);
    } else {
        mp3Module->playFolderTrack(previewFolderNum, selection);
    }
    // TODO: Check if this is needed
    delay(1000);
}
