#include "Modifier.h"

class SleepTimer : Modifier
{
// private:
//     unsigned long sleepAtMillis = 0;

// public:
//     void loop()
//     {
//         if (this->sleepAtMillis != 0 && millis() > this->sleepAtMillis)
//         {
//             Serial.println(F("=== SleepTimer::loop() -> SLEEP!"));
//             mp3.pause();
//             // TODO uncomment
//             // StandbyTimerService::setTimer(adminSettings, );
//             // activeModifier = NULL;
//             delete this;
//         }
//     }

//     SleepTimer(uint8_t minutes)
//     {
//         Serial.println(F("=== SleepTimer()"));
//         Serial.println(minutes);
//         this->sleepAtMillis = millis() + minutes * 60000;
//         //      if (isPlaying())
//         //        mp3.playAdvertisement(302);
//         //      delay(500);
//     }
//     uint8_t getActive()
//     {
//         Serial.println(F("== SleepTimer::getActive()"));
//         return 1;
//     }
};