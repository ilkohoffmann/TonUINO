#include "Modifier.h"

// An modifier can also do somethings in addition to the modified action
// by returning false (not handled) at the end
// This simple FeedbackModifier will tell the volume before changing it and
// give some feedback once a RFID card is detected.
class FeedbackModifier : public Modifier
{
public:
    virtual bool handleVolumeDown()
    {
        // if (mp3PlayerSettings.volume > adminSettings->minVolume)
        // {
        //     mp3.playAdvertisement(mp3PlayerSettings.volume - 1);
        // }
        // else
        // {
        //     mp3.playAdvertisement(mp3PlayerSettings.volume);
        // }
        // delay(500);
        // Serial.println(F("== FeedbackModifier::handleVolumeDown()!"));
        // return false;
    }
    virtual bool handleVolumeUp()
    {
        // if (mp3PlayerSettings.volume < adminSettings->maxVolume)
        // {
        //     mp3.playAdvertisement(mp3PlayerSettings.volume + 1);
        // }
        // else
        // {
        //     mp3.playAdvertisement(mp3PlayerSettings.volume);
        // }
        // delay(500);
        // Serial.println(F("== FeedbackModifier::handleVolumeUp()!"));
        // return false;
    }
    virtual bool handleRFID(NfcCard *newCard)
    {
        Serial.println(F("== FeedbackModifier::handleRFID()"));
        return false;
    }
};