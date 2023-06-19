#include "Modifier.h"
#include "../config/structs.h"

Modifier::Modifier() {}

Modifier::Modifier(AdminSettings *adminSettings) {
    this->adminSettings = adminSettings;
}

void Modifier::loop() {}
bool Modifier::handlePause() { return false; }
bool Modifier::handleNext() { return false; }
bool Modifier::handlePrevious() { return false; }
bool Modifier::handleNextButton() { return false; }
bool Modifier::handlePreviousButton() { return false; }
bool Modifier::handleVolumeUp() { return false; }
bool Modifier::handleVolumeDown() { return false; }
bool Modifier::handleRFID(NfcCard *newCard) { return false; }
uint8_t Modifier::getActive() { return 0; }
