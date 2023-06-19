#ifndef MP3MODULE_H
#define MP3MODULE_H

#include <Arduino.h>
#include <DFMiniMp3.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

#include "../config/config.h"
#include "../config/structs.h"
#include "../controller/SettingsController.h"
#include "../controller/StandbyTimerController.h"
#include "../enums/FolderModeEnum.h"
#include "../modifier/Modifier.h"
#include "../utils/Utils.h"
#include "Mp3Notify.h"

class MP3Module {
   public:
    // Constructor
    MP3Module(StandbyTimerController *standbyTimerController,
              SettingsController *settingsController);

    // Functions
    void loop();
    void pause();
    void start();
    void sleep();
    bool isPlaying();
    void playFolder(const Folder folder);
    void playMp3FolderTrack(const uint16_t track);
    void playAdvertisementTrack();
    void playFolderTrack(const uint8_t folderNum, const uint8_t track);
    void waitForTrackToFinish();
    void updateEqualizer();
    void increaseVolume();
    void decreaseVolume();
    void nextTrack();
    void previousTrack();
    uint16_t getFolderTrackCount(const Folder folder);

   private:
    // Members
    DFMiniMp3<SoftwareSerial, Mp3Notify> *dfMini;
    StandbyTimerController *standbyTimerController;
    SettingsController *settingsController;
    TonuinoState *tonuinoState;

    uint16_t currentTrack;
    uint16_t firstTrack;
    uint8_t queue[255];
    uint8_t volume;
    uint16_t lastTrackFinished;

    // Helper functions
    void initializeDFMini();
    void shuffleQueue();
};

#endif