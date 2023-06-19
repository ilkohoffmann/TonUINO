#ifndef MODIFIER_H
#define MODIFIER_H

#include <Arduino.h>

// Forward declaration of NfcCard
struct NfcCard;
struct AdminSettings;

class Modifier {
   public:
    Modifier();
    Modifier(AdminSettings* adminSettings);
    virtual void loop();
    virtual bool handlePause();
    virtual bool handleNext();
    virtual bool handlePrevious();
    virtual bool handleNextButton();
    virtual bool handlePreviousButton();
    virtual bool handleVolumeUp();
    virtual bool handleVolumeDown();
    virtual bool handleRFID(NfcCard* newCard);
    virtual uint8_t getActive();

   protected:
    AdminSettings* adminSettings;
};

#endif