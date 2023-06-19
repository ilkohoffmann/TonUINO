#ifndef RFIDModule_H
#define RFIDModule_H

#include <Arduino.h>
#include <MFRC522.h>

#include "../config/config.h"
#include "../config/structs.h"
#include "../controller/ButtonController.h"
#include "../controller/MenuController.h"
#include "../enums/MP3TrackEnum.h"
#include "../utils/Utils.h"
#include "MP3Module.h"

class RFIDModule {
   public:
    RFIDModule(MP3Module* mp3Module, ButtonController* buttonController, TonuinoState* tonuinoState);

    MFRC522 mfrc522;
    bool successRead;
    byte sector;
    byte blockAddr;
    byte trailerBlock;
    MFRC522::MIFARE_Key key;
    MFRC522::StatusCode status;

    bool isNewCardPresent();
    void setupCard(const Folder folder);
    void readCard();
    void writeCard(const NfcCard nfcCard);
    void sleep();

   private:
    MP3Module* mp3Module;
    ButtonController* buttonController;
    TonuinoState* tonuinoState;

    void initializeMfr522();
    void authenticateCard(MFRC522::PICC_Type mifareType);
    MFRC522::PICC_Type getCardType();
};

#endif