#include "TonuinoComponents.h"

TonuinoComponents::TonuinoComponents() {
    // Initialize state
    tonuinoState = new TonuinoState();
    
    // Initialize controllers and modules
    settingsController = new SettingsController();
    standbyTimerController = 
        new StandbyTimerController(settingsController);
    mp3Module = new MP3Module(standbyTimerController, settingsController);
    buttonController = new ButtonController(mp3Module);
    rfidModule = new RFIDModule(mp3Module, buttonController, tonuinoState);
    menuController = new MenuController(mp3Module, rfidModule, buttonController,
                                        settingsController,
                                        standbyTimerController);
}

void TonuinoComponents::loopTemp() {
    Serial.println(F("== start loopTemp()"));

    // Listen for reply
    mp3Module->loop();

    bool isPauseButtonReleased = false;
    bool isPauseButtonLongPressed = false;
    bool isUpButtonReleased = false;
    bool isUpButtonLongPressed = false;
    bool isDownButtonReleased = false;
    bool isDownButtonLongPressed = false;

    do {
        buttonController->readButtons();

        // Check for pause button
        isPauseButtonReleased = buttonController->pauseButtonReleased();
        isPauseButtonLongPressed =
            buttonController->pauseButtonLongPressed();

        // Check for up button
        isUpButtonReleased = buttonController->upButtonReleased();
        isUpButtonLongPressed = buttonController->upButtonLongPressed();

        // Check for down button
        isDownButtonReleased = buttonController->downButtonReleased();
        isDownButtonLongPressed = buttonController->downButtonLongPressed();

        if (buttonController->someMainButtonLongPressed() &&
            buttonController->allMainButtonsPressed()) {
            handleAllButtonsPress();
        } else if (isPauseButtonReleased) {
            handlePauseButtonPress();
        } else if (isPauseButtonLongPressed) {
            handlePauseButtonLongPress();
        } else if (isUpButtonReleased) {
            handleUpButtonPress();
        } else if (isUpButtonLongPressed) {
            handleUpButtonLongPress();
        } else if (isDownButtonReleased) {
            handleDownButtonPress();
        } else if (isDownButtonLongPressed) {
            handleDownButtonLongPress();
        }
    } while (!rfidModule->isNewCardPresent());

    handleNewRFIDCard();

    Serial.println(F("== end loopTemp()"));
}

void TonuinoComponents::reset() {
    // TODO: Something else to do?
    settingsController->resetSettings();
}

// Private functions
void TonuinoComponents::handleAllButtonsPress() {
    Serial.println(F("== All buttons pressed!"));
    mp3Module->pause();
    while (buttonController->someMainButtonPressed()) {
        // Wait until all buttons are released.
    }
    menuController->adminMenu(false);
    return;
}

void TonuinoComponents::handlePauseButtonPress() {
    Serial.println(F("=== handlePauseButtonPress()"));

    NfcCard* activeNfcCard = tonuinoState->activeNfcCard;
    Modifier* activeModifier = tonuinoState->activeModifier;
    bool hasActiveModifier = (activeModifier != nullptr);

    if (Utils::isKnownCard(activeNfcCard)) {
        // Invoke handlePause() from Modifier
        if (hasActiveModifier) {
            Serial.println(F("Handle modifier!"));
            activeModifier->handlePause();
            return;
        }

        if (mp3Module->isPlaying()) {
            Serial.println(F("pause player"));
            // Pause the player and start the standby timer
            mp3Module->pause();
            standbyTimerController->setTimer();
        }
    } else {
        Serial.println(F("Unknown or no card presented!"));
    }
}

void TonuinoComponents::handlePauseButtonLongPress() {
    Serial.println(F("=== handlePauseButtonLongPress()"));

    NfcCard* activeNfcCard = tonuinoState->activeNfcCard;
    Modifier* activeModifier = tonuinoState->activeModifier;
    bool hasActiveModifier = (activeModifier != nullptr);

    buttonController->waitForButtonRelease(ButtonPress::PAUSE);

    if (Utils::isKnownCard(activeNfcCard)) {
        // Invoke handlePause() from Modifier
        if (hasActiveModifier) {
            Serial.println(F("Handle modifier!"));
            activeModifier->handlePause();
            return;
        }
        if (mp3Module->isPlaying()) {
            // Play the advertisement track
            mp3Module->playAdvertisementTrack();
        } else {
            playShortcut(0);
        }
    } else {
        Serial.println(F("Unknown or no card presented!"));
    }
}

void TonuinoComponents::handleUpButtonPress() {
    Serial.println(F("=== handleUpButtonPress()"));

    // Get the admin settings
    AdminSettings* adminSettings = settingsController->adminSettings;
    Modifier* activeModifier = tonuinoState->activeModifier;
    NfcCard* activeNfcCard = tonuinoState->activeNfcCard;
    bool hasActiveModifier = (activeModifier != nullptr);

    // Check for modifiers and play next track
    if (Utils::isKnownCard(activeNfcCard)) {
        if (!adminSettings->invertVolumeButtons) {
            if (hasActiveModifier &&
                activeModifier->handleNextButton() == true) {
                Serial.println(F("Handle modifier!"));
                // Modifier handled next-button event, so exit early
                return;
            }
            mp3Module->nextTrack();
            delay(1000);
        } else {
            if (hasActiveModifier && activeModifier->handleVolumeUp()) {
                // Modifier handled volume-up event, so exit early
                return;
            }
            // Increase volume normally
            mp3Module->increaseVolume();
        }
    } else {
        Serial.println(F("Unknown or no card presented!"));
    }
}

void TonuinoComponents::handleUpButtonLongPress() {
    Serial.println(F("=== handleUpButtonLongPress()"));

    // Get the admin settings
    AdminSettings* adminSettings = settingsController->adminSettings;
    Modifier* activeModifier = tonuinoState->activeModifier;
    NfcCard* activeNfcCard = tonuinoState->activeNfcCard;
    bool hasActiveModifier = (activeModifier != nullptr);

    buttonController->waitForButtonRelease(ButtonPress::UP);
#ifndef FIVEBUTTONS
    // Check for modifiers and play next track
    if (Utils::isKnownCard(activeNfcCard)) {
        // Check volume and modifier settings
        if (mp3Module->isPlaying()) {
            if (!adminSettings->invertVolumeButtons) {
                if (hasActiveModifier && activeModifier->handleVolumeUp()) {
                    // Modifier handled volume-up event, so exit early
                    return;
                }
                // Increase volume normally
                mp3Module->increaseVolume();
            } else {
                // Check for modifiers and play next track
                if (hasActiveModifier &&
                    activeModifier->handleNextButton() == true) {
                    // Modifier handled next-button event, so exit early
                    return;
                }
                mp3Module->nextTrack();
                delay(1000);
            }
        } else {
            // Play shortcut
            playShortcut(1);
        }
    } else {
        Serial.println(F("Unknown or no card presented!"));
    }
#endif
}

void TonuinoComponents::handleDownButtonPress() {
    Serial.println(F("=== handleDownButtonPress()"));

    // Get the admin settings
    AdminSettings* adminSettings = settingsController->adminSettings;
    Modifier* activeModifier = tonuinoState->activeModifier;
    NfcCard* activeNfcCard = tonuinoState->activeNfcCard;
    bool hasActiveModifier = (activeModifier != nullptr);

    if (Utils::isKnownCard(activeNfcCard)) {
        if (!adminSettings->invertVolumeButtons) {
            // If volume buttons are not inverted play previous track or
            // handle previous button modifier
            if (hasActiveModifier && activeModifier->handlePreviousButton()) {
                // If active modifier handled the event,
                // skip further processing
                return;
            }
            mp3Module->previousTrack();
        } else {
            // If volume buttons are inverted, decrease volume or handle
            // volume down modifier
            if (hasActiveModifier && activeModifier->handleVolumeDown()) {
                // If active modifier handled the event,
                // skip further processing
                return;
            }
            mp3Module->decreaseVolume();
        }
    } else {
        Serial.println(F("Unknown or no card presented!"));
    }
}

void TonuinoComponents::handleDownButtonLongPress() {
    Serial.println(F("=== handleDownButtonLongPress()"));

    // Get the admin settings
    AdminSettings* adminSettings = settingsController->adminSettings;
    Modifier* activeModifier = tonuinoState->activeModifier;
    NfcCard* activeNfcCard = tonuinoState->activeNfcCard;
    bool hasActiveModifier = (activeModifier != nullptr);

    buttonController->waitForButtonRelease(ButtonPress::DOWN);
    // If down button was long pressed
#ifndef FIVEBUTTONS
    if (Utils::isKnownCard(activeNfcCard)) {
        if (mp3Module->isPlaying()) {
            // If music is currently playing
            if (!adminSettings->invertVolumeButtons) {
                // If volume buttons are not inverted, decrease volume or
                // handle volume down modifier
                if (hasActiveModifier && activeModifier->handleVolumeDown()) {
                    // If active modifier handled the event,
                    // skip further processing
                    return;
                }
                mp3Module->decreaseVolume();
            } else {
                // If volume buttons are inverted, play previous track or
                // handle previous button modifier
                if (hasActiveModifier &&
                    activeModifier->handlePreviousButton()) {
                    // If active modifier handled the event,
                    // skip further processing
                    return;
                }
                mp3Module->previousTrack();
            }
        } else {
            // If music is not playing, play shortcut #2
            playShortcut(2);
        }
    } else {
        Serial.println(F("Unknown or no card presented!"));
    }
#endif
}

void TonuinoComponents::handleNewRFIDCard() {
    Serial.println(F("== handleNewRFIDCard()"));
    rfidModule->readCard();

    // Modifier* activeModifier = tonuinoState->activeModifier;
    // bool hasActiveModifier = (activeModifier != nullptr);
    NfcCard* activeNfcCard = tonuinoState->activeNfcCard;
    Folder activeFolder = activeNfcCard->folder;

    // Check if card is unknown
    if (activeNfcCard->cookie == CARD_COOKIE && activeFolder.number != 0) {
        mp3Module->playFolder(activeFolder);
        // Handle folder settings
        // if (hasActiveModifier && activeNfcCard->folder.number != 0) {
        //     if (activeModifier->handleRFID(activeNfcCard) == true) {
        //         return;
        //     }
        // } else if (activeNfcCard->folder.number == 0) {
        //     // switch (activeNfcCard->folder.mode) {
        //     //     case FolderMode::NO_MODE:
        //     //     case FolderMode::NO_IDEA_MODE:
        //     //         menuController->adminMenu(true);
        //     //         break;

        //     //     case 1:
        //     //         // activeModifier = new
        //     //         // SleepTimer(tempCard.nfcFolderSettings.special);
        //     //         break;

        //     //     case 2:
        //     //         // activeModifier = new FreezeDance();
        //     //         break;

        //     //     case 3:
        //     //         // activeModifier = new Locked();
        //     //         break;

        //     //     case 4:
        //     //         // activeModifier = new ToddlerMode();
        //     //         break;

        //     //     case 5:
        //     //         // activeModifier = new KindergardenMode();
        //     //         break;

        //     //     case 6:
        //     //         // activeModifier = new RepeatSingleModifier();
        //     //         break;
        //     // }
        //     // delay(2000);
        // }
    } else {
        menuController->adminMenu(true);
    }
}

void TonuinoComponents::playShortcut(uint8_t shortcutIndex) {
    Serial.println(F("== playShortcut()"));

    // Get the shortcut folder
    AdminSettings* adminSettings = settingsController->adminSettings;
    Folder shortcutFolder = adminSettings->shortCuts[shortcutIndex];

    // Check if shortcut is configured
    if (shortcutFolder.number != 0) {
        // Activate shortcut's folder settings
        // TODO: Folder could be independent of card
        // state->currentNfcCard->folder = shortcutFolder;
        standbyTimerController->disableTimer();
        mp3Module->playFolder(shortcutFolder);
        delay(1000);
    } else {
        Serial.println(F("Shortcut not configured!"));
    }
}