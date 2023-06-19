#include "ButtonController.h"

ButtonController::ButtonController(MP3Module* mp3Module)
    : pauseButton(CONFIG_BUTTON_PAUSE_PIN),
      upButton(CONFIG_BUTTON_UP_PIN),
      downButton(CONFIG_BUTTON_DOWN_PIN),
      mp3Module(mp3Module) {}

void ButtonController::readButtons() {
    pauseButton.read();
    upButton.read();
    downButton.read();
#ifdef FIVEBUTTONS
    buttonFour.read();
    buttonFive.read();
#endif
}

bool ButtonController::pauseButtonLongPressed(bool shouldReadButtons) {
    if (shouldReadButtons) {
        readButtons();
    }
    return pauseButton.pressedFor(LONG_PRESS);
}
bool ButtonController::upButtonLongPressed(bool shouldReadButtons) {
    if (shouldReadButtons) {
        readButtons();
    }
    return upButton.pressedFor(LONG_PRESS);
}
bool ButtonController::downButtonLongPressed(bool shouldReadButtons) {
    if (shouldReadButtons) {
        readButtons();
    }
    return downButton.pressedFor(LONG_PRESS);
}

bool ButtonController::someMainButtonLongPressed(bool shouldReadButtons) {
    if (shouldReadButtons) {
        readButtons();
    }
    return pauseButton.pressedFor(LONG_PRESS) ||
           upButton.pressedFor(LONG_PRESS) || downButton.pressedFor(LONG_PRESS);
}

bool ButtonController::someMainButtonPressed(bool shouldReadButtons) {
    if (shouldReadButtons) {
        readButtons();
    }
    return pauseButton.isPressed() || upButton.isPressed() ||
           downButton.isPressed();
}

bool ButtonController::allMainButtonsPressed(bool shouldReadButtons) {
    if (shouldReadButtons) {
        readButtons();
    }
    return pauseButton.isPressed() && upButton.isPressed() &&
           downButton.isPressed();
}

bool ButtonController::pauseButtonReleased(bool shouldReadButtons) {
    if (shouldReadButtons) {
        readButtons();
    }
    return pauseButton.wasReleased();
}

bool ButtonController::upButtonReleased(bool shouldReadButtons) {
    if (shouldReadButtons) {
        readButtons();
    }
    return upButton.wasReleased();
}

bool ButtonController::downButtonReleased(bool shouldReadButtons) {
    if (shouldReadButtons) {
        readButtons();
    }
    return downButton.wasReleased();
}

bool ButtonController::upOrDownButtonReleased(bool shouldReadButtons) {
    if (shouldReadButtons) {
        readButtons();
    }
    return upButton.wasReleased() || downButton.wasReleased();
}

void ButtonController::waitForButtonRelease(ButtonPress buttonPress) {
    Serial.println(F("=== waitForButtonRelease()"));

    // wait for button release
    switch (buttonPress) {
        case ButtonPress::PAUSE:
        case ButtonPress::LONG_PAUSE:
            while (!pauseButtonReleased()) {
                readButton(buttonPress);
            }
            break;
        case ButtonPress::UP:
        case ButtonPress::LONG_UP:
            while (!upButtonReleased()) {
                readButton(buttonPress);
            }
            break;
        case ButtonPress::DOWN:
        case ButtonPress::LONG_DOWN:
            while (!downButtonReleased()) {
                readButton(buttonPress);
            }
            break;
    }
}

ButtonPress ButtonController::waitForButtonPress(bool shouldReadButtons) {
    Serial.println(F("=== waitForButtonPress()"));
    if (shouldReadButtons) {
        readButtons();
    }

    while (true) {
        if (upButtonLongPressed()) {
            return ButtonPress::UP;
        } else if (upButtonReleased()) {
            return ButtonPress::LONG_UP;
        } else if (downButtonLongPressed()) {
            return ButtonPress::DOWN;
        } else if (downButtonReleased()) {
            return ButtonPress::LONG_DOWN;
        } else if (pauseButtonLongPressed()) {
            return ButtonPress::PAUSE;
        } else if (pauseButtonReleased()) {
            return ButtonPress::LONG_PAUSE;
        }
    }
}

// Private
void ButtonController::readButton(ButtonPress buttonPress) {
    switch (buttonPress) {
        case ButtonPress::PAUSE:
        case ButtonPress::LONG_PAUSE:
            pauseButton.read();
            break;
        case ButtonPress::UP:
        case ButtonPress::LONG_UP:
            upButton.read();
            break;
        case ButtonPress::DOWN:
        case ButtonPress::LONG_DOWN:
            downButton.read();
            break;
    }
}