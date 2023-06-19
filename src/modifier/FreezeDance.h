#include "Modifier.h"

class FreezeDance : public Modifier
{
private:
    unsigned long nextStopAtMillis = 0;
    const uint8_t minSecondsBetweenStops = 5;
    const uint8_t maxSecondsBetweenStops = 30;

    void setNextStopAtMillis()
    {
        uint16_t seconds = random(this->minSecondsBetweenStops, this->maxSecondsBetweenStops + 1);
        Serial.println(F("=== FreezeDance::setNextStopAtMillis()"));
        Serial.println(seconds);
        this->nextStopAtMillis = millis() + seconds * 1000;
    }

public:
    void loop() {}
    // {
    //     if (this->nextStopAtMillis != 0 && millis() > this->nextStopAtMillis)
    //     {
    //         Serial.println(F("== FreezeDance::loop() -> FREEZE!"));
    //         if (mp3Module->isPlaying())
    //         {
    //             mp3Module.playAdvertisement(301);
    //             delay(500);
    //         }
    //         setNextStopAtMillis();
    //     }
    // }
    // FreezeDance(MP3PlayerSettings mp3PlayerSettings,
    //             AdminSettings *adminSettings,
    //             DFMiniMp3<SoftwareSerial, Mp3Notify> &mp3) : Modifier(mp3PlayerSettings,
    //                                                                   adminSettings,
    //                                                                   mp3)
    // {
    //     Serial.println(F("=== FreezeDance()"));
    //     if (BoardService::isPlaying())
    //     {
    //         delay(1000);
    //         mp3.playAdvertisement(300);
    //         delay(500);
    //     }
    //     setNextStopAtMillis();
    // }
    // uint8_t getActive()
    // {
    //     Serial.println(F("== FreezeDance::getActive()"));
    //     return 2;
    // }
};