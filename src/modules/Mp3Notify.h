#ifndef MP3NOTIFY_H
#define MP3NOTIFY_H

#include <Arduino.h>
#include <DFMiniMp3.h>

#include "MP3Module.h"

class Mp3Notify {
   public:
    static void OnError(DFMiniMp3<HardwareSerial, Mp3Notify>&, uint16_t errorCode);
    static void OnPlayFinished(DFMiniMp3<HardwareSerial, Mp3Notify>&, DfMp3_PlaySources source,
                               uint16_t track);
    static void OnPlaySourceOnline(DFMiniMp3<HardwareSerial, Mp3Notify>&, DfMp3_PlaySources source);
    static void OnPlaySourceInserted(DFMiniMp3<HardwareSerial, Mp3Notify>&, DfMp3_PlaySources source);
    static void OnPlaySourceRemoved(DFMiniMp3<HardwareSerial, Mp3Notify>&, DfMp3_PlaySources source);
    static void ResetLastTrackFinished() { lastTrackFinished = 0; }

   private:
    static void PrintlnSourceAction(DfMp3_PlaySources source,
                                    const __FlashStringHelper* action);
    static uint16_t lastTrackFinished;
};

#endif