#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#include <Arduino.h>
#include <JC_Button.h>

#include "../config/config.h"
#include "../config/structs.h"
#include "../enums/ButtonPress.h"
#include "../modules/MP3Module.h"

class ButtonController {
   public:
    ButtonController(MP3Module* mp3Module);

    void readButtons();
   
    bool pauseButtonLongPressed(bool shouldReadButtons = false);
    bool upButtonLongPressed(bool shouldReadButtons = false);
    bool downButtonLongPressed(bool shouldReadButtons = false);
    bool someMainButtonLongPressed(bool shouldReadButtons = false);
    bool someMainButtonPressed(bool shouldReadButtons = false);
    bool allMainButtonsPressed(bool shouldReadButtons = false);
    bool pauseButtonReleased(bool shouldReadButtons = false);
    bool upButtonReleased(bool shouldReadButtons = false);
    bool downButtonReleased(bool shouldReadButtons = false);
    bool upOrDownButtonReleased(bool shouldReadButtons = false);
    void waitForButtonRelease(ButtonPress buttonPress);
    ButtonPress waitForButtonPress(bool shouldReadButtons = false);

   private:
    Button pauseButton;
    Button upButton;
    Button downButton;
#ifdef FIVEBUTTONS
    Button buttonFour;
    Button buttonFive;
#endif
    MP3Module* mp3Module;
    void readButton(ButtonPress buttonPress);
};

#endif