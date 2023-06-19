#include "MP3Module.h"

MP3Module::MP3Module(StandbyTimerController *standbyTimerController,
                     SettingsController *settingsController)
    : standbyTimerController(standbyTimerController),
      settingsController(settingsController) {
    initializeDFMini();
}

void MP3Module::loop() { dfMini->loop(); }

void MP3Module::pause() { dfMini->pause(); }

void MP3Module::start() { dfMini->start(); }

void MP3Module::sleep() { dfMini->sleep(); }

bool MP3Module::isPlaying() { return !digitalRead(CONFIG_BUSY_PIN); }

void MP3Module::playMp3FolderTrack(uint16_t track) {
    Serial.println(F("== playMp3FolderTrack()"));
    dfMini->playMp3FolderTrack(track);
    waitForTrackToFinish();
}

void MP3Module::playAdvertisementTrack() {
    Serial.println(F("=== playAdvertisementTrack()"));

    uint8_t advertTrack;
    Folder currentFolder = tonuinoState->activeNfcCard->folder;
    FolderMode activeMode = currentFolder.mode;
    switch (activeMode) {
        case FolderMode::PARTY_MODE:
            advertTrack = queue[currentTrack - 1];
            break;
        case FolderMode::SPECIAL_MODE_1:
        case FolderMode::SPECIAL_MODE_2:
            advertTrack = advertTrack - currentFolder.special + 1;
            break;
        default:
            advertTrack = currentTrack;
    }

    Serial.print("F(AdvertTrack is: ");
    Serial.println(advertTrack);

    dfMini->playAdvertisement(advertTrack);
    waitForTrackToFinish();
}

void MP3Module::playFolderTrack(const uint8_t folderNum, const uint8_t track) {
    Serial.println(F("== playFolderTrack()"));
    Serial.print(F("FolderNumber: "));
    Serial.println(folderNum);
    Serial.print(F("Track: "));
    Serial.println(track);

    dfMini->playFolderTrack(folderNum, track);
}

void MP3Module::waitForTrackToFinish() {
    Serial.println(F("== waitForTrackToFinish()"));

    unsigned long maxWaitTime = millis() + TRACK_START_TIMEOUT;

    // Loop once before loop
    dfMini->loop();

    // Wait for the track to start playing
    while (!isPlaying() && millis() < maxWaitTime) {
        dfMini->loop();
    }

    // Wait until the track has finished playing
    while (isPlaying()) {
        dfMini->loop();
    }
}

void MP3Module::playFolder(const Folder folder) {
    Serial.println(F("=== playFolder()"));
    // Serial.print(lastTrackFinished);
    Serial.println(F("Address dfmini: "));
    Serial.println((unsigned long)dfMini);

    // standbyTimerController->disableTimer();

    lastTrackFinished = 0;
    // uint16_t trackCount = dfMini->getFolderTrackCount(folder.number);
    firstTrack = 1;

    Serial.print(F("Dateien in Ordner: "));
    Serial.println(folder.number);

    // switch (folder.mode) {
    //     // Hörspielmodus: eine zufällige Datei aus dem Ordner
    //     case FolderMode::RANDOM_MODE:
    //         Serial.println(F("Hörspielmodus -> zufälligen Track
    //         wiedergeben"));
    //         // Select random track in folder
    //         currentTrack = random(1, trackCount + 1);
    //         Serial.println(currentTrack);
    //         // Play selected track
    //         dfMini->playFolderTrack(folder.number, currentTrack);
    //         break;

    //     // Album Modus: kompletten Ordner spielen
    //     case FolderMode::ALBUM_MODE:
    //         Serial.println(F("Album Modus -> kompletten Ordner
    //         wiedergeben"));
    //         // Start from first track in folder
    //         currentTrack = 1;
    //         // Play first track
    //         dfMini->playFolderTrack(folder.number, currentTrack);
    //         break;

    //     // Party Modus: Ordner in zufälliger Reihenfolge
    //     case FolderMode::PARTY_MODE:
    //         Serial.println(F(
    //             "Party Modus -> Ordner in zufälliger Reihenfolge
    //             wiedergeben"));
    //         shuffleQueue();
    //         // Start from first track in shuffled queue
    //         currentTrack = 1;
    //         // Play first track in queue
    //         dfMini->playFolderTrack(folder.number, queue[currentTrack - 1]);
    //         break;

    //     // Einzel Modus: eine Datei aus dem Ordner abspielen
    //     case FolderMode::SINGLE_MODE:
    //         Serial.println(
    //             F("Einzel Modus -> eine Datei aus dem Odrdner abspielen"));
    //         // Select track specified in special variable
    //         currentTrack = folder.special;
    //         // Play selected track
    //         dfMini->playFolderTrack(folder.number, currentTrack);
    //         break;

    //     // Hörbuch Modus: kompletten Ordner spielen und Fortschritt merken
    //     case FolderMode::AUDIOBOOK_MODE:
    //         Serial.println(
    //             F("Hörbuch Modus -> kompletten Ordner spielen und Fortschritt
    //             "
    //               "merken"));
    //         // Retrieve saved progress from EEPROM
    //         currentTrack = EEPROM.read(folder.number);
    //         if (currentTrack == 0 ||
    //             // If progress is invalid, start
    //             // from first track
    //             currentTrack > trackCount) {
    //             currentTrack = 1;
    //         }
    //         dfMini->playFolderTrack(folder.number, currentTrack);
    //         break;

    //     // Spezialmodus Von-Bin: Hörspiel: eine zufällige Datei aus dem
    //     Ordner case FolderMode::SPECIAL_MODE_1:
    //         Serial.println(
    //             F("Spezialmodus Von-Bin: Hörspiel -> zufälligen Track "
    //               "wiedergeben"));
    //         Serial.print(folder.special);
    //         Serial.print(F(" bis "));
    //         Serial.println(folder.special2);
    //         // Set new track count based on
    //         // number in special2 variable
    //         trackCount = folder.special2;
    //         // Select random track between special
    //         // and special2 variables
    //         currentTrack = random(folder.special, trackCount + 1);
    //         Serial.println(currentTrack);
    //         dfMini->playFolderTrack(folder.number, currentTrack);
    //         break;

    //     // Spezialmodus Von-Bis: Album:
    //     // alle Dateien zwischen Start und End spielen
    //     case FolderMode::SPECIAL_MODE_2:
    //         Serial.println(
    //             F("Spezialmodus Von-Bis: Album: alle Dateien zwischen Start-
    //             "
    //               "und Enddatei spielen"));
    //         Serial.print(folder.special);
    //         Serial.print(F(" bis "));
    //         Serial.println(folder.special2);
    //         // Set new track count based on
    //         // number in special2 variable
    //         trackCount = folder.special2;
    //         // Start from track specified in special variable
    //         // Play first track
    //         currentTrack = folder.special;
    //         dfMini->playFolderTrack(folder.number, currentTrack);
    //         break;

    //     // Spezialmodus Von-Bis: Party Ordner in zufälliger Reihenfolge
    //     case FolderMode::SPECIAL_MODE_3:
    //         Serial.println(
    //             F("Spezialmodus Von-Bis: Party -> Ordner in zufälliger "
    //               "Reihenfolge wiedergeben"));
    //         // Set first track based on
    //         // number in special variable
    //         firstTrack = folder.special;
    //         // Set new track count based on
    //         // number in special2 variable
    //         shuffleQueue();
    //         trackCount = folder.special2;
    //         // Start from first track in shuffled queue
    //         currentTrack = 1;
    //         // Play first track in queue
    //         dfMini->playFolderTrack(folder.number, queue[currentTrack - 1]);
    //         break;
    //     default:
    //         Serial.println(F("Kein Modus -> nichts tun"));
    //         break;
    // }
}

void MP3Module::updateEqualizer() {
    Serial.println(F("=== updateEqualizer()"));
    AdminSettings *adminSettings = settingsController->adminSettings;
    dfMini->setEq(static_cast<DfMp3_Eq>(adminSettings->eq - 1));
}

void MP3Module::increaseVolume() {
    Serial.println(F("=== increaseVolume()"));

    AdminSettings *adminSettings = settingsController->adminSettings;
    if (volume < adminSettings->maxVolume) {
        volume++;
        dfMini->increaseVolume();
    }
    Serial.print(F("Volume: "));
    Serial.println(volume);
}

void MP3Module::decreaseVolume() {
    Serial.println(F("=== decreaseVolume()"));

    AdminSettings *adminSettings = settingsController->adminSettings;
    if (volume > adminSettings->minVolume) {
        volume--;
        dfMini->decreaseVolume();
    }
    Serial.print(F("Volume: "));
    Serial.println(volume);
}

void MP3Module::nextTrack() {
    Serial.println(F("=== nextTrack()"));

    Folder currentFolder = tonuinoState->activeNfcCard->folder;
    uint16_t trackCount = getFolderTrackCount(currentFolder);
    uint16_t nextTrack = currentTrack;
    Modifier *activeModifier = tonuinoState->activeModifier;
    NfcCard *activeNfcCard = tonuinoState->activeNfcCard;

    // Check if there is an active modifier and let it handle next track instead
    if (activeModifier && activeModifier->handleNext()) {
        Serial.println(F("Handle next track by modifier"));
        return;
    }

    if (!Utils::isKnownCard(activeNfcCard)) {
        // If a new card is presented, do not process end of track
        return;
    }

    // Check the current mode and perform actions accordingly
    switch (currentFolder.mode) {
        case FolderMode::RANDOM_MODE:
        case FolderMode::SPECIAL_MODE_1:
            // Don't play next track
            Serial.println(
                F("Hörspielmodus ist aktiv -> keinen neuen Track spielen"));
            standbyTimerController->setTimer();
            break;
        case FolderMode::ALBUM_MODE:
        case FolderMode::SPECIAL_MODE_2:
            if (currentTrack != trackCount) {
                // Play the next track if not at the end of the album
                nextTrack = ++currentTrack;
                playFolderTrack(currentFolder.number, nextTrack);
                Serial.print(F("Albummodus ist aktiv -> nächster Track: "));
            } else {
                standbyTimerController->setTimer();
            }
            break;
        case FolderMode::PARTY_MODE:
        case FolderMode::SPECIAL_MODE_3:
            if (currentTrack != trackCount - firstTrack + 1) {
                // Continue playing the queue
                Serial.println(F("Party -> weiter in der Queue"));
                nextTrack = ++currentTrack;
            } else {
                // Start from beginning when at the end of the queue
                Serial.println(F("Ende der Queue -> beginne von vorne"));
                currentTrack = 1;
            }
            // Play next track from queue
            playFolderTrack(currentFolder.number, queue[currentTrack - 1]);
            Serial.println(queue[currentTrack - 1]);
            break;
        case FolderMode::SINGLE_MODE:
            // Don't play next track
            Serial.println(F("Einzel Modus aktiv -> Strom sparen"));
            standbyTimerController->setTimer();
            break;
        case FolderMode::AUDIOBOOK_MODE:
            if (currentTrack != trackCount) {
                // Play next track for AUDIOBOOK_MODE
                // and save progress in EEPROM
                uint16_t nextTrack = ++currentTrack;
                Serial.print(
                    F("Hörbuch Modus ist aktiv -> nächster Track und "
                      "Fortschritt speichern: "));
                // Serial.println(nextTrack);
                playFolderTrack(currentFolder.number, nextTrack);
                EEPROM.update(currentFolder.number, currentTrack);
            } else {
                // Reset progress to the beginning when at the end of the
                // audiobook and set standby timer
                EEPROM.update(currentFolder.number, 1);
                standbyTimerController->setTimer();
            }
        default:
            Serial.println(F("Kein Modus -> nichts tun"));
            break;
    }
}

void MP3Module::previousTrack() {
    Serial.println(F("=== previousTrack()"));

    Folder currentFolder = tonuinoState->activeNfcCard->folder;
    uint16_t trackCount = getFolderTrackCount(currentFolder);

    if (currentFolder.mode == FolderMode::PARTY_MODE) {
        // For PARTY_MODE, play previous track in queue or
        // shuffle queue and play last track
        if (currentTrack > 1) {
            dfMini->playFolderTrack(currentFolder.number,
                                    queue[--currentTrack - 1]);
        } else {
            shuffleQueue();
            currentTrack = trackCount;
            dfMini->playFolderTrack(currentFolder.number,
                                    queue[currentTrack - 1]);
        }
    } else {
        // For other modes, play previous track or
        // play last track if at beginning
        if (currentTrack > 1) {
            dfMini->playFolderTrack(currentFolder.number, --currentTrack);
        } else {
            currentTrack = trackCount;
            dfMini->playFolderTrack(currentFolder.number, currentTrack);
        }
    }
}

uint16_t MP3Module::getFolderTrackCount(const Folder folder) {
    Serial.println(F("=== getFolderTrackCount()"));
    Serial.print(F("FolderNumber: "));
    Serial.println(folder.number);
    Serial.print(F("TrackCount: "));
    Serial.println(dfMini->getFolderTrackCount(folder.number));
    return dfMini->getFolderTrackCount(folder.number);
}

// Private functions

void MP3Module::initializeDFMini() {
    Serial.println(F("=== initializeDFMini()"));

    AdminSettings *adminSettings = settingsController->adminSettings;
    // Initialize DFPlayr Mini
    SoftwareSerial softwareSerial = SoftwareSerial(2, 3);  // RX, TX
    dfMini = new DFMiniMp3<SoftwareSerial, Mp3Notify>(softwareSerial);
    // Initialize DFPlayer Mini
    dfMini->begin();
    // Wait two seconds for the DFPlayer Mini to be initialized
    // delay(2000);
    // Set volume to initial volume
    volume = adminSettings->initVolume;
    dfMini->setVolume(volume);
    Serial.println("Address dfmini: ");
    Serial.println((unsigned long)dfMini);
    // Set equalizer to initial
}

void MP3Module::shuffleQueue() {
    Serial.println(F("=== shuffleQueue()"));

    Folder currentFolder = tonuinoState->activeNfcCard->folder;
    uint16_t trackCount = getFolderTrackCount(currentFolder);

    // Create queue for random playback
    for (uint8_t x = 0; x <= trackCount - firstTrack; x++)
        queue[x] = x + firstTrack;

    // Shuffle the queue
    for (uint8_t i = 0; i <= trackCount - firstTrack; i++) {
        // Pick a random index from current to end of queue
        uint8_t j = random(0, trackCount - firstTrack + 1);
        // Swap the current track with the randomly chosen one
        uint8_t t = queue[i];
        queue[i] = queue[j];
        queue[j] = t;
    }

    // Fill the rest of the queue with 0s
    for (uint8_t x = trackCount - firstTrack + 1; x < 255; x++) {
        queue[x] = 0;
    }
}