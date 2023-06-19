#include "Modifier.h"

class RepeatSingleModifier : Modifier
{
public:
    // virtual bool handleNext()
    // {
    //     Serial.println(F("== RepeatSingleModifier::handleNext() -> REPEAT CURRENT TRACK"));
    //     delay(50);
    //     if (BoardService::isPlaying())
    //         return true;
    //     if (folderSettings.mode == 3 || folderSettings.mode == 9)
    //     {
    //         mp3.playFolderTrack(folderSettings.number, mp3PlayerSettings.queue[mp3PlayerSettings.currentTrack - 1]);
    //     }
    //     else
    //     {
    //         mp3.playFolderTrack(folderSettings.number, mp3PlayerSettings.currentTrack);
    //     }
    //     mp3PlayerSettings.lastTrackFinished = 0;
    //     return true;
    // }
    // RepeatSingleModifier()
    // {
    //     Serial.println(F("=== RepeatSingleModifier()"));
    // }
    // uint8_t getActive()
    // {
    //     Serial.println(F("== RepeatSingleModifier::getActive()"));
    //     return 6;
    // }
};