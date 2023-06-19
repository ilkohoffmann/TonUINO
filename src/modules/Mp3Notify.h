#ifndef MP3NOTIFY_H
#define MP3NOTIFY_H

#include <Arduino.h>
#include <DFMiniMp3.h>

class Mp3Notify
{
public:
    static void OnError(uint16_t errorCode);
    static void PrintlnSourceAction(DfMp3_PlaySources source, const char *action);
    static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track);
    static void OnPlaySourceOnline(DfMp3_PlaySources source);
    static void OnPlaySourceInserted(DfMp3_PlaySources source);
    static void OnPlaySourceRemoved(DfMp3_PlaySources source);
};

#endif