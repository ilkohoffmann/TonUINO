#include "Modifier.h"

class KindergardenMode : public Modifier
{
private:
    NfcCard nextCard;
    bool cardQueued = false;

public:
    virtual bool handleNext()
    {
    //     Serial.println(F("== KindergardenMode::handleNext() -> NEXT"));
    //     // if (this->nextCard.cookie == cardCookie && this->nextCard.nfcFolderSettings.folder != 0 && this->nextCard.nfcFolderSettings.mode != 0) {
    //     // folderSettings = &this->nextCard.nfcFolderSettings;
    //     if (this->cardQueued == true)
    //     {
    //         this->cardQueued = false;

    //         nfcCard = nextCard;
    //         folderSettings = nfcCard.folder;
    //         Serial.println(folderSettings.number);
    //         Serial.println(folderSettings.mode);
    //         // TODO: Comment int
    //         // playFolder();
    //         return true;
    //     }
    //     return false;
    // }
    // //    virtual bool handlePause()     {
    // //      Serial.println(F("== KindergardenMode::handlePause() -> LOCKED!"));
    // //      return true;
    // //    }
    // virtual bool handleNextButton()
    // {
    //     Serial.println(F("== KindergardenMode::handleNextButton() -> LOCKED!"));
    //     return true;
    // }
    // virtual bool handlePreviousButton()
    // {
    //     Serial.println(F("== KindergardenMode::handlePreviousButton() -> LOCKED!"));
    //     return true;
    // }
    // virtual bool handleRFID(NfcCard *newCard)
    // { // lot of work to do!
    //     Serial.println(F("== KindergardenMode::handleRFID() -> queued!"));
    //     this->nextCard = *newCard;
    //     this->cardQueued = true;
    //     if (!BoardService::isPlaying())
    //     {
    //         handleNext();
    //     }
    //     return true;
    // }
    // KindergardenMode()
    // {
    //     Serial.println(F("=== KindergardenMode()"));
    //     //      if (isPlaying())
    //     //        mp3.playAdvertisement(305);
    //     //      delay(500);
    // }
    // uint8_t getActive()
    // {
    //     Serial.println(F("== KindergardenMode::getActive()"));
    //     return 5;
    }
};