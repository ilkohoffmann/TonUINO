#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include "../config/config.h"
#include "../config/structs.h"

class Utils {
   public:
    static void dump_byte_array(byte* buffer, byte bufferSize);
    static bool isKnownCard(NfcCard* nfcCard);
};

#endif