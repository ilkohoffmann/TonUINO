#include "RFIDModule.h"

RFIDModule::RFIDModule(MP3Module* mp3Module, ButtonController* buttonController,
                       TonuinoState* tonuinoState)
    : mp3Module(mp3Module),
      buttonController(buttonController),
      tonuinoState(tonuinoState) {
        initializeMfr522();
      }

bool RFIDModule::isNewCardPresent() {
    return mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial();
}

bool RFIDModule::isCardRemoved() {
    // Buffer to store the response from the PICC
    byte bufferATQA[2];
    byte bufferSize = sizeof(bufferATQA);

    // Send a Wake-up command and store the result
    MFRC522::StatusCode result = mfrc522.PICC_WakeupA(bufferATQA, &bufferSize);

    // If the result is not STATUS_OK, then the card is not present.
    if (result != MFRC522::STATUS_OK) {
        Serial.println(F("Card removed."));
        return true;
    }

    return false;
}

void RFIDModule::setupCard(const Folder folder) {
    NfcCard nfcCard;

    Serial.println(F("=== setupCard()"));

    mp3Module->pause();
    mp3Module->playMp3FolderTrack(MP3Track::WATING_FOR_CARD);

    // Wait for new RFID card,
    // allowing user to abort with button press
    while (!mfrc522.PICC_IsNewCardPresent()) {
        if (buttonController->upOrDownButtonReleased()) {
            Serial.println(F("Aborted!"));
            mp3Module->playMp3FolderTrack(MP3Track::ABORT);
            return;
        }
    }

    // Read card serial number; if reading fails,
    // it's most likely due to no card
    // being present or not detected correctly.
    // In this case, immediately return from the
    // function to avoid further processing with a null card ID.
    if (!mfrc522.PICC_ReadCardSerial()) return;

    Serial.println(F("Card is being reconfigured!"));
    nfcCard.folder = folder;

    // Persist configuration
    writeCard(nfcCard);

    // Deactivate currently active PICC
    mfrc522.PICC_HaltA();
    // Stop any ongoing encrypted communication
    // and clear authentication state
    mfrc522.PCD_StopCrypto1();

    // TODO: Check if this is needed
    delay(1000);
}

void RFIDModule::readCard() {
    Serial.println(F("=== readCard()"));

    // Get card UID
    String cardUID = getCardUID();
    Serial.print(F("Card UID: "));
    Serial.println(cardUID);

    // Initialize variables
    NfcCard nfcCard;
    byte buffer[18];
    byte size = sizeof(buffer);

    // Get card type and authenticate card
    MFRC522::PICC_Type mifareType = getCardType();
    authenticateCard(mifareType); 

    // Read data from the block based on card type
    if (status != MFRC522::STATUS_ERROR) {
        switch (mifareType) {
            case MFRC522::PICC_TYPE_MIFARE_MINI:
            case MFRC522::PICC_TYPE_MIFARE_1K:
            case MFRC522::PICC_TYPE_MIFARE_4K:
                Serial.print(F("Reading data from block "));
                Serial.print(blockAddr);
                Serial.println(F(" ..."));
                status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
                break;

            case MFRC522::PICC_TYPE_MIFARE_UL:
                for (byte block = 8; block <= 11; ++block) {
                    byte blockBuffer[18];
                    byte blockSize = sizeof(blockBuffer);
                    status =
                        mfrc522.MIFARE_Read(block, blockBuffer, &blockSize);
                    memcpy(buffer + (4 * (block - 8)), blockBuffer, 4);
                }
                break;

            default:
                Serial.print(F("No matching card type found: "));
                status = MFRC522::STATUS_ERROR;
                break;
        }
    }

    // Handle errors
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        mp3Module->playMp3FolderTrack(MP3Track::ERROR);
        return;
    }

    // Print data on card and interpret buffer
    Serial.print(F("Data on Card:"));
    Utils::dump_byte_array(buffer, 16);

    uint32_t tempCookie = ((uint32_t)buffer[0] << 24) |
                          ((uint32_t)buffer[1] << 16) |
                          ((uint32_t)buffer[2] << 8) | (uint32_t)buffer[3];
    nfcCard.uid = cardUID;
    nfcCard.cookie = tempCookie;
    nfcCard.version = buffer[4];
    nfcCard.folder.number = buffer[5];
    nfcCard.folder.mode = (FolderMode)buffer[6];
    nfcCard.folder.special = buffer[7];
    nfcCard.folder.special2 = buffer[8];

    // Replace activeNfcCard with new card
    *(tonuinoState->activeNfcCard) = nfcCard;

    Serial.print(F("Folder number nfc: "));
    Serial.println(nfcCard.folder.number);
    Serial.print(F("Folder number activeNfcCard: "));
    Serial.println(tonuinoState->activeNfcCard->folder.number);

   
    // TODO: Check if thats the right place to call this functions
    // Deactivate currently active PICC
    mfrc522.PICC_HaltA();
    // Stop any ongoing encrypted communication
    // and clear authentication state
    mfrc522.PCD_StopCrypto1();
}

void RFIDModule::writeCard(const NfcCard nfcCard) {
    // Initialize a byte buffer with the necessary information needed
    // for communication between the MFRC522 module and an associated tag
    byte buffer[16] = {0x13,  // corresponding to CARD_COOKIE
                       0x37,
                       0xb3,
                       0x47,
                       0x02,  // version number
                       nfcCard.folder.number,
                       nfcCard.folder.mode,
                       nfcCard.folder.special,
                       nfcCard.folder.special2,
                       0x00,
                       0x00,
                       0x00,
                       0x00,
                       0x00,
                       0x00,
                       0x00};

    MFRC522::PICC_Type mifareType = getCardType();
    authenticateCard(mifareType);

    if (status == MFRC522::STATUS_OK) {
        // Write data to the block
        Serial.print(F("Writing data into block "));
        Serial.print(blockAddr);
        Serial.println(F(" ..."));

        // Print buffer content
        Utils::dump_byte_array(buffer, 16);

        // Perform write operation depending on tag type
        switch (mifareType) {
            // For MIFARE Classic tags,
            // write data to specified block
            case MFRC522::PICC_TYPE_MIFARE_MINI:
            case MFRC522::PICC_TYPE_MIFARE_1K:
            case MFRC522::PICC_TYPE_MIFARE_4K: {
                status = mfrc522.MIFARE_Write(blockAddr, buffer, 16);
            } break;
            // For MIFARE Ultralight tags,
            // write data to multiple pages
            case MFRC522::PICC_TYPE_MIFARE_UL: {
                byte buffer2[16];
                byte size2 = sizeof(buffer2);

                // Write first 4 bytes of buffer to page 8
                memset(buffer2, 0, size2);
                memcpy(buffer2, buffer, 4);
                status = mfrc522.MIFARE_Write(8, buffer2, 16);

                // Write next 4 bytes of buffer to page 9
                memset(buffer2, 0, size2);
                memcpy(buffer2, buffer + 4, 4);
                status = mfrc522.MIFARE_Write(9, buffer2, 16);

                // Write next 4 bytes of buffer to page 10
                memset(buffer2, 0, size2);
                memcpy(buffer2, buffer + 8, 4);
                status = mfrc522.MIFARE_Write(10, buffer2, 16);

                // Write last 4 bytes of buffer to page 11
                memset(buffer2, 0, size2);
                memcpy(buffer2, buffer + 12, 4);
                status = mfrc522.MIFARE_Write(11, buffer2, 16);
            } break;
            case MFRC522::PICC_TYPE_UNKNOWN: {
                Serial.print(F("No matching card type found: "));
                status = MFRC522::STATUS_ERROR;
            } break;
            default: {
                Serial.print(F("Card type not supported: "));
                status = MFRC522::STATUS_ERROR;
            }
        }
    } else {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        mp3Module->playMp3FolderTrack(MP3Track::ERROR);
    }

    // Deactivate currently active PICC
    mfrc522.PICC_HaltA();
    // Stop any ongoing encrypted communication
    // and clear authentication state
    mfrc522.PCD_StopCrypto1();
}

// reset card === reconfigure card
// void RFIDModule::resetCard() {
//     mp3Module->playMp3FolderTrack(MP3Track::WATING_FOR_CARD);
//     do {
//         if (buttonController->upOrDownButtonReleased()) {
//             Serial.print(F("Abgebrochen!"));
//             mp3Module->playMp3FolderTrack(MP3Track::ABORT);
//             return;
//         }
//     } while (!mfrc522.PICC_IsNewCardPresent());

//     // Read card serial number; if reading fails,
//     // it's most likely due to no card
//     // being present or not detected correctly.
//     // In this case, immediately return from the
//     // function to avoid further processing with a null card ID.
//     if (!mfrc522.PICC_ReadCardSerial()) return;

//     Serial.print(F("Karte wird neu konfiguriert!"));
//     setupCard();
// };

void RFIDModule::sleep() {
    // http://discourse.voss.earth/t/intenso-s10000-powerbank-automatische-abschaltung-software-only/805
    // powerdown to 27mA (powerbank switches off after 30-60s)
    mfrc522.PCD_AntennaOff();
    mfrc522.PCD_SoftPowerDown();
}

void handleCardEvent(CardEvent cardEvent) {

    switch (cardEvent) {
    case CardEvent::NEW_CARD: {
         break;
    }
    case CardEvent::CARD_REMOVED: {
         break;
    }
    default:
        break;
    }
}

String RFIDModule::getCardUID() {
    String cardUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        cardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        cardUID += String(mfrc522.uid.uidByte[i], HEX);
    }
    cardUID.toUpperCase();
    return cardUID;
}

// Private members
void RFIDModule::initializeMfr522() {
    Serial.println(F("=== initializeMfr522()"));

    // Initialize members
    successRead = false;
    sector = 1;
    blockAddr = 4;
    trailerBlock = 7;

    // Set default key to 0xFF for all bytes
    // (this is a commonly used default key)
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    // Initialize RFID module
    mfrc522 = MFRC522(CONFIG_SS_PIN, CONFIG_RST_PIN);

    // Initialize SPI bus
    SPI.begin();

    // Initialize MFRC522 RFID module
    mfrc522.PCD_Init();

    // Print firmware version of MFRC522 to serial console
    mfrc522.PCD_DumpVersionToSerial();
}

void RFIDModule::authenticateCard(MFRC522::PICC_Type mifareType) {
    // Check if the card is a MIFARE MINI, MIFARE 1K, or MIFARE 4K
    switch (mifareType) {
        case MFRC522::PICC_TYPE_MIFARE_MINI:
        case MFRC522::PICC_TYPE_MIFARE_1K:
        case MFRC522::PICC_TYPE_MIFARE_4K: {
            Serial.println(F("Authenticating MIFARE classic car using key A"));
            status =
                mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                         trailerBlock, &key, &(mfrc522.uid));
        } break;
        case MFRC522::PICC_TYPE_MIFARE_UL: {
            byte pACK[] = {0, 0};

            Serial.println(
                F("Authenticating MIFARE Ultralight card using pACK"));
            status = mfrc522.PCD_NTAG216_AUTH(key.keyByte, pACK);
        } break;
        case MFRC522::PICC_TYPE_UNKNOWN: {
            Serial.print(F("No matching card type found: "));
            status = MFRC522::STATUS_ERROR;
            mp3Module->playMp3FolderTrack(MP3Track::ERROR);  // Play error sound
        } break;
        default: {
            Serial.print(F("Card type not supported: "));
            status = MFRC522::STATUS_ERROR;
            mp3Module->playMp3FolderTrack(MP3Track::ERROR);  // Play error sound
        }
    }
}

// Retrieves the type of MIFARE card that has been scanned by the MFRC522
// RFID reader The SAK (Select Acknowledge) byte is part of the RFID
// protocol and is used to identify the type of card that has been scanned.
MFRC522::PICC_Type RFIDModule::getCardType() {
    MFRC522::PICC_Type mifareType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.print(F("PICC type: "));
    Serial.println(mfrc522.PICC_GetTypeName(mifareType));
    return mifareType;
}
